#include "str_list.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    StrList *list;

    assert((list = str_list_create()) != LIST_CREATION_FAILED);

    char *initial = "Initial string!";

    for (size_t j = 0; j < STR_LIST_INIT_LEN / 8; j++) { for (size_t i = 0; i < 8; i++) {
        char *str;
        
        if (j == 0 && i == 0) {
            str = initial;
        } else {
            str = "Other string!";
        }
        
        assert(str_list_push_back(list, str) == 0);

        assert(strlen(list->elements[list->num-1]) == strlen(str));
        assert(strcmp(list->elements[list->num-1], str) == 0);
    } }

    assert(list->num == STR_LIST_INIT_LEN);
    assert(list->len == STR_LIST_INIT_LEN);

    char *str = "Resize string!";
    
    assert(str_list_push_back(list, str) == 0);
    
    assert(strlen(list->elements[list->num-1]) == strlen(str));
    assert(strcmp(list->elements[list->num-1], str) == 0);
    
    assert(strlen(list->elements[0]) == strlen(initial));
    assert(strcmp(list->elements[0], initial) == 0); 

    assert(list->num == STR_LIST_INIT_LEN + 1);
    assert(list->len == STR_LIST_INIT_LEN * STR_LIST_RESIZE_FACTOR);

    FREE(list);

    return 0;
}
