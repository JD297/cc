#include <assert.h>
#include <stdio.h>

#include <jd297/list.h>
#include <jd297/sv.h>

#include "ir.h"
#include "codegen.h"

static int codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_imm(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_push(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_pop(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_sal(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_sar(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_add(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_sub(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_mul(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_div(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_mod(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_or(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_xor(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_and(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_eq(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_neq(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_gt(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_lt(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_gte(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_lte(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_jmp_zero(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_jmp_not_zero(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_store(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_load(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_param_push(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_param_pop(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_call(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_string(IR_CTX *ctx, FILE *output, IRCode *code);
static int codegen_x86_64_load_string(IR_CTX *ctx, FILE *output, IRCode *code);

static const char *codegen_x86_64_register_name(IRRegister reg)
{
	switch (reg) {
		case IR_REG1:
			return "a";
		case IR_REG2:
			return "c";
		default:
			assert(0 && "NOT REACHABLE");
	}
}

static const char *codegen_x86_64_param_register_name(IRPrimitiveType ptype, size_t n)
{
	switch (ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			switch (n) {
				case 1:
					return "edi";
				case 2:
					return "esi";
				case 3:
					return "edx";
				case 4:
					return "ecx";
				case 5:
					return "r8d";
				case 6:
					return "r9d";
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			switch (n) {
				case 1:
					return "rdi";
				case 2:
					return "rsi";
				case 3:
					return "rdx";
				case 4:
					return "rcx";
				case 5:
					return "r8";
				case 6:
					return "r9";
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
}

int codegen_x86_64_run(IR_CTX *ctx, FILE *output)
{
	assert(ctx != NULL);
	assert(output != NULL);

	for (list_node_t *it = list_begin(ctx->code); it != list_end(ctx->code); it = list_next(it)) {
		IRCode *code;

		assert(it != NULL);
		assert(it->value != NULL);

		code = it->value;

		switch (code->op) {
			case IR_OC_FUNC_BEGIN: {
				if (codegen_x86_64_func_begin(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_IMM: {
				if (codegen_x86_64_imm(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_PUSH: {
				if (codegen_x86_64_push(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_POP: {
				if (codegen_x86_64_pop(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_SAL: {
				if (codegen_x86_64_sal(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_SAR: {
				if (codegen_x86_64_sar(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_ADD: {
				if (codegen_x86_64_add(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_SUB: {
				if (codegen_x86_64_sub(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_MUL: {
				if (codegen_x86_64_mul(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_DIV: {
				if (codegen_x86_64_div(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_MOD: {
				if (codegen_x86_64_mod(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_OR: {
				if (codegen_x86_64_or(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_XOR: {
				if (codegen_x86_64_xor(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_AND: {
				if (codegen_x86_64_and(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_EQ: {
				if (codegen_x86_64_eq(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_NEQ: {
				if (codegen_x86_64_neq(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_GT: {
				if (codegen_x86_64_gt(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_LT: {
				if (codegen_x86_64_lt(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_GTE: {
				if (codegen_x86_64_gte(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_LTE: {
				if (codegen_x86_64_lte(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_LABEL: {
				if (codegen_x86_64_label(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_JMP: {
				if (codegen_x86_64_jmp(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_JMP_ZERO: {
				if (codegen_x86_64_jmp_zero(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_JMP_NOT_ZERO: {
				if (codegen_x86_64_jmp_not_zero(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_FUNC_END: {
				if (codegen_x86_64_func_end(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_STORE: {
				if (codegen_x86_64_store(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_LOAD: {
				if (codegen_x86_64_load(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_PARAM_PUSH: {
				if (codegen_x86_64_param_push(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_PARAM_POP: {
				if (codegen_x86_64_param_pop(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_CALL: {
				if (codegen_x86_64_call(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_STRING: {
				if (codegen_x86_64_string(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_LOAD_STRING: {
				if (codegen_x86_64_load_string(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_LOCAL:
				assert(0 && "IR_OC_LOCAL should not be in list of code when codegen_x86_64_run is called");
		
			default:
				printf(">>>>>%d\n", code->op);
				assert(0 && "OP Code not implemented yet");
		}
	}

	return 0;
}

static int codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code)
{
	size_t stack_size = code->arg1.num;

	(void) ctx;

	fprintf(output, "\t.text\n");
	fprintf(output, "\t.globl " SV_FMT "\n", SV_PARAMS(code->result.view));
	fprintf(output, SV_FMT ":\n", SV_PARAMS(code->result.view));
	fprintf(output, "\tendbr64\n");
	fprintf(output, "\tpushq\t%%rbp\n");
	fprintf(output, "\tmovq\t%%rsp, %%rbp\n");

	// align to 16byte word boundary
	stack_size += stack_size % 16;

	fprintf(output, "\tsubq\t$%zu, %%rsp\n", stack_size);

	return 0;
}

static int codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	codegen_x86_64_label(ctx, output, code);

	fprintf(output, "\tleave\n");
	fprintf(output, "\tret\n");

	return 0;
}

int codegen_x86_64_imm(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovl\t$%u, %%e%sx", code->arg1.literal.u, codegen_x86_64_register_name(code->result.reg));
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovq\t$%lu, %%r%sx", code->arg1.literal.lu, codegen_x86_64_register_name(code->result.reg));
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	fprintf(output, "\n");

	return 0;
}

static int codegen_x86_64_push(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tpushl\t");
			
			switch (code->atype1) {
				case IR_ATYPE_REG: {
					fprintf(output, "%%e%sx", codegen_x86_64_register_name(code->arg1.reg));
				} break;
				case IR_ATYPE_LITERAL: {
					fprintf(output, "$%u", code->arg1.literal.u);
				} break;
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tpushq\t");
			
			switch (code->atype1) {
				case IR_ATYPE_REG: {
					fprintf(output, "%%r%sx", codegen_x86_64_register_name(code->arg1.reg));
				} break;
				case IR_ATYPE_LITERAL: {
					fprintf(output, "$%lu", code->arg1.literal.lu);
				} break;
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	fprintf(output, "\n");

	return 0;
}

static int codegen_x86_64_pop(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tpopl\t");
			
			switch (code->atype1) {
				case IR_ATYPE_REG: {
					fprintf(output, "%%e%sx", codegen_x86_64_register_name(code->arg1.reg));
				} break;
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tpopq\t");
			
			switch (code->atype1) {
				case IR_ATYPE_REG: {
					fprintf(output, "%%r%sx", codegen_x86_64_register_name(code->arg1.reg));
				} break;
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	fprintf(output, "\n");

	return 0;
}

#define CODEGEN_X86_64_BIN_OP_IMPL(_OpCode_) \
	(void) ctx;\
\
	assert(code->atype1 == IR_ATYPE_REG && "TODO: not implemented");\
	assert(code->arg1.reg == code->result.reg && "TODO: not implemented");\
\
	fprintf(output, "\t%s", _OpCode_);\
\
	switch (code->ptype) {\
		case IR_U8_T:\
		case IR_S8_T:\
		case IR_U16_T:\
		case IR_S16_T:\
		case IR_U32_T:\
		case IR_S32_T: {\
			fprintf(output, "l\t");\
\
			switch (code->atype2) {\
				case IR_ATYPE_REG: {\
					fprintf(output, "%%e%sx", codegen_x86_64_register_name(code->arg2.reg));\
				} break;\
				case IR_ATYPE_LITERAL: {\
					fprintf(output, "$%u", code->arg2.literal.u);\
				} break;\
				default:\
					assert(0 && "NOT REACHABLE");\
			}\
\
			fprintf(output, ", ");\
\
			switch (code->atype1) {\
				case IR_ATYPE_REG: {\
					fprintf(output, "%%e%sx", codegen_x86_64_register_name(code->arg1.reg));\
				} break;\
				default:\
					assert(0 && "NOT REACHABLE");\
			}\
		} break;\
		case IR_U64_T:\
		case IR_S64_T:\
		case IR_PTR_T: {\
			fprintf(output, "q\t");\
\
			switch (code->atype2) {\
				case IR_ATYPE_REG: {\
					fprintf(output, "%%r%sx", codegen_x86_64_register_name(code->arg2.reg));\
				} break;\
				/* code->arg1.reg == code->result.reg */\
				/*case IR_ATYPE_LITERAL: {*/\
				/*	fprintf(output, "$%lu", code->arg1.literal.lu);*/\
				/*} break;*/\
				default:\
					assert(0 && "NOT REACHABLE");\
			}\
\
			fprintf(output, ", ");\
\
			switch (code->atype1) {\
				case IR_ATYPE_REG: {\
					fprintf(output, "%%r%sx", codegen_x86_64_register_name(code->arg1.reg));\
				} break;\
				default:\
					assert(0 && "NOT REACHABLE");\
			}\
		} break;\
		case IR_F32_T:\
		case IR_F64_T:\
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");\
		default:\
			assert(0 && "NOT REACHABLE");\
	}\
\
	fprintf(output, "\n")

static int codegen_x86_64_sal(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("sal");
	
	return 0;
}

static int codegen_x86_64_sar(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("sar");
	
	return 0;
}

static int codegen_x86_64_add(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("add");
	
	return 0;
}

static int codegen_x86_64_sub(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("sub");
	
	return 0;
}

static int codegen_x86_64_mul(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("mul");
	
	return 0;
}

static int codegen_x86_64_div(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("div");
	
	return 0;
}

static int codegen_x86_64_mod(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *arg1_reg_name;

	(void)ctx;
	
	arg1_reg_name = codegen_x86_64_register_name(code->arg1.reg);

	codegen_x86_64_div(ctx, output, code);
	
	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovl\t%%edx, %%e%sx", arg1_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovq\t%%rdx, %%r%sx", arg1_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	fprintf(output, "\n");

	return 0;
}

static int codegen_x86_64_or(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("or");
	
	return 0;
}

static int codegen_x86_64_xor(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("xor");

	return 0;
}

static int codegen_x86_64_and(IR_CTX *ctx, FILE *output, IRCode *code)
{
	CODEGEN_X86_64_BIN_OP_IMPL("and");

	return 0;
}

static int codegen_x86_64_eq(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	CODEGEN_X86_64_BIN_OP_IMPL("cmp");
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);
	
	fprintf(output, "\tsete\t%%%sl\n", ret_reg_name);

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovzbe\t%%%sl, %%e%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovzbq\t%%%sl, %%r%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_neq(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	CODEGEN_X86_64_BIN_OP_IMPL("cmp");
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);
	
	fprintf(output, "\tsetne\t%%%sl\n", ret_reg_name);

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovzbe\t%%%sl, %%e%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovzbq\t%%%sl, %%r%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_gt(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	CODEGEN_X86_64_BIN_OP_IMPL("cmp");
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);
	
	fprintf(output, "\tsetg\t%%%sl\n", ret_reg_name);

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovzbe\t%%%sl, %%e%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovzbq\t%%%sl, %%r%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_lt(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	CODEGEN_X86_64_BIN_OP_IMPL("cmp");
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);
	
	fprintf(output, "\tsetl\t%%%sl\n", ret_reg_name);

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovzbe\t%%%sl, %%e%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovzbq\t%%%sl, %%r%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_gte(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	CODEGEN_X86_64_BIN_OP_IMPL("cmp");
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);
	
	fprintf(output, "\tsetge\t%%%sl\n", ret_reg_name);

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovzbe\t%%%sl, %%e%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovzbq\t%%%sl, %%r%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_lte(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	CODEGEN_X86_64_BIN_OP_IMPL("cmp");
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);
	
	fprintf(output, "\tsetle\t%%%sl\n", ret_reg_name);

	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovzbe\t%%%sl, %%e%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovzbq\t%%%sl, %%r%sx\n", ret_reg_name, ret_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	switch (code->rtype) {
		case IR_ATYPE_NUM: {
			fprintf(output, "\tjmp\t.L%zu\n", code->result.num);
		} break;
		case IR_ATYPE_ADDR: {
			fprintf(output, "\tjmp\t.L%zu\n", *code->result.addr);
		} break;
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_jmp_zero(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *arg1_reg_name;

	(void) ctx;

	arg1_reg_name = codegen_x86_64_register_name(code->arg1.reg);
	
	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tcmpl\t$0, %%e%sx\n", arg1_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tcmpq\t$0, %%r%sx\n", arg1_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	fprintf(output, "\tje\t.L%zu\n", code->result.num);

	return 0;
}

static int codegen_x86_64_jmp_not_zero(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *arg1_reg_name;

	(void) ctx;

	arg1_reg_name = codegen_x86_64_register_name(code->arg1.reg);
	
	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tcmpl\t$0, %%e%sx\n", arg1_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tcmpq\t$0, %%r%sx\n", arg1_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	fprintf(output, "\tjne\t.L%zu\n", code->result.num);

	return 0;
}

static int codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	switch (code->rtype) {
		case IR_ATYPE_NUM: {
			fprintf(output, ".L%zu:\n", code->result.num);
		} break;
		case IR_ATYPE_ADDR: {
			fprintf(output, ".L%zu:\n", *code->result.addr);
		} break;
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_store(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *arg1_reg_name;

	(void) ctx;
	
	assert(code->result.addr != NULL);

	arg1_reg_name = codegen_x86_64_register_name(code->arg1.reg);
	
	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovl\t%%e%sx, ", arg1_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovq\t%%r%sx, ", arg1_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	fprintf(output, "-%zu(%%rbp)\n", *code->result.addr);

	return 0;
}

static int codegen_x86_64_load(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *result_reg_name;

	(void) ctx;
	
	assert(code->arg1.addr != NULL);

	result_reg_name = codegen_x86_64_register_name(code->result.reg);
	
	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tmovl\t-%zu(%%rbp), %%e%sx\n", *code->arg1.addr, result_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tmovq\t-%zu(%%rbp), %%r%sx\n", *code->arg1.addr, result_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}

	return 0;
}

static int codegen_x86_64_param_push(IR_CTX *ctx, FILE *output, IRCode *code)
{
	return codegen_x86_64_push(ctx, output, code);
}

static int codegen_x86_64_param_pop(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *arg1_reg_name;

	(void) ctx;
	
	assert(code->atype1 == IR_ATYPE_NUM);
	
	assert(code->arg1.num <= 6 && "Function Call ABI on x86_64 needs to leave the remaining (p7..pn) arguments on the stack");
	
	arg1_reg_name = codegen_x86_64_param_register_name(code->ptype, code->arg1.num);
	
	switch (code->ptype) {
		case IR_U8_T:
		case IR_S8_T:
		case IR_U16_T:
		case IR_S16_T:
		case IR_U32_T:
		case IR_S32_T: {
			fprintf(output, "\tpopl\t%%%s", arg1_reg_name);
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			fprintf(output, "\tpopq\t%%%s", arg1_reg_name);
		} break;
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	fprintf(output, "\n");

	return 0;
}

static int codegen_x86_64_call(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO really??
	// TODO call from a reg value
	// TODO call from a imm value
	// TODO need to check more
	fprintf(output, "\tcall\t"SV_FMT"@PLT\n", SV_PARAMS(code->result.view));

	return 0;
}

static int codegen_x86_64_string(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	assert(code->arg2.num == 1 && "TODO: not implemented: arg2 with other value then 1");

	fprintf(output, "\t.section\t.rodata\n");
	fprintf(output, ".L.str.%zu:\n", code->result.num);
	fprintf(output, "\t.asciz\t\""SV_FMT"\"\n", SV_PARAMS(&code->arg1.literal.sv));
	fprintf(output, "\t.size\t.L.str.%zu, %zu\n", code->result.num, code->arg1.literal.sv.len);

	return 0;
}

static int codegen_x86_64_load_string(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);

	fprintf(output, "\tleaq\t.L.str.%zu(%%rip), %%r%sx\n", code->arg1.num, ret_reg_name);

	return 0;
}
