#include "lexer_c.h"
#include "lexer_list_c.h"
#include "str_list.h"
#include "token_list_c.h"
#include "token_list_named_c.h"

#ifndef JD297_CC_PREPROCESSOR_C_H
#define JD297_CC_PREPROCESSOR_C_H

/**
 * Preprocessor structure for the C language.
 *
 * MEMBERS
 *     include_dirs : A list of directories to search for include files. 
 *                    The first element is attempted first.
 *
 *     source_files : A list of input files that will be preprocessed.
 *
 *     defines      : A list of Token_C pointers that reference tokens 
 *                    after a T_MACRO_DEFINE + T_IDENTIFIER, the identifier is stored as name in list.
 *
 *     lexers       : A list of Lexer_C pointers that reference all 
 *                    lexers that have been processed.
 *
 *     output       : A list of Token_C pointers containing the final 
 *                    result of the preprocessor_c_run function.
 *
 *     error        : A string that holds the latest error message 
 *                    encountered during the preprocessing process.
 *
 * SEE ALSO
 *     include/str_list.h
 *     include/preprocessor_c.h: preprocessor_c_run(Preprocessor_C *preprocessor)
 *     include/lexer_list_c.h
 *     include/lexer_c.h
 *     include/token_list_c.h
 *     include/token_c.h
 */
typedef struct Preprocessor_C {
    StrList *include_dirs; 
    StrList *source_files;

    LexerList_C *lexers;

	TokenListNamed_C *defines;
	TokenList_C *output;

	char* error;
} Preprocessor_C;

#define PREPROCESSOR_CREATION_FAILED (void *) -1

/**
 * Creates a Preprocessor_C from specified include directories, source files, 
 * and definitions.
 *
 * DESCRIPTION
 *     This function initializes a Preprocessor_C structure using a list of 
 *     include directories, source files, and definitions. The definitions 
 *     typically include default settings for the compiler, such as the 
 *     compiler version number, supported C standards (e.g., ISO), and 
 *     command-line arguments provided by the user that contain definitions 
 *     to configure the source code.
 *
 * RETURN VALUE
 *     On success, returns a pointer to the newly created Preprocessor_C. 
 *     On error, returns PREPROCESSOR_CREATION_FAILED (i.e., (void *) -1), 
 *     and sets errno to indicate the specific error encountered.
 */
extern void *preprocessor_c_create(StrList *include_dirs, StrList *source_files, TokenListNamed_C *defines);

#define PREPROCESSOR_INCLUDE_MODE_LIBRARARY 0x0
#define PREPROCESSOR_INCLUDE_MODE_STRING    0x1

/**
 * Finds the path of an include file based on the specified pathname.
 *
 * DESCRIPTION
 *     This function searches for an include file using the specified 
 *     pathname within the directories listed in Preprocessor_C->include_dirs. 
 *     It checks each directory in the list to locate the file.
 *
 *     The mode flags are as follows:
 *
 *     PREPROCESSOR_INCLUDE_MODE_LIBRARY
 *          The given pathname will only be searched in the directories 
 *          specified in Preprocessor_C->include_dirs.
 *
 *     PREPROCESSOR_INCLUDE_MODE_STRING
 *          The given pathname will first be searched using a relative 
 *          path (i.e., starting from the current directory ".").
 *
 * RETURN VALUE
 *     Returns a pointer to the path of the include file if found, or 
 *     NULL if the file could not be located in any of the directories 
 *     specified in Preprocessor_C->include_dirs.
 */
extern char *preprocessor_c_find_include_file(Preprocessor_C *preprocessor, const char *pathname, int mode);

