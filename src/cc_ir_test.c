#include <assert.h>
#include <stdio.h>

#include <jd297/sv.h>
#include <jd297/list.h>

#include "ir.h"
#include "optimizer.h"
#include "codegen.h"
#include "symtbl.h"

int main(void)
{
	SymTbl *symtbl_root = symtbl_create(NULL, NULL);

	list_t ir_code;

	if (list_create(&ir_code) == -1) {
		assert(0 && "FAILED list_create(&ir_code)");
	}

	// TODO get default from uname arch maybe ??
	// TODO add argv flag for cross compilation
	codegen_func = codegen_x86_64_run;

	IR_CTX ctx = {
		.code = &ir_code,
		.symtbl = symtbl_root,
		.stack_offset = 0
	};

	// /---------------------------------------------------------------CODE HERE
	SymTblEnt r1 = (SymTblEnt) {.type = REG, .use = REGISTER};
	SymTblEnt r2 = (SymTblEnt) {.type = REG, .use = REGISTER};
	
	sv_t id_main = {
		.value = "main",
		.len = 4
	};
	
	sv_t id_a = {
		.value = "a",
		.len = 1
	};
	
	sv_t id_b = {
		.value = "b",
		.len = 1
	};
	
	sv_t id_c = {
		.value = "c",
		.len = 1	
	};

	SymTblEnt *main_entry = symtbl_add_entry(ctx.symtbl, &id_main, INT, FUNCTION, NULL);

	SymTblEnt *a_entry = symtbl_add_entry(ctx.symtbl, &id_a, INT, LOCAL, NULL);
	SymTblEnt *b_entry = symtbl_add_entry(ctx.symtbl, &id_b, INT, LOCAL, NULL);
	SymTblEnt *c_entry = symtbl_add_entry(ctx.symtbl, &id_c, INT, LOCAL, NULL);

	list_insert(ctx.code, list_end(ctx.code), &(IRCode) {
		.op = IR_OC_FUNC_BEGIN,
		.result.ptr = main_entry
	});

	/* int a = 5; */
	// LOCAL (INT, "a") // offset 0
	list_insert(ctx.code, list_end(ctx.code), &(IRCode) {
		.op = IR_OC_LOCAL,
		.result.ptr = a_entry
	});
	// PUTR1(INT, "5")
	// LOAD("a")
	
	/* int b; */
	// LOCAL (INT, "b") // offset 4
	list_insert(ctx.code, list_end(ctx.code), &(IRCode) {
		.op = IR_OC_LOCAL,
		.result.ptr = b_entry
	});
	
	/* int c = 7 + 13 + 1; */
	// LOCAL (INT, "c") // offset 8
	list_insert(ctx.code, list_end(ctx.code), &(IRCode) {
		.op = IR_OC_LOCAL,
		.result.ptr = c_entry
	});
	// PUTR1(INT, "7")
	// PUTR2(INT, "13")
	// ADD
	// PUTR2(INT, "1")
	// ADD
	// LOAD("c")
	
	/* b = 3 */
	// PUTR1(INT, "3")
	// LOAD("b")

	/* return c - b -a; */
	// PUTR1(INT "c")
	// PUTR2(INT "b")
	// SUB
	// PUTR2(INT "a")
	// SUB
	// RET
	/*list_insert(ctx.code, list_end(ctx.code), &(IRCode) {
		.op = IR_OC_RET,
		.result = main_entry,
		.arg1 = &r1
	});*/

	list_insert(ctx.code, list_end(ctx.code), &(IRCode) {
		.op = IR_OC_FUNC_END,
		.result.ptr = main_entry
	});
	// ^^^-------------------------------------------------------------CODE HERE

	assert(optimizer_run(&ctx) == 0);

	assert(codegen_func(&ctx, stdout) == 0);

	list_free(&ir_code);

	return 0;
}
