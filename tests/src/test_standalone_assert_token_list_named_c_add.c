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
    
    TokenList_C *list;
    
    assert((list = token_list_c_create()) != LIST_CREATION_FAILED);
    
    Token_C *token;

    assert((token = token_c_create(T_STRING, "back")) != TOKEN_CREATION_FAILED);
    list->elements[0] = token;
    assert(token_list_named_c_add(named_list, list, "back") == 0);
    assert(strcmp(named_list->elements[4]->elements[0]->value, "back") == 0);

    assert((token = token_c_create(T_STRING, "override")) != TOKEN_CREATION_FAILED);
    list->elements[0] = token;
    assert(token_list_named_c_add(named_list, list, "world") == 0);
    assert(strcmp(named_list->elements[1]->elements[0]->value, "override") == 0);

    named_list->elements[2] = NULL;
    named_list->elements_names[2] = NULL;
    assert((token = token_c_create(T_STRING, "null")) != TOKEN_CREATION_FAILED);
    list->elements[0] = token;
    assert(token_list_named_c_add(named_list, list, "null_key") == 0);
    assert(strcmp(named_list->elements[2]->elements[0]->value, "null") == 0);

    FREE(named_list);

    return 0;
}
