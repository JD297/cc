#include "token_type_c.h"

#include <assert.h>
#include <stddef.h>

const char *token_type_c_to_representation(TokenType_C type)
{
    switch (type) {
        case T_IDENTIFIER:
        case T_STRING:
        case T_TYPE_CAST:
        case T_NUMBER:
        case T_MACRO_INCLUDE_LIBRARY_PATH:
        case T_COMMENT_LINE:
        case T_COMMENT_MULTILINE:
        return NULL;
        case T_OPEN_BRACKET: return "[";
        case T_CLOSING_BRACKET: return "]";
        case T_OPEN_PARENT: return "(";
        case T_CLOSING_PARENT: return ")";
        case T_OPEN_BRACE: return "{";
        case T_CLOSING_BRACE: return "}";
        case T_DOT: return ".";
        case T_ARROW: return "->";
        case T_COMMA: return ",";
        case T_COLON: return ":";
        case T_SEMICOLON: return ";";
        case T_ASTERISK: return "*";
        case T_ASSIGNMENT: return "=";
        case T_TILDE: return "~";
        case T_PLUS: return "+";
        case T_MINUS: return "-";
        case T_MULTIPLY: return "*";
        case T_DIVIDE: return "/";
        case T_MODULUS: return "%";
        case T_UNARY_PLUS: return "+";
        case T_UNARY_MINUS: return "-";
        case T_INCREMENT: return "++";
        case T_DECREMENT: return "--";
        case T_LESS_THAN: return "<";
        case T_GREATER_THAN: return ">";
        case T_LESS_THAN_OR_EQUAL_TO: return "<=";
        case T_GREATER_THAN_OR_EQUAL_TO: return ">=";
        case T_EQUAL_TO: return "==";
        case T_NOT_EQUAL_TO: return "!=";
        case T_LOGICAL_AND: return "&&";
        case T_LOGICAL_OR: return "||";
        case T_LOGICAL_NOT: return "!";
        case T_BITWISE_AND: return "&";
        case T_BITWISE_OR: return "|";
        case T_BITWISE_XOR: return "^";
        case T_BITWISE_FIRST_COMPLEMENT: return "~";
        case T_BITWISE_LEFTSHIFT: return "<<";
        case T_BITWISE_RIGHTSHIFT: return ">>";
        case T_PLUS_ASSIGN: return "+=";
        case T_MINUS_ASSIGN: return "-=";
        case T_MULTIPLY_ASSIGN: return "*=";
        case T_DIVIDE_ASSIGN: return "/=";
        case T_MODULUS_ASSIGN: return "%=";
        case T_BITWISE_AND_ASSIGN: return "&=";
        case T_BITWISE_OR_ASSIGN: return "|=";
        case T_BITWISE_XOR_ASSIGN: return "^=";
        case T_BITWISE_RIGHTSHIFT_ASSIGN: return ">>=";
        case T_BITWISE_LEFTSHIFT_ASSIGN: return "<<=";
        case T_TERNARY: return "?";
        case T_ADDRESSOF: return "&";
        case T_DEREFERENCE: return "*";
        case T_ALIGNAS: return "alignas";
        case T_ALIGNOF: return "alignof";
        case T_AUTO: return "auto";
        case T_BOOL: return "bool";
        case T_BREAK: return "break";
        case T_CASE: return "case";
        case T_CHAR: return "char";
        case T_CONST: return "const";
        case T_CONSTEXPR: return "constexpr";
        case T_CONTINUE: return "continue";
        case T_DEFAULT: return "default";
        case T_DO: return "do";
        case T_DOUBLE: return "double";
        case T_ELSE: return "else";
        case T_ENUM: return "enum";
        case T_EXTERN: return "extern";
        case T_FALSE: return "false";
        case T_FLOAT: return "float";
        case T_FOR: return "for";
        case T_GOTO: return "goto";
        case T_IF: return "if";
        case T_INLINE: return "inline";
        case T_INT: return "int";
        case T_LONG: return "long";
        case T_NULLPTR: return "nullptr";
        case T_REGISTER: return "register";
        case T_RESTRICT: return "restrict";
        case T_RETURN: return "return";
        case T_SHORT: return "short";
        case T_SIGNED: return "signed";
        case T_SIZEOF: return "sizeof";
        case T_STATIC: return "static";
        case T_STATIC_ASSERT: return "static_assert";
        case T_STRUCT: return "struct";
        case T_SWITCH: return "switch";
        case T_THREAD_LOCAL: return "thread_local";
        case T_TRUE: return "true";
        case T_TYPEDEF: return "typedef";
        case T_TYPEOF: return "typeof";
        case T_TYPEOF_UNQUAL: return "typeof_unqual";
        case T_UNION: return "union";
        case T_UNSIGNED: return "unsigned";
        case T_VOID: return "void";
        case T_VOLATILE: return "volatile";
        case T_WHILE: return "while";
        case T_MACRO_IF: return "#if";
        case T_MACRO_ELIF: return "#elif";
        case T_MACRO_ELSE: return "#else";
        case T_MACRO_ENDIF: return "#endif";
        case T_MACRO_IFDEF: return "#ifdef";
        case T_MACRO_IFNDEF: return "#ifndef";
        case T_MACRO_ELIFDEF: return "#elifdef";
        case T_MACRO_ELIFNDEF: return "#elifndef";
        case T_MACRO_DEFINE: return "#define";
        case T_MACRO_UNDEF: return "#undef";
        case T_MACRO_INCLUDE: return "#include";
        case T_MACRO_EMBED: return "#embed";
        case T_MACRO_LINE: return "__LINE__";
        case T_MACRO_FILE: return "__FILE__";
        case T_MACRO_ERROR: return "#error";
        case T_MACRO_PRAGMA: return "#pragma";
        case T_MACRO_DEFINDED: return "#defined";
        case T_MACRO___HAS_INCLUDE: return "__has_include";
        case T_MACRO___HAS_EMBED: return "__has_embed";
        case T_MACRO___HAS_C_ATTRIBUTE: return "__has_c_attribute";
        case T_MACRO_ASM: return "__asm__";
        case T_WHITESPACE_TAB: return "\t";
        case T_WHITESPACE_LINE_FEED: return "\n";
        case T_WHITESPACE_LINE_TABULATION: return "\v";
        case T_WHITESPACE_FORM_FEED: return "\f";
        case T_WHITESPACE_CARRIAGE_RETURN: return "\r";
        case T_WHITESPACE_SPACE: return " ";
        case T_WHITESPACE_NEXT_LINE: return "\\";
        case T_EOF: return "\0";
        default: assert(0 && "TokenType_C (type) not implemented!");
    }
}

int token_type_c_is_in_expected_token_types(TokenType_C type, size_t num_types, /* TokenType_C types */...)
{
    if (num_types == 0) {
        return 0;
    }

    va_list ap;
    va_start(ap, num_types);

    for (size_t i = 0; i < num_types; i++) {
        if (type == va_arg(ap, TokenType_C)) {
            va_end(ap);

            return 1;
        }
    }

    va_end(ap);

    return 0;
}

