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

	ctx = calloc(1, sizeof(IR_CTX));

	assert(ctx != NULL);

	code = malloc(sizeof(list_t));

	assert(code != NULL);
	
	assert(list_create(code) != -1);

	ctx->code = code;
	ctx->symtbl = symtbl;
	
	return ctx;
}

void ir_ctx_destroy(IR_CTX *ctx)
{
	assert(ctx != NULL);
	assert(ctx->code != NULL);

	list_free(ctx->code);

	free(ctx->code);
	free(ctx);
}

void ir_emit_push(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, /* IRRegister|IRLiteral*/...)
{
	va_list ap;
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);
	
	va_start(ap, atype1);

	code->op = IR_OC_PUSH;
	code->atype1 = atype1;
	code->ptype = ptype;
	
	switch (atype1) {
		case IR_ATYPE_REG:
			code->arg1.reg = va_arg(ap, IRRegister);
			break;
		case IR_ATYPE_LITERAL:
			code->arg1.literal = va_arg(ap, IRLiteral);
			break;
		default:
			assert(0 && "IRArgType atype must be IR_TYPE_REG or IR_TYPE_LITERAL");
	}

	va_end(ap);
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_pop(IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_POP;
	code->ptype = ptype;
	code->atype1 = IR_ATYPE_REG;
	code->arg1.reg = arg1;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_func_begin(IR_CTX *ctx, sv_t *result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_FUNC_BEGIN;
	code->arg1.num = 0;
	code->result.view = result;

	ctx->label_func_end = ctx->label_tmp++;

	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_func_end(IR_CTX *ctx)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_FUNC_END;
	code->atype1 = IR_ATYPE_NUM;
	code->arg1.num = 0;
	code->rtype = IR_ATYPE_NUM;
	code->result.num = ctx->label_func_end;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

#define IR_EMIT_BIN_OP_IMPL(_IROpCode_)\
	va_list ap;\
	IRCode *code;\
\
	assert(ctx != NULL);\
\
	code = malloc(sizeof(IRCode));\
\
	assert(code != NULL);\
\
	va_start(ap, rreg);\
\
	code->op = (_IROpCode_);\
	code->ptype = ptype;\
	code->atype1 = atype1;\
	code->atype2 = atype2;\
	code->result.reg = rreg;\
\
	switch (atype1) {\
		case IR_ATYPE_REG:\
			code->arg1.reg = va_arg(ap, IRRegister);\
			break;\
		case IR_ATYPE_LITERAL:\
			code->arg1.literal = va_arg(ap, IRLiteral);\
			break;\
		default:\
			assert(0 && "IRArgType atype must be IR_TYPE_REG or IR_TYPE_LITERAL");\
	}\
\
	switch (atype2) {\
		case IR_ATYPE_REG:\
			code->arg2.reg = va_arg(ap, IRRegister);\
			break;\
		case IR_ATYPE_LITERAL:\
			code->arg2.literal = va_arg(ap, IRLiteral);\
			break;\
		default:\
			assert(0 && "IRArgType atype must be IR_TYPE_REG or IR_TYPE_LITERAL");\
	}\
\
	va_end(ap);\
\
	list_insert(ctx->code, list_end(ctx->code), code)

void ir_emit_sal(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_SAL);
}

void ir_emit_sar(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_SAR);
}

void ir_emit_add(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_ADD);
}

void ir_emit_sub(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_SUB);
}

void ir_emit_mul(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_MUL);
}

void ir_emit_div(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_DIV);
}

void ir_emit_mod(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_MOD);
}

void ir_emit_or(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_OR);
}

void ir_emit_xor(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_XOR);
}

void ir_emit_and(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_AND);
}

void ir_emit_eq(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_EQ);
}

void ir_emit_neq(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_NEQ);
}

void ir_emit_gt(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_GT);
}

void ir_emit_lt(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_LT);
}

void ir_emit_gte(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_GTE);
}

void ir_emit_lte(IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral */...)
{
	IR_EMIT_BIN_OP_IMPL(IR_OC_LTE);
}

