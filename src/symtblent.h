#include "lexer_c.h"

#include <stddef.h>

#ifndef JD297_CC_SYMTBLENT_H
#define JD297_CC_SYMTBLENT_H

typedef enum {
	I32,
	I64
} symtbltype;

typedef enum {
	FUNCTION,
	LABEL,
	VARIABLE
} symtbluse;

typedef struct {
	int ptrc; // 0 = no pointer, 1 = *, 2 = **, ...
	symtbltype type;
	symtbluse use;
	size_t addr_r2b;
	Lexer_Location_C loc;
} symtblent;

#endif
