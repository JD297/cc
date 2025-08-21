#ifndef JD297_CC_COMPILER_C_H
#define JD297_CC_COMPILER_C_H

#include <stddef.h>
#include <stdio.h>

#include "parse_tree_node_c.h"

typedef struct {
	FILE *input;
    FILE *output;
} Compiler_C;

extern int compiler_c_run(Compiler_C *compiler);

extern int compiler_c_codegen(Compiler_C *compiler, ParseTreeNode_C *translation_unit);

extern int compiler_c_codegen_function_definition(Compiler_C *compiler, ParseTreeNode_C *node);

extern int compiler_c_codegen_compound_statement(Compiler_C *compiler, ParseTreeNode_C *node);

#endif
