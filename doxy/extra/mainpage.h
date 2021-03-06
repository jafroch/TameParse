//
//  MainPage.h
//  TameParse
//
//  Created by Andrew Hunter on 09/03/2012.
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

///
/// \mainpage
///
/// # Getting started
///
/// TameParse consists of a library and a command-line tool. The tool makes it
/// possible to convert TameParse grammars into working parsers for a variety
/// of languages, including C++. The library makes it possible to run these
/// parsers, and also provides a more thorough API making it possible to build
/// new parsers and lexers from scratch.
///
/// To manually test a grammar, run the tool with a start symbol and the `--test`
/// option:
///
///     tameparse --test --start-symbol='<Example>' example.tp
///
/// If the grammar compiles correctly, the tool will attempt to match the
/// language from the standard input.
///
/// To build a C++ parser, leave out the --test option and specify C++ as the
/// target language, along with an output filename:
///
///     tameparse --output-language=cplusplus --output-file=example --start-symbol='<Translation-Unit>' AnsiC.tp
///
/// This will generate C++ source for a parser for the requested language. This
/// parser depends on the tameparse library to operate.
///
/// ## Running the C++ parser
///
/// (The following API details are preliminary, I plan to improve them so that
/// less knowledge of the detail of how a parser is put together is required)
///
/// The generated files will contain two classes of interest: `ast_parser_type`,
/// representing the parser itself and `parser_actions`, representing the actions
/// necessary to actually build the AST. It also contains a definition of an
/// object, `ast_parser` which provides the parser itself (of type `ast_parser_type`)
///
/// To parse a language, three things are required. The first is the lexeme_streamobject, which
/// reads from a source stream and produces a series of lexemes. The second is the
/// parser actions object, generated by the tool and which supplies the output from
/// the lexer to the parser and also builds the output (an AST). The final object is
/// a parser state object, which performs the actual parser actions.
///
/// One way these objects can be set up is as follows:
///
///     dfa::lexeme_stream* stream = example::lexer.create_stream_from<wchar_t>(std::wcin);
///     example::parser_actions* actions = new example::parser_actions(stream);
///     example::ast_parser_type::state* state = example::ast_parser.create_parser(actions);
///
/// The actions that can be performed on a state are fully documented in the 
/// lr::parser::state class. To simply read everything from the input stream
/// and produce the corresponding AST, only the lr::parser::state::parse()
/// method is required:
///
///     bool success = state->parse();
///
/// If this returns false, there has been an error. It is possible to manually
/// update the state to recover from this, as well as to get information about
/// where the problem occurred.
///
/// When successful, the results of a parse are returned as an AST, whose
/// structure is the same as the grammar. The names of the classes depend on how
/// the grammar was constructed; they can be found defined in the header file.
/// In general a nonterminal called `<Example>` will produce a new class called
/// `example::Example_n` representing its syntax tree.
///
/// This node can be retrieved using the lr::parser::state::get_item() method
/// and a cast:
///
///     example::Example_n* ast = (example::Example_n*) state->get_item().item();
///
/// Individual fields represent the terminals and nonterminals matched by the
/// parser and are generally named to match their representation. For example,
/// this definition from the grammar:
///
///     <Example> = identifier
///
/// will have an `identifier` field containing the details about the token that
/// was matched.
