#include "token_c.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    #define TOKEN_VALUE "main"

    Token_C *token = token_c_create(T_IDENTIFIER, TOKEN_VALUE);

    assert(token != TOKEN_CREATION_FAILED);

    assert(token->type == T_IDENTIFIER);
    assert(strcmp(token->value, TOKEN_VALUE) == 0);

    FREE(token);

    return 0;
}
