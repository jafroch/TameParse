//
//  version.cpp
//  TameParse
//
//  Created by Andrew Hunter on 24/09/2011.
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

#include "TameParse/version.h"

using namespace std;
using namespace tameparse;

/// \brief The major version number of the library
const int version::major_version = 0;

/// \brief The minor version of the library
const int version::minor_version = 1;

/// \brief The revision of the library
const int version::revision = 0;

/// \brief A string representing the version of this library
const std::string version::version_string("0.1.0 (" __DATE__ ")");

/// \brief A string describing the copyright for this version of the library
const std::string version::copyright_string("Written by Andrew Hunter");

/// \brief A string describing how to contact the author
const std::string version::contact_string("<andrew@logicalshift.co.uk>");

/// \brief A string containing the date that this library was compiled
const std::string compile_date(__DATE__);

/// \brief A string containing the release date for this library
const std::string release_date("Feb 24 2012"); // TODO: update somehow

/// \brief A string describing the license for this version of the library
const std::string version::license_string(
"Copyright (c) 2011-2012 Andrew Hunter\n"
"\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy \n"
"of this software and associated documentation files (the \"Software\"), to \n"
"deal in the Software without restriction, including without limitation the \n"
"rights to use, copy, modify, merge, publish, distribute, sublicense, and/or \n"
"sell copies of the Software, and to permit persons to whom the Software is \n"
"furnished to do so, subject to the following conditions:\n"
"\n"
"The above copyright notice and this permission notice shall be included in all\n"
"copies or substantial portions of the Software.\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \n"
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \n"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE \n"
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \n"
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING \n"
"FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS \n"
"IN THE SOFTWARE."
);

/// \brief A string describing the warranty for this version of the library
const std::string version::warranty_string(
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \n"
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \n"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE \n"
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \n"
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING \n"
"FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS \n"
"IN THE SOFTWARE."
);
