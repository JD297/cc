#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef JD297_CC_LEXER_C_H
#define JD297_CC_LEXER_C_H

typedef enum TokenType_C {
	/* TOKENS */
	T_IDENTIFIER,
	T_STRING,
	T_NUMBER,

	/* SYMBOLS */
	T_OPEN_BRACKET,
	T_CLOSING_BRACKET,
	T_OPEN_PARENT,
	T_CLOSING_PARENT,
	T_OPEN_BRACE,
	T_CLOSING_BRACE,
	T_DOT,
	T_ARROW,
	T_COMMA,
	T_COLON,
	T_SEMICOLON,
	T_ASTERISK,
	T_ASSIGNMENT,
	T_TILDE,

	/* OPERATORS */
	/* ARITHMETRIC */
	T_PLUS,
	T_MINUS,
	T_MULTIPLY,
	T_DIVIDE,
	T_MODULUS,
	T_UNARY_PLUS,
	T_UNARY_MINUS,
	T_INCREMENT,
	T_DECREMENT,

	/* RELATIONAL */
	T_LESS_THAN,
	T_GREATER_THAN,
	T_LESS_THAN_OR_EQUAL_TO,
	T_GREATER_THAN_OR_EQUAL_TO,
	T_EQUAL_TO,
	T_NOT_EQUAL_TO,

	/* LOGICAL */
	T_LOGICAL_AND,
	T_LOGICAL_OR,
	T_LOGICAL_NOT,

	/* BITWISE */
	T_BITWISE_AND,
	T_BITWISE_OR,
	T_BITWISE_XOR,
	T_BITWISE_FIRST_COMPLEMENT,
	T_BITWISE_LEFTSHIFT,
	T_BITWISE_RIGHTSHIFT,

	/* ASSIGNMENT */
	// T_ASSIGNMENT
	T_PLUS_ASSIGN,
	T_MINUS_ASSIGN,
	T_MULTIPLY_ASSIGN,
	T_DIVIDE_ASSIGN,
	T_MODULUS_ASSIGN,
	T_BITWISE_AND_ASSIGN,
	T_BITWISE_OR_ASSIGN,
	T_BITWISE_XOR_ASSIGN,
	T_BITWISE_RIGHTSHIFT_ASSIGN,
	T_BITWISE_LEFTSHIFT_ASSIGN,

	/* CONDITIONAL */
	T_TERNARY,

	/* CAST */
	T_TYPE_CAST,

	T_ADDRESSOF,
	T_DEREFERENCE,

	/* NORMAL KEYWORDS */
	T_ALIGNAS,
	T_ALIGNOF,
	T_AUTO,
	T_BOOL,
	T_BREAK,
	T_CASE,
	T_CHAR,
	T_CONST,
	T_CONSTEXPR,
	T_CONTINUE,
	T_DEFAULT,
	T_DO,
	T_DOUBLE,
	T_ELSE,
	T_ENUM,
	T_EXTERN,
	T_FALSE,
	T_FLOAT,
	T_FOR,
	T_GOTO,
	T_IF,
	T_INLINE,
	T_INT,
	T_LONG,
	T_NULLPTR,
	T_REGISTER,
	T_RESTRICT,
	T_RETURN,
	T_SHORT,
	T_SIGNED,
	T_SIZEOF,
	T_STATIC,
	T_STATIC_ASSERT,
	T_STRUCT,
	T_SWITCH,
	T_THREAD_LOCAL,
	T_TRUE,
	T_TYPEDEF,
	T_TYPEOF,
	T_TYPEOF_UNQUAL,
	T_UNION,
	T_UNSIGNED,
	T_VOID,
	T_VOLATILE,
	T_WHILE,

	/* PREPROCESSOR KEYWORDS*/
	T_MACRO_IF,
	T_MACRO_ELIF,
	T_MACRO_ELSE,
	T_MACRO_ENDIF,
	T_MACRO_IFDEF,
	T_MACRO_IFNDEF,
	T_MACRO_ELIFDEF,
	T_MACRO_ELIFNDEF,
	T_MACRO_DEFINE,
	T_MACRO_UNDEF,
	T_MACRO_INCLUDE,
	T_MACRO_EMBED,
	T_MACRO_LINE,
	T_MACRO_ERROR,
	T_MACRO_WARNING,
	T_MACRO_PRAGMA,
	T_MACRO_DEFINDED,
	T_MACRO___HAS_INCLUDE,
	T_MACRO___HAS_EMBED,
	T_MACRO___HAS_C_ATTRIBUTE,
	T_MACRO_ASM,
	T_MACRO_FORTRAN,

	/* WHITESPACE */
	T_WHITESPACE_TAB,
	T_WHITESPACE_LINE_FEED,
	T_WHITESPACE_LINE_TABULATION,
	T_WHITESPACE_FORM_FEED,
	T_WHITESPACE_CARRIAGE_RETURN,
	T_WHITESPACE_SPACE,
	T_WHITESPACE_NEXT_LINE,

	T_EOF
} TokenType_C;

