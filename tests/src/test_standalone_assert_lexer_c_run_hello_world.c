#include "lexer_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    #include "asset_test_token_list_source_hello_world.h"

    Lexer_C *lexer;

    assert((lexer = lexer_c_create("tests/assets/asset_test_hello_world.c")) != LEXER_CREATION_FAILED);

    assert(lexer_c_run(lexer) == 0);

    assert(*lexer->pbuf == '\0');
    assert(lexer->tokens->len == LIST_INIT_LEN);
    assert(lexer->tokens->num == asset_test_token_list_hello_world->num);
    assert(lexer->error == NULL);
    
    Token_C *token;
    
    for (size_t i = 0; i < asset_test_token_list_hello_world->num; i++) {
        assert(lexer->tokens->elements[i]->type == asset_test_token_list_hello_world->elements[i]->type);
        assert(lexer->tokens->elements[i]->len == asset_test_token_list_hello_world->elements[i]->len);
        assert(strncmp(lexer->tokens->elements[i]->value, asset_test_token_list_hello_world->elements[i]->value, asset_test_token_list_hello_world->elements[i]->len) == 0);
    }

    FREE(lexer);

    return 0;
}
