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

/**
 * Retrieves an element from a TokenListNamed_C by its associated name.
 *
 * DESCRIPTION
 *     This function searches for a token in the TokenListNamed_C structure 
 *     that matches the specified name. If a matching token is found, a pointer 
 *     to the corresponding TokenList_C is returned. If no matching token is 
 *     found, NULL is returned.
 *
 * RETURN VALUE
 *     On success, returns a pointer to the TokenList_C associated with the 
 *     specified name. If the name is not found, returns NULL.
 */
extern TokenList_C *token_list_named_c_get(TokenListNamed_C *list, const char *name);

/**
 * Adds a named token list to a TokenListNamed_C.
 *
 * DESCRIPTION
 *     This function adds the specified token list (element) to the TokenListNamed_C. 
 *     The token list can be added either at the end of the list or in an empty (NULL) 
 *     slot between the beginning and the end of the list. If a key (name) already 
 *     exists, the existing entry will be overwritten with the new token list.
 *
 * RETURN VALUE
 *     On success, returns 0. On error, returns -1 and sets errno to indicate 
 *     the specific error encountered.
 */
extern int token_list_named_c_add(TokenListNamed_C *list, TokenList_C *element, const char *name);

/**
 * Removed an element from a TokenListNamed_C.
 *
 * DESCRIPTION
 *     This function removes an element by name from the TokenListNamed_C.
 */
extern void token_list_named_c_remove(TokenListNamed_C *list, const char *name);

#endif

