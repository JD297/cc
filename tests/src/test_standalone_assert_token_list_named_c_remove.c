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

    const char *value[] = { "hello", "world", "test", "123" };
    
    for (size_t i = 0; i < 4; i++) {
        TokenList_C *list;

        assert((list = token_list_c_create()) != LIST_CREATION_FAILED);

        Token_C *token;
        assert((token = test_token_c_create(T_STRING, value[i])) != TOKEN_CREATION_FAILED);
        assert(token_list_c_push_back(list, token) == 0);
        
        assert(token_list_named_c_push_back(named_list, list, value[i]) == 0);
    }

    token_list_named_c_remove(named_list, "world");
    assert(named_list->elements[1] == NULL);
    assert(named_list->elements_names[1] == NULL);

    token_list_named_c_remove(named_list, "123");
    assert(named_list->elements[3] == NULL);
    assert(named_list->elements_names[3] == NULL);

    assert(named_list->elements[0] != NULL);
    assert(named_list->elements_names[0] != NULL);
    
    assert(named_list->elements[2] != NULL);
    assert(named_list->elements_names[2] != NULL);

    FREE(named_list);

    return 0;
}
