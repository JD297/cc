#define _GNU_SOURCE

#include "lexer_list_c.h"
#include "lexer_c.h"

#include <stddef.h>
#include <sys/mman.h>

void *lexer_list_c_create()
{
    LexerList_C *list = (LexerList_C *)mmap(NULL, sizeof(LexerList_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (list == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    list->len = LEXER_LIST_INIT_LEN;
	list->num = 0;
	list->elements = (Lexer_C **)mmap(NULL, list->len * sizeof(Lexer_C *), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (list->elements == MAP_FAILED) {
		return LIST_CREATION_FAILED;
	}

    return list;
}

int lexer_list_c_push_back(LexerList_C *list, Lexer_C *element)
{
    if (list->num == list->len) {
        size_t new_len = list->len * LEXER_LIST_RESIZE_FACTOR;

        list->elements = (Lexer_C **)mremap(list->elements, list->len * sizeof(Lexer_C *), new_len * sizeof(Lexer_C *), MREMAP_MAYMOVE);

        if (list->elements == MAP_FAILED) {
            return -1;
        }

        list->len = new_len;
    }

    list->elements[list->num] = element;

    list->num++;

    return 0;
}

