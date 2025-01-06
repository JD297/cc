#include "token_list_c.h"
#include "token_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

#define test_token_c_create(T, S) token_c_create((T), (S), strlen((S)))

int main()
{
    TokenList_C *list;

    assert((list = token_list_c_create()) != LIST_CREATION_FAILED);

    TokenType_C token_types[8] = { T_DOT, T_ARROW, T_COMMA, T_COLON,
                                   T_SEMICOLON, T_ASTERISK, T_ASSIGNMENT, T_TILDE };

    char *token_values[8] = { ".", "->", ",", ":", ";", "*", "=", "~" };

    for (size_t j = 0; j < LIST_INIT_LEN / 8; j++) { for (size_t i = 0; i < 8; i++) {
        Token_C *token;

        assert((token = test_token_c_create(token_types[i], token_values[i])) != TOKEN_CREATION_FAILED);
        assert(token_list_c_push_back(list, token) == 0);
        assert(token_type_c_is_in_expected_token_types(list->elements[list->num-1]->type, 1, token->type) == 1);
    } }

    assert(list->num == LIST_INIT_LEN);
    assert(list->len == LIST_INIT_LEN);

    Token_C *token;
    assert((token = test_token_c_create(token_types[0], token_values[0])) != TOKEN_CREATION_FAILED);

    assert(token_list_c_push_back(list, token) == 0);
    assert(token_type_c_is_in_expected_token_types(list->elements[list->num-1]->type, 1, T_DOT) == 1);

    assert(list->num == LIST_INIT_LEN + 1);
    assert(list->len == LIST_INIT_LEN * LIST_RESIZE_FACTOR);

    FREE(list);

    return 0;
}
