#include "token_list_c.h"

#include <sys/stat.h>

#ifndef JD297_CC_LEXER_C_H
#define JD297_CC_LEXER_C_H

/**
 * Lexer structure for the C language.
 *
 * DESCRIPTION
 *     This structure is used for parsing C language source code. It holds 
 *     information about the input file, the source code, and the tokens 
 *     generated during the lexing process.
 *
 * MEMBERS
 *     pathname   : A string representing the path to the input file.
 *
 *     sb         : A struct stat that holds statistics about the input file, 
 *                  such as its size (st_size).
 *
 *     buf        : A buffer containing the source code read from the input file.
 *
 *     pbuf       : A pointer to the current position in the source code buffer.
 *
 *     tokens     : A pointer to a TokenList_C structure that holds the tokens 
 *                  generated from the source code.
 *
 *     error      : A string that contains the latest error message encountered 
 *                  during the lexing process.
 */
typedef struct Lexer_C {
    const char *pathname;
    struct stat sb;

    char *buf;
    char *pbuf;

    TokenList_C *tokens;

    char *error;
} Lexer_C;

#define LEXER_CREATION_FAILED (void *) -1

/**
 * Creates a Lexer_C structure from the specified input file (pathname).
 *
 * DESCRIPTION
 *     This function allocates and initializes a new Lexer_C structure using 
 *     the provided pathname. It reads the contents of the input file into 
 *     a buffer and prepares the lexer for parsing the C language source code.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Lexer_C is returned. On error, 
 *     the value LEXER_CREATION_FAILED (i.e., (void *) -1) is returned, and 
 *     errno is set to indicate the specific error.
 */
extern void *lexer_c_create(const char *pathname);

#define LEXER_NEXT_SKIPPED (void *) 0
#define LEXER_NEXT_FAILED (void *) -1

/**
 * Processes the next T_MACRO_INCLUDE_LIBRARY_PATH from the Lexer_C.
 *
 * DESCRIPTION
 *     If the previous TOKEN_TYPE_C (excluding all whitespaces) is 
 *     T_MACRO_INCLUDE, this function searches for the '<' character in 
 *     lexer->pbuf. If found, it then searches for the '>' character. The 
 *     text between '<' and '>' is set as Token_C->value. If either 
 *     character is not found, the value LEXER_NEXT_SKIPPED (i.e., (void *) 0) 
 *     is returned, indicating that no action was taken and pbuf remains unchanged.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED, which indicates that the pattern was not 
 *     matched (pbuf will stay the same). On error, the value 
 *     LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and Lexer_C->error is set 
 *     to indicate the specific error.
 *
 * ERRORS
 *     If no '>' character is encountered, LEXER_NEXT_FAILED is returned, 
 *     and Lexer_C->error is set to 'missing terminating > character'.
 *
 *     If Token_C->value exceeds T_MACRO_INCLUDE_LIBRARY_PATH_MAX_LEN, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'filename too long'.
 */
extern void *lexer_c_next_macro_include_library_path(Lexer_C *lexer);

/**
 * Processes the T_IDENTIFIER from the Lexer_C.
 *
 * DESCRIPTION
 *     If the first character of Lexer_C->pbuf is an '_' or an alphabetic 
 *     character, and as long as the subsequent characters are either '_' 
 *     or alphanumeric, all valid characters are stored as the value of 
 *     the identifier. The pbuf is then advanced past the identifier.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED, which indicates that the pattern was not 
 *     matched (pbuf will stay the same). On error, the value 
 *     LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and 
 *     Lexer_C->error is set to indicate the specific error.
 *
 * ERRORS
 *     If Token_C->value exceeds T_IDENTIFIER_MAX_LEN, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'identifier too long'.
 */
extern void *lexer_c_next_identifier(Lexer_C *lexer);

/**
 * Processes the next T_COMMENT_LINE from the Lexer_C.
 *
 * DESCRIPTION
 *     If Lexer_C->pbuf matches the string "//", all characters are read 
 *     as a comment until a newline or EOF is encountered. The comment 
 *     is stored as the value of the Token_C.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED, which indicates that the pattern was not 
 *     matched (pbuf will stay the same). On error, the value 
 *     LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and 
 *     Lexer_C->error is set to indicate the specific error.
 *
 * ERRORS
 *     If Token_C->value exceeds T_COMMENT_LINE_MAX_LEN, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'comment too long'.
 */
extern void *lexer_c_next_comment_line(Lexer_C *lexer);

/**
 * Processes the next T_COMMENT_MULTILINE from the Lexer_C.
 *
 * DESCRIPTION
 *     If Lexer_C->pbuf matches the character '/' followed by a '*', all 
 *     characters are read as a multiline comment until a '*' followed by 
 *     a '/' is encountered. The comment is stored as the value of the 
 *     Token_C.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED (i.e., (void *) 0), which indicates that 
 *     the pattern was not matched (pbuf will stay the same). On error, 
 *     the value LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and 
 *     Lexer_C->error is set to indicate the specific error.
 *
 * ERRORS
 *     If Token_C->value exceeds T_COMMENT_LINE_MAX_LEN, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'multiline comment too long'.
 *
 *     If no terminating '*' followed by a '/' is found, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'unterminated comment'.
 */
extern void *lexer_c_next_comment_multiline(Lexer_C *lexer);

