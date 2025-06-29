#include "token_type_c.h"
#include "token_type_skipable_lookup.h"

#include <regex.h>
#include <stddef.h>
#include <stdlib.h>

const char *token_type_c_pattern(const TokenType_C type)
{
    switch (type) {
        case T_IDENTIFIER: return "^[_a-zA-Z][_a-zA-Z0-9]*";
        case T_STRING: return "^\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\""; // TODO ?? remove capture group: then we could replace the ending " with a NULL terminator to get a termianted string
        case T_MACRO_INCLUDE_FILE: return "^<[^>]+>";
        case T_CHARACTER: return "^'[^']{1}'";
        case T_NUMBER: return "^(0[xX][0-9a-fA-F]+|[-+]?[0-9]+(\\.[0-9]+)?([eE][-+]?[0-9]+)?)";
        case T_COMMENT_LINE: return "^//[^\n]*";
        case T_COMMENT_MULTILINE: return "^/\\*([^*]|[[:space:]]|(\\*+([^*/]|[[:space:]])))*\\*+/";
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
        case T_ALIGNAS: return "^alignas[[:>:]]";
        case T_ALIGNOF: return "^alignof[[:>:]]";
        case T_AUTO: return "^auto[[:>:]]";
        case T_BOOL: return "^bool[[:>:]]";
        case T_BREAK: return "^break[[:>:]]";
        case T_CASE: return "^case[[:>:]]";
        case T_CHAR: return "^char[[:>:]]";
        case T_CONST: return "^const[[:>:]]";
        case T_CONSTEXPR: return "^constexpr[[:>:]]";
        case T_CONTINUE: return "^continue[[:>:]]";
        case T_DEFAULT: return "^default[[:>:]]";
        case T_DO: return "^do[[:>:]]";
        case T_DOUBLE: return "^double[[:>:]]";
        case T_ELSE: return "^else[[:>:]]";
        case T_ENUM: return "^enum[[:>:]]";
        case T_EXTERN: return "^extern[[:>:]]";
        case T_FALSE: return "^false[[:>:]]";
        case T_FLOAT: return "^float[[:>:]]";
        case T_FOR: return "^for[[:>:]]";
        case T_GOTO: return "^goto[[:>:]]";
        case T_IF: return "^if[[:>:]]";
        case T_INLINE: return "^inline[[:>:]]";
        case T_INT: return "^int[[:>:]]";
        case T_LONG: return "^long[[:>:]]";
        case T_NULLPTR: return "^nullptr[[:>:]]";
        case T_REGISTER: return "^register[[:>:]]";
        case T_RESTRICT: return "^restrict[[:>:]]";
        case T_RETURN: return "^return[[:>:]]";
        case T_SHORT: return "^short[[:>:]]";
        case T_SIGNED: return "^signed[[:>:]]";
        case T_SIZEOF: return "^sizeof[[:>:]]";
        case T_STATIC: return "^static[[:>:]]";
        case T_STATIC_ASSERT: return "^static_assert[[:>:]]";
        case T_STRUCT: return "^struct[[:>:]]";
        case T_SWITCH: return "^switch[[:>:]]";
        case T_THREAD_LOCAL: return "^thread_local[[:>:]]";
        case T_TRUE: return "^true[[:>:]]";
        case T_TYPEDEF: return "^typedef[[:>:]]";
        case T_TYPEOF: return "^typeof[[:>:]]";
        case T_TYPEOF_UNQUAL: return "^typeof_unqual[[:>:]]";
        case T_UNION: return "^union[[:>:]]";
        case T_UNSIGNED: return "^unsigned[[:>:]]";
        case T_VOID: return "^void[[:>:]]";
        case T_VOLATILE: return "^volatile[[:>:]]";
        case T_WHILE: return "^while[[:>:]]";
        case T_MACRO_TOKEN_SEQUENZE: return "^[^\\\n]*[^\\]";
        case T_MACRO_IF: return "^#[[:space:]]*if[[:>:]]";
        case T_MACRO_ELIF: return "^#[[:space:]]*elif[[:>:]]";
        case T_MACRO_ELSE: return "^#[[:space:]]*else[[:>:]]";
        case T_MACRO_ENDIF: return "^#[[:space:]]*endif[[:>:]]";
        case T_MACRO_IFDEF: return "^#[[:space:]]*ifdef[[:>:]]";
        case T_MACRO_IFNDEF: return "^#[[:space:]]*ifndef[[:>:]]";
        case T_MACRO_ELIFDEF: return "^#[[:space:]]*elifdef[[:>:]]";
        case T_MACRO_ELIFNDEF: return "^#[[:space:]]*elifndef[[:>:]]";
        case T_MACRO_DEFINE: return "^#[[:space:]]*define[[:>:]]";
        case T_MACRO_UNDEF: return "^#[[:space:]]*undef[[:>:]]";
        case T_MACRO_INCLUDE: return "^#[[:space:]]*include[[:>:]]";
        case T_MACRO_LINE: return "^#[[:space:]]*line[[:>:]]";
        case T_MACRO_ERROR: return "^#[[:space:]]*error[[:>:]]";
        case T_MACRO_PRAGMA: return "^#[[:space:]]*pragma[[:>:]]";
        case T_MACRO_DEFINDED: return "^defined[[:>:]]";
        case T_MACRO_HAS_INCLUDE: return "^__has_include[[:>:]]";
        case T_MACRO_ASM: return "^asm[[:>:]]";
        case T_WHITESPACE: return "^[[:space:]]";
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

int token_type_c_regex_create(void)
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

void token_type_c_regex_destroy(void)
{
    for (TokenType_C type = 0; type < TOKEN_TYPE_C_LENGTH; type++) {
        regfree(token_type_c_regex[type]);
        free(token_type_c_regex[type]);
    }
    
    free(token_type_c_regex);
}
