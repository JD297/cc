#include "lexer_c.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

#include <jd297/lmap.h>

typedef struct {
	int anonymous_block_count;
	int error_count;
	Lexer_C *lexer;
	lmap_t *symtbl;
} Parser_C_CTX;

extern ParseTreeNode_C *parser_c_parse(Parser_C_CTX *ctx);

#define parser_c_parse_required(ctx, root_node, name, empty_label) \
        if ((name = parser_c_parse_##name((ctx))) == NULL) { \
            goto empty_label; \
        } \
        \
        parse_tree_node_c_add((this_node), (name));

#define parser_c_parse_opt(ctx, root_node, name, next_label) \
        if ((name = parser_c_parse_##name((ctx))) != NULL) { \
            parse_tree_node_c_add((this_node), (name)); \
        \
            goto next_label; \
        }

#define parser_c_parse_list_required(ctx, root_node, name, empty_label) \
        size_t count_##name; \
        for (count_##name = 0; (name = parser_c_parse_##name((ctx))) != NULL; count_##name++) { \
            parse_tree_node_c_add((root_node), name); \
        }\
        if (count_##name == 0) { \
            goto empty_label; \
        }

#define parser_c_parse_list_opt(ctx, root_node, name) \
        while ((name = parser_c_parse_##name((ctx))) != NULL) { \
            parse_tree_node_c_add((root_node), name); \
        }

extern ParseTreeNode_C *parser_c_parse_translation_unit(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_external_declaration(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_function_definition(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_declaration(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_declaration_specifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_declarator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_compound_statement(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_storage_class_specifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_type_specifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_type_qualifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_struct_or_union_specifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_enum_specifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_typedef_name(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_struct_or_union(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_identifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_struct_declaration(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_specifier_qualifier(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_struct_declarator_list(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_struct_declarator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_constant_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_pointer(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_direct_declarator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_parameter_type_list(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_conditional_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_logical_or_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_logical_and_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_inclusive_or_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_exclusive_or_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_and_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_equality_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_relational_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_shift_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_additive_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_multiplicative_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_cast_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_unary_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_type_name(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_postfix_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_unary_operator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_primary_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_assignment_expression(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_constant(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_string(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_assignment_operator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_abstract_declarator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_parameter_list(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_parameter_declaration(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_direct_abstract_declarator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_enumerator_list(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_enumerator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_init_declarator(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_initializer(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_initializer_list(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_statement(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_labeled_statement(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_expression_statement(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_selection_statement(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_iteration_statement(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_jump_statement(Parser_C_CTX *ctx);

extern ParseTreeNode_C *parser_c_parse_preprocessor_conditional(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_preprocessor_if_line(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_preprocessor_text(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_preprocessor_elif_parts(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_preprocessor_else_part(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_preprocessor_elif_line(Parser_C_CTX *ctx);
extern ParseTreeNode_C *parser_c_parse_preprocessor_else_line(Parser_C_CTX *ctx);