/**
 * Parses and includes a file based on a T_MACRO_INCLUDE directive.
 *
 * DESCRIPTION
 *     When a T_MACRO_INCLUDE is encountered, this function expects either 
 *     a T_STRING or a T_MACRO_INCLUDE_LIBRARY_PATH. If the input does not 
 *     match this condition, Preprocessor_C->error will be set to 
 *     "#include expects 'FILENAME' or <FILENAME>". The function 
 *     preprocessor_c_find_include_file will be called once all conditions 
 *     are met. If this function returns NULL, Preprocessor_C->error will 
 *     be set to "{Token_C->value}: {strerror(errno)}". After locating the 
 *     file, the include process begins by creating a new Lexer_C using 
 *     lexer_c_create with the located include file. This new Lexer_C will 
 *     then be added to Preprocessor_C->lexers. The created Lexer_C will be 
 *     executed by calling lexer_c_run, and the finished Lexer_C will be 
 *     parsed by calling preprocessor_c_parse with the current preprocessor 
 *     and the lexer.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error encountered.
 *
 * SEE ALSO
 *     include/preprocessor_c.h: preprocessor_c_find_include_file(Preprocessor_C *preprocessor, char *pathname)
 *     include/lexer_c.h: lexer_c_create(char *pathname)
 *     include/lexer_list_c.h: lexer_list_c_push_back(LexerList_C *list, Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_run(Lexer_C *lexer)
 *     include/preprocessor_c.h: preprocessor_c_parse(Preprocessor_C *preprocessor, Lexer_C *lexer)
 */
