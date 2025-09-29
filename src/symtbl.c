#include <stdlib.h>

#include <jd297/sv.h>
#include <jd297/lmap_sv.h>
#include <jd297/vector.h>

#include "symtbl.h"

vector_t symtbl_refs;

SymTbl *symtbl_create(sv_t *id, SymTbl *parent)
{
	SymTbl *tbl = calloc(1, sizeof(SymTbl));

	if (tbl == NULL) {
		return NULL;
	}

	tbl->id = id;
	tbl->parent = parent;

	vec_push_back(&symtbl_refs, tbl);

	return tbl;
}

void symtbl_free(SymTbl *tbl)
{
	lmap_sv_free(&tbl->entries);

	free(tbl);
}

int symtbl_add_entry(SymTbl *tbl, sv_t *id, SymTblEntType type, SymTblEntUse use)
{
	SymTblEnt *ent = calloc(1, sizeof(SymTblEnt));

	if (ent == NULL) {
		return -1;	
	}

	ent->id = id;
	ent->type = type;
	ent->use = use;

	return lmap_sv_add(&tbl->entries, id, ent);
}

SymTblEnt *symtbl_get(SymTbl *tbl, sv_t *id)
{
	SymTblEnt *ent = lmap_sv_get(&tbl->entries, id);

	if (ent != NULL) {
		return ent;
	}

	if (tbl->parent == NULL) {
		return NULL;
	}

	return symtbl_get(tbl->parent, id);
}

SymTblEnt *symtbl_function(SymTbl *tbl)
{
	if (tbl->id == NULL) {
		if (tbl->parent == NULL) {
			return NULL;
		}

		return symtbl_function(tbl->parent);
	}

	SymTblEnt *ent = symtbl_get(tbl, tbl->id);

	if (ent == NULL) {
		return NULL;
	}

	if (ent->use == FUNCTION) {
		return ent;
	}

	return symtbl_function(tbl->parent);
}
