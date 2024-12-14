#include "token_c.h"

#include <sys/mman.h>

void *token_c_create(TokenType_C type, const char* value)
{
    Token_C *token = (Token_C *)mmap(NULL, sizeof(Token_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (token == MAP_FAILED) {
        return TOKEN_CREATION_FAILED;
    }

    token->type = type;
    token->value = value;

    return token;
}

