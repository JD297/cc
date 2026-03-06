#include <assert.h>
#include <stdio.h>

#include <jd297/list.h>
#include <jd297/sv.h>

#include "ir.h"
#include "codegen.h"

#define REGISTER_ACCUMULATOR        0 // RAX (volatile)
#define REGISTER_FORMERLY_BASE      1 // RBX (non-volatile)
#define REGISTER_COUNTER            2 // RCX (volatile)
#define REGISTER_DATA               3 // RDX (volatile)
#define REGISTER_SOURCE_INDEX       4 // RSI (volatile)
#define REGISTER_DESTINATION_INDEX  5 // RDI (volatile)
#define REGISTER_BASE_POINTER       6 // RBP (non-volatile)
#define REGISTER_STACK_POINTER      7 // RSP (non-volatile)
#define REGISTER_ADDITIONAL_1       8 // R8  (volatile)
#define REGISTER_ADDITIONAL_2       9 // R9  (volatile)
#define REGISTER_ADDITIONAL_3      10 // R10 (volatile)
#define REGISTER_ADDITIONAL_4      11 // R11 (volatile)
#define REGISTER_ADDITIONAL_5      12 // R12 (non-volatile)
#define REGISTER_ADDITIONAL_6      13 // R13 (non-volatile)
#define REGISTER_ADDITIONAL_7      14 // R14 (non-volatile)
#define REGISTER_ADDITIONAL_8      15 // R15 (non-volatile)
#define REGISTER_TEMPORARY         11 // R11 (volatile) is the compiler temp register
#define REGISTER_RETURN             0 // RAX (volatile)

static const char *codegen_x86_64_suffix(IRPrimitiveType ptype);
static const char *codegen_x86_64_reg_name(IRPrimitiveType ptype, IRSSAEnt *reg);
static int codegen_x86_64_fput_operand(FILE *output, IRPrimitiveType ptype, IRSSAEnt *operand);

static void codegen_x86_64_mov(IR_CTX *ctx, FILE *output, IRPrimitiveType ptype, IRSSAEnt *src, IRSSAEnt *dst);
static void codegen_x86_64_bin_op(IR_CTX *ctx, FILE *output, IRCode *code, const char *op);

