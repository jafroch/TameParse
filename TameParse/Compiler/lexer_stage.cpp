//
//  lexer_stage.cpp
//  Parse
//
//  Created by Andrew Hunter on 21/08/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

// TODO: it'd be nice to have a way of designing this so it's not dependent on the language_stage stage
//       (we don't do this at the moment to keep things reasonably simple, there's a lot of dependencies that means that DI
//       wouldn't really fix the problem, and would just create a new 'giant constructor of doom' problem)

#include <sstream>
#include "TameParse/Compiler/lexer_stage.h"

using namespace std;
using namespace dfa;
using namespace contextfree;
using namespace lr;
using namespace compiler;

/// \brief Creates a new lexer compiler
///
/// The compiler will not 'own' the objects passed in to this constructor; however, they must have a lifespan
/// that is at least as long as the compiler itself (it's safe to call the destructor but no other call if they
/// have been destroyed)
lexer_stage::lexer_stage(console_container& console, const std::wstring& filename, language_stage* languageCompiler)
: compilation_stage(console, filename)
, m_Language(languageCompiler)
, m_WeakSymbols(languageCompiler->grammar())
, m_Dfa(NULL)
, m_Lexer(NULL) {
}

/// \brief Destroys the lexer compiler
lexer_stage::~lexer_stage() {
    // Destroy the DFA if it exists
    if (m_Dfa) {
        delete m_Dfa;
    }
    
    if (m_Lexer) {
        delete m_Lexer;
    }
}

