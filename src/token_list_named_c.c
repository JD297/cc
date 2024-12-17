#define _GNU_SOURCE

#include "token_list_named_c.h"
#include "token_list_c.h"

#include <stddef.h>
#include <string.h>
#include <sys/mman.h>

void *token_list_named_c_create()
{
    TokenListNamed_C *list = (TokenListNamed_C *)mmap(NULL, sizeof(TokenListNamed_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (list == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    list->len = TOKEN_LIST_NAMED_INIT_LEN;
	list->num = 0;
	list->elements = (TokenList_C **)mmap(NULL, list->len * sizeof(TokenList_C *), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	list->elements_names = (char **)mmap(NULL, list->len * sizeof(char *), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (list->elements == MAP_FAILED || list->elements_names == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    return list;
}

int token_list_named_c_push_back(TokenListNamed_C *list, TokenList_C *element, char *name)
{
    if (list->num == list->len) {
        size_t new_len = list->len * TOKEN_LIST_NAMED_RESIZE_FACTOR;

        list->elements = (TokenList_C **)mremap(list->elements, list->len * sizeof(TokenList_C *), new_len * sizeof(TokenList_C *), MREMAP_MAYMOVE);
        list->elements_names = (char **)mremap(list->elements_names, list->len * sizeof(char *), new_len * sizeof(char *), MREMAP_MAYMOVE);

        if (list->elements == MAP_FAILED || list->elements_names == MAP_FAILED) {
            return -1;
        }

        list->len = new_len;
    }

    list->elements[list->num] = element;
    list->elements_names[list->num] = name;

    list->num++;

    return 0;
}

TokenList_C *token_list_named_c_get(TokenListNamed_C *list, const char *name)
{
    for (size_t i = 0; i < list->num; i++) {
        if (list->elements_names[i] == NULL) {
            continue;
        }

        if (strcmp(list->elements_names[i], name) == 0) {
            return list->elements[i];
        }
    }
    return NULL;
}

int token_list_named_c_add(TokenListNamed_C *list, TokenList_C *element, const char *name)
{
    size_t null_element_index = list->num;

    for (size_t i = 0; i < list->num; i++) {
        if (list->elements_names[i] == NULL) {
            null_element_index = i;
        
            break;
        }

        if (strcmp(list->elements_names[i], name) == 0) {
            list->elements[i] = element;
            
            return 0;
        }
    }
    
    if (null_element_index == list->num) {
        return token_list_named_c_push_back(list, element, name);
    }
    
    list->elements[null_element_index] = element;
    
    return 0;
}