/**
 * Processes the next T_STRING from the Lexer_C.
 *
 * DESCRIPTION
 *     If Lexer_C->pbuf matches the character '"', all characters are read 
 *     as a string until another '"' is encountered (unless it is escaped 
 *     with '\'). The string is stored as the value of the Token_C.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED (i.e., (void *) 0), which indicates that 
 *     the pattern was not matched (pbuf will stay the same). On error, 
 *     the value LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and 
 *     Lexer_C->error is set to indicate the specific error.
 *
 * ERRORS
 *     If Token_C->value exceeds T_STRING_MAX_LEN, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'string too long'.
 *
 *     If no terminating '"' is found, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'missing terminating " character'.
 */
extern void *lexer_c_next_string(Lexer_C *lexer);

/**
 * Processes the next T_TYPE_CAST from the Lexer_C.
 *
 * DESCRIPTION
 *     TODO: Not implemented will just return LEXER_NEXT_SKIPPED (i.e., (void *) 0)!
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED (i.e., (void *) 0), which indicates that 
 *     the pattern was not matched (pbuf will stay the same). On error, 
 *     the value LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and 
 *     Lexer_C->error is set to indicate the specific error.
 *
 * ERRORS
 *     TODO: Not implemented errors will not be returned!
 */
extern void *lexer_c_next_type_cast(Lexer_C *lexer);

/**
 * Processes the next T_NUMBER from the Lexer_C.
 *
 * DESCRIPTION
 *     If Lexer_C->pbuf is a digit, all subsequent characters are read as 
 *     a number as long as they are digits or a '.' (decimal point). The 
 *     number is stored as the value of the Token_C.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED (i.e., (void *) 0), which indicates that 
 *     the pattern was not matched (pbuf will stay the same). On error, 
 *     the value LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and 
 *     Lexer_C->error is set to indicate the specific error.
 *
 * ERRORS
 *     If more than one '.' is encountered, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'too many decimal points in number'.
 *
 *     If Token_C->value exceeds T_NUMBER_MAX_LEN, 
 *     LEXER_NEXT_FAILED is returned, and Lexer_C->error is set to 
 *     'number too long'.
 */
extern void *lexer_c_next_number(Lexer_C *lexer);

/**
 * Processes a token that can be represented from the Lexer_C.
 *
 * DESCRIPTION
 *     This function analyzes the current position in the Lexer_C and 
 *     attempts to identify and create a corresponding Token_C. It 
 *     handles various token types that can be easily represented, 
 *     such as T_PLUS, which corresponds to the "+" symbol. 
 *     If the token is one of TOKEN_TYPE_C_ALNUM_REPRESENTATION, 
 *     and the found character is followed by an alphanumeric character 
 *     or an underscore ('_'), the function returns LEXER_NEXT_SKIPPED, 
 *     indicating that the pattern was not matched, and pbuf will remain unchanged.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned, or the 
 *     value LEXER_NEXT_SKIPPED (i.e., (void *) 0), which indicates that 
 *     the pattern was not matched (pbuf will stay the same).
 *
 * SEE ALSO
 *     include/lexer_c.h: lexer_c_run
 *     include/lexer_c.h: lexer_c_next
 *     include/token_type_c_is_in_expected_token_types
 */
extern void *lexer_c_next_with_representation(Lexer_C *lexer, TokenType_C type);

/**
 * Retrieves the next token from a Lexer_C.
 *
 * DESCRIPTION
 *     The next token is processed using the current position indicated by 
 *     Lexer_C->pbuf. Upon successful retrieval of the token, 
 *     Lexer_C->pbuf is incremented to point to the next position in the 
 *     source code buffer.
 *
 * RETURN VALUE
 *     On success, a pointer to the next Token_C is returned. On error, 
 *     the value LEXER_NEXT_FAILED (i.e., (void *) -1) is returned, and 
 *     Lexer_C->error is set to indicate the specific error.
 *
 * NOTES
 *     If Token_C->type is T_EOF, it indicates that the input file has been 
 *     lexed successfully. In this case, when this function is called by 
 *     lexer_c_run, the method should return 0.
 *     If an error occurs and this function is called by lexer_c_run(Lexer_C *lexer), 
 *     the method should return -1.
 *
 * SEE ALSO
 *     include/lexer_c.h: lexer_c_run(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_macro_include_library_path(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_identifier(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_comment_line(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_comment_multiline(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_string(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_number(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_type_cast(Lexer_C *lexer)
 *     include/lexer_c.h: lexer_c_next_with_representation(Lexer_C *lexer)
 */
extern void *lexer_c_next(Lexer_C *lexer);

/**
 * Runs the lexing process for the given Lexer_C.
 *
 * DESCRIPTION
 *     This function initiates the lexing process for the specified 
 *     Lexer_C structure. It processes the input file, generating tokens 
 *     and storing them in the associated TokenList_C. The function will 
 *     continue until the end of the file is reached or an error occurs.
 *
 * RETURN VALUE
 *     Returns 0 on success. On error, -1 is returned, and 
 *     Lexer_C->error is set to indicate the specific error encountered 
 *     during the lexing process.
 *
 * SEE ALSO
 *     include/lexer_c.h: lexer_c_next(Lexer_C *lexer)
 *     include/token_list_c.h: token_list_c_push_back(TokenList_C *list, Token_C *element)
 */
extern int lexer_c_run(Lexer_C *lexer);

#endif

