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
 * The T_IDENTIFIER will be replaced by tokens in Preprocessor_C->defines->elements[i] with the index: "i" where the identifier is found Preprocessor_C->defines->elements_names[i]
 * the search will take place in reversed order, so if a define exists multiple times this would act like a override of this value.
 * TODO
 */
extern int preprocessor_c_parse_identifier(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * TODO
 */
extern int preprocessor_c_parse_define(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * TODO
 */
extern int preprocessor_c_parse_ifndef(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * TODO
 */
extern int preprocessor_c_parse_whitespace(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * TODO
 */
extern int preprocessor_c_parse_next(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken);

/**
 * TODO
 */
extern int preprocessor_c_parse(Preprocessor_C *preprocessor, const char* filepath);

/**
 * Runs the preprocessing process for the given Preprocessor_C.
 * 
 * TODO
 *
 * RETURN VALUE
 *	Return 0 on success. On error, -1 is returned, and Preprocessor_C->error is set to indicate the error.
 */
extern int preprocessor_c_run(Preprocessor_C *preprocessor);

#endif
