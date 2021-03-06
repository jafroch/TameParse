//
//  lr1_rewriter.h
//  TameParse
//
//  Created by Andrew Hunter on 13/01/2012.
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

#ifndef _LR_LR1_REWRITER_H
#define _LR_LR1_REWRITER_H

#include "TameParse/Lr/action_rewriter.h"

namespace lr {
    ///
    /// \brief Action rewriter that rewrites a LALR parser so that it is a LR(1) parser
    ///
    /// LALR parsers may produce reduce/reduce conflicts where a LR(1) parser would
    /// not. These conflicts can be resolved (provided the grammar is indeed LR(1)) as
    /// they will have the property that only one side will result in a reduction.
    /// Reduce/reduce conflicts of this type can be tricky to resolve as the grammar
    /// will appear not to contain any conflicts.
    ///
    /// This rewriter will detect reduce/reduce conflicts where only one side will
    /// eventually shift the symbol and replace one side with a weak reduction.
    /// The technique is to find all the places that the conflicting lookahead for
    /// both items can come from (this will be the symbol that is shifted after the
    /// the reduction takes place). Provided that the conflicted nonterminal only
    /// occurs in a single place in the rules reached by the reduction, we can
    /// replace one of the reductions with a weak reduce to resolve the conflict.
    ///
    /// Additionally, if the conflict becomes a shift/reduce we can also resolve
    /// it this way (this allows other rewriters to resolve the conflict, and also
    /// makes it easier to see why the grammar is conflicted)
    ///
    class lr1_rewriter : public action_rewriter {
    public:
        /// \brief Modifies the specified set of actions according to the rules in this rewriter
        ///
        /// This call should modify the contents of the supplied action set according to whatever rules it considers 
        /// suitable.
        /// 
        /// Typical rewriting actions might include removing actions so that ambiguous grammars can be accepted, replacing
        /// rules with alternatives (such as weak reduce actions for cases where a grammar can be parsed as LR(1)) or
        /// adding new rules.
        virtual void rewrite_actions(int state, lr_action_set& actions, const lalr_builder& builder) const;
        
        /// \brief Creates a clone of this rewriter
        virtual action_rewriter* clone() const;
    };
}

#endif
