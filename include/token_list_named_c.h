#include "token_list_c.h"

#include <stddef.h>
#include <unistd.h>

#ifndef JD297_CC_TOKEN_LIST_NAMED_C_H
#define JD297_CC_TOKEN_LIST_NAMED_C_H

/**
 * Structure representing a list of named token lists in the C language.
 *
 * MEMBERS
 *     elements       : A pointer to an array of TokenList_C pointers, 
 *                      representing the token lists.
 *
 *     element_names  : An array of strings containing the names for the 
 *                      elements at the corresponding indices in the 
 *                      elements array.
 *
 *     len            : The allocated capacity for the number of elements 
 *                      that can be stored in the elements array.
 *
 *     num            : The current count of elements stored in the 
 *                      elements array.
 */
typedef struct TokenListNamed_C {
    TokenList_C **elements;
    char **elements_names;
    size_t len;
    size_t num;
} TokenListNamed_C;


#define LIST_CREATION_FAILED (void *) -1

#define TOKEN_LIST_NAMED_INIT_LEN sysconf(_SC_PAGESIZE) / sizeof(TokenList_C *)
#define TOKEN_LIST_NAMED_RESIZE_FACTOR 2

/**
 * Creates and initializes a named token list structure for the C language.
 *
 * DESCRIPTION
 *     This function allocates and initializes a new TokenListNamed_C structure, 
 *     which can be used to store a named token list for the C language. The initial 
 *     capacity of the list is set to a predefined value.
 *
 * RETURN VALUE
 *     On success, returns a pointer to the newly created TokenListNamed_C. 
 *     On error, returns LIST_CREATION_FAILED (i.e., (void *) -1), and 
 *     sets errno to indicate the specific error encountered.
 */
extern void *token_list_named_c_create();

/**
 * Appends a named token list to the end of a TokenListNamed_C.
 *
 * DESCRIPTION
 *     This function adds the specified token list (element) to the end of the 
 *     TokenListNamed_C (list) with the associated identifier (name). If the 
 *     number of elements (list->num) reaches the current capacity (list->len), 
 *     the elements array will be resized to accommodate additional tokens.
 *
 * RETURN VALUE
 *     On success, returns 0. On error, returns -1, and sets errno to indicate 
 *     the specific error encountered.
 */
extern int token_list_named_c_push_back(TokenListNamed_C *list, TokenList_C *element, char *name);

#endif

