#include <stddef.h>
#include <unistd.h>

#ifndef JD297_CC_STR_LIST_C_H
#define JD297_CC_STR_LIST_C_H

/**
 * List structure for strings.
 *
 * MEMBERS
 *     elements  : Pointer to an array of char pointers,
 *                 representing the strings.
 *
 *     len       : The allocated capacity for the number of elements
 *                 that can be stored in elements.
 *
 *     num       : The current count of elements stored in elements.
 */
typedef struct StrList {
    char **elements;
    size_t len;
    size_t num;
} StrList;


#define LIST_CREATION_FAILED (void *) -1

#define STR_LIST_INIT_LEN sysconf(_SC_PAGESIZE) / sizeof(char *)
#define STR_LIST_RESIZE_FACTOR 2

/**
 * Creates a string list structure.
 *
 * DESCRIPTION
 *     This function allocates and initializes a new StrList structure, 
 *     which can be used to store char pointer. The initial 
 *     capacity of the list is set to a predefined value.
 *
 * RETURN VALUE
 *     On success, a pointer to the created StrList is returned. On error, 
 *     the value LIST_CREATION_FAILED (i.e., (void *) -1) is returned, and 
 *     errno is set to indicate the specific error.
 */
extern void *str_list_create();


/**
 * Adds a string to the end of the StrList.
 *
 * DESCRIPTION
 *     The function appends the specified string (element) to the end of the 
 *     StrList (list). If the number of elements (list->num) reaches the 
 *     capacity (list->len), the elements array will be resized to accommodate 
 *     additional char pointer.
 *
 * RETURN VALUE
 *     On success, 0 is returned. On error, -1 is returned, and errno is set 
 *     to indicate the specific error.
 */
extern int str_list_push_back(StrList *list, char *element);

#endif

