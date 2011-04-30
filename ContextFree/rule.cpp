//
//  rule.cpp
//  Parse
//
//  Created by Andrew Hunter on 30/04/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "rule.h"
#include "standard_items.h"

using namespace contextfree;

/// \brief Creates a copy of a rule
rule::rule(const rule& copyFrom)
: m_NonTerminal(copyFrom.m_NonTerminal)
, m_Items(copyFrom.m_Items) {
}

/// \brief Creates a copy of a rule with an alternative nonterminal
rule::rule(const rule& copyFrom, const item_container& nonTerminal)
: m_NonTerminal(nonTerminal)
, m_Items(copyFrom.m_Items) {
}

/// \brief Creates an empty rule, which reduces to the specified item
rule::rule(const item_container& nonTerminal)
: m_NonTerminal(nonTerminal) {
}

/// \brief Creates an empty rule with a nonterminal identifier
rule::rule(const int nonTerminal)
: m_NonTerminal(contextfree::nonterminal(nonTerminal)) {
}

/// \brief Copies the content of a rule into this one
rule& rule::operator=(const rule& copyFrom) {
    m_NonTerminal   = copyFrom.m_NonTerminal;
    m_Items         = copyFrom.m_Items;
}

/// \brief Orders this rule relative to another
bool rule::operator<(const rule& compareTo) const {
    // Number of items is the fastest thing to compare, so check that first
    if (m_Items.size() < compareTo.m_Items.size())  return true;
    if (m_Items.size() > compareTo.m_Items.size())  return false;
    
    // The nonterminal should be reasonably fast to compare
    if (m_NonTerminal < compareTo.m_NonTerminal)    return true;
    
    // Need to compare each item in turn
    item_list::const_iterator ourItem   = m_Items.begin();
    item_list::const_iterator theirItem = compareTo.begin();
    
    for (;ourItem != m_Items.end() && theirItem != compareTo.end(); ourItem++, theirItem++) {
        if (*ourItem < *theirItem) return true;
    }
    
    return false;
}

/// \brief Determines if this rule is the same as another
bool rule::operator==(const rule& compareTo) const {
    // Number of items is the fastest thing to compare, so check that first
    if (m_Items.size() != compareTo.m_Items.size()) return false;
    
    // The nonterminal should be reasonably fast to compare
    if (m_NonTerminal != compareTo.m_NonTerminal)   return false;
    
    // Need to compare each item in turn
    item_list::const_iterator ourItem   = m_Items.begin();
    item_list::const_iterator theirItem = compareTo.begin();
    
    for (;ourItem != m_Items.end() && theirItem != compareTo.end(); ourItem++, theirItem++) {
        if (*ourItem != *theirItem) return false;
    }
    
    return true;
}

/// \brief Appends the specified item to this rule
rule& rule::operator<<(const item_container& item) {
    // Add this item to the list of items
    m_Items.push_back(item);
}