/**
 * Token structure for c language
 *
 * DESCRIPTION
 *	This structure is used for tokens of c language only.
 *
 *   MEMBERS
 *	type      : is set to TokenType_C enum
 *	value     : is a pointer to the beginning of the token value
 *	value_len : is the length of the token value
 */
typedef struct Token_C {
	TokenType_C type;

	char* value;
	size_t value_len;
} Token_C;

#define TOKEN_CREATION_FAILED (void *) -1

/**
 * Create a Token_C structure from type and value.
 *
 * RETURN VALUE
 *	On success returns a pointer to the created Token_C. On error, the value TOKEN_CREATION_FAILED (that is, (void *) -1) is returned, and errno is set to indicate the error.
 */
extern void *lexer_c_create_token(TokenType_C type, const char* value, size_t value_len);

/**
 * Returns the string representation of a TokenType_C.
 *
 * RETURN VALUE
 *	The token type is returned as a string token symbol. For example, the TokenType_C T_PLUS would return the value "+".
 */
extern char *lexer_c_token_type_representation(TokenType_C type);

/**
 * Checks if a TokenType_C is present in a variadic list of TokenType_C's.
 *
 * RETURN VALUE
 *	If the TokenType_C is present, 1 is returned; 0 otherwise.
 */
extern int lexer_c_token_type_is_in_expected_token_types(TokenType_C type, size_t num_types, /* TokenType_C types */...);

/**
 * Lexer structure for the c language
 *
 * DESCRIPTION
 *	This structure is used for parsing c language only.
 *
 *   MEMBERS
 *	pathname  : is set to the input file
 *	sb        : holds stats about input file like st_size
 *	buf       : contains the source code the input file
 *	pbuf      : is a pointer to the current position
 *	tokens    : is a list of pointers to Token_C's
 *	token_len : is set to the current size of possible elements in tokens
 *	tokens_num: is set to the current number of elements in tokens
 *	error     : is set to the latest error message in the lexing process
 */
typedef struct Lexer_C {
	char* pathname;
	struct stat sb;

	char* buf;
	char* pbuf;

	Token_C **tokens;
	size_t tokens_len;
	size_t tokens_num;

	char* error;
} Lexer_C;

#define LEXER_CREATION_FAILED (void *) -1
#define LEXER_CREATION_TOKENS_LEN sysconf(_SC_PAGESIZE) / sizeof(Token_C *)

/**
 * Create a Lexer_C structure from an input file (pathname).
 *
 * RETURN VALUE
 * 	On success returns a pointer to the created Lexer_C. On error, the value LEXER_CREATION_FAILED (that is, (void *) -1) is returned, and errno is set to indicate the error.
 */
extern void *lexer_c_create_lexer(char* pathname);

/**
 * Appends a Token_C to the list of tokens in the given Lexer_C.
 *
 * DESCRIPTION
 *	Lexer_C->tokens will be resized if the Lexer_C->tokens_num reached Lexer_C->tokens_len.
 *
 * RETURN VALUE
 *	On success 0 is returned. On error, -1 is returned, and errno is set to indicate the error.
 */
extern int lexer_c_push_back_token(Lexer_C *lexer, Token_C *token);

#define LEXER_NEXT_FAILED (void *) -1

/**
 * Retrieve the next token from a Lexer_C
 *
 * DESCRIPTION
 *	The next token will be processed via Lexer_C->pbuf. After success Lexer_C_>pbuf will be incremented to the next position.
 *
 * RETURN VALUE
 *	On succes returns a pointer to the next Token_C. On error, the value LEXER_NEXT_FAILED (that is, (void *) -1) is returned, and Lexer_C->error is set to indicate the error.
 *
 * Notes
 * 	When Token_C->type is T_EOF than the input file was lexed successfully. When this function is called by lexer_c_run(Lexer_C *lexer) the method should return 0 in this case.
 *	When an error occures and this function is called by lexer_c_run(Lexer_C *lexer) the method should return -1 in this case.
 */
extern void *lexer_c_next(Lexer_C *lexer);

/**
 * Runs the lexing process for the given Lexer_C
 *
 * RETURN VALUE
 *	Return 0 on success. On error, -1 is returned, and Lexer_C->error is set to indicate the error.
 */
extern int lexer_c_run(Lexer_C *lexer);

#endif

