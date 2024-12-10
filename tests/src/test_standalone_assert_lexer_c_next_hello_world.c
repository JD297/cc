#include "lexer_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
	Lexer_C *lexer;

	assert((lexer = lexer_c_create_lexer("tests/assets/asset_test_hello_world.c")) != LEXER_CREATION_FAILED);

	#define ASSET_TEST_C_TOKEN_NUM 35

	Token_C asset_test_tokens[ASSET_TEST_C_TOKEN_NUM] = {
		{T_MACRO_INCLUDE, "#include"}, {T_WHITESPACE_SPACE, " "}, {T_MACRO_INCLUDE_LIBRARY_PATH, "stdio.h"}, {T_WHITESPACE_LINE_FEED, "\n"},	// LINE 1
		{T_WHITESPACE_LINE_FEED, "\n"},														// LINE 2
		{T_INT, "int"}, {T_WHITESPACE_SPACE, " "}, {T_IDENTIFIER, "main"}, {T_OPEN_PARENT, "("}, {T_CLOSING_PARENT, ")"}, {T_WHITESPACE_LINE_FEED, "\n"},			// LINE 3
		{T_OPEN_BRACE, "{"}, {T_WHITESPACE_LINE_FEED, "\n"},											// LINE 4
		{T_WHITESPACE_TAB, "\t"}, {T_COMMENT_LINE, "// This is a comment!"}, {T_WHITESPACE_LINE_FEED, "\n"},					// LINE 5
		{T_WHITESPACE_TAB, "\t"}, {T_COMMENT_MULTILINE, "/*\n\t* This is a multi line\n\t* comment!\n\t*/"}, {T_WHITESPACE_LINE_FEED, "\n"},	// LINE 6 - 9
		{T_WHITESPACE_TAB, "\t"}, {T_IDENTIFIER, "printf"}, {T_OPEN_PARENT, "("}, {T_STRING, "Hello world!\n"}, {T_CLOSING_PARENT, ")"},	// LINE 10
		{T_SEMICOLON, ";"}, {T_WHITESPACE_LINE_FEED, "\n"}, 											// LINE 10
		{T_WHITESPACE_TAB, "\t"}, {T_WHITESPACE_LINE_FEED, "\n"},										// LINE 11
		{T_WHITESPACE_TAB, "\t"}, {T_RETURN, "return"}, {T_NUMBER, "0"}, {T_SEMICOLON, ";"}, {T_WHITESPACE_LINE_FEED, "\n"},			// LINE 12
		{T_CLOSING_BRACE, "}"}, {T_WHITESPACE_LINE_FEED, "\n"}, {T_EOF, "\0"},									// LINE 13
	};

	for (size_t i = 0; i < ASSET_TEST_C_TOKEN_NUM; i++) {
		Token_C *token = lexer_c_next(lexer);

		assert(token != LEXER_NEXT_FAILED);

		assert(lexer_c_push_back_token(lexer, token) == 0);

		assert(token->type == asset_test_tokens[i].type);

		assert(strcmp(token->value, asset_test_tokens[i].value) == 0);

		if (ASSET_TEST_C_TOKEN_NUM - 1 == i) {
			assert(lexer_c_token_type_is_in_expected_token_types(asset_test_tokens[i].type, 1, T_EOF) == 1);
			assert(*lexer->pbuf == '\0');
		}
	}

	FREE(lexer);

	return 0;
}
