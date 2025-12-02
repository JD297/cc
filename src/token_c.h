#include "token_type_c.h"

#include <jd297/sv.h>

#include <stddef.h>

#ifndef JD297_CC_TOKEN_C_H
#define JD297_CC_TOKEN_C_H

typedef struct {
    TokenType_C type;
    sv_t view;
    
    union {
    	int d;
    	unsigned int u;
    	long int ld;
    	unsigned long int lu;
    	float f;
    	double lf;
    	long double Lf;
    	sv_t sv;
    } literal;
} Token_C;

#endif
