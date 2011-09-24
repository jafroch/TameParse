//
//  parser_stage.h
//  TameParse
//
//  Created by Andrew Hunter on 24/09/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _COMPILER_PARSER_STAGE_H
#define _COMPILER_PARSER_STAGE_H

#include "TameParse/Compiler/compilation_stage.h"
#include "TameParse/Language/language_parser.h"

namespace compiler {
    ///
    /// \brief Compiler stage that reads in and translates a file
    ///
    class parser_stage : public compilation_stage {
    private:
        /// \brief The language parser object
        language::language_parser m_Parser;
        
    public:
        /// \brief Creates a new compilation stage which will use the specified console object
        parser_stage(console_container& console, const std::wstring& filename);
        
        /// \brief Performs the actions associated with this compilation stage
        virtual void compile();
    };
}

#endif