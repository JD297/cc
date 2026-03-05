#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <jd297/sv.h>
#include <jd297/list.h>
#include <jd297/lmap_sv.h>
#include <jd297/vector.h>

#include "ir.h"

IRLiteral ir_literal_from_d(int d)
{
	IRLiteral lit;
	
	lit.d = d;
	
	return lit;
}

IRLiteral ir_literal_from_u(unsigned int u)
{
	IRLiteral lit;
	
	lit.u = u;
	
	return lit;
}

IRLiteral ir_literal_from_ld(long int ld)
{
	IRLiteral lit;
	
	lit.ld = ld;
	
	return lit;
}

IRLiteral ir_literal_from_lu(unsigned long int lu)
{
	IRLiteral lit;
	
	lit.lu = lu;
	
	return lit;
}

IRLiteral ir_literal_from_f(float f)
{
	IRLiteral lit;
	
	lit.f = f;
	
	return lit;
}

IRLiteral ir_literal_from_lf(double lf)
{
	IRLiteral lit;
	
	lit.lf = lf;
	
	return lit;
}

IRLiteral ir_literal_from_Lf(long double Lf)
{
	IRLiteral lit;
	
	lit.Lf = Lf;
	
	return lit;
}

IRLiteral ir_literal_from_sv(sv_t sv)
{
	IRLiteral lit;
	
	lit.sv = sv;
	
	return lit;
}

vector_t ir_symtbl_refs;

IRSymTbl *ir_symtbl_create(sv_t *id, IRSymTbl *parent)
{
	IRSymTbl *tbl = calloc(1, sizeof(IRSymTbl));

	if (tbl == NULL) {
		return NULL;
	}

	tbl->id = id;
	tbl->parent = parent;

	vec_push_back(&ir_symtbl_refs, tbl);

	return tbl;
}

void ir_symtbl_free(IRSymTbl *tbl)
{
	lmap_sv_free(&tbl->entries);
	lmap_sv_free(&tbl->labels);

	free(tbl);
}

IRSymTblEnt *ir_symtbl_add_entry(IRSymTbl *tbl, sv_t *id, IRPrimitiveType type, IRSymTblEntUse use)
{
	IRSymTblEnt *ent = calloc(1, sizeof(IRSymTblEnt));

	if (ent == NULL) {
		return NULL;	
	}

	ent->id = id;
	ent->type = type;
	ent->use = use;

	switch (use) {
		case IR_SYMUSE_LABEL: {
			IRSymTbl *func_tbl;
			
			func_tbl = ir_symtbl_current_function(tbl);

			if (func_tbl == NULL) {
				return NULL;
			}
			
			if (lmap_sv_add(&func_tbl->labels, id, ent) == -1) {
				free(ent);
				
				return NULL;
			}
		} break;
		default: {
			if (lmap_sv_add(&tbl->entries, id, ent) == -1) {
				free(ent);
				
				return NULL;
			}
		} break;
	}
	
	return ent;
}

IRSymTblEnt *ir_symtbl_get(IRSymTbl *tbl, sv_t *id, IRSymTblEntUse use)
{
	IRSymTblEnt *ent;
	
	switch (use) {
		case IR_SYMUSE_LABEL:
			ent = lmap_sv_get(&tbl->labels, id);
			break;
		default:
			ent = lmap_sv_get(&tbl->entries, id);
			break;
	}

	if (ent != NULL) {
		return ent;
	}

	if (tbl->parent == NULL) {
		return NULL;
	}

	return ir_symtbl_get(tbl->parent, id, use);
}

IRSymTblEnt *ir_symtbl_function(IRSymTbl *tbl)
{
	if (tbl->id == NULL) {
		if (tbl->parent == NULL) {
			return NULL;
		}

		return ir_symtbl_function(tbl->parent);
	}

	IRSymTblEnt *ent = ir_symtbl_get(tbl, tbl->id, IR_SYMUSE_FUNCTION);

	if (ent == NULL) {
		return NULL;
	}

	if (ent->use == IR_SYMUSE_FUNCTION) {
		return ent;
	}

	return ir_symtbl_function(tbl->parent);
}

IRSymTbl *ir_symtbl_current_function(IRSymTbl *tbl)
{
	IRSymTbl *func_tbl = tbl;

	while (1) {
		if (func_tbl->id == NULL) {
			if (func_tbl->parent == NULL) {
				return NULL;
			}

			func_tbl = func_tbl->parent;

			continue;
		}

		IRSymTblEnt *ent = ir_symtbl_get(func_tbl, func_tbl->id, IR_SYMUSE_FUNCTION);

		if (ent == NULL) {
			return NULL;
		}

		if (ent->use == IR_SYMUSE_FUNCTION) {
			return func_tbl;
		}
		
		func_tbl = func_tbl->parent;
	}
	
	return NULL;
}

