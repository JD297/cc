#include "token_c.h"

#include <stddef.h>
#include <unistd.h>

#ifndef JD297_CC_TOKEN_LIST_C_H
#define JD297_CC_TOKEN_LIST_C_H

/**
 * List structure for tokens in the C language.
 *
 * MEMBERS
 *     elements  : Pointer to an array of Token_C pointers,
 *                 representing the tokens.
 *
 *     len       : The allocated capacity for the number of elements
 *                 that can be stored in elements.
 *
 *     num       : The current count of elements stored in elements.
 */
typedef struct TokenList_C {
    Token_C **elements;
    size_t len;
    size_t num;
} TokenList_C;


#define LIST_CREATION_FAILED (void *) -1

#define LIST_INIT_LEN sysconf(_SC_PAGESIZE) / sizeof(Token_C *)
#define LIST_RESIZE_FACTOR 2

/**
 * Creates a token list structure for the C language.
 *
 * DESCRIPTION
 *     This function allocates and initializes a new TokenList_C structure, 
 *     which can be used to store tokens for the C language. The initial 
 *     capacity of the list is set to a predefined value.
 *
 * RETURN VALUE
 *     On success, a pointer to the created TokenList_C is returned. On error, 
 *     the value LIST_CREATION_FAILED (i.e., (void *) -1) is returned, and 
 *     errno is set to indicate the specific error.
 */
extern void *token_list_c_create();


/**
 * Adds a token to the end of the TokenList_C.
 *
 * DESCRIPTION
 *     The function appends the specified token (element) to the end of the 
 *     TokenList_C (list). If the number of elements (list->num) reaches the 
 *     capacity (list->len), the elements array will be resized to accommodate 
 *     additional tokens.
 *
 * RETURN VALUE
 *     On success, 0 is returned. On error, -1 is returned, and errno is set 
 *     to indicate the specific error.
 */
extern int token_list_c_push_back(TokenList_C *list, Token_C *element);

#endif