void ir_emit_jmp_zero(IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1, size_t result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_JMP_ZERO;
	code->ptype = ptype;
	code->atype1 = IR_ATYPE_REG;
	code->arg1.reg = arg1;
	code->result.num = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_jmp_not_zero(IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1, size_t result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_JMP_NOT_ZERO;
	code->ptype = ptype;
	code->atype1 = IR_ATYPE_REG;
	code->arg1.reg = arg1;
	code->result.num = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_string(IR_CTX *ctx, IRLiteral arg1, size_t arg2, size_t result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_STRING;
	code->atype1 = IR_ATYPE_LITERAL;
	code->arg1.literal = arg1;
	code->atype2 = IR_ATYPE_NUM;
	code->arg2.num = arg2;
	code->result.num = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_load_string(IR_CTX *ctx, size_t arg1, IRRegister result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_LOAD_STRING;
	code->atype1 = IR_ATYPE_NUM;
	code->arg1.num = arg1;

	code->rtype = IR_ATYPE_REG;
	code->result.reg = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_param_push(IR_CTX *ctx, size_t arg2, IRPrimitiveType ptype, IRArgType atype1, /* IRRegister|IRLiteral*/...)
{
	va_list ap;
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);
	
	va_start(ap, atype1);

	code->op = IR_OC_PARAM_PUSH;
	code->ptype = ptype;
	code->atype1 = atype1;
	code->arg2.num = arg2;

	switch (atype1) {
		case IR_ATYPE_REG:
			code->arg1.reg = va_arg(ap, IRRegister);
			break;
		case IR_ATYPE_LITERAL:
			code->arg1.literal = va_arg(ap, IRLiteral);
			break;
		default:
			assert(0 && "IRArgType atype must be IR_TYPE_REG or IR_TYPE_LITERAL");
	}

	va_end(ap);
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_param_pop(IR_CTX *ctx, IRPrimitiveType ptype, size_t arg1)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_PARAM_POP;
	code->atype1 = IR_ATYPE_NUM;
	code->ptype = ptype;
	code->arg1.num = arg1;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_call(IR_CTX *ctx, size_t arg1, sv_t *result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_CALL;
	code->arg1.num = arg1;
	code->result.view = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_imm(IR_CTX *ctx, IRPrimitiveType ptype, IRLiteral arg1, IRRegister result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_IMM;
	code->ptype = ptype;
	code->atype1 = IR_ATYPE_LITERAL;
	code->arg1.literal = arg1;
	code->result.reg = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_local(IR_CTX *ctx, IRPrimitiveType ptype, size_t *result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_LOCAL;
	code->ptype = ptype;
	code->rtype = IR_ATYPE_ADDR;
	code->result.addr = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_store(IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1, size_t *result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_STORE;
	code->ptype = ptype;
	code->atype1 = IR_ATYPE_REG;
	code->arg1.reg = arg1;
	code->rtype = IR_ATYPE_ADDR;
	code->result.addr = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_load(IR_CTX *ctx, IRPrimitiveType ptype, size_t *arg1, IRRegister result)
{
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	code->op = IR_OC_LOAD;
	code->ptype = ptype;
	code->atype1 = IR_ATYPE_ADDR;
	code->arg1.addr = arg1;
	code->rtype = IR_ATYPE_REG;
	code->result.reg = result;
	
	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_jmp(IR_CTX *ctx, IRArgType rtype, /* size_t|size_t* result */...)
{
	va_list ap;
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);
	
	va_start(ap, rtype);
	
	code->op = IR_OC_JMP;
	code->rtype = rtype;
	
	switch (rtype) {
		case IR_ATYPE_NUM:
			code->result.num = va_arg(ap, size_t);
			break;
		case IR_ATYPE_ADDR:
			code->result.addr = va_arg(ap, size_t *);
			break;
		default:
			assert(0 && "IRArgType rtype must be IR_ATYPE_NUM or IR_ATYPE_ADDR");
	}
	

	va_end(ap);

	list_insert(ctx->code, list_end(ctx->code), code);
}

void ir_emit_label(IR_CTX *ctx, IRArgType rtype, /* size_t|size_t* result */...)
{
	va_list ap;
	IRCode *code;
	
	assert(ctx != NULL);
	
	code = malloc(sizeof(IRCode));

	assert(code != NULL);

	va_start(ap, rtype);

	code->op = IR_OC_LABEL;
	code->rtype = rtype;

	switch (rtype) {
		case IR_ATYPE_NUM:
			code->result.num = va_arg(ap, size_t);
			break;
		case IR_ATYPE_ADDR:
			code->result.addr = va_arg(ap, size_t *);
			break;
		default:
			assert(0 && "IRArgType rtype must be IR_ATYPE_NUM or IR_ATYPE_ADDR");
	}

	va_end(ap);
	
	list_insert(ctx->code, list_end(ctx->code), code);
}
