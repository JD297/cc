#include "lexer_c.h"

#include <stddef.h>
#include <unistd.h>

#ifndef JD297_CC_LEXER_LIST_C_H
#define JD297_CC_LEXER_LIST_C_H

/**
 * List structure for lexers in the C language.
 *
 * MEMBERS
 *     elements  : Pointer to an array of Lexer_C pointers.
 *
 *     len       : The allocated capacity for the number of elements
 *                 that can be stored in elements.
 *
 *     num       : The current count of elements stored in elements.
 */
typedef struct LexerList_C {
    Lexer_C **elements;
    size_t len;
    size_t num;
} LexerList_C;

#define LIST_CREATION_FAILED (void *) -1

#define LEXER_LIST_INIT_LEN sysconf(_SC_PAGESIZE) / sizeof(Lexer_C *)
#define LEXER_LIST_RESIZE_FACTOR 2

/**
 * Creates a lexer list structure for the C language.
 *
 * DESCRIPTION
 *     This function allocates and initializes a new LexerList_C structure, 
 *     which can be used to store lexers for the C language. The initial 
 *     capacity of the list is set to a predefined value.
 *
 * RETURN VALUE
 *     On success, a pointer to the created LexerList_C is returned. On error, 
 *     the value LIST_CREATION_FAILED (i.e., (void *) -1) is returned, and 
 *     errno is set to indicate the specific error.
 */
extern void *lexer_list_c_create();


/**
 * Adds a lexer to the end of the LexerList_C.
 *
 * DESCRIPTION
 *     The function appends the specified lexer (element) to the end of the 
 *     LexerList_C (list). If the number of elements (list->num) reaches the 
 *     capacity (list->len), the elements array will be resized to accommodate 
 *     additional tokens.
 *
 * RETURN VALUE
 *     On success, 0 is returned. On error, -1 is returned, and errno is set 
 *     to indicate the specific error.
 */
extern int lexer_list_c_push_back(LexerList_C *list, Lexer_C *element);

#endif

