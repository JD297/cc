#include "lexer_c.h"
#include "lexer_list_c.h"
#include "token_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

#define test_token_c_create(T, S) token_c_create((T), (S), strlen((S)))

int main()
{
    LexerList_C *list;

    assert((list = lexer_list_c_create()) != LIST_CREATION_FAILED);

    for (size_t j = 0; j < LIST_INIT_LEN / 8; j++) { for (size_t i = 0; i < 8; i++) {
        Lexer_C *lexer;

        assert((lexer = lexer_c_create(__FILE__)) != LEXER_CREATION_FAILED);
        
        assert(lexer_list_c_push_back(list, lexer) == 0);
        assert(list->elements[list->num-1]->tokens->len == LIST_INIT_LEN);
        
        if (j == 0 && i == 0) {
            Token_C *token = test_token_c_create(T_IDENTIFIER, "test_token");
            token_list_c_push_back(list->elements[list->num-1]->tokens, token);
        }
    } }

    assert(list->num == LEXER_LIST_INIT_LEN);
    assert(list->len == LEXER_LIST_INIT_LEN);

    Lexer_C *lexer;
    
    assert((lexer = lexer_c_create(__FILE__)) != LEXER_CREATION_FAILED);
    assert(lexer_list_c_push_back(list, lexer) == 0);
    assert(list->elements[list->num-1]->tokens->len == LIST_INIT_LEN);
    
    assert(list->elements[list->num-1]->tokens->num == 0);
    assert(list->elements[0]->tokens->num == 1);

    assert(list->num == LEXER_LIST_INIT_LEN + 1);
    assert(list->len == LEXER_LIST_INIT_LEN * LEXER_LIST_RESIZE_FACTOR);

    FREE(list);

    return 0;
}
