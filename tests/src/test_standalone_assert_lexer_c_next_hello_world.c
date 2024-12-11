#include "lexer_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
	#include "asset_test_hello_world.h"

	Lexer_C *lexer;

	assert((lexer = lexer_c_create_lexer("tests/assets/asset_test_hello_world.c")) != LEXER_CREATION_FAILED);

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
