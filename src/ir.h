#ifndef JD297_CC_IR_H
#define JD297_CC_IR_H

#include "symtblent.h"
#include "parse_tree_node_c.h"

#include <jd297/list.h>
#include <jd297/lmap.h>

typedef struct {
	list_t *code;
	lmap_t *symtbl;
	size_t label_func_count;
} IR_CTX;

typedef enum {
	IR_OC_FUNC_BEGIN,
	IR_OC_FUNC_END,
	IR_OC_STORE,
	IR_OC_RET
} IROpCode;

typedef struct {
	IROpCode op;
	SymtblEnt *arg1;
	SymtblEnt *arg2;
	SymtblEnt *result;
} IRCode;

extern int ir_run(IR_CTX *ctx, ParseTreeNode_C *translation_unit);

#endif

