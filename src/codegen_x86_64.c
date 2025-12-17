#include <assert.h>
#include <stdio.h>

#include <jd297/list.h>
#include <jd297/sv.h>

#include "ir.h"
#include "codegen.h"

extern int codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_imm(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_push(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_pop(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_sal(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_sar(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_add(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_sub(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_mul(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_div(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_mod(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_or(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_xor(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_and(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_eq(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_neq(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_gt(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_lt(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_gte(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_lte(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_jmp_func_end(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_stack_alloc(IR_CTX *ctx, FILE *output, IRCode *code);

int codegen_x86_64_run(IR_CTX *ctx, FILE *output)
{
	for (list_node_t *it = list_begin(ctx->code); it != list_end(ctx->code); it = list_next(it)) {
		IRCode *code = it->value;
		
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
			case IR_OC_JMP_FUNC_END: {
				if (codegen_x86_64_jmp_func_end(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_FUNC_END: {
				if (codegen_x86_64_func_end(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_STACK_ALLOC: {
				if (codegen_x86_64_stack_alloc(ctx, output, code) != 0) {
					return -1;
				}
			} break;
			case IR_OC_STACK_DEALLOC: break; // asm leave does the job
		
			default: printf(">>>>>%d\n", code->op); assert(0 && "OP Code not implemented yet");
		}
	}

	return 0;
}

int codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, "\t.text\n");
	fprintf(output, "\t.globl " SV_FMT "\n", SV_PARAMS(code->result.ptr->id));
	fprintf(output, SV_FMT ":\n", SV_PARAMS(code->result.ptr->id));
	fprintf(output, "\tendbr64\n");
	fprintf(output, "\tpushq\t%%rbp\n");
	fprintf(output, "\tmovq\t%%rsp, %%rbp\n");

	return 0;
}

int codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, ".func_end_" SV_FMT ":\n", SV_PARAMS(code->result.ptr->id));
	fprintf(output, "\tleave\n");
	fprintf(output, "\tret\n");

	return 0;
}

int codegen_x86_64_imm(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register label instead of always rax
	fprintf(output, "\tmovq\t$" SV_FMT ", %%rax\n", SV_PARAMS(code->arg1.sv));

	return 0;
}

int codegen_x86_64_push(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register label instead of always rax
	(void) code;
	fprintf(output, "\tpushq\t%%rax\n");

	return 0;
}

int codegen_x86_64_pop(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register label instead of always rcx
	(void) code;
	fprintf(output, "\tpopq\t%%rcx\n");

	return 0;
}

int codegen_x86_64_sal(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax << cl
	(void) code;
	fprintf(output, "\tsalq\t%%cl, %%rax\n");

	return 0;
}

int codegen_x86_64_sar(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax >> cl
	(void) code;
	fprintf(output, "\tsarq\t%%cl, %%rax\n");

	return 0;
}

int codegen_x86_64_add(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax + rcx
	(void) code;
	fprintf(output, "\taddq\t%%rcx, %%rax\n");

	return 0;
}

int codegen_x86_64_sub(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax + rcx
	(void) code;
	fprintf(output, "\tsubq\t%%rcx, %%rax\n");

	return 0;
}

int codegen_x86_64_mul(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax * rcx
	(void) code;
	fprintf(output, "\timulq\t%%rcx\n");

	return 0;
}

int codegen_x86_64_div(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax / rcx
	(void) code;
	fprintf(output, "\txor\t%%rdx, %%rdx\n");
	fprintf(output, "\tidivq\t%%rcx\n");

	return 0;
}

int codegen_x86_64_mod(IR_CTX *ctx, FILE *output, IRCode *code)
{
	// TODO use register labels instead of always rax = rax % rcx
	// TODO ?? maybe mod is the same as div only that the result of the
	// TODO mod is in rdx
	codegen_x86_64_div(ctx, output, code);
	
	fprintf(output, "\tmovq\t%%rdx, %%rax\n");

	return 0;
}

int codegen_x86_64_or(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax | rcx
	(void) code;
	fprintf(output, "\torq\t%%rcx, %%rax\n");

	return 0;
}

int codegen_x86_64_xor(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax ^ rcx
	(void) code;
	fprintf(output, "\txorq\t%%rcx, %%rax\n");

	return 0;
}

int codegen_x86_64_and(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax & rcx
	(void) code;
	fprintf(output, "\tandq\t%%rcx, %%rax\n");

	return 0;
}

int codegen_x86_64_eq(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax == rcx
	(void) code;
	fprintf(output, "\tcmpq\t%%rcx, %%rax\n");
	fprintf(output, "\tsete\t%%al\n");
	fprintf(output, "\tmovzbq\t%%al, %%rax\n");

	return 0;
}

int codegen_x86_64_neq(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax != rcx
	(void) code;
	fprintf(output, "\tcmpq\t%%rcx, %%rax\n");
	fprintf(output, "\tsetne\t%%al\n");
	fprintf(output, "\tmovzbq\t%%al, %%rax\n");

	return 0;
}

int codegen_x86_64_gt(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax > rcx
	(void) code;
	fprintf(output, "\tcmpq\t%%rcx, %%rax\n");
	fprintf(output, "\tsetg\t%%al\n");
	fprintf(output, "\tmovzbq\t%%al, %%rax\n");

	return 0;
}

int codegen_x86_64_lt(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax < rcx
	(void) code;
	fprintf(output, "\tcmpq\t%%rcx, %%rax\n");
	fprintf(output, "\tsetl\t%%al\n");
	fprintf(output, "\tmovzbq\t%%al, %%rax\n");

	return 0;
}

int codegen_x86_64_gte(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax == rcx
	(void) code;
	fprintf(output, "\tcmpq\t%%rcx, %%rax\n");
	fprintf(output, "\tsetge\t%%al\n");
	fprintf(output, "\tmovzbq\t%%al, %%rax\n");

	return 0;
}

int codegen_x86_64_lte(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	// TODO use register labels instead of always rax = rax == rcx
	(void) code;
	fprintf(output, "\tcmpq\t%%rcx, %%rax\n");
	fprintf(output, "\tsetle\t%%al\n");
	fprintf(output, "\tmovzbq\t%%al, %%rax\n");

	return 0;
}

int codegen_x86_64_jmp_func_end(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, "\tjmp .func_end_" SV_FMT "\n", SV_PARAMS(code->result.ptr->id));

	return 0;
}

int codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, "\tjmp " SV_FMT "\n", SV_PARAMS(code->result.ptr->id));

	return 0;
}

int codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, SV_FMT ":\n", SV_PARAMS(code->result.ptr->id));

	return 0;
}

int codegen_x86_64_stack_alloc(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, "\tsubq\t$%zu, %%rsp\n", code->result.stack.addr);

	return 0;
}
