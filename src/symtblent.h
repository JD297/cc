#include "lexer_c.h"

#include <stddef.h>

#ifndef JD297_CC_SYMTBLENT_H
#define JD297_CC_SYMTBLENT_H

typedef enum {
	I32,
	I64
} SymtblType;

typedef enum {
	FUNCTION,
	LABEL,
	VARIABLE
} SymtblUse;

typedef struct {
	int ptrc; // 0 = no pointer, 1 = *, 2 = **, ...
	char *id;
	void *value;
	SymtblType type;
	SymtblUse use;
	size_t addr_r2b;
	Lexer_Location_C loc;
} SymtblEnt;

#endif
