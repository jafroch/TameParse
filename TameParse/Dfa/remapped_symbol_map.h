//
//  remapped_symbol_set.h
//  Parse
//
//  Created by Andrew Hunter on 26/04/2011.
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

#ifndef _DFA_REMAPPED_SYMBOL_MAP_H
#define _DFA_REMAPPED_SYMBOL_MAP_H

#include <map>
#include <set>

#include "TameParse/Dfa/symbol_map.h"

namespace dfa {
    ///
    /// \brief Class that represents a symbol map that has been remapped from another symbol map
    ///
    class remapped_symbol_map : public symbol_map {
    public:
        /// \brief Represents the set of new symbol sets that an old symbol set was remapped to
        typedef std::set<int> new_symbol_set;

    private:
        /// \brief Constant value indicating 'no symbols'
        static const new_symbol_set s_NoSymbols;
        
        /// \brief Maps IDs in the old symbol set to IDs in this map
        typedef std::map<int, new_symbol_set> old_to_new_map;
        
        /// \brief Maps IDs in the old set to IDs in this map
        old_to_new_map m_OldToNew;
        
        /// \brief Disabled assignment
        remapped_symbol_map& operator=(const remapped_symbol_map& assignFrom);
        
    public:
        /// \brief Generates an empty remapped symbol map
        remapped_symbol_map();
        
        /// \brief Copy constructor
        remapped_symbol_map(const remapped_symbol_map& copyFrom);
        
        /// \brief Finds or adds an identifier for the specified symbol, adding the specified set of new symbols as the new symbols for this set
        int identifier_for_symbols(const symbol_set& symbols, const new_symbol_set& oldSymbols);
        
        /// \brief Given a symbol set ID in the old set, returns the symbols in this set that it maps to
        inline const new_symbol_set& new_symbols(int oldSetId) const { 
            old_to_new_map::const_iterator found = m_OldToNew.find(oldSetId);
            if (found != m_OldToNew.end()) return found->second;
            return s_NoSymbols;
        }
        
        /// \brief Factory method that generates a remapped symbol map by removing duplicates
        ///
        /// This finds all the symbol sets that overlap in the original, and splits them up so that any given symbol is only in one set.
        /// It sets up the remapping so it is possible to find the new set IDs for any symbol set in the original.
        static remapped_symbol_map* deduplicate(const symbol_map& source);
    };
}

#endif
