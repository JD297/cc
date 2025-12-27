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
	
	ir_emit_imm(ctx, IR_S32_T, ir_literal_from_lu(2), IR_REG1);

	ir_emit_add(ctx, IR_S32_T, IR_ATYPE_REG, IR_ATYPE_LITERAL, IR_REG1, IR_REG1, ir_literal_from_lu(3));

	assert(optimizer_run(ctx) == 0);

	assert(codegen_x86_64_run(ctx, stdout) == 0);
	
	ir_ctx_destroy(ctx);

	return 0;
}
