#include "token_type_c.h"

#include <stddef.h>

#ifndef JD297_CC_TOKEN_C_H
#define JD297_CC_TOKEN_C_H

/**
 * Token structure for the C language.
 *
 * DESCRIPTION
 *     This structure represents a token in the C language. It is used to 
 *     store information about the type and value of the token.
 *
 * MEMBERS
 *     type   : Specifies the type of the token, represented by
 *              the TokenType_C enum.
 *
 *     value  : A pointer to the beginning of the string that
 *              contains the token's value.
 */
typedef struct Token_C {
    TokenType_C type;
    const char *value;
    size_t len;
} Token_C;


#define TOKEN_CREATION_FAILED (void *) -1

/**
 * Creates a Token_C structure from the specified type and value.
 *
 * DESCRIPTION
 *     This function allocates and initializes a new Token_C structure using 
 *     the provided token type and value. The value is copied into the 
 *     structure, and the type is set accordingly.
 *
 * RETURN VALUE
 *     On success, a pointer to the created Token_C is returned. On error, 
 *     the value TOKEN_CREATION_FAILED (i.e., (void *) -1) is returned, and 
 *     errno is set to indicate the specific error.
 */
extern void *token_c_create(TokenType_C type, const char *value, size_t len);

#endif