IR_CTX *ir_ctx_create(IRSymTbl *symtbl)
{
	IR_CTX *ctx;
	list_t *code;
	list_t *ssa;

	ctx = calloc(1, sizeof(IR_CTX));

	assert(ctx != NULL);

	code = malloc(sizeof(list_t));

	assert(code != NULL);
	
	assert(list_create(code) != -1);

	ssa = malloc(sizeof(list_t));

	assert(ssa != NULL);
	
	assert(list_create(ssa) != -1);

	ctx->code = code;
	ctx->ssa = ssa;
	ctx->symtbl = symtbl;
	
	return ctx;
}

void ir_ctx_destroy(IR_CTX *ctx)
{
	assert(ctx != NULL);
	assert(ctx->code != NULL);
	assert(ctx->ssa != NULL);

	/* TODO free entries in code and ssa */

	list_free(ctx->code);
	list_free(ctx->ssa);

	free(ctx->code);
	free(ctx->ssa);
	free(ctx);
}

IRSSAEnt *ir_ssa_latest(IR_CTX *ctx)
{
	return ctx->ssa_latest;
	// return (IRSSAEnt *)list_prev(list_end(ctx->ssa))->value;
}

IRSSAEnt *ir_ssa_default(IR_CTX *ctx)
{
	IRSSAEnt *ssa = malloc(sizeof(IRSSAEnt));

	ssa->type = IR_ATYPE_NUM;
	ssa->as.num = 0;

	list_insert(ctx->ssa, list_end(ctx->ssa), ssa);

	return ssa;
}

IRSSAEnt *ir_ssa_from_view(IR_CTX *ctx, sv_t *view)
{
	IRSSAEnt *ssa = malloc(sizeof(IRSSAEnt));

	ssa->type = IR_ATYPE_VIEW;
	ssa->as.view = view;

	list_insert(ctx->ssa, list_end(ctx->ssa), ssa);

	return ssa;
}

IRSSAEnt *ir_ssa_from_num(IR_CTX *ctx, size_t num)
{
	IRSSAEnt *ssa = malloc(sizeof(IRSSAEnt));

	ssa->type = IR_ATYPE_NUM;
	ssa->as.num = num;

	list_insert(ctx->ssa, list_end(ctx->ssa), ssa);

	return ssa;
}

IRSSAEnt *ir_ssa_from_stack(IR_CTX *ctx, size_t *stack)
{
	IRSSAEnt *ssa = malloc(sizeof(IRSSAEnt));

	ssa->type = IR_ATYPE_STACK;
	ssa->as.stack = stack;

	list_insert(ctx->ssa, list_end(ctx->ssa), ssa);

	return ssa;
}

IRSSAEnt *ir_ssa_from_addr(IR_CTX *ctx, size_t *addr)
{
	IRSSAEnt *ssa = malloc(sizeof(IRSSAEnt));

	ssa->type = IR_ATYPE_ADDR;
	ssa->as.addr = addr;

	list_insert(ctx->ssa, list_end(ctx->ssa), ssa);

	return ssa;
}

IRSSAEnt *ir_ssa_from_literal(IR_CTX *ctx, IRLiteral literal)
{
	IRSSAEnt *ssa = malloc(sizeof(IRSSAEnt));

	ssa->type = IR_ATYPE_LIT;
	ssa->as.literal = literal;

	list_insert(ctx->ssa, list_end(ctx->ssa), ssa);

	return ssa;
}

IRSSAEnt *ir_ssa_from_reg(IR_CTX *ctx, size_t reg)
{
	IRSSAEnt *ssa = malloc(sizeof(IRSSAEnt));

	ssa->type = IR_ATYPE_REG;
	ssa->as.reg = reg;

	list_insert(ctx->ssa, list_end(ctx->ssa), ssa);

	return ssa;
}

void ir_emit(IR_CTX *ctx, IROpCode op, IRPrimitiveType ptype, IRSSAEnt *result, IRSSAEnt *arg1, IRSSAEnt *arg2)
{
	IRCode *code;

	assert(ctx != NULL);

	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = op;
	code->ptype = ptype;
	code->arg1 = arg1;
	code->arg2 = arg2;
	code->result = result;

	list_insert(ctx->code, list_end(ctx->code), code);
	
	if (result != NULL) {
		ctx->ssa_latest = result;
	}
}
