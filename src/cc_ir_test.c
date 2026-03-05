#include <assert.h>
#include <stdio.h>

#include <jd297/sv.h>
#include <jd297/list.h>

#include "ir.h"
#include "optimizer.h"
#include "codegen.h"

int main(void)
{
	IR_CTX *ctx;
	
	assert((ctx = ir_ctx_create(NULL)) != NULL);
	
	// ir_emit(ctx, IR_OC_IMM, IR_S32_T, ir_ssa_from_literal(ctx, ir_literal_from_lu(2)), ir_ssa_default(ctx), NULL);

	ir_emit(ctx, IR_OC_IMM, IR_S64_T, ir_ssa_from_reg(ctx, 1), ir_ssa_from_literal(ctx, ir_literal_from_lu(17)), NULL);

	ir_emit(ctx, IR_OC_ADD, IR_S64_T, ir_ssa_from_reg(ctx, 0), ir_ssa_from_literal(ctx, ir_literal_from_lu(19)), ir_ssa_from_reg(ctx, 1));
	
	ir_emit(ctx, IR_OC_ADD, IR_S16_T, ir_ssa_from_reg(ctx, 0), ir_ssa_from_literal(ctx, ir_literal_from_lu(6)), ir_ssa_from_literal(ctx, ir_literal_from_lu(7)));

	// ir_emit(ctx, IR_OC_ADD, IR_S32_T, ir_ssa_from_literal(ctx, ir_literal_from_lu(3)), ir_ssa_from_literal(ctx, ir_literal_from_lu(7)), ir_ssa_from_reg(ctx, 0));

	assert(optimizer_run(ctx) == 0);

	codegen_x86_64_run(ctx, stdout);
	
	ir_ctx_destroy(ctx);

	return 0;
}
