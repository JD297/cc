#include <stdlib.h>

#include "ir.h"
#include "optimizer.h"
#include "codegen.h"

#include <assert.h>

extern int optimizer_func_begin(IR_CTX *ctx, list_node_t *it);

extern int optimizer_func_end(IR_CTX *ctx, list_node_t *it);

extern int optimizer_stack_allocation(IR_CTX *ctx, list_node_t *it);

extern int optimizer_string(IR_CTX *ctx, list_node_t *it);

int optimizer_run(IR_CTX *ctx)
{
	for (list_node_t *it = list_begin(ctx->code); it != list_end(ctx->code); it = list_next(it)) {
		IRCode *code = it->value;
		
		switch (code->op) {
			case IR_OC_FUNC_BEGIN: {
				if (optimizer_func_begin(ctx, it) != 0) {
					return -1;
				}
			} break;
			case IR_OC_FUNC_END: {
				if (optimizer_func_end(ctx, it) != 0) {
					return -1;
				}
			} break;
			default: break;
		}
	}

	return 0;
}

int optimizer_func_begin(IR_CTX *ctx, list_node_t *it)
{
	if (optimizer_stack_allocation(ctx, it) == -1) {
		return -1;	
	}

	return 0;
}

int optimizer_func_end(IR_CTX *ctx, list_node_t *it)
{
	if (optimizer_string(ctx, it) == -1) {
		return -1;	
	}

	return 0;
}

int optimizer_stack_allocation(IR_CTX *ctx, list_node_t *begin)
{
	IRCode *func_begin_code = begin->value;

	for (list_node_t *it = begin; it != list_end(ctx->code); ) {
		IRCode *code = it->value;
		
		switch (code->op) {
			case IR_OC_LOCAL: {
				// TODO align to 8byte is there to make it simple
				ctx->stack_offset += 8; // codegen_get_type_size(code->result.rtype);
			
				*code->result.addr = ctx->stack_offset;

				it = list_erase(ctx->code, it);
			} break;
			case IR_OC_FUNC_END: {
				func_begin_code->arg1.num = ctx->stack_offset;
				code->arg1.num = ctx->stack_offset;

				ctx->stack_offset = 0;

				return 0;
			} break;
			default: it = list_next(it); break;
		}
	}

	return -1;
}

int optimizer_string(IR_CTX *ctx, list_node_t *end)
{
	for (list_node_t *it = end; it != list_begin(ctx->code); ) {
		IRCode *code = it->value;
		
		switch (code->op) {
			case IR_OC_STRING: {
				list_insert(ctx->code, list_next(end), code);
				it = list_erase(ctx->code, it);
			} break;
			case IR_OC_FUNC_BEGIN: return 0;
			default: it = list_prev(it); break;
		}
	}
	
	return 0;
}
