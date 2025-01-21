#include "lexer_c.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

typedef struct Parser_C {
    Lexer_C *lexer;

    ParseTreeNode_C *parse_tree;

    size_t expected_token_types[TOKEN_TYPE_C_LENGTH];

    size_t num;
} Parser_C;


extern void *parser_c_create(const char *src);

extern void parser_c_destroy(Parser_C *parser);

extern int parser_c_parse(Parser_C *parser);

extern ParseTreeNode_C *parser_c_parse_translation_unit(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_external_declaration(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_function_definition(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_declaration(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_declaration_specifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_declarator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_compound_statement(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_storage_class_specifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_type_specifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_type_qualifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_struct_or_union_specifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_enum_specifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_typedef_name(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_struct_or_union(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_identifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_struct_declaration(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_specifier_qualifier(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_struct_declarator_list(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_struct_declarator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_constant_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_pointer(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_direct_declarator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_parameter_type_list(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_conditional_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_logical_or_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_logical_and_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_inclusive_or_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_exclusive_or_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_and_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_equality_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_relational_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_shift_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_additive_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_multiplicative_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_cast_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_unary_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_type_name(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_postfix_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_unary_operator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_primary_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_assignment_expression(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_constant(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_string(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_integer_constant(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_character_constant(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_floating_constant(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_enumeration_constant(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_assignment_operator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_abstract_declarator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_parameter_list(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_parameter_declaration(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_direct_abstract_declarator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_enumerator_list(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_enumerator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_init_declarator(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_initializer(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_initializer_list(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_statement(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_labeled_statement(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_expression_statement(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_selection_statement(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_iteration_statement(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_jump_statement(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_control_line(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_macro_include(Parser_C *parser);
extern ParseTreeNode_C *parser_c_parse_macro_undef(Parser_C *parser);
