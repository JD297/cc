#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "compiler_c.h"
#include "lexer_c.h"
#include "parse_tree_type_c.h"
#include "parse_tree_node_c.h"
#include "parser_c.h"
#include "ir.h"
#include "optimizer.h"
#include "codegen.h"
#include "symtbl.h"

#include <jd297/lmap_sv.h>
#include <jd297/list.h>

int compiler_c_run(Compiler_C *compiler)
{
	int fd = fileno(compiler->input);
	off_t filesize = ftello(compiler->input);
	char *src = (char *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);

	fclose(compiler->input);

	if (src == MAP_FAILED) {
		warn("error");
		return -1;
	}

    Lexer_C lexer;
	lexer_c_create(&lexer, sv_from_cstr(""), src, LEXER_MODE_NORMAL);
    
    SymTbl *symtbl_root = symtbl_create(NULL, NULL);

	Parser_C_CTX parser_ctx = {
		.error_count = 0,
		.lexer = &lexer,
		.symtbl = symtbl_root
	};

	ParseTreeNode_C *translation_unit = parser_c_parse(&parser_ctx);

	if (parser_ctx.error_count > 0) {
		printf("%d errors generated.", parser_ctx.error_count);
		return -1;
	}

	if (translation_unit == NULL) {
		return -1;
	}

	list_t ir_code;

	if (list_create(&ir_code) == -1) {
		return -1;
	}

	IR_CTX ir_ctx = {
		.code = &ir_code,
		.symtbl = symtbl_root,
		.stack_offset = 0,
		.label_tmp = 0,
		.label_func_end = 0,
		.label_iter_begin = 0,
		.label_iter_end = 0,
		.label_select_begin = 0,
		.label_select_end = 0,
	};

	ir_run(&ir_ctx, translation_unit);

	// TODO HARD
	// TODO GLOBAL VAR HACK needed in optimizer_run
	codegen_func = codegen_x86_64_run;

	assert(optimizer_run(&ir_ctx) == 0);

	assert(codegen_func(&ir_ctx, compiler->output) == 0);

	parse_tree_node_c_destroy(translation_unit);
	munmap(src, filesize);

	list_free(&ir_code);

    return 0;
}