extern int preprocessor_c_parse_include(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * Replaces a token T_IDENTIFIER if it is in Preprocessor_C->defines.
 *
 * DESCRIPTION
 *     This function searches for a token of type T_IDENTIFIER within the 
 *     Preprocessor_C->defines list. If a matching definition is found, 
 *     the T_IDENTIFIER is replaced with the corresponding define tokens, 
 *     which are then stored in the output. If no matching definition is 
 *     found, the original identifier is retained and stored in the output 
 *     as is.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error 
 *     encountered.
 */
extern int preprocessor_c_parse_identifier(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * Processes a token of type T_MACRO_DEFINE.
 *
 * DESCRIPTION
 *     This function processes a token of type T_MACRO_DEFINE by first 
 *     skipping all whitespace tokens that follow it. If the identifier 
 *     of the token is not of type T_IDENTIFIER, -1 is returned, and 
 *     Preprocessor_C->error is set to "macro names must be identifiers". 
 *     The value (name) of the T_IDENTIFIER token is then temporarily 
 *     stored. A TokenList_C is allocated to hold subsequent tokens. 
 *     These tokens are stored until either a T_WHITESPACE_LINE_FEED or 
 *     T_EOF token is encountered. Only a single T_WHITESPACE_SPACE is 
 *     added between the found tokens if a T_WHITESPACE_X token is 
 *     encountered. The resulting TokenList_C is then added to the 
 *     TokenListNamed_C preprocessor->defines with the name of the 
 *     originally discovered identifier.
 *
 * NOTES
 *     An empty TokenList is also valid, which can be useful for checking 
 *     if a name is defined, as is the case with the macros T_MACRO_IFDEF 
 *     and T_MACRO_IFNDEF.
 *
 * TODO
 *     The function currently does not handle the case where a 
 *     T_WHITESPACE_LINE_FEED can be escaped with '\' for multi-line 
 *     defines.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error 
 *     encountered.
 */
extern int preprocessor_c_parse_define(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * Parses the T_MACRO_IFNDEF token and processes conditional compilation.
 *
 * DESCRIPTION
 *     This function skips all T_WHITESPACE_SPACE and T_WHITESPACE_TAB tokens 
 *     until a non-whitespace token is found. It then checks if the next token 
 *     is of type T_IDENTIFIER. If not, it sets 
 *     Preprocessor_C->error to "macro names must be identifiers" and returns -1. 
 *     The value of the found T_IDENTIFIER token is stored for later use. 
 *     The function then searches for the next T_MACRO_ENDIF token. If the 
 *     depth is 0, it checks if the identifier's value exists in 
 *     Preprocessor_C->defines. If other T_MACRO tokens that also use 
 *     T_MACRO_IFNDEF are encountered, the depth is incremented. 
 *     If the depth is greater than 0 and no T_MACRO_ENDIF is found, 
 *     Preprocessor_C->error is set to "unterminated #ifndef" and -1 is returned.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error 
 *     encountered.
 *
 * SEE ALSO
 *     include/preprocessor_c.h: preprocessor_c_parse_define
 */
extern int preprocessor_c_parse_ifndef(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);


/**
 * Skips all whitespace tokens and processes line breaks.
 *
 * DESCRIPTION
 *     This function skips all whitespace tokens in the TokenList_C, 
 *     ensuring that the pointer ptoken does not point to any whitespace 
 *     tokens. It then checks if the last added element is a line break; 
 *     if so, the function exits. Otherwise, it traverses backward through 
 *     the current file until it encounters either a non-whitespace token 
 *     or the beginning of the list. If a line break is found during this 
 *     traversal, the function exits and adds a line break to the output. 
 *     If no line break is found, a space is added instead.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error 
 *     encountered.
 */
extern int preprocessor_c_parse_whitespace(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * Calls the appropriate subroutines for the current token pointed to by ptoken.
 *
 * DESCRIPTION
 *     This function processes the token currently pointed to by ptoken. 
 *     By default, ptoken is added to the Preprocessor_C->output list, 
 *     and the cursor is incremented by 1. If ptoken is of type T_EOF, 
 *     a no-operation (NOP) is performed, and 0 is returned. Tokens that 
 *     are used for optimization are skipped (the cursor is incremented by 
 *     1, and 0 is returned). All types of comments are also skipped. 
 *     If ptoken is of type T_MACRO_ENDIF, it is similarly skipped. 
 *     Unimplemented tokens are handled as errors by calling assert(3), 
 *     and the message "TokenType_C (ptoken->type) not implemented!" 
 *     should be displayed. The program will terminate, and the function 
 *     will not return. T_IDENTIFIER and T_MACRO_X tokens are processed 
 *     separately in their own subroutines.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error 
 *     encountered.
 *
 * SEE ALSO
 *     include/preprocessor_c.h: preprocessor_c_parse_include
 *     include/preprocessor_c.h: preprocessor_c_parse_identifier
 *     include/preprocessor_c.h: preprocessor_c_parse_define
 *     include/preprocessor_c.h: preprocessor_c_parse_ifndef
 *     include/preprocessor_c.h: preprocessor_c_parse_whitespace
 */
extern int preprocessor_c_parse_next(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * Parses a single source file given by filepath, appending the result to 
 * Preprocessor_C->output list.
 *
 * DESCRIPTION
 *     This function creates a Lexer_C using the provided filepath and 
 *     adds it to the Preprocessor_C->lexers list. It then calls 
 *     lexer_c_run with the created lexer to process the file. The resulting 
 *     tokens are processed using the function preprocessor_c_parse_next 
 *     until a token of type T_EOF is encountered. The parsed tokens are 
 *     appended to the Preprocessor_C->output list.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error 
 *     encountered.
 *
 * NOTES
 *     This function is called by preprocessor_c_run.
 *
 * SEE ALSO
 *     include/preprocessor_c.h: preprocessor_c_run(Preprocessor_C *preprocessor)
 *     include/preprocessor_c.h: preprocessor_c_parse_next(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken)
 */
extern int preprocessor_c_parse(Preprocessor_C *preprocessor, const char* filepath);

/**
 * Runs the preprocessing process for the given Preprocessor_C.
 *
 * DESCRIPTION
 *     This function iterates over all source files listed in 
 *     Preprocessor_C->source_files and processes each file by calling 
 *     preprocessor_c_parse for parsing. The preprocessing steps are 
 *     executed in the order the source files are provided.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Preprocessor_C->error is set to indicate the specific error 
 *     encountered.
 *
 * SEE ALSO
 *     include/preprocessor_c.h: preprocessor_c_parse(Preprocessor_C *preprocessor, const char* filepath)
 */
extern int preprocessor_c_run(Preprocessor_C *preprocessor);

#endif
