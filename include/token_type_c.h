#include <stdarg.h>
#include <stddef.h>

#ifndef JD297_CC_TOKEN_TYPE_C_H
#define JD297_CC_TOKEN_TYPE_C_H

typedef enum TokenType_C {
    /* TOKENS */
    T_STRING,
    T_NUMBER,

    /* PREPROCESSOR KEYWORD */
    T_MACRO_INCLUDE_LIBRARY_PATH,

    /* COMMENTS */
    T_COMMENT_LINE,
    T_COMMENT_MULTILINE,

    /* CAST */
    T_TYPE_CAST,

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

    T_IDENTIFIER,

    T_EOF
} TokenType_C;

#define TOKEN_TYPE_C_WITH_NO_REPRESENTATION 7, T_IDENTIFIER, T_STRING, T_TYPE_CAST, T_NUMBER, T_MACRO_INCLUDE_LIBRARY_PATH, T_COMMENT_LINE, T_COMMENT_MULTILINE
#define TOKEN_TYPE_C_ALL_WHITESPACES 7, T_WHITESPACE_TAB, T_WHITESPACE_LINE_FEED, T_WHITESPACE_LINE_TABULATION, T_WHITESPACE_FORM_FEED, T_WHITESPACE_CARRIAGE_RETURN, T_WHITESPACE_SPACE, T_WHITESPACE_NEXT_LINE

/**
 * Returns the string representation of a TokenType_C.
 *
 * DESCRIPTION
 *     This function takes a TokenType_C value and returns its corresponding 
 *     string representation. For example, the TokenType_C value T_PLUS 
 *     would return the string "+".
 *
 * RETURN VALUE
 *     The string representation of the specified token type is returned.
 */
extern const char *token_type_c_to_representation(TokenType_C type);

/**
 * Checks if a TokenType_C is present in a variadic list of TokenType_C values.
 *
 * DESCRIPTION
 *     This function checks whether the specified TokenType_C is included 
 *     in a list of token types provided as a variadic argument. It iterates 
 *     through the list and returns a result based on the presence of the 
 *     specified type.
 *
 * RETURN VALUE
 *     If the specified TokenType_C is present in the list, 1 is returned; 
 *     otherwise, 0 is returned.
 */
extern int token_type_c_is_in_expected_token_types(TokenType_C type, size_t num_types, /* TokenType_C types */...);


#endif

