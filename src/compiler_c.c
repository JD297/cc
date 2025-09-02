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

int compiler_c_run(Compiler_C *compiler)
{
	int fd = fileno(compiler->input);
	off_t filesize = ftello(compiler->input);
	char *src = (char *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);

	if (src == MAP_FAILED) {
		warn("error");
		return -1;
	}

    fclose(compiler->input);

    Lexer_C lexer = {
        .buf = src,
        .pbuf = src,
        .loc = {
            .pathname = "",
            .row = 1,
            .col = 1
        }
    };
    
    lmap_t symtbl = { 0 };

	Parser_C_CTX parser_ctx = {
		.anonymous_block_count = 0,
		.error_count = 0,
		.lexer = &lexer,
		.symtbl = &symtbl
	};

	ParseTreeNode_C *translation_unit = parser_c_parse(&parser_ctx);

	if (translation_unit == NULL) {
		return -1;
	}

	compiler_c_codegen(compiler, translation_unit);

	parse_tree_node_c_destroy(translation_unit);

    munmap(src, filesize);

    return 0;
}

int compiler_c_codegen(Compiler_C *compiler, ParseTreeNode_C *translation_unit)
{
	for (size_t i = 0; i < translation_unit->num; ++i) {
		ParseTreeNode_C *external_declaration = translation_unit->elements[i];

		for (size_t j = 0; j < external_declaration->num; ++j) {
			ParseTreeNode_C *node = external_declaration->elements[j];

			switch (node->type) {
				case PTT_C_FUNCTION_DEFINITION: {
					if (compiler_c_codegen_function_definition(compiler, node) != 0) {
						return EXIT_FAILURE;
					}
				} break;
				case PTT_C_DECLARATION: {
					assert(0 && "PTT_C_DECLARATION NOT IMPLEMENTED");
				} break;
				default: {
					assert(0 && "NOT REACHABLE");
				}
			}
		}
	}

	return 0;
}

int compiler_c_codegen_function_definition(Compiler_C *compiler, ParseTreeNode_C *function_definition)
{
	for (size_t i = 0; i < function_definition->num; ++i) {
		ParseTreeNode_C *node = function_definition->elements[i];

		switch (node->type) {
			// TODO IGNORE
			case PTT_C_DECLARATION_SPECIFIER: break;

			// retrieve identifier from declarator
			case PTT_C_DECLARATOR: {
				ParseTreeNode_C *direct_decl = node->elements[0];
				
				if (direct_decl->type != PTT_C_DIRECT_DECLARATOR) {
					assert(0 && "Function Pointers are not supported!");
				}
				
				ParseTreeNode_C *identifier_node = direct_decl->elements[0];
				
				if (identifier_node->type != PTT_C_IDENTIFIER) {
					assert(0 && "Only simple functions with identifieres are supporteds!");
				}

				char *identifier = malloc(identifier_node->token.len + 1);
				
				assert(identifier != NULL);

				strlcpy(identifier, identifier_node->token.value, identifier_node->token.len + 1);
			
				fprintf(compiler->output, "\t// IR [IR_OC_FUNC_BEGIN     , IR_T_I32, NULL, NULL, \"main\"]\n");
				fprintf(compiler->output, "\t.text\n");
				fprintf(compiler->output, "\t.globl %s\n", identifier);
				fprintf(compiler->output, "%s:\n", identifier);
				fprintf(compiler->output, "\tendbr64\n");
				fprintf(compiler->output, "\tpushq\t%%rbp\n");
				fprintf(compiler->output, "\tmovq\t%%rsp, %%rbp\n");
				
				free(identifier);
			} break;

			// TODO IGNORE
			case PTT_C_DECLARATION: break;

			case PTT_C_COMPOUND_STATEMENT: {
				// TODO compiler_c_codegen_compound_statement(compiler, stmt);
				
				fprintf(compiler->output, "\t// IR [IR_OC_STORE     , IR_T_I32, \"42\", NULL, \"t1\"]\n");
				fprintf(compiler->output, "\tmovl\t$42, -4(%%rbp)\n");

				fprintf(compiler->output, "\t// IR [IR_OC_RET     , IR_T_I32, \"t1\", NULL, NULL]\n");
				fprintf(compiler->output, "\tmovl\t-4(%%rbp), %%eax\n");
				fprintf(compiler->output, "\tjmp .Lfunc_end_0\n");

				// epilog
				// TODO IR [IR_OC_FUNC_END] , IR_T_I32, NULL, NULL, NULL]
				fprintf(compiler->output, "\t// IR [IR_OC_FUNC_END    , IR_T_I32, NULL, NULL, NULL]\n");
				fprintf(compiler->output, ".Lfunc_end_0:\n");
				fprintf(compiler->output, "\tleave\n");
				fprintf(compiler->output, "\tret\n");
				
			} break;

			default: assert(0 && "NOT REACHABLE");
		}
	}

	return 0;
}
