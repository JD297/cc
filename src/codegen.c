#include <assert.h>
#include <stddef.h>

#include "codegen.h"
#include "ir.h"

codegen_run_func codegen_func;

size_t codegen_get_type_size(IRPrimitiveType type)
{
	if (
		codegen_func == codegen_x86_64_run ||
		codegen_func == codegen_aarch64_run
	) {
		switch (type) {
			case IR_U8_T:
			case IR_S8_T:
				return 1;
			case IR_U16_T:
			case IR_S16_T:
				return 2;
			case IR_U32_T:
			case IR_S32_T:
				return 4;
			case IR_U64_T:
			case IR_S64_T:
			case IR_PTR_T:
				return 8;
			case IR_F32_T:
				assert(0 && "TODO not implemented: IR_F32_T (on *x64)");
			case IR_F64_T:
				assert(0 && "TODO not implemented: IR_F64_T (on *x64)");
			default:
				assert(0 && "NOT REACHABLE");
		}
	} else {
		assert(0 && "TODO not implemented: platform_size for CODEGEN_FUNC");
	}
}
