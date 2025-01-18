#include "parse_tree_type_c.h"
#include "token_c.h"

#ifndef JD297_CC_PARSE_TREE_NODE_C_H
#define JD297_CC_PARSE_TREE_NODE_C_H

typedef struct ParseTreeNode_C {
    ParseTreeType_C type;
    Token_C *token;

    struct ParseTreeNode_C **elements;
    size_t len;
    size_t num;
} ParseTreeNode_C;

extern void *parse_tree_node_c_create(ParseTreeType_C type, Token_C *token);

extern int parse_tree_node_c_add(ParseTreeNode_C *root, ParseTreeNode_C *child);

extern void parse_tree_node_c_destroy(ParseTreeNode_C *node);

#endif