static void codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_imm(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_sal(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_sar(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_add(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_sub(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_mul(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_div(IR_CTX *ctx, FILE *output, IRCode *code);
// static void codegen_x86_64_mod(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_or(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_xor(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_and(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_eq(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_neq(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_gt(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_lt(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_gte(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_lte(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_jmp_zero(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_jmp_not_zero(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_store(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_load(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_param(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_call(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_string(IR_CTX *ctx, FILE *output, IRCode *code);
//static void codegen_x86_64_load_string(IR_CTX *ctx, FILE *output, IRCode *code);
static void codegen_x86_64_ret(IR_CTX *ctx, FILE *output, IRCode *code);

static const char *codegen_x86_64_suffix(IRPrimitiveType ptype)
{
	switch (ptype) {
		case IR_U8_T:
		case IR_S8_T:
			return "b";
		case IR_U16_T:
		case IR_S16_T:
			return "w";
		case IR_U32_T:
		case IR_S32_T:
			return "l";
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T:
			return "q";
		case IR_F32_T:
		case IR_F64_T:
			assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
		default:
			assert(0 && "NOT REACHABLE");
	}
}

static const char *codegen_x86_64_reg_name(IRPrimitiveType ptype, IRSSAEnt *reg)
{
	switch (ptype) {
		case IR_U8_T:
		case IR_S8_T: {
			switch (reg->as.reg) {
				case REGISTER_ACCUMULATOR:
					return "al";				
				case REGISTER_FORMERLY_BASE:
					return "bl";				
				case REGISTER_COUNTER:
					return "cl";				
				case REGISTER_DATA:
					return "dl";				
				case REGISTER_SOURCE_INDEX:
					return "sil";
				case REGISTER_DESTINATION_INDEX:
					return "dil";
				case REGISTER_BASE_POINTER:
					return "bpl";
				case REGISTER_STACK_POINTER:
					return "spl";
				case REGISTER_ADDITIONAL_1:
					return "r8b";
				case REGISTER_ADDITIONAL_2:
					return "r9b";
				case REGISTER_ADDITIONAL_3:
					return "r10b";
				case REGISTER_ADDITIONAL_4:
					return "r11b";				
				case REGISTER_ADDITIONAL_5:
					return "r12b";				
				case REGISTER_ADDITIONAL_6:
					return "r13b";				
				case REGISTER_ADDITIONAL_7:
					return "r14b";				
				case REGISTER_ADDITIONAL_8:
					return "r15b";
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_U16_T:
		case IR_S16_T: {
			switch (reg->as.reg) {
				case REGISTER_ACCUMULATOR:
					return "ax";				
				case REGISTER_FORMERLY_BASE:
					return "bx";				
				case REGISTER_COUNTER:
					return "cx";				
				case REGISTER_DATA:
					return "dx";				
				case REGISTER_SOURCE_INDEX:
					return "si";
				case REGISTER_DESTINATION_INDEX:
					return "di";
				case REGISTER_BASE_POINTER:
					return "bp";
				case REGISTER_STACK_POINTER:
					return "sp";
				case REGISTER_ADDITIONAL_1:
					return "r8w";
				case REGISTER_ADDITIONAL_2:
					return "r9w";
				case REGISTER_ADDITIONAL_3:
					return "r10w";
				case REGISTER_ADDITIONAL_4:
					return "r11w";				
				case REGISTER_ADDITIONAL_5:
					return "r12w";				
				case REGISTER_ADDITIONAL_6:
					return "r13w";				
				case REGISTER_ADDITIONAL_7:
					return "r14w";				
				case REGISTER_ADDITIONAL_8:
					return "r15w";
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_U32_T:
		case IR_S32_T: {
			switch (reg->as.reg) {
				case REGISTER_ACCUMULATOR:
					return "eax";				
				case REGISTER_FORMERLY_BASE:
					return "ebx";				
				case REGISTER_COUNTER:
					return "ecx";				
				case REGISTER_DATA:
					return "edx";				
				case REGISTER_SOURCE_INDEX:
					return "esi";
				case REGISTER_DESTINATION_INDEX:
					return "edi";
				case REGISTER_BASE_POINTER:
					return "ebp";
				case REGISTER_STACK_POINTER:
					return "esp";
				case REGISTER_ADDITIONAL_1:
					return "r8d";
				case REGISTER_ADDITIONAL_2:
					return "r9d";
				case REGISTER_ADDITIONAL_3:
					return "r10d";
				case REGISTER_ADDITIONAL_4:
					return "r11d";				
				case REGISTER_ADDITIONAL_5:
					return "r12d";				
				case REGISTER_ADDITIONAL_6:
					return "r13d";				
				case REGISTER_ADDITIONAL_7:
					return "r14d";				
				case REGISTER_ADDITIONAL_8:
					return "r15d";
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_U64_T:
		case IR_S64_T:
		case IR_PTR_T: {
			switch (reg->as.reg) {
				case REGISTER_ACCUMULATOR:
					return "rax";				
				case REGISTER_FORMERLY_BASE:
					return "rbx";				
				case REGISTER_COUNTER:
					return "rcx";				
				case REGISTER_DATA:
					return "rdx";				
				case REGISTER_SOURCE_INDEX:
					return "rsi";
				case REGISTER_DESTINATION_INDEX:
					return "rdi";
				case REGISTER_BASE_POINTER:
					return "rbp";
				case REGISTER_STACK_POINTER:
					return "rsp";
				case REGISTER_ADDITIONAL_1:
					return "r8";
				case REGISTER_ADDITIONAL_2:
					return "r9";
				case REGISTER_ADDITIONAL_3:
					return "r10";
				case REGISTER_ADDITIONAL_4:
					return "r11";				
				case REGISTER_ADDITIONAL_5:
					return "r12";				
				case REGISTER_ADDITIONAL_6:
					return "r13";				
				case REGISTER_ADDITIONAL_7:
					return "r14";				
				case REGISTER_ADDITIONAL_8:
					return "r15";
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

static int codegen_x86_64_fput_operand(FILE *output, IRPrimitiveType ptype, IRSSAEnt *operand)
{
	switch (operand->type) {
		case IR_ATYPE_REG:
			fputs("%", output);

			return fputs(codegen_x86_64_reg_name(ptype, operand), output) + 1;
		case IR_ATYPE_LIT: {
			switch (ptype) {
				case IR_U8_T:
				case IR_S8_T:
				case IR_U16_T:
				case IR_S16_T:
				case IR_U32_T:
				case IR_S32_T:
					return fprintf(output, "$%u", operand->as.literal.u);
				case IR_U64_T:
				case IR_S64_T:
				case IR_PTR_T:
					return fprintf(output, "$%lu", operand->as.literal.lu);
				case IR_F32_T:
				case IR_F64_T:
					assert(0 && "TODO: not implemented: IR_F32_T and IR_F64_T");
				default:
					assert(0 && "NOT REACHABLE");
			}
		} break;
		case IR_ATYPE_STACK:
			return fprintf(output, "-%zu(%%rbp)", *operand->as.stack);
		case IR_ATYPE_ADDR:
			return fprintf(output, "-%zu(%%rbp)", *operand->as.addr);
		case IR_ATYPE_NUM:
			return fprintf(output, "-%zu(%%rbp)", operand->as.num);
		case IR_ATYPE_MEM:
			return fprintf(output, SV_FMT"(%%rip)", SV_PARAMS(operand->as.view));
		case IR_ATYPE_VIEW:
			assert(0 && "TODO: not implemented: IR_ATYPE_VIEW");
		default:
			assert(0 && "NOT REACHABLE");
	}
}

/*
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
*/

void codegen_x86_64_run(IR_CTX *ctx, FILE *output)
{
	assert(ctx != NULL);
	assert(output != NULL);

	for (list_node_t *it = list_begin(ctx->code); it != list_end(ctx->code); it = list_next(it)) {
		IRCode *code;

		assert(it != NULL);
		assert(it->value != NULL);

		code = it->value;

		switch (code->op) {
			case IR_OC_FUNC_BEGIN:
				codegen_x86_64_func_begin(ctx, output, code);
				break;
			case IR_OC_IMM:
				codegen_x86_64_imm(ctx, output, code);
				break;
			case IR_OC_POP:
				assert(0 && "TODO: IR_OC_POP");
				// codegen_x86_64_pop(ctx, output, code);
				break;
			case IR_OC_SAL:
				codegen_x86_64_sal(ctx, output, code);
				break;
			case IR_OC_SAR:
				codegen_x86_64_sar(ctx, output, code);
				break;
			case IR_OC_ADD:
				codegen_x86_64_add(ctx, output, code);
				break;
			case IR_OC_SUB:
				codegen_x86_64_sub(ctx, output, code);
				break;
			case IR_OC_MUL:
				codegen_x86_64_mul(ctx, output, code);
				break;
			case IR_OC_DIV:
				codegen_x86_64_div(ctx, output, code);
				break;
			case IR_OC_MOD:
				assert(0 && "TODO: IR_OC_MOD");
				// codegen_x86_64_mod(ctx, output, code);
				break;
			case IR_OC_OR:
				codegen_x86_64_or(ctx, output, code);
				break;
			case IR_OC_XOR:
				codegen_x86_64_xor(ctx, output, code);
				break;
			case IR_OC_AND:
				codegen_x86_64_and(ctx, output, code);
				break;
			case IR_OC_EQ:
				assert(0 && "TODO: IR_OC_EQ");
				// codegen_x86_64_eq(ctx, output, code);
				break;
			case IR_OC_NEQ:
				assert(0 && "TODO: IR_OC_NEQ");
				// codegen_x86_64_neq(ctx, output, code);
				break;
			case IR_OC_GT:
				assert(0 && "TODO: IR_OC_GT");
				// codegen_x86_64_gt(ctx, output, code);
				break;
			case IR_OC_LT:
				assert(0 && "TODO: IR_OC_LT");
				// codegen_x86_64_lt(ctx, output, code);
				break;
			case IR_OC_GTE:
				assert(0 && "TODO: IR_OC_GTE");
				// codegen_x86_64_gte(ctx, output, code);
				break;
			case IR_OC_LTE:
				assert(0 && "TODO: IR_OC_LTE");
				// codegen_x86_64_lte(ctx, output, code);
				break;
			case IR_OC_LABEL:
				codegen_x86_64_label(ctx, output, code);
				break;
			case IR_OC_JMP:
				codegen_x86_64_jmp(ctx, output, code);
				break;
			case IR_OC_JMP_ZERO:
				assert(0 && "TODO: IR_OC_JMP_ZERO");
				// codegen_x86_64_jmp_zero(ctx, output, code);
				break;
			case IR_OC_JMP_NOT_ZERO:
				assert(0 && "TODO: IR_OC_JMP_NOT_ZERO");
				// codegen_x86_64_jmp_not_zero(ctx, output, code);
				break;
			case IR_OC_FUNC_END:
				codegen_x86_64_func_end(ctx, output, code);
				break;
			case IR_OC_STORE:
				codegen_x86_64_store(ctx, output, code);
				break;
			case IR_OC_LOAD:
				codegen_x86_64_load(ctx, output, code);
				break;
			case IR_OC_CALL:
				assert(0 && "TODO: IR_OC_CALL");
				// codegen_x86_64_call(ctx, output, code);
				break;
			case IR_OC_STRING:
				assert(0 && "TODO: IR_OC_STRING");
				// codegen_x86_64_string(ctx, output, code);
				break;
			case IR_OC_LOAD_STRING:
				assert(0 && "TODO: IR_OC_LOAD_STRING");
				// codegen_x86_64_load_string(ctx, output, code);
				break;
			case IR_OC_RET:
				codegen_x86_64_ret(ctx, output, code);
				break;
			case IR_OC_LOCAL:
				assert(0 && "IR_OC_LOCAL should not be in list of code when codegen_x86_64_run is called");
		
			default:
				printf(">>>>>%d\n", code->op);
				assert(0 && "OP Code not implemented yet");
		}
	}
}

static void codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code)
{
	size_t stack_size = code->arg1->as.num;

	(void) ctx;

	fputs("\t.text\n", output);
	fprintf(output, "\t.globl " SV_FMT "\n", SV_PARAMS(code->result->as.view));
	fprintf(output, SV_FMT ":\n", SV_PARAMS(code->result->as.view));
	fputs("\tendbr64\n", output);
	fputs("\tpushq\t%rbp\n", output);
	fputs("\tmovq\t%rsp, %rbp\n", output);

	// align to 16byte word boundary
	stack_size += stack_size % 16;

	fprintf(output, "\tsubq\t$%zu, %%rsp\n", stack_size);
}

static void codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_label(ctx, output, code);

	fputs("\tleave\n", output);
	fputs("\tret\n", output);
}

static void codegen_x86_64_imm(IR_CTX *ctx, FILE *output, IRCode *code)
{
	fprintf(output, "\t# %s, %d\n", __FUNCTION__, __LINE__);
	codegen_x86_64_mov(ctx, output, code->ptype, code->arg1, code->result);
}

static void codegen_x86_64_mov(IR_CTX *ctx, FILE *output, IRPrimitiveType ptype, IRSSAEnt *src, IRSSAEnt *dst)
{
	if (
		(src->type == IR_ATYPE_NUM || src->type == IR_ATYPE_STACK || src->type == IR_ATYPE_ADDR) 
			&& 
		(dst->type == IR_ATYPE_NUM || dst->type == IR_ATYPE_STACK || dst->type == IR_ATYPE_ADDR)
	) {
		IRSSAEnt *tmp = ir_ssa_from_reg(ctx, REGISTER_TEMPORARY);
	
		codegen_x86_64_mov(ctx, output, ptype, src, tmp);
		
		src = tmp;
	}

	fputs("\tmov", output);

	fputs(codegen_x86_64_suffix(ptype), output);

	fputs("\t", output);

	codegen_x86_64_fput_operand(output, ptype, src);

	fputs(", ", output);

	codegen_x86_64_fput_operand(output, ptype, dst);
	
	fputs("\n", output);
}

static void codegen_x86_64_bin_op(IR_CTX *ctx, FILE *output, IRCode *code, const char *op)
{
	IRSSAEnt *operand;

	fprintf(output, "\t# %s, %d\n", __FUNCTION__, __LINE__);
	
	/* optimization to avoid mutation of arg1 and setting the result correct right away */
	if (code->result->type == IR_ATYPE_REG) {
		if (code->result != code->arg1) {
			codegen_x86_64_mov(ctx, output, code->ptype, code->arg1, code->result);
		}
		
		operand = code->result;
	} else {
		operand = ir_ssa_from_reg(ctx, REGISTER_TEMPORARY);

		codegen_x86_64_mov(ctx, output, code->ptype, code->arg1, operand);
	}

	fprintf(output, "\t%s", op);
	
	fputs(codegen_x86_64_suffix(code->ptype), output);

	fputs("\t", output);

	codegen_x86_64_fput_operand(output, code->ptype, code->arg2);

	fputs(", ", output);

	codegen_x86_64_fput_operand(output, code->ptype, operand);

	fputs("\n", output);

	if (code->result != operand) {
		codegen_x86_64_mov(ctx, output, code->ptype, operand, code->result);
	}
}

static void codegen_x86_64_sal(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "sal");
}

static void codegen_x86_64_sar(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "sar");
}

static void codegen_x86_64_add(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "add");
}

static void codegen_x86_64_sub(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "sub");
}

static void codegen_x86_64_mul(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "mul");
}

static void codegen_x86_64_div(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void)ctx;
	(void)output;
	(void)code;

	assert("TODO: not implemented");
}
/*
static void codegen_x86_64_mod(IR_CTX *ctx, FILE *output, IRCode *code)
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
}
*/

static void codegen_x86_64_or(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "or");
}

static void codegen_x86_64_xor(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "xor");
}

static void codegen_x86_64_and(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_bin_op(ctx, output, code, "and");
}
/*
static void codegen_x86_64_eq(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	codegen_x86_64_bin_op(ctx, output, code, "cmp");
	
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
}

static void codegen_x86_64_neq(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	codegen_x86_64_bin_op(ctx, output, code, "cmp");
	
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
}

static void codegen_x86_64_gt(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	codegen_x86_64_bin_op(ctx, output, code, "cmp");
	
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
}

static void codegen_x86_64_lt(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	codegen_x86_64_bin_op(ctx, output, code, "cmp");
	
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
}

static void codegen_x86_64_gte(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	codegen_x86_64_bin_op(ctx, output, code, "cmp");
	
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
}

static void codegen_x86_64_lte(IR_CTX *ctx, FILE *output, IRCode *code)
{
	const char *ret_reg_name;

	(void) ctx;

	codegen_x86_64_bin_op(ctx, output, code, "cmp");
	
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
}*/

static void codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	switch (code->result->type) {
		case IR_ATYPE_NUM: {
			fprintf(output, "\tjmp\t.L%zu\n", code->result->as.num);
		} break;
		case IR_ATYPE_ADDR: {
			fprintf(output, "\tjmp\t.L%zu\n", *code->result->as.addr);
		} break;
		default:
			assert(0 && "NOT REACHABLE");
	}
}
/*
static void codegen_x86_64_jmp_zero(IR_CTX *ctx, FILE *output, IRCode *code)
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
}

static void codegen_x86_64_jmp_not_zero(IR_CTX *ctx, FILE *output, IRCode *code)
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
}
*/
static void codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	switch (code->result->type) {
		case IR_ATYPE_NUM: {
			fprintf(output, ".L%zu:\n", code->result->as.num);
		} break;
		case IR_ATYPE_ADDR: {
			fprintf(output, ".L%zu:\n", *code->result->as.addr);
		} break;
		default:
			assert(0 && "NOT REACHABLE");
	}
}

static void codegen_x86_64_store(IR_CTX *ctx, FILE *output, IRCode *code)
{
	fprintf(output, "\t# %s, %d\n", __FUNCTION__, __LINE__);
	codegen_x86_64_mov(ctx, output, code->ptype, code->arg1, code->result);
	/*
	TODO DELETE:
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
	*/
}

static void codegen_x86_64_load(IR_CTX *ctx, FILE *output, IRCode *code)
{
	fprintf(output, "\t# %s, %d\n", __FUNCTION__, __LINE__);
	codegen_x86_64_mov(ctx, output, code->ptype, code->arg1, code->result);
	/*
	TODO DELETE:

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
	*/
}
/*
static void codegen_x86_64_param_push(IR_CTX *ctx, FILE *output, IRCode *code)
{
	return codegen_x86_64_push(ctx, output, code);
}

static void codegen_x86_64_param_pop(IR_CTX *ctx, FILE *output, IRCode *code)
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
}

static void codegen_x86_64_call(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO really??
	// TODO call from a reg value
	// TODO call from a imm value
	// TODO need to check more
	fprintf(output, "\tcall\t"SV_FMT"@PLT\n", SV_PARAMS(code->result.view));
}

static void codegen_x86_64_string(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	assert(code->arg2.num == 1 && "TODO: not implemented: arg2 with other value then 1");

	fprintf(output, "\t.section\t.rodata\n");
	fprintf(output, ".L.str.%zu:\n", code->result.num);
	fprintf(output, "\t.asciz\t\""SV_FMT"\"\n", SV_PARAMS(&code->arg1.literal.sv));
	fprintf(output, "\t.size\t.L.str.%zu, %zu\n", code->result.num, code->arg1.literal.sv.len);
}

static void codegen_x86_64_load_string(IR_CTX *ctx, FILE *output, IRCode *code)
{
	codegen_x86_64_mov(ctx, output, code->ptype, code->arg1, code->result);
	TODO DELETE: ?? leaq string is a novalty...

	const char *ret_reg_name;

	(void) ctx;
	
	ret_reg_name = codegen_x86_64_register_name(code->result.reg);

	fprintf(output, "\tleaq\t.L.str.%zu(%%rip), %%r%sx\n", code->arg1.num, ret_reg_name);
}*/

static void codegen_x86_64_ret(IR_CTX *ctx, FILE *output, IRCode *code)
{
	fprintf(output, "\t# %s, %d\n", __FUNCTION__, __LINE__);
	codegen_x86_64_mov(ctx, output, code->ptype, code->result, ir_ssa_from_reg(ctx, REGISTER_RETURN));
}
