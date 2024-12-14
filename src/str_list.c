#define _GNU_SOURCE

#include "str_list.h"

#include <stddef.h>
#include <sys/mman.h>

void *str_list_create()
{
    StrList *list = (StrList *)mmap(NULL, sizeof(StrList), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (list == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    list->len = STR_LIST_INIT_LEN;
	list->num = 0;
	list->elements = (char **)mmap(NULL, list->len * sizeof(char *), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (list->elements == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    return list;
}

int str_list_push_back(StrList *list, char *element)
{
    if (list->num == list->len) {
        size_t new_len = list->len * STR_LIST_RESIZE_FACTOR;

        list->elements = (char **)mremap(list->elements, list->len * sizeof(char *), new_len * sizeof(char *), MREMAP_MAYMOVE);

        if (list->elements == MAP_FAILED) {
            return -1;
        }

        list->len = new_len;
    }

    list->elements[list->num] = element;

    list->num++;

    return 0;
}

