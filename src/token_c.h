#include "ir.h"
#include "token_type_c.h"

#include <jd297/sv.h>

#include <stddef.h>

#ifndef JD297_CC_TOKEN_C_H
#define JD297_CC_TOKEN_C_H

typedef struct {
    TokenType_C type;
    sv_t view;
    IRLiteral literal;
} Token_C;

#endif
