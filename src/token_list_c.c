#define _GNU_SOURCE

#include "token_list_c.h"
#include "token_c.h"

#include <stddef.h>
#include <sys/mman.h>

void *token_list_c_create()
{
    TokenList_C *list = (TokenList_C *)mmap(NULL, sizeof(TokenList_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (list == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    list->len = LIST_INIT_LEN;
	list->num = 0;
	list->elements = (Token_C **)mmap(NULL, list->len * sizeof(Token_C *), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (list->elements == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    return list;
}

int token_list_c_push_back(TokenList_C *list, Token_C *element)
{
    if (list->num == list->len) {
        size_t new_len = list->len * LIST_RESIZE_FACTOR;

        list->elements = (Token_C **)mremap(list->elements, list->len * sizeof(Token_C *), new_len * sizeof(Token_C *), MREMAP_MAYMOVE);

        if (list->elements == MAP_FAILED) {
            return -1;
        }

        list->len = new_len;
    }

    list->elements[list->num] = element;

    list->num++;

    return 0;
}

