#include "lexer_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
	Lexer_C *lexer;

	assert((lexer = lexer_c_create_lexer(__FILE__)) != LEXER_CREATION_FAILED);

	TokenType_C token_types[8] = { T_DOT, T_ARROW, T_COMMA, T_COLON,
                                       T_SEMICOLON, T_ASTERISK, T_ASSIGNMENT, T_TILDE };
	char *token_values[8] = { ".", "->", ",", ":", ";", "*", "=", "~" };

	for (size_t j = 0; j < LEXER_CREATION_TOKENS_LEN / 8; j++) { for (size_t i = 0; i < 8; i++) {
		Token_C *token;

		assert((token = lexer_c_create_token(token_types[i], token_values[i], strlen(token_values[i]))) != TOKEN_CREATION_FAILED);
		assert(lexer_c_push_back_token(lexer, token) == 0);
		assert(lexer_c_token_type_is_in_expected_token_types(lexer->tokens[lexer->tokens_num-1]->type, 1, token->type) == 1);
	} }

	assert(lexer->tokens_num == LEXER_CREATION_TOKENS_LEN);
	assert(lexer->tokens_len == LEXER_CREATION_TOKENS_LEN);

	Token_C *token;
	assert((token = lexer_c_create_token(token_types[0], token_values[0], strlen(token_values[0]))) != TOKEN_CREATION_FAILED);

	assert(lexer_c_push_back_token(lexer, token) == 0);
	assert(lexer_c_token_type_is_in_expected_token_types(lexer->tokens[lexer->tokens_num-1]->type, 1, T_DOT) == 1);

	assert(lexer->tokens_num == LEXER_CREATION_TOKENS_LEN + 1);
	assert(lexer->tokens_len == LEXER_CREATION_TOKENS_LEN * 2);

	FREE(lexer);

	return 0;
}