/// \brief Compiles the lexer
void lexer_stage::compile() {
    // Grab the input
    const lexer_data*       lex             = m_Language->lexer();
    terminal_dictionary*    terminals       = m_Language->terminals();
    const set<int>*         weakSymbolIds   = m_Language->weak_symbols();
    
    // Reset the weak symbols
    m_WeakSymbols = lr::weak_symbols(m_Language->grammar());
    
    // Sanity check
    if (!lex || !terminals || !weakSymbolIds) {
        cons().report_error(error(error::sev_bug, filename(), L"BUG_LEXER_BAD_PARAMETERS", L"Missing input for the lexer stage", position(-1, -1, -1)));
        return;
    }
    
    // Output a staging message
    cons().verbose_stream() << L"  = Constructing final lexer" << endl;

    // Create the ndfa
    typedef lexer_data::item_list item_list;

    dfa::ndfa_regex* stage0 = new ndfa_regex();

    // Iterate through the definition lists for each item
    for (lexer_data::iterator itemList = lex->begin(); itemList != lex->end(); itemList++) {
        // Iterate through the individual definitions for this item
        for (item_list::const_iterator item = itemList->second.begin(); item != itemList->second.end(); item++) {
            // Ignore items without a valid accept action (this is a bug)
            if (!item->accept) {
                cons().report_error(error(error::sev_bug, filename(), L"BUG_MISSING_ACTION", L"Missing action for lexer symbol", position(-1, -1, -1)));
                continue;
            }
            
            // Add the corresponding items
            switch (item->type) {
                case lexer_item::regex:
                    stage0->set_case_insensitive(item->case_insensitive);
                    stage0->add_regex(0, item->definition, *item->accept);
                    break;

                case lexer_item::literal:
                    stage0->set_case_insensitive(item->case_insensitive);
                    stage0->add_literal(0, item->definition, *item->accept);
                    break;
            }
        }
    }

    // Write out some stats about the ndfa
    cons().verbose_stream() << L"    Number states in the NDFA:              " << stage0->count_states() << endl;
    
    // Compile the NDFA to a NDFA without overlapping symbol sets
    dfa::ndfa* stage1 = stage0->to_ndfa_with_unique_symbols();
    
    if (!stage1) {
        cons().report_error(error(error::sev_bug, filename(), L"BUG_DFA_FAILED_TO_CONVERT", L"Failed to create an NDFA with unique symbols", position(-1, -1, -1)));
        return;
    }
    
    // Write some information about the first stage
    cons().verbose_stream() << L"    Initial number of character sets:       " << stage0->symbols().count_sets() << endl;
    cons().verbose_stream() << L"    Final number of character sets:         " << stage1->symbols().count_sets() << endl;

    delete stage0;
    stage0 = NULL;
    
    // Compile the NDFA to a DFA
    dfa::ndfa* stage2 = stage1->to_dfa();
    delete stage1;
    stage1 = NULL;
    
    if (!stage2) {
        cons().report_error(error(error::sev_bug, filename(), L"BUG_DFA_FAILED_TO_COMPILE", L"Failed to compile DFA", position(-1, -1, -1)));
        return;
    }
    
    // Identify any terminals that are always replaced by other terminals (warning)
    set<int>            unusedTerminals;
    map<int, set<int> > clashes;
    for (int terminalId = 0; terminalId < m_Language->terminals()->count_symbols(); terminalId++) {
        unusedTerminals.insert(terminalId);
    }
    
    for (int stateId = 0; stateId < stage2->count_states(); stateId++) {
        // Get the actions for this state
        const ndfa::accept_action_list& acceptActions = stage2->actions_for_state(stateId);
        
        // Ignore empty action sets
        if (acceptActions.empty()) continue;
        
        // Pick the highest action
        ndfa::accept_action_list::const_iterator action = acceptActions.begin();
        accept_action* highest = *action;
        
        action++;
        for (; action != acceptActions.end(); action++) {
            if (*highest < **action) {
                clashes[highest->symbol()].insert((*action)->symbol());
                highest = *action;
            } else {
                clashes[(*action)->symbol()].insert(highest->symbol());
            }
        }
        
        // Remove from the set of unused terminals
        unusedTerminals.erase(highest->symbol());
    }
    
    // Report warnings for any terminals that are never generated by the lexer
    for (set<int>::const_iterator unusedSymbol = unusedTerminals.begin(); unusedSymbol != unusedTerminals.end(); unusedSymbol++) {
        // Get the position of this terminal
        position        pos     = m_Language->terminal_definition_pos(*unusedSymbol);
        const wstring&  file    = m_Language->terminal_definition_file(*unusedSymbol);
        
        // Get the name of the terminal
        const wstring& name = m_Language->terminals()->name_for_symbol(*unusedSymbol);
        
        // Build the warning message
        wstringstream msg;
        msg << L"Lexer symbol can never be generated: " << name;
        
        cons().report_error(error(error::sev_warning, file, L"SYMBOL_CANNOT_BE_GENERATED", msg.str(), pos));
        
        // Get the symbols that clash with this one
        set<int>& clashSet = clashes[*unusedSymbol];
        if (!clashSet.empty()) {
            // Write out the symbols that are generated instead
            for (set<int>::iterator clashSymbol = clashSet.begin(); clashSymbol != clashSet.end(); clashSymbol++) {
                wstringstream msg2;
                msg2 << L"'" << name << L"' clashes with: " << m_Language->terminals()->name_for_symbol(*clashSymbol);
                cons().report_error(error(error::sev_detail, m_Language->terminal_definition_file(*clashSymbol), L"SYMBOL_CLASHES_WITH", msg2.str(), m_Language->terminal_definition_pos(*clashSymbol)));
            }
        }
    }
    
    // TODO: also identify any terminals that clash with terminals at the same level (warning)
    
    // Build up the weak symbols set if there are any
    if (weakSymbolIds->size() > 0) {
        // Build up the weak symbol set as a series of items
        item_set weakSymSet(m_Language->grammar());
        
        // Count how many symbols there were initially
        int initialSymCount = terminals->count_symbols();
        
        // Iterate through the symbol IDs
        for (set<int>::const_iterator weakSymId = weakSymbolIds->begin(); weakSymId != weakSymbolIds->end(); weakSymId++) {
            weakSymSet.insert(item_container(new terminal(*weakSymId), true));
        }
        
        // Add these symbols to the weak symbols object
        m_WeakSymbols.add_symbols(*stage2, weakSymSet, *terminals);
        
        // Display how many new terminal symbols were added
        int finalSymCount = terminals->count_symbols();
        
        cons().verbose_stream() << L"    Number of extra weak symbols:           " << finalSymCount - initialSymCount << endl;
    }
    
    // Compact the resulting DFA
    cons().verbose_stream() << L"    Number of states in the lexer DFA:      " << stage2->count_states() << endl;
    dfa::ndfa* stage3 = stage2->to_compact_dfa();
    delete stage2;
    stage2 = NULL;
    
    // Write some information about the DFA we just produced
    cons().verbose_stream() << L"    Number of states in the compacted DFA:  " << stage3->count_states() << endl;
    
    // Eliminate any unnecessary symbol sets
    dfa::ndfa* stage4 = stage3->to_ndfa_with_merged_symbols();
    delete stage3;
    stage3 = NULL;
    
    // Write some information about the DFA we just produced
    cons().verbose_stream() << L"    Number of symbols in the compacted DFA: " << stage4->symbols().count_sets() << endl;
    
    m_Dfa = stage4;
    
    // Build the final lexer
    m_Lexer = new lexer(*m_Dfa);
    
    // Write some parting words
    // (Well, this is really kibibytes but I can't take blibblebytes seriously as a unit of measurement)
    cons().verbose_stream() << L"    Approximate size of final lexer:        " << (m_Lexer->size() + 512) / 1024 << L" kilobytes" << endl;
}
