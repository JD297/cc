#include "lexer_c.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

extern ParseTreeNode_C *parser_c_parse(Lexer_C *lexer);

#define parser_c_parse_required(lexer, root_node, name, empty_label) \
        if ((name = parser_c_parse_##name(lexer)) == NULL) { \
            goto empty_label; \
        } \
        \
        parse_tree_node_c_add(this_node, name);

#define parser_c_parse_opt(lexer, root_node, name, next_label) \
        if ((name = parser_c_parse_##name(lexer)) != NULL) { \
            parse_tree_node_c_add(this_node, name); \
        \
            goto next_label; \
        }

#define parser_c_parse_list_required(lexer, root_node, name, empty_label) \
        size_t count_##name; \
        for (count_##name = 0; (name = parser_c_parse_##name((lexer))) != NULL; count_##name++) { \
            parse_tree_node_c_add((root_node), name); \
        }\
        if (count_##name == 0) { \
            goto empty_label; \
        }

#define parser_c_parse_list_opt(lexer, root_node, name) \
        while ((name = parser_c_parse_##name((lexer))) != NULL) { \
            parse_tree_node_c_add((root_node), name); \
        }

extern ParseTreeNode_C *parser_c_parse_translation_unit(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_external_declaration(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_function_definition(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_declaration(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_declaration_specifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_declarator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_compound_statement(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_storage_class_specifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_type_specifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_type_qualifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_struct_or_union_specifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_enum_specifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_typedef_name(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_struct_or_union(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_identifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_struct_declaration(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_specifier_qualifier(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_struct_declarator_list(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_struct_declarator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_constant_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_pointer(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_direct_declarator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_parameter_type_list(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_conditional_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_logical_or_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_logical_and_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_inclusive_or_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_exclusive_or_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_and_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_equality_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_relational_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_shift_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_additive_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_multiplicative_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_cast_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_unary_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_type_name(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_postfix_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_unary_operator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_primary_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_assignment_expression(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_constant(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_string(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_assignment_operator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_abstract_declarator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_parameter_list(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_parameter_declaration(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_direct_abstract_declarator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_enumerator_list(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_enumerator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_init_declarator(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_initializer(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_initializer_list(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_statement(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_labeled_statement(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_expression_statement(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_selection_statement(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_iteration_statement(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_jump_statement(Lexer_C *lexer);

extern ParseTreeNode_C *parser_c_parse_preprocessor_conditional(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_preprocessor_if_line(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_preprocessor_text(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_preprocessor_elif_parts(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_preprocessor_else_part(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_preprocessor_elif_line(Lexer_C *lexer);
extern ParseTreeNode_C *parser_c_parse_preprocessor_else_line(Lexer_C *lexer);
