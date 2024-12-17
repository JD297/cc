#include "token_list_named_c.h"
#include "token_list_c.h"
#include "token_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    TokenListNamed_C *named_list;

    assert((named_list = token_list_named_c_create()) != LIST_CREATION_FAILED);

    const char *value[] = { "hello", "world", "test", "123" };
    
    for (size_t i = 0; i < 4; i++) {
        TokenList_C *list;

        assert((list = token_list_c_create()) != LIST_CREATION_FAILED);

        Token_C *token;
        assert((token = token_c_create(T_STRING, value[i])) != TOKEN_CREATION_FAILED);
        assert(token_list_c_push_back(list, token) == 0);
        
        assert(token_list_named_c_push_back(named_list, list, value[i]) == 0);
    }

    TokenList_C *list_hello = token_list_named_c_get(named_list, "hello");
    assert(list_hello != NULL);
    assert(list_hello->elements[0]->type == T_STRING);
    assert(strcmp(list_hello->elements[0]->value, "hello") == 0);

    TokenList_C *list_world = token_list_named_c_get(named_list, "world");
    assert(list_world != NULL);
    assert(list_world->elements[0]->type == T_STRING);
    assert(strcmp(list_world->elements[0]->value, "world") == 0);

    assert(token_list_named_c_get(named_list, "no_key") == NULL);

    named_list->elements[1] = NULL;
    named_list->elements_names[1] = NULL;
    assert(token_list_named_c_get(named_list, "world") == NULL);

    FREE(named_list);

    return 0;
}
