#include "parse_tree_node_c.h"
#include "parse_tree_type_c.h"
#include "token_c.h"

#include <stddef.h>
#include <stdlib.h>

void *parse_tree_node_c_create(ParseTreeType_C type, Token_C *token)
{
    ParseTreeNode_C *node = (ParseTreeNode_C *)calloc(1, sizeof(ParseTreeNode_C));

    if (node == NULL) {
        return NULL;
    }

    node->type = type;

    if (token != NULL) {
        node->token = *token;
    }

    return node;
}

int parse_tree_node_c_add(ParseTreeNode_C *root, ParseTreeNode_C *child)
{
    if (root->elements == NULL) {
        root->elements = (ParseTreeNode_C **)calloc(1, sizeof(ParseTreeNode_C *));

        if (root->elements == NULL) {
            return -1;
        }

        root->elements[root->num] = child;

        root->len = 1;
        root->num++;

        return 0;
    }

    if (root->len == root->num) {
        root->elements = (ParseTreeNode_C **)realloc(root->elements, sizeof(ParseTreeNode_C *) * root->len * 2);

        if (root->elements == NULL) {
            return -1;
        }

        root->elements[root->num] = child;

        root->len *= 2;
        root->num++;

        return 0;
    }

    root->elements[root->num] = child;
    root->num++;

    return 0;
}

void parse_tree_node_c_destroy(ParseTreeNode_C *node)
{
    if (node == NULL) {
        return;
    }

    if (node->elements != NULL) {
        for (size_t i = 0; i < node->num; i++) {
            parse_tree_node_c_destroy(node->elements[i]);
        }
    }

    free(node);
}
