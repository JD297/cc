#include "token_c.h"

#include <stdlib.h>

void *token_c_create(TokenType_C type, const char* value, size_t len)
{
    Token_C *token = (Token_C *)malloc(sizeof(Token_C));

    if (token == NULL) {
        return NULL;
    }

    token->type = type;
    token->value = value;
    token->len = len;

    return token;
}

void token_c_destroy(Token_C *token)
{
    free(token);
}
