//
//  lexer_data.cpp
//  TameParse
//
//  Created by Andrew Hunter on 12/11/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "TameParse/Compiler/Data/lexer_data.h"

using namespace compiler;

/// \brief Adds a new lexer definition to this object
void lexer_data::add_definition(const std::wstring& term, const lexer_item& newItem) {
	m_Definitions[term].push_back(newItem);
}

/// \brief Removes all of th definitions for a terminal with a particular name from this item
void lexer_data::remove_definition(const std::wstring& term) {
	m_Definitions.erase(term);
}

/// \brief Returns all of the definitions for a particular terminal value
const lexer_data::item_list& lexer_data::get_definitions(const std::wstring& term) const {
	return m_Definitions[term];
}

/// \brief The first item defined in this data object
lexer_data::iterator lexer_data::begin() const {
	return m_Definitions.begin();
}

/// \brief The item after the final item defined in this data object
lexer_data::iterator lexer_data::end() const {
	return m_Definitions.end();
}