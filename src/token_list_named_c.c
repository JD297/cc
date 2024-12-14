#define _GNU_SOURCE

#include "token_list_named_c.h"
#include "token_list_c.h"

#include <stddef.h>
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

