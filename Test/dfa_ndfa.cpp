//
//  dfa_ndfa.cpp
//  Parse
//
//  Created by Andrew Hunter on 27/04/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "dfa_ndfa.h"

#include "Dfa/ndfa.h"

using namespace dfa;

void test_dfa_ndfa::run_tests() {
    // NDFA with two transitions on 'a'
    ndfa twoAs;
    twoAs >> 'a' >> accept_action(0);
    twoAs >> 'a' >> accept_action(1);
    
    // Turn into a DFA
    ndfa* twoAsDfa = twoAs.to_dfa();
    
    // Should have two states
    report("DFA-ndfa-reduced1", twoAsDfa->count_states() == 2);
    
    // Both accept actions should be in the second state
    report("DFA-ndfa-accept1", twoAsDfa->actions_for_state(1).size() == 2);
    
    delete twoAsDfa;
}