//
//  lalr_machine.cpp
//  Parse
//
//  Created by Andrew Hunter on 01/05/2011.
//  
//  Copyright (c) 2011-2012 Andrew Hunter
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the \"Software\"), to 
//  deal in the Software without restriction, including without limitation the 
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
//  sell copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//  IN THE SOFTWARE.
//

#include <queue>

#include "TameParse/Lr/lalr_machine.h"

using namespace std;
using namespace contextfree;
using namespace lr;

static const empty_item an_empty_item;

/// \brief Creates an empty LALR machine, which will reference the specified gramamr
lalr_machine::lalr_machine(contextfree::grammar& gram)
: m_Grammar(&gram) {
}

/// \brief Adds a new state to this machine, or retrieves the identifier for the existing state
///
/// This returns the identifier for the state, and the state is updated so that its identifier 
/// reflects this. The state should not have any more lr1_items added to it, but it is permissable
/// to alter the lookahead set after calling this.
///
/// Typically, a state will be added with no lookahead set, and the lookahead will be added later.
/// (An algorithm that builds the LALR parser from an LR(1) set might do this differently, though)
///
int lalr_machine::add_state(container& newState) {
    // Just return the state ID if the new state already has one
    if (newState->identifier() >= 0) {
        return newState->identifier();
    }
    
    // Try to find the existing state
    state_to_identifier::iterator found = m_StateIds.find(newState);
    
    if (found != m_StateIds.end()) {
        // Set the identifier for the state that was passed in
        newState->set_identifier(found->second);
        
        // Return the result
        return found->second;
    }
    
    // The new ID is the last entry in the state table
    int newId = (int) m_States.size();
    
    // Set the new ID
    newState->set_identifier(newId);
    
    // Store this state
    m_StateIds[newState] = newId;
    m_States.push_back(newState);
    m_Transitions.push_back(transition_set());

    // Result is the new state ID
    return newId;
}

/// \brief Adds a transition to this state machine
///
/// Transitions involving terminals create shift actions in the final parser. Nonterminals and EBNF
/// items go into the goto table for the final parser. The empty item should be ignored. Guard items
/// are a little weird: they act like shift actions if they are matched.
void lalr_machine::add_transition(int stateId, const contextfree::item_container& item, int newStateId) {
    // Do nothing if the new state ID is invalid
    if (newStateId < 0 || newStateId >= (int) m_States.size()) return;
    
    // Set this transition
    m_Transitions[stateId].insert(transition(item, newStateId));
}

/// \brief Adds the specified set of lookahead items to the state with the supplied ID
bool lalr_machine::add_lookahead(int stateId, int itemId, const contextfree::item_set& newLookahead) {
    if (itemId < 0) return false;
    item_set& la = m_States[stateId]->lookahead_for(itemId);
    
    bool result = false;
    for (item_set::const_iterator lookaheadItem = newLookahead.begin(); lookaheadItem != newLookahead.end(); ++lookaheadItem) {
        // Ignore the empty item
        if ((*lookaheadItem)->type() == item::empty) continue;
        
        // Otherwise, carry on
        if (la.insert(*lookaheadItem)) result = true;
    }
    
    return result;
}
     
/// \brief Adds the specified set of lookahead items to the state with the supplied ID
bool lalr_machine::add_lookahead(int stateId, const lr0_item& item, const contextfree::item_set& newLookahead) {
    return add_lookahead(stateId, m_States[stateId]->find_identifier(item), newLookahead);
}
