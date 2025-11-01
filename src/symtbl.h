#include "lexer_c.h"

#include <jd297/sv.h>
#include <jd297/lmap_sv.h>
#include <jd297/vector.h>

#include <stddef.h>

#ifndef JD297_CC_SYMTBL_H
#define JD297_CC_SYMTBL_H

typedef enum {
	REG,
	INT
} SymTblEntType;

typedef enum {
	REGISTER,
	FUNCTION,
	LABEL,
	LOCAL,
	CONST
} SymTblEntUse;

typedef struct {
	sv_t *id;
	SymTblEntType type;
	SymTblEntUse use;
	size_t addr;
	Lexer_Location_C loc;
	sv_t val;
} SymTblEnt;

typedef struct SymTbl {
	sv_t *id;
	struct SymTbl *parent;
	lmap_sv_t entries;
} SymTbl;

extern vector_t symtbl_refs;

extern SymTbl *symtbl_create(sv_t *id, SymTbl *parent);

extern void symtbl_free(SymTbl *tbl);

extern SymTblEnt *symtbl_add_entry(SymTbl *tbl, sv_t *id, SymTblEntType type, SymTblEntUse use, sv_t *val);

extern SymTblEnt *symtbl_get(SymTbl *tbl, sv_t *id);

extern SymTblEnt *symtbl_function(SymTbl *tbl);

#endif
