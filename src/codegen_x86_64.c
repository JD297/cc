#include <assert.h>
#include <stdio.h>

#include <jd297/list.h>
#include <jd297/sv.h>

#include "ir.h"
#include "codegen.h"

extern int codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_ret(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code);
extern int codegen_x86_64_jmp_func_end(IR_CTX *ctx, FILE *output, IRCode *code);

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
			case IR_OC_RET: {
				if (codegen_x86_64_ret(ctx, output, code) != 0) {
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
		
			default: assert(0 && "OP Code not implemented yet");
		}
	}

	return 0;
}

int codegen_x86_64_func_begin(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, "\t.text\n");
	fprintf(output, "\t.globl " SV_FMT "\n", SV_PARAMS(code->result->id));
	fprintf(output, SV_FMT ":\n", SV_PARAMS(code->result->id));
	fprintf(output, "\tendbr64\n");
	fprintf(output, "\tpushq\t%%rbp\n");
	fprintf(output, "\tmovq\t%%rsp, %%rbp\n");

	return 0;
}

int codegen_x86_64_func_end(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, ".func_end_" SV_FMT "\n", SV_PARAMS(code->result->id));
	fprintf(output, "\tleave\n");
	fprintf(output, "\tret\n");

	return 0;
}

int codegen_x86_64_jmp_func_end(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, "\tjmp .func_end_" SV_FMT "\n", SV_PARAMS(code->result->id));

	return 0;
}

int codegen_x86_64_ret(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;
	(void) code;

	// TODO get func SymtblType e.g. I32
	// IF SymtblType == I32 THEN
	// fprintf(output, "\tmovl -4(%%rbp), %%eax\n");
	// IF SymtblType == I64 THEN
	// fprintf(output, "\tmovq -8(%%rbp), %%rax\n");
	fprintf(output, "\t# TODO codegen_x86_64_ret\n");

	return 0;
}

int codegen_x86_64_jmp(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, "\tjmp " SV_FMT "\n", SV_PARAMS(code->result->id));

	return 0;
}

int codegen_x86_64_label(IR_CTX *ctx, FILE *output, IRCode *code)
{
	(void) ctx;

	fprintf(output, SV_FMT ":\n", SV_PARAMS(code->result->id));

	return 0;
}
