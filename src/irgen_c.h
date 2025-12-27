#ifndef JD297_CC_IRGEN_C_H
#define JD297_CC_IRGEN_C_H

#include <stddef.h>

#include <jd297/list.h>

#include "ir.h"
#include "parse_tree_node_c.h"

extern int irgen_c_run(IR_CTX *ctx, ParseTreeNode_C *translation_unit);

#endif
