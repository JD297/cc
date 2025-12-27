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

typedef struct {
	list_t *code;
	IRSymTbl *symtbl;
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
	IR_OC_PARAM_PUSH,
	IR_OC_PARAM_POP,
	IR_OC_CALL,
	IR_OC_STRING,
	IR_OC_LOAD_STRING,
} IROpCode;

typedef enum {
	IR_ATYPE_ADDR,
	IR_ATYPE_LITERAL,
	IR_ATYPE_NUM,
	IR_ATYPE_VIEW,
	IR_ATYPE_REG,
	// TODO ADD: IR_ATYPE_MEM, which is IR_ATYPE_VIEW ??
} IRArgType;

typedef enum {
	IR_REG1,
	IR_REG2,
} IRRegister;

typedef struct {
	IROpCode op;
	IRArgType atype1;
	IRArgType atype2;
	IRArgType rtype;
	IRPrimitiveType ptype;
	union {
		IRLiteral literal;
		IRRegister reg;
		size_t num;
		size_t *addr;
	} arg1;
	union {
		size_t num;
		IRLiteral literal;
		IRRegister reg;
	} arg2;
	union {
		size_t *addr;
		size_t num;
		sv_t *view;
		IRRegister reg;
	} result;
} IRCode;

extern void ir_emit_push        (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, /* IRRegister|IRLiteral*/...                                   );
extern void ir_emit_pop         (IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1                                                                   );
extern void ir_emit_func_begin  (IR_CTX *ctx, sv_t *result                                                                                          );
extern void ir_emit_func_end    (IR_CTX *ctx);
extern void ir_emit_sal         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_sar         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_add         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_sub         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_mul         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_div         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_mod         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_or          (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_xor         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_and         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_eq          (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_neq         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_gt          (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_lt          (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_gte         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_lte         (IR_CTX *ctx, IRPrimitiveType ptype, IRArgType atype1, IRArgType atype2, IRRegister rreg, /* IRRegister|IRLiteral*/...);
extern void ir_emit_jmp_zero    (IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1      , size_t result                                                                    );
extern void ir_emit_jmp_not_zero(IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1      , size_t result                                                                    );
extern void ir_emit_string      (IR_CTX *ctx, IRLiteral arg1       , size_t arg2     , size_t result                                                  );
extern void ir_emit_load_string (IR_CTX *ctx, size_t arg1          , IRRegister result                                                                );
extern void ir_emit_param_push  (IR_CTX *ctx, size_t arg2          , IRPrimitiveType ptype, IRArgType atype1, /* IRRegister|IRLiteral*/...            );
extern void ir_emit_param_pop   (IR_CTX *ctx, IRPrimitiveType ptype, size_t arg1                                                                      );
extern void ir_emit_call        (IR_CTX *ctx, size_t arg1          , sv_t *result                                                                     );
extern void ir_emit_imm         (IR_CTX *ctx, IRPrimitiveType ptype, IRLiteral arg1, IRRegister result                                                );
extern void ir_emit_local       (IR_CTX *ctx, IRPrimitiveType ptype, size_t *result);
extern void ir_emit_store       (IR_CTX *ctx, IRPrimitiveType ptype, IRRegister arg1, size_t *result);
extern void ir_emit_load        (IR_CTX *ctx, IRPrimitiveType ptype, size_t *arg1, IRRegister result);
extern void ir_emit_jmp         (IR_CTX *ctx, IRArgType rtype, /* size_t|size_t* result */...                                                                                           );
extern void ir_emit_label       (IR_CTX *ctx, IRArgType rtype, /* size_t|size_t* result */...                                                                                           );

#endif

