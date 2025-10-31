#ifndef JD297_CC_IR_H
#define JD297_CC_IR_H

#include "symtbl.h"
#include "parse_tree_node_c.h"

#include <jd297/list.h>
#include <jd297/lmap_sv.h>

typedef struct {
	list_t *code;
	SymTbl *symtbl; // TODO remove ??
} IR_CTX;

typedef enum {
	IR_OC_FUNC_BEGIN,
	IR_OC_FUNC_END,
	IR_OC_JMP_FUNC_END,
	IR_OC_LABEL,
	IR_OC_JMP,
	IR_OC_RET,
	IR_OC_STORE_CONST
} IROpCode;

typedef struct {
	IROpCode op;
	SymTblEnt *arg1;
	SymTblEnt *arg2;
	SymTblEnt *result;
} IRCode;

extern int ir_run(IR_CTX *ctx, ParseTreeNode_C *translation_unit);

extern int ir_translation_unit(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_external_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_function_definition(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_declaration_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_compound_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_storage_class_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_type_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_type_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_struct_or_union_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_enum_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_typedef_name(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_struct_or_union(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_identifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_struct_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_specifier_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_struct_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_struct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_constant_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_pointer(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_direct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_parameter_type_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_conditional_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_logical_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_logical_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_inclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_exclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_equality_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_relational_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_shift_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_additive_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_multiplicative_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_cast_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_unary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_type_name(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_postfix_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_unary_operator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_primary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_assignment_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_constant(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_string(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_assignment_operator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_parameter_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_parameter_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_direct_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_enumerator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_enumerator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_init_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_initializer(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_initializer_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_labeled_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_expression_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_selection_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_iteration_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
extern int ir_jump_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);

#endif

