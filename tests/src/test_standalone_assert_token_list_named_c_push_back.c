#include "token_list_named_c.h"
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
    TokenListNamed_C *named_list;

    assert((named_list = token_list_named_c_create()) != LIST_CREATION_FAILED);

    TokenType_C token_types[8] = { T_DOT, T_ARROW, T_COMMA, T_COLON,
                                   T_SEMICOLON, T_ASTERISK, T_ASSIGNMENT, T_TILDE };

    char *token_values[8] = { ".", "->", ",", ":", ";", "*", "=", "~" };

    TokenList_C *list;

    assert((list = token_list_c_create()) != LIST_CREATION_FAILED);

    for (size_t j = 0; j < LIST_INIT_LEN / 8; j++) { for (size_t i = 0; i < 8; i++) {
        Token_C *token;

        assert((token = test_token_c_create(token_types[i], token_values[i])) != TOKEN_CREATION_FAILED);
        assert(token_list_c_push_back(list, token) == 0);
        assert(token_type_c_is_in_expected_token_types(list->elements[list->num-1]->type, 1, token->type) == 1);
    } }

    char *initial = "initial";
    char *other = "other";
    char *resized = "resized";

    char *ptr;

    for (size_t j = 0; j < TOKEN_LIST_NAMED_INIT_LEN / 8; j++) { for (size_t i = 0; i < 8; i++) {

        if (j == 0 && i == 0) {
            ptr = initial;
        } else {
            ptr = other;
        }

        assert(token_list_named_c_push_back(named_list, list, ptr) == 0);
        assert(strcmp(named_list->elements_names[named_list->num-1], ptr) == 0);
    } }

    assert(list->num == TOKEN_LIST_NAMED_INIT_LEN);
    assert(list->len == TOKEN_LIST_NAMED_INIT_LEN);

    ptr = resized;

    assert(token_list_named_c_push_back(named_list, list, ptr) == 0);
    assert(strcmp(named_list->elements_names[named_list->num-1], resized) == 0);
    assert(strcmp(named_list->elements_names[0], initial) == 0);

    assert(named_list->num == TOKEN_LIST_NAMED_INIT_LEN + 1);
    assert(named_list->len == TOKEN_LIST_NAMED_INIT_LEN * TOKEN_LIST_NAMED_RESIZE_FACTOR);

    FREE(named_list);
    FREE(list);

    return 0;
}
