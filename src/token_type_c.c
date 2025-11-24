#include "token_type_c.h"
#include <jd297/lmap_sv.h>

lmap_sv_t token_type_c_lookup_keywords;

lmap_sv_t token_type_c_lookup_preprocessor;

static TokenType_C_LookupEntry token_type_c_lookup_keyword_entries[] = {
	{.value = {.value = "auto", .len = 4}, .type = T_AUTO},
	{.value = {.value = "break", .len = 5}, .type = T_BREAK},
	{.value = {.value = "case", .len = 4}, .type = T_CASE},
	{.value = {.value = "char", .len = 4}, .type = T_CHAR},
	{.value = {.value = "const", .len = 5}, .type = T_CONST},
	{.value = {.value = "continue", .len = 8}, .type = T_CONTINUE},
	{.value = {.value = "default", .len = 7}, .type = T_DEFAULT},
	{.value = {.value = "do", .len = 2}, .type = T_DO},
	{.value = {.value = "double", .len = 6}, .type = T_DOUBLE},
	{.value = {.value = "else", .len = 4}, .type = T_ELSE},
	{.value = {.value = "enum", .len = 4}, .type = T_ENUM},
	{.value = {.value = "extern", .len = 6}, .type = T_EXTERN},
	{.value = {.value = "float", .len = 5}, .type = T_FLOAT},
	{.value = {.value = "for", .len = 3}, .type = T_FOR},
	{.value = {.value = "goto", .len = 4}, .type = T_GOTO},
	{.value = {.value = "if", .len = 2}, .type = T_IF},
	{.value = {.value = "int", .len = 3}, .type = T_INT},
	{.value = {.value = "long", .len = 4}, .type = T_LONG},
	{.value = {.value = "register", .len = 8}, .type = T_REGISTER},
	{.value = {.value = "return", .len = 6}, .type = T_RETURN},
	{.value = {.value = "short", .len = 5}, .type = T_SHORT},
	{.value = {.value = "signed", .len = 6}, .type = T_SIGNED},
	{.value = {.value = "sizeof", .len = 6}, .type = T_SIZEOF},
	{.value = {.value = "static", .len = 6}, .type = T_STATIC},
	{.value = {.value = "struct", .len = 6}, .type = T_STRUCT},
	{.value = {.value = "switch", .len = 4}, .type = T_SWITCH},
	{.value = {.value = "typedef", .len = 7}, .type = T_TYPEDEF},
	{.value = {.value = "union", .len = 5}, .type = T_UNION},
	{.value = {.value = "unsigned", .len = 8}, .type = T_UNSIGNED},
	{.value = {.value = "void", .len = 4}, .type = T_VOID},
	{.value = {.value = "volatile", .len = 8}, .type = T_VOLATILE},
	{.value = {.value = "while", .len = 5}, .type = T_WHILE},
};

static TokenType_C_LookupEntry token_type_c_lookup_preprocessor_entries[] = {
	{.value = {.value = "ifndef", .len = 6}, .type = T_MACRO_IFNDEF},
	{.value = {.value = "if", .len = 2}, .type = T_MACRO_IF},
	{.value = {.value = "elif", .len = 4}, .type = T_MACRO_ELIF},
	{.value = {.value = "else", .len = 4}, .type = T_MACRO_ELSE},
	{.value = {.value = "endif", .len = 5}, .type = T_MACRO_ENDIF},
	{.value = {.value = "define", .len = 6}, .type = T_MACRO_DEFINE},
	{.value = {.value = "undef", .len = 5}, .type = T_MACRO_UNDEF},
	{.value = {.value = "include", .len = 7}, .type = T_MACRO_INCLUDE},
	{.value = {.value = "line", .len = 4}, .type = T_MACRO_LINE},
	{.value = {.value = "error", .len = 5}, .type = T_MACRO_ERROR},
	{.value = {.value = "pragma", .len = 6}, .type = T_MACRO_PRAGMA},
};

static int token_type_c_create_lookup_keywords(void)
{
	int ret = 0;
	
	for (size_t i = 0; i < sizeof(token_type_c_lookup_keyword_entries) / sizeof(TokenType_C_LookupEntry) && ret == 0; i++) {
		ret = lmap_sv_add(&token_type_c_lookup_keywords, &token_type_c_lookup_keyword_entries[i].value, &token_type_c_lookup_keyword_entries[i]);
	}
	
	return ret;
}

