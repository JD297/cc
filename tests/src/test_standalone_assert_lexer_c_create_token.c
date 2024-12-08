#include "lexer_c.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
	#define TOKEN_VALUE "main"
	#define TOKEN_VALUE_LEN strlen(TOKEN_VALUE)

	Token_C *token = lexer_c_create_token(T_IDENTIFIER, TOKEN_VALUE, TOKEN_VALUE_LEN);

	assert(token != TOKEN_CREATION_FAILED);

	assert(token->type == T_IDENTIFIER);
	assert(strcmp(token->value, TOKEN_VALUE) == 0);
	assert(token->value_len == TOKEN_VALUE_LEN);

	FREE(token);

	return 0;
}
