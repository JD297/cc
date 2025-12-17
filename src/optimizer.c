#include <stdlib.h>

#include "ir.h"
#include "optimizer.h"
#include "codegen.h"

#include <assert.h>

extern int optimizer_func_begin(IR_CTX *ctx, list_node_t *it);

extern int optimizer_stack_allocation(IR_CTX *ctx, list_node_t *it);

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

int optimizer_stack_allocation(IR_CTX *ctx, list_node_t *begin)
{
	for (list_node_t *it = begin; it != list_end(ctx->code); ) {
		IRCode *code = it->value;
		
		switch (code->op) {
			case IR_OC_LOCAL: {
				// TODO align to 8byte is there to make it simple
				ctx->stack_offset += 8; // codegen_get_type_size(code->result.ptr->type);
			
				code->result.ptr->addr = ctx->stack_offset;

				it = list_erase(ctx->code, it);
			} break;
			case IR_OC_FUNC_END: {
				if (ctx->stack_offset == 0) {
					return 0;
				}

				IRCode *stack_dealloc = malloc(sizeof(IRCode));

				assert(stack_dealloc != NULL);

				*stack_dealloc = (IRCode) {
					.op = IR_OC_STACK_DEALLOC,
					.result.stack = (SymTblEnt) { .addr = ctx->stack_offset }
				};
			
				list_insert(ctx->code, list_next(begin), stack_dealloc);

				IRCode *stack_alloc = malloc(sizeof(IRCode));

				assert(stack_alloc != NULL);

				*stack_alloc = (IRCode) {
					.op = IR_OC_STACK_ALLOC,
					.result.stack = (SymTblEnt) { .addr = ctx->stack_offset }
				};

				list_insert(ctx->code, list_next(begin), stack_alloc);

				ctx->stack_offset = 0;

				return 0;
			} break;
			default: it = list_next(it); break;
		}
	}

	return -1;
}
