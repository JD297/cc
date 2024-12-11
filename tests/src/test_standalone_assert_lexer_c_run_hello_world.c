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

	assert(lexer_c_run(lexer) == 0);

	assert(*lexer->pbuf == '\0');
	assert(lexer->tokens_len == LEXER_CREATION_TOKENS_LEN);
	assert(lexer->tokens_num == ASSET_TEST_C_TOKEN_NUM);
	assert(lexer->error == NULL);

	for (size_t i = 0; i < ASSET_TEST_C_TOKEN_NUM; i++) {
		assert(lexer->tokens[i]->type == asset_test_tokens[i].type);
		assert(strcmp(lexer->tokens[i]->value, asset_test_tokens[i].value) == 0);
	}

	FREE(lexer);

	return 0;
}
