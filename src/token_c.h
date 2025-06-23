#include "token_type_c.h"

#include <stddef.h>

#ifndef JD297_CC_TOKEN_C_H
#define JD297_CC_TOKEN_C_H

typedef struct {
    TokenType_C type;
    const char *value;
    size_t len;
} Token_C;

#endif
