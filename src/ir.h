#ifndef JD297_CC_IR_H
#define JD297_CC_IR_H

#include <stddef.h>

#include <jd297/sv.h>
#include <jd297/list.h>
#include <jd297/vector.h>
#include <jd297/lmap_sv.h>

typedef enum {
	IR_U8_T,
	IR_S8_T,
	IR_U16_T,
	IR_S16_T,
	IR_U32_T,
	IR_S32_T,
	IR_U64_T,
	IR_S64_T,
	IR_F32_T,
	IR_F64_T,
	IR_PTR_T,
} IRPrimitiveType;

typedef union {
	int d;
	unsigned int u;
	long int ld;
	unsigned long int lu;
	float f;
	double lf;
	long double Lf;
	sv_t sv;
} IRLiteral;

extern IRLiteral ir_literal_from_d(int d);
extern IRLiteral ir_literal_from_u(unsigned int u);
extern IRLiteral ir_literal_from_ld(long int ld);
extern IRLiteral ir_literal_from_lu(unsigned long int lu);
extern IRLiteral ir_literal_from_f(float d);
extern IRLiteral ir_literal_from_lf(double d);
extern IRLiteral ir_literal_from_Lf(long double Lf);
extern IRLiteral ir_literal_from_sv(sv_t sv);

typedef enum {
	IR_SYMUSE_FUNCTION,
	IR_SYMUSE_LABEL,
	IR_SYMUSE_LOCAL,
	IR_SYMUSE_DATA,
	IR_SYMUSE_CLASS,
} IRSymTblEntUse;

typedef struct {
	sv_t *id;
	IRPrimitiveType type;
	IRSymTblEntUse use;
	size_t addr;
	// Lexer_Location_C loc;
} IRSymTblEnt;

typedef struct IRSymTbl {
	sv_t *id;
	struct IRSymTbl *parent;
	lmap_sv_t entries;
	lmap_sv_t labels;
	/* TODO maybe */
	/* lmap_sv_t locals; */
	/* lmap_sv_t ...; */
} IRSymTbl;

extern vector_t ir_symtbl_refs;

extern IRSymTbl *ir_symtbl_create(sv_t *id, IRSymTbl *parent);

extern void ir_symtbl_free(IRSymTbl *tbl);

extern IRSymTblEnt *ir_symtbl_add_entry(IRSymTbl *tbl, sv_t *id, IRPrimitiveType type, IRSymTblEntUse use);

extern IRSymTblEnt *ir_symtbl_get(IRSymTbl *tbl, sv_t *id, IRSymTblEntUse use);

extern IRSymTblEnt *ir_symtbl_function(IRSymTbl *tbl);

extern IRSymTbl *ir_symtbl_current_function(IRSymTbl *tbl);

typedef struct IRSSAEnt IRSSAEnt;

typedef struct {
	list_t *code;
	list_t *ssa;
	IRSymTbl *symtbl;
	IRSSAEnt *ssa_latest;
	IRSSAEnt *switch_expression;
	list_node_t *switch_stmt_section;
	list_node_t *switch_test_section;
	list_node_t *code_current;
	size_t stack_offset;
	size_t label_tmp;
	size_t label_func_end;
	size_t label_iter_begin;
	size_t label_iter_end;
	size_t label_select_begin;
	size_t label_select_end;
	size_t label_str;
} IR_CTX;

extern IR_CTX *ir_ctx_create(IRSymTbl *symtbl);
extern void ir_ctx_destroy(IR_CTX *ctx);

typedef enum {
	IR_OC_FUNC_BEGIN,
	IR_OC_FUNC_END,
	IR_OC_IMM,
	IR_OC_PUSH,
	IR_OC_POP,
	IR_OC_SAL,
	IR_OC_SAR,
	IR_OC_ADD,
	IR_OC_SUB,
	IR_OC_MUL,
	IR_OC_DIV,
	IR_OC_MOD,
	IR_OC_OR,
	IR_OC_XOR,
	IR_OC_AND,
	IR_OC_EQ,
	IR_OC_NEQ,
	IR_OC_GT,
	IR_OC_LT,
	IR_OC_GTE,
	IR_OC_LTE,
	IR_OC_LABEL,
	IR_OC_JMP,
	IR_OC_JMP_ZERO,
	IR_OC_JMP_NOT_ZERO,
	IR_OC_RET,
	IR_OC_LOCAL,
	IR_OC_LOAD,
	IR_OC_STORE,
	IR_OC_STACK_ALLOC,
	IR_OC_STACK_DEALLOC,
	IR_OC_PARAM,
	IR_OC_CALL,
	IR_OC_STRING,
	IR_OC_LOAD_STRING,
} IROpCode;

typedef enum {
	IR_ATYPE_NUM,
	IR_ATYPE_ADDR,
	IR_ATYPE_LIT,
	IR_ATYPE_REG,
	IR_ATYPE_STACK,
	IR_ATYPE_MEM,
	IR_ATYPE_STR,
	IR_ATYPE_VIEW,
	IR_ATYPE_SSA,
	IR_ATYPE_NONE
} IRArgType;
	
struct IRSSAEnt {
	IRArgType type;
	
	union {
		size_t num;
		size_t str;
		size_t *addr;
		IRLiteral literal;
		size_t reg;
		size_t *stack;
		sv_t mem;
		sv_t *view;
		IRSSAEnt *ssa;
	} as;
};

typedef struct {
	IROpCode op;
	IRPrimitiveType ptype;
	IRSSAEnt *arg1;
	IRSSAEnt *arg2;
	IRSSAEnt *result;
} IRCode;

extern IRSSAEnt *ir_ssa_latest(IR_CTX *ctx);
extern IRSSAEnt *ir_ssa_default(IR_CTX *ctx);
extern IRSSAEnt *ir_ssa_from_view(IR_CTX *ctx, sv_t *view);
extern IRSSAEnt *ir_ssa_from_num(IR_CTX *ctx, size_t num);
extern IRSSAEnt *ir_ssa_from_str(IR_CTX *ctx, size_t str);
extern IRSSAEnt *ir_ssa_from_stack(IR_CTX *ctx, size_t *stack);
extern IRSSAEnt *ir_ssa_from_addr(IR_CTX *ctx, size_t *addr);
extern IRSSAEnt *ir_ssa_from_literal(IR_CTX *ctx, IRLiteral literal);
extern IRSSAEnt *ir_ssa_from_reg(IR_CTX *ctx, size_t reg);

extern void ir_emit(IR_CTX *ctx, IROpCode op, IRPrimitiveType ptype, IRSSAEnt *result, IRSSAEnt *arg1, IRSSAEnt *arg2);

#endif

