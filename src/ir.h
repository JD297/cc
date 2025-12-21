#ifndef JD297_CC_IR_H
#define JD297_CC_IR_H

#include "symtbl.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

#include <jd297/list.h>
#include <jd297/lmap_sv.h>
#include <jd297/sv.h>

typedef struct {
	list_t *code;
	SymTbl *symtbl;
	size_t stack_offset;
	size_t label_tmp;
	size_t label_func_end;
	size_t label_iter_begin;
	size_t label_iter_end;
	size_t label_select_begin;
	size_t label_select_end;
	size_t label_str;
} IR_CTX;

typedef enum {
	IR_OC_FUNC_BEGIN,
	IR_OC_FUNC_END,
	IR_OC_IMM_I32,
	IR_OC_IMM_I64,
	IR_OC_PUSH,
	IR_OC_POP,
	IR_OC_SAL,
	IR_OC_SAR,
	IR_OC_ADD,
	IR_OC_SUB,
	IR_OC_MUL,
	IR_OC_DIV,
	IR_OC_MOD,
	IR_OC_OR,
	IR_OC_XOR,
	IR_OC_AND,
	IR_OC_EQ,
	IR_OC_NEQ,
	IR_OC_GT,
	IR_OC_LT,
	IR_OC_GTE,
	IR_OC_LTE,
	IR_OC_LABEL,
	IR_OC_JMP,
	IR_OC_JMP_ZERO,
	IR_OC_JMP_NOT_ZERO,
	IR_OC_RET,
	IR_OC_LOCAL,
	IR_OC_LOAD,
	IR_OC_STORE,
	IR_OC_STACK_ALLOC,
	IR_OC_STACK_DEALLOC,
	IR_OC_PARAM,
	IR_OC_CALL,
	IR_OC_STRING,
	IR_OC_LOAD_STRING,
} IROpCode;

typedef enum {
	IR_TYPE_PTR,
	IR_TYPE_STACK,
	IR_TYPE_NUM,
	IR_TYPE_VIEW,
} IRType;

typedef struct {
	IROpCode op;
	IRType type;
	union {
		SymTblEnt *ptr;
		SymTblEnt stack;
		literal_t literal;
		size_t num;
	} arg1;
	union {
		SymTblEnt *ptr;
		SymTblEnt stack;
	} arg2;
	union {
		SymTblEnt *ptr;
		SymTblEnt stack;
		size_t num;
		sv_t *view;
	} result;
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
extern int ir_init_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
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

