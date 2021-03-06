//
//  stringreader.h
//  TameParse
//
//  Created by Andrew Hunter on 19/09/2011.
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

#ifndef _TAMEPARSE_STRINGREADER_H
#define _TAMEPARSE_STRINGREADER_H

#include <string>

namespace util {
    ///
    /// \brief Class that supplies a string to another class via the '>>' operator
    ///
    /// Unlike stringstream, this class does not need to copy the string that it is supplied, so it is suitable for use
    /// with very large strings (such as might result from reading a source file entirely into memory).
    ///
    /// This does not implement the full functionality of C++'s iostream library, but rather supplies enough of it that it can 
    /// be used with a lexer class as an alternative.
    ///
    /// This class maintains a reference to the supplied string for as long as it is in existence, so it will become invalid
    /// if the string is ever freed up.
    ///
    template<typename char_type, typename traits = std::char_traits<char_type>, typename Alloc = std::allocator<char_type> >
    class basic_stringreader {
    public:
        /// \brief Reference to our own type
        typedef basic_stringreader<char_type, traits, Alloc> reader;
        
        /// \brief The type of string that this reader will reference
        typedef std::basic_string<char_type, traits, Alloc> string_type;

        /// \brief Offset type (required by gcc)
        typedef int off_type;

        /// \brief Integer type (required by gcc)
        typedef int int_type;

        /// \brief Position type (required by gcc)
        typedef size_t pos_type;
        
    private:
        /// \brief A reference to the source string
        const string_type& m_SourceString;
        
        /// \brief Position within the string
        size_t m_Pos;
        
    public:
        /// \brief Creates a new stringreader that references the supplied string
        basic_stringreader(const string_type& string)
        : m_SourceString(string)
        , m_Pos(0) {
        }
        
        /// \brief Returns the next character in the string (or 0 if all of the characters have been read)
        inline basic_stringreader& get(char_type& target) {
            if (m_Pos >= m_SourceString.size()) {
                // At the end of the string
                target = 0;
            } else {
                // Return the next character
                target = m_SourceString[m_Pos++];
            }
            
            return *this;
        }
        
        /// \brief Returns true if there are more characters to read from the string
        inline bool good() const {
            return m_Pos < m_SourceString.size();
        }
    };
    
    typedef basic_stringreader<char> stringreader;
    typedef basic_stringreader<wchar_t> wstringreader;
}

#endif