static int token_type_c_create_lookup_preprocessor(void)
{
	int ret = 0;
	
	for (size_t i = 0; i < sizeof(token_type_c_lookup_preprocessor_entries) / sizeof(TokenType_C_LookupEntry) && ret == 0; i++) {
		ret = lmap_sv_add(&token_type_c_lookup_preprocessor, &token_type_c_lookup_preprocessor_entries[i].value, &token_type_c_lookup_preprocessor_entries[i]);
	}
	
	return ret;
}

int token_type_c_create_lookups(void)
{
	if (token_type_c_create_lookup_keywords() != 0) {
		return -1;
	}
	
	if (token_type_c_create_lookup_preprocessor() != 0) {
		return -1;
	}
	
	return 0;
}

// TODO token_type_c_destroy_lookups

#if 3 > 4

const char *token_type_c_pattern(const TokenType_C type)
{
    switch (type) {
        //case T_IDENTIFIER: return "^[_a-zA-Z][_a-zA-Z0-9]*";
        case T_STRING: return "^\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\"";
        case T_MACRO_INCLUDE_FILE: return "^<[^>]+>";
        case T_CHARACTER: return "^'[^']{1}'";
        case T_NUMBER: return "^(0[xX][0-9a-fA-F]+|[-+]?[0-9]+(\\.[0-9]+)?([eE][-+]?[0-9]+)?)";
        //case T_COMMENT_LINE: return "^//[^\n]*";
        //case T_COMMENT_MULTILINE: return "^/\\*([^*]|[[:space:]]|(\\*+([^*/]|[[:space:]])))*\\*+/";
        //case T_OPEN_BRACKET: return "^^\\[";
        //case T_CLOSING_BRACKET: return "^\\]";
        //case T_OPEN_PARENT: return "^[(]";
        //case T_CLOSING_PARENT: return "^[)]";
        //case T_OPEN_BRACE: return "^[{]";
        //case T_CLOSING_BRACE: return "^[}]";
        //case T_DOT_DOT_DOT: return "^[.][.][.]";
        //case T_DOT: return "^[.]";
        //case T_ARROW: return "^[-][>]";
        //case T_COMMA: return "^,";
        //case T_COLON: return "^:";
        //case T_SEMICOLON: return "^[;]";
        //case T_ASSIGNMENT: return "^=";
        //case T_TILDE: return "^~";
        //case T_PLUS: return "^[+]";
        //case T_MINUS: return "^[-]";
        //case T_MULTIPLY: return "^[*]";
        //case T_DIVIDE: return "^/";
        //case T_MODULUS: return "^%";
        //case T_INCREMENT: return "^[+][+]";
        //case T_DECREMENT: return "^[-][-]";
        //case T_LESS_THAN: return "^[<]";
        //case T_GREATER_THAN: return "^[>]";
        //case T_LESS_THAN_OR_EQUAL_TO: return "^<=";
        //case T_GREATER_THAN_OR_EQUAL_TO: return "^>=";
        //case T_EQUAL_TO: return "^==";
        //case T_NOT_EQUAL_TO: return "^!=";
        //case T_LOGICAL_AND: return "^&&";
        //case T_LOGICAL_OR: return "^\\|\\|";
        //case T_LOGICAL_NOT: return "^!";
        //case T_BITWISE_AND: return "^&";
        //case T_BITWISE_OR: return "^\\|";
        //case T_BITWISE_XOR: return "^\\^";
        //case T_BITWISE_LEFTSHIFT: return "^<<";
        //case T_BITWISE_RIGHTSHIFT: return "^>>";
        //case T_PLUS_ASSIGN: return "^[+][=]";
        //case T_MINUS_ASSIGN: return "^[-][=]";
        //case T_MULTIPLY_ASSIGN: return "^[*][=]";
        //case T_DIVIDE_ASSIGN: return "^[/][=]";
        //case T_MODULUS_ASSIGN: return "^[%][=]";
        //case T_BITWISE_AND_ASSIGN: return "^[&][=]";
        //case T_BITWISE_OR_ASSIGN: return "^\\|[=]";
        //case T_BITWISE_XOR_ASSIGN: return "^\\^[=]";
        //case T_BITWISE_RIGHTSHIFT_ASSIGN: return "^>>=";
        //case T_BITWISE_LEFTSHIFT_ASSIGN: return "^<<=";
        //case T_TERNARY: return "^\\?";
        /*case T_ALIGNAS: return "^alignas[[:>:]]";
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
        case T_WHILE: return "^while[[:>:]]";*/
        case T_MACRO_TOKEN_SEQUENZE: return "^[^\\\n]*[^\\]";
        /*case T_MACRO_IF: return "^#[[:space:]]*if[[:>:]]";
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
        case T_EOF: return "^\x00";*/
        default: return NULL;
    }
}

#endif
