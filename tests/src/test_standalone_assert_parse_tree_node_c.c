#include "parse_tree_node_c.h"
#include "parse_tree_type_c.h"

#include <assert.h>

int main(void)
{
    ParseTreeNode_C *root = parse_tree_node_c_create(PTTC_STATEMENT, NULL);

    assert(root != NULL);
    assert(root->type == PTTC_STATEMENT);
    assert(root->token == NULL);
    assert(root->elements == NULL);
    assert(root->num == 0);
    assert(root->len == 0);

    ParseTreeNode_C *child_level_1_num_0 = parse_tree_node_c_create(PTTC_TRANSLATION_UNIT, NULL);

    assert(parse_tree_node_c_add(root, child_level_1_num_0) != -1);

    assert(root->num == 1);
    assert(root->len == 1);
    assert(root->elements != NULL);
    assert(root->elements[0]->type == PTTC_TRANSLATION_UNIT);

    ParseTreeNode_C *child_level_2_num_0 = parse_tree_node_c_create(PTTC_POINTER, NULL);
    ParseTreeNode_C *child_level_2_num_1 = parse_tree_node_c_create(PTTC_AND_EXPRESSION, NULL);
    ParseTreeNode_C *child_level_2_num_2 = parse_tree_node_c_create(PTTC_CONSTANT, NULL);

    assert(parse_tree_node_c_add(child_level_1_num_0, child_level_2_num_0) != -1);
    assert(parse_tree_node_c_add(child_level_1_num_0, child_level_2_num_1) != -1);
    assert(parse_tree_node_c_add(child_level_1_num_0, child_level_2_num_2) != -1);

    assert(child_level_1_num_0->num == 3);
    assert(child_level_1_num_0->len == 4);
    assert(child_level_1_num_0->elements != NULL);

    assert(child_level_1_num_0->elements[0]->type == PTTC_POINTER);
    assert(child_level_1_num_0->elements[1]->type == PTTC_AND_EXPRESSION);
    assert(child_level_1_num_0->elements[2]->type == PTTC_CONSTANT);

    parse_tree_node_c_destroy(root);

    return 0;
}
