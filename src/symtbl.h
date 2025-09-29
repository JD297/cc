#include "lexer_c.h"

#include <jd297/sv.h>
#include <jd297/lmap_sv.h>
#include <jd297/vector.h>

#include <stddef.h>

#ifndef JD297_CC_SYMTBL_H
#define JD297_CC_SYMTBL_H

typedef enum {
	I32,
	I64
} SymTblEntType;

typedef enum {
	FUNCTION,
	LABEL,
	VARIABLE
} SymTblEntUse;

typedef struct {
	sv_t *id;
	SymTblEntType type;
	SymTblEntUse use;
	size_t addr;
	Lexer_Location_C loc;
} SymTblEnt;

typedef struct SymTbl {
	sv_t *id;
	struct SymTbl *parent;
	lmap_sv_t entries;
} SymTbl;

extern vector_t symtbl_refs;

extern SymTbl *symtbl_create(sv_t *id, SymTbl *parent);

extern void symtbl_free(SymTbl *tbl);

extern int symtbl_add_entry(SymTbl *tbl, sv_t *id, SymTblEntType type, SymTblEntUse use);

extern SymTblEnt *symtbl_get(SymTbl *tbl, sv_t *id);

extern SymTblEnt *symtbl_function(SymTbl *tbl);

#endif
