#include "lexer_c.h"

#include <assert.h>
#include <string.h>

int main()
{
	TokenType_C token_types_with_no_representation[TOKEN_TYPE_C_WITH_NO_REPRESENTATION_NUM] = { TOKEN_TYPE_C_WITH_NO_REPRESENTATION };

	for (size_t i = 0; i < TOKEN_TYPE_C_WITH_NO_REPRESENTATION_NUM; i++) {
		assert(lexer_c_token_type_is_in_expected_token_types(token_types_with_no_representation[i], TOKEN_TYPE_C_WITH_NO_REPRESENTATION_NUM, TOKEN_TYPE_C_WITH_NO_REPRESENTATION));
		assert(lexer_c_token_type_representation(token_types_with_no_representation[i]) == NULL);
	}

	/* TOKENS */
	assert(lexer_c_token_type_representation(T_IDENTIFIER) == NULL);
	assert(lexer_c_token_type_representation(T_STRING) == NULL);
	assert(lexer_c_token_type_representation(T_NUMBER) == NULL);

	/* SYMBOLS */
	assert(strcmp(lexer_c_token_type_representation(T_OPEN_BRACKET), "[") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CLOSING_BRACKET), "]") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_OPEN_PARENT), "(") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CLOSING_PARENT), ")") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_OPEN_BRACE), "{") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CLOSING_BRACE), "}") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DOT), ".") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_ARROW), "->") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_COMMA), ",") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_SEMICOLON), ";") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_ASTERISK), "*") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_ASSIGNMENT), "=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_TILDE), "~") == 0);

	/* OPERATORS */
	assert(strcmp(lexer_c_token_type_representation(T_PLUS), "+") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MINUS), "-") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MULTIPLY), "*") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DIVIDE), "/") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MODULUS), "%") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_UNARY_PLUS), "+") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_UNARY_MINUS), "-") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_INCREMENT), "++") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DECREMENT), "--") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_LESS_THAN), "<") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_GREATER_THAN), ">") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_LESS_THAN_OR_EQUAL_TO), "<=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_GREATER_THAN_OR_EQUAL_TO), ">=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_EQUAL_TO), "==") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_NOT_EQUAL_TO), "!=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_LOGICAL_AND), "&&") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_LOGICAL_OR), "||") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_LOGICAL_NOT), "!") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_AND), "&") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_OR), "|") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_XOR), "^") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_FIRST_COMPLEMENT), "~") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_LEFTSHIFT), "<<") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_RIGHTSHIFT), ">>") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_PLUS_ASSIGN), "+=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MINUS_ASSIGN), "-=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MULTIPLY_ASSIGN), "*=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DIVIDE_ASSIGN), "/=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MODULUS_ASSIGN), "%=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_AND_ASSIGN), "&=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_OR_ASSIGN), "|=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_XOR_ASSIGN), "^=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_RIGHTSHIFT_ASSIGN), ">>=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BITWISE_LEFTSHIFT_ASSIGN), "<<=") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_TERNARY), "?") == 0);
	assert(lexer_c_token_type_representation(T_TYPE_CAST) == NULL);
	assert(strcmp(lexer_c_token_type_representation(T_ADDRESSOF), "&") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DEREFERENCE), "*") == 0);

	/* NORMAL KEYWORDS */
	assert(strcmp(lexer_c_token_type_representation(T_ALIGNAS), "alignas") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_ALIGNOF), "alignof") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_AUTO), "auto") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BOOL), "bool") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_BREAK), "break") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CASE), "case") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CHAR), "char") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CONST), "const") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CONSTEXPR), "constexpr") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_CONTINUE), "continue") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DEFAULT), "default") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DO), "do") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_DOUBLE), "double") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_ELSE), "else") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_ENUM), "enum") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_EXTERN), "extern") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_FALSE), "false") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_FLOAT), "float") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_FOR), "for") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_GOTO), "goto") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_IF), "if") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_INLINE), "inline") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_INT), "int") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_LONG), "long") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_NULLPTR), "nullptr") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_REGISTER), "register") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_RESTRICT), "restrict") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_RETURN), "return") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_SHORT), "short") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_SIGNED), "signed") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_SIZEOF), "sizeof") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_STATIC), "static") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_STATIC_ASSERT), "static_assert") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_STRUCT), "struct") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_SWITCH), "switch") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_THREAD_LOCAL), "thread_local") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_TRUE), "true") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_TYPEDEF), "typedef") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_TYPEOF), "typeof") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_TYPEOF_UNQUAL), "typeof_unqual") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_UNION), "union") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_UNSIGNED), "unsigned") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_VOID), "void") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_VOLATILE), "volatile") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_WHILE), "while") == 0);

	/* PREPROCESSOR KEYWORDS*/
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_IF), "#if") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_ELIF), "#elif") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_ELSE), "#else") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_ENDIF), "#endif") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_IFDEF), "#ifdef") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_IFNDEF), "#ifndef") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_ELIFDEF), "#elifdef") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_ELIFNDEF), "#elifndef") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_DEFINE), "#define") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_UNDEF), "#undef") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_INCLUDE), "#include") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_EMBED), "#embed") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_LINE), "#line") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_ERROR), "#error") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_WARNING), "#warning") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_PRAGMA), "#pragma") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_DEFINDED), "#defined") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO___HAS_INCLUDE), "__has_include") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO___HAS_EMBED), "__has_embed") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO___HAS_C_ATTRIBUTE), "__has_c_attribute") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_ASM), "__asm__") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_MACRO_FORTRAN), "__fortran__") == 0);

	/* WHITESPACE */
	assert(strcmp(lexer_c_token_type_representation(T_WHITESPACE_TAB), "\t") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_WHITESPACE_LINE_FEED), "\n") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_WHITESPACE_LINE_TABULATION), "\v") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_WHITESPACE_FORM_FEED), "\f") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_WHITESPACE_CARRIAGE_RETURN), "\r") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_WHITESPACE_SPACE), " ") == 0);
	assert(strcmp(lexer_c_token_type_representation(T_WHITESPACE_NEXT_LINE), "\\") == 0);

	assert(strcmp(lexer_c_token_type_representation(T_EOF), "\0") == 0);

	return 0;
}

