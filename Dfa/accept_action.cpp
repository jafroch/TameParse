//
//  accept_action.cpp
//  Parse
//
//  Created by Andrew Hunter on 26/04/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "accept_action.h"

using namespace dfa;

/// \brief Creates a standard accept action for the specified symbol
accept_action::accept_action(int symbol)
: m_Eager(false)
, m_Symbol(symbol) { 
}

/// \brief Creates an accept action with the specified 'eagerness'
accept_action::accept_action(int symbol, bool isEager)
: m_Symbol(symbol)
, m_Eager(isEager) {
}

/// \brief For subclasses, allows the NDFA to clone this accept action for storage purposes
accept_action* accept_action::clone() const {
    return new accept_action(m_Symbol, m_Eager);
}

/// \brief Destructor
accept_action::~accept_action() { }
