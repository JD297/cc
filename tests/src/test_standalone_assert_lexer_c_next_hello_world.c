#include "lexer_c.h"
#include "token_list_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    #include "asset_test_token_list_source_hello_world.h"

    Lexer_C *lexer;

    assert((lexer = lexer_c_create("tests/assets/asset_test_hello_world.c")) != LEXER_CREATION_FAILED);

    Token_C *token;

    for (size_t i = 0; i < asset_test_token_list_hello_world->num; i++) {
        assert((token = lexer_c_next(lexer)) != LEXER_NEXT_FAILED);
        
        assert(token_list_c_push_back(lexer->tokens, token) == 0);

        assert(token->type == asset_test_token_list_hello_world->elements[i]->type);
        assert(token->len == asset_test_token_list_hello_world->elements[i]->len);
        assert(strncmp(token->value, asset_test_token_list_hello_world->elements[i]->value, asset_test_token_list_hello_world->elements[i]->len) == 0);
    }

    FREE(lexer);

    return 0;
}
