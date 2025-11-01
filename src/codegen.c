#include <assert.h>

#include "codegen.h"
#include "symtbl.h"

codegen_run_func codegen_func;

size_t codegen_get_type_size(SymTblEntType type)
{
	if (
		codegen_func == codegen_x86_64_run ||
		codegen_func == codegen_aarch64_run
	) {
		switch (type) {
				case INT: return 4; // TODO constant
				default: assert(0 && "TODO not implemented: TYPE FOR x64");
			}
	} else {
		assert(0 && "TODO not implemented: platform_size for CODEGEN_FUNC");
	}
}
