//
//  main.cpp
//  Test
//
//  Created by Andrew Hunter on 13/03/2011.
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

#include <iostream>

#include "dfa_range.h"
#include "dfa_symbol_set.h"
#include "dfa_symbol_deduplicate.h"
#include "dfa_symbol_translator.h"
#include "dfa_ndfa.h"
#include "dfa_single_regex.h"
#include "contextfree_firstset.h"
#include "contextfree_followset.h"
#include "lr_weaksymbols.h"
#include "lr_lalr_general.h"
#include "language_bootstrap.h"
#include "language_primary.h"
#include "dfa_multi_regex.h"

using namespace std;

static int s_Run    = 0;
static int s_Failed = 0;

static void run(test_fixture& fixture) {
    fixture.run();
    
    s_Run += fixture.count_run();
    s_Failed += fixture.count_failed();
}

int main (int argc, const char * argv[])
{
    test_dfa_range              dfa_range;      run(dfa_range);
    test_dfa_symbol_set         symbol_set;     run(symbol_set);
    test_dfa_symbol_deduplicate dedupe;         run(dedupe);
    test_dfa_ndfa               ndfa;           run(ndfa);
    test_dfa_symbol_translator  trans;          run(trans);
    test_dfa_single_regex       singleregex;    run(singleregex);
    test_dfa_multi_regex        multiregex;     run(multiregex);
    
    test_contextfree_firstset   firstset;       run(firstset);
    test_contextfree_followset  followset;      run(followset);
    
    test_lr_weaksymbols         weakSymbols;    run(weakSymbols);
    test_lalr_general           lalr1;          run(lalr1);
    
    test_language_bootstrap     bootstrap;      run(bootstrap);
    test_language_primary       primary;        run(primary);
    
    int exitCode = 0;
    if (s_Failed > 0) {
        cerr << endl << s_Failed << "/" << s_Run << " tests failed" << endl;
        exitCode = s_Failed;
    } else {
        cerr << endl << s_Run << " tests successful." << endl;
    }
    
    return exitCode;
}

