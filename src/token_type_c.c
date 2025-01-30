#include "token_type_c.h"

#include <regex.h>
#include <stddef.h>
#include <stdlib.h>

const char *token_type_c_pattern(const TokenType_C type)
{
    switch (type) {
        case T_IDENTIFIER: return "^[_a-zA-Z][_a-zA-Z0-9]*";
        case T_STRING: return "^\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\"";
        case T_CHARACTER: return "^'[^']{1}'";
        case T_NUMBER: return "^(0[xX][0-9a-fA-F]+|[-+]?[0-9]+(\\.[0-9]+)?([eE][-+]?[0-9]+)?)";
        case T_COMMENT_LINE: return "^//[^\n]*";
        case T_COMMENT_MULTILINE: return "^/\\*.*?\\*/";
        case T_OPEN_BRACKET: return "^^\\[";
        case T_CLOSING_BRACKET: return "^\\]";
        case T_OPEN_PARENT: return "^[(]";
        case T_CLOSING_PARENT: return "^[)]";
        case T_OPEN_BRACE: return "^[{]";
        case T_CLOSING_BRACE: return "^[}]";
        case T_DOT_DOT_DOT: return "^[.][.][.]";
        case T_DOT: return "^[.]";
        case T_ARROW: return "^[-][>]";
        case T_COMMA: return "^,";
        case T_COLON: return "^:";
        case T_SEMICOLON: return "^[;]";
        case T_ASSIGNMENT: return "^=";
        case T_TILDE: return "^~";
        case T_PLUS: return "^[+]";
        case T_MINUS: return "^[-]";
        case T_MULTIPLY: return "^[*]";
        case T_DIVIDE: return "^/";
        case T_MODULUS: return "^%";
        case T_INCREMENT: return "^[+][+]";
        case T_DECREMENT: return "^[-][-]";
        case T_LESS_THAN: return "^[<]";
        case T_GREATER_THAN: return "^[>]";
        case T_LESS_THAN_OR_EQUAL_TO: return "^<=";
        case T_GREATER_THAN_OR_EQUAL_TO: return "^>=";
        case T_EQUAL_TO: return "^==";
        case T_NOT_EQUAL_TO: return "^!=";
        case T_LOGICAL_AND: return "^&&";
        case T_LOGICAL_OR: return "^\\|\\|";
        case T_LOGICAL_NOT: return "^!";
        case T_BITWISE_AND: return "^&";
        case T_BITWISE_OR: return "^\\|";
        case T_BITWISE_XOR: return "^\\^";
        case T_BITWISE_LEFTSHIFT: return "^<<";
        case T_BITWISE_RIGHTSHIFT: return "^>>";
        case T_PLUS_ASSIGN: return "^[+][=]";
        case T_MINUS_ASSIGN: return "^[-][=]";
        case T_MULTIPLY_ASSIGN: return "^[*][=]";
        case T_DIVIDE_ASSIGN: return "^[/][=]";
        case T_MODULUS_ASSIGN: return "^[%][=]";
        case T_BITWISE_AND_ASSIGN: return "^[&][=]";
        case T_BITWISE_OR_ASSIGN: return "^\\|[=]";
        case T_BITWISE_XOR_ASSIGN: return "^\\^[=]";
        case T_BITWISE_RIGHTSHIFT_ASSIGN: return "^>>=";
        case T_BITWISE_LEFTSHIFT_ASSIGN: return "^<<=";
        case T_TERNARY: return "^\\?";
        case T_ALIGNAS: return "^alignas\\b";
        case T_ALIGNOF: return "^alignof\\b";
        case T_AUTO: return "^auto\\b";
        case T_BOOL: return "^bool\\b";
        case T_BREAK: return "^break\\b";
        case T_CASE: return "^case\\b";
        case T_CHAR: return "^char\\b";
        case T_CONST: return "^const\\b";
        case T_CONSTEXPR: return "^constexpr\\b";
        case T_CONTINUE: return "^continue\\b";
        case T_DEFAULT: return "^default\\b";
        case T_DO: return "^do\\b";
        case T_DOUBLE: return "^double\\b";
        case T_ELSE: return "^else\\b";
        case T_ENUM: return "^enum\\b";
        case T_EXTERN: return "^extern\\b";
        case T_FALSE: return "^false\\b";
        case T_FLOAT: return "^float\\b";
        case T_FOR: return "^for\\b";
        case T_GOTO: return "^goto\\b";
        case T_IF: return "^if\\b";
        case T_INLINE: return "^inline\\b";
        case T_INT: return "^int\\b";
        case T_LONG: return "^long\\b";
        case T_NULLPTR: return "^nullptr\\b";
        case T_REGISTER: return "^register\\b";
        case T_RESTRICT: return "^restrict\\b";
        case T_RETURN: return "^return\\b";
        case T_SHORT: return "^short\\b";
        case T_SIGNED: return "^signed\\b";
        case T_SIZEOF: return "^sizeof\\b";
        case T_STATIC: return "^static\\b";
        case T_STATIC_ASSERT: return "^static_assert\\b";
        case T_STRUCT: return "^struct\\b";
        case T_SWITCH: return "^switch\\b";
        case T_THREAD_LOCAL: return "^thread_local\\b";
        case T_TRUE: return "^true\\b";
        case T_TYPEDEF: return "^typedef\\b";
        case T_TYPEOF: return "^typeof\\b";
        case T_TYPEOF_UNQUAL: return "^typeof_unqual\\b";
        case T_UNION: return "^union\\b";
        case T_UNSIGNED: return "^unsigned\\b";
        case T_VOID: return "^void\\b";
        case T_VOLATILE: return "^volatile\\b";
        case T_WHILE: return "^while\\b";
        case T_MACRO_IF: return "^#\\s*if\\b";
        case T_MACRO_ELIF: return "^#\\s*elif\\b";
        case T_MACRO_ELSE: return "^#\\s*else\\b";
        case T_MACRO_ENDIF: return "^#\\s*endif\\b";
        case T_MACRO_IFDEF: return "^#\\s*ifdef\\b";
        case T_MACRO_IFNDEF: return "^#\\s*ifndef\\b";
        case T_MACRO_ELIFDEF: return "^#\\s*elifdef\\b";
        case T_MACRO_ELIFNDEF: return "^#\\s*elifndef\\b";
        case T_MACRO_DEFINE: return "^#\\s*define\\b";
        case T_MACRO_UNDEF: return "^#\\s*undef\\b";
        case T_MACRO_INCLUDE: return "^#\\s*include\\s*<([^>]+)>|^#\\s*include\\s*\"([^\"]+)\"";
        case T_MACRO_LINE: return "^#\\s*line\\b";
        case T_MACRO_ERROR: return "^#\\s*error\\b";
        case T_MACRO_PRAGMA: return "^#\\s*pragma\\b";
        case T_MACRO_DEFINDED: return "^defined\\b";
        case T_MACRO_HAS_INCLUDE: return "^__has_include\\b";
        case T_MACRO_ASM: return "^asm\\b";
        case T_WHITESPACE: return "^\\s";
        case T_EOF: return "^\x00";
        default: return NULL;
    }
}

regex_t **token_type_c_regex;

regex_t *token_type_c_regcomp(const TokenType_C type)
{
    regex_t *regex = (regex_t *)malloc(sizeof(regex_t));
    
    if (regex == NULL) {
        return NULL;
    }

    if (regcomp(regex, token_type_c_pattern(type), REG_EXTENDED) != 0) {
        return NULL;
    }

    return regex;
}

int token_type_c_regex_create()
{
    token_type_c_regex = (regex_t **)malloc(sizeof(regex_t *) * TOKEN_TYPE_C_LENGTH);
    
    if (token_type_c_regex == NULL) {
        return -1;
    }
    
    for (TokenType_C type = 0; type < TOKEN_TYPE_C_LENGTH; type++) {
        token_type_c_regex[type] = token_type_c_regcomp(type);
    }
    
    return 0;
}

void token_type_c_regex_destroy()
{
    for (TokenType_C type = 0; type < TOKEN_TYPE_C_LENGTH; type++) {
        regfree(token_type_c_regex[type]);
        free(token_type_c_regex[type]);
    }
    
    free(token_type_c_regex);
}
