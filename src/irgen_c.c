#include <assert.h>
#include <stdlib.h>

#include "irgen_c.h"
#include "ir.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

static void irgen_c_translation_unit(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_external_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_function_definition(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_declaration_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_compound_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_storage_class_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_type_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_type_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_struct_or_union_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_enum_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_typedef_name(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_struct_or_union(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_identifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_struct_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_specifier_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_struct_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_struct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_constant_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_pointer(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_direct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_parameter_type_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_conditional_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_logical_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_logical_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_inclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_exclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_equality_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_relational_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_shift_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_additive_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_multiplicative_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_cast_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_unary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_type_name(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_postfix_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_unary_operator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_primary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_assignment_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_constant(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_string(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_assignment_operator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_parameter_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_parameter_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_direct_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_enumerator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_enumerator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_init_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_init_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_initializer(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_initializer_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_labeled_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_expression_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_selection_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_iteration_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static void irgen_c_jump_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);

void irgen_c_run(IR_CTX *ctx, ParseTreeNode_C *translation_unit)
{
	irgen_c_translation_unit(ctx, translation_unit);
}

static void irgen_c_translation_unit(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	for (size_t i = 0; i < this_node->num; ++i) {
		irgen_c_external_declaration(ctx, this_node->elements[i]);
	}
}

static void irgen_c_external_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	for (size_t j = 0; j < this_node->num; ++j) {
		ParseTreeNode_C *node = this_node->elements[j];

		switch (node->type) {
			case PTT_C_FUNCTION_DEFINITION: {
				irgen_c_function_definition(ctx, node);
			} break;
			case PTT_C_DECLARATION: {
				irgen_c_declaration(ctx, node);
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			}
		}
	}
}

static void irgen_c_function_definition(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	ctx->symtbl = this_node->symtbl;

	ir_emit(ctx, IR_OC_FUNC_BEGIN, IR_PTR_T, ir_ssa_from_view(ctx, this_node->symtbl->id), ir_ssa_from_num(ctx, 0), NULL);
	
	ctx->label_func_end = ctx->label_tmp++;

	for (size_t i = 0; i < this_node->num; ++i) {
		ParseTreeNode_C *node = this_node->elements[i];

		switch (node->type) {
			case PTT_C_DECLARATION_SPECIFIER: break;

			case PTT_C_DECLARATOR: break; // TODO IGNORE ??

			// TODO IGNORE
			case PTT_C_DECLARATION: break; // TODO save param count

			case PTT_C_COMPOUND_STATEMENT: {
				irgen_c_compound_statement(ctx, node);
			} break;

			default:
				assert(0 && "NOT REACHABLE");
		}
	}

	ir_emit(ctx, IR_OC_FUNC_END, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_func_end), ir_ssa_from_num(ctx, 0), NULL);
	
	ctx->symtbl = ctx->symtbl->parent;
}

static void irgen_c_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	if (this_node->num != 2) {
		// TODO: fprintf(stderr, "<file>:<line>:<col>: warning: declaration does not declare anything [-Wmissing-declarations]\n");
		return;
	}
	
	irgen_c_init_declarator_list(ctx, this_node->elements[1]);
}

static void irgen_c_declaration_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_compound_statement(IR_CTX *ctx, ParseTreeNode_C *compound_statement)
{
	for (size_t i = 0; i < compound_statement->num; ++i) {
		ParseTreeNode_C *node = compound_statement->elements[i];
		
		switch (node->type) {
			case PTT_C_DECLARATION: {
				irgen_c_declaration(ctx, node);
			} break;
			case PTT_C_STATEMENT: {
				irgen_c_statement(ctx, node);
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			}
		}
	}
}

static void irgen_c_storage_class_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_type_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_type_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_struct_or_union_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_enum_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_typedef_name(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_struct_or_union(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_identifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_struct_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_specifier_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_struct_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_struct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_constant_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        irgen_c_conditional_expression(ctx, this_node->elements[0]);
}

static void irgen_c_pointer(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_direct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_parameter_type_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_conditional_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        IRSSAEnt *dssa;
        
        assert(this_node->num == 1 || this_node->num == 3);

        irgen_c_logical_or_expression(ctx, this_node->elements[0]);
        
        if (this_node->num == 1) {
        	return;
        }
        
        const size_t logical_label_end = ctx->label_tmp++;
        const size_t logical_label_false = ctx->label_tmp++;

        dssa = ir_ssa_default(ctx);

        ir_emit(ctx, IR_OC_JMP_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, logical_label_false), ir_ssa_latest(ctx), NULL);

        irgen_c_expression(ctx, this_node->elements[1]);

		ir_emit(ctx, IR_OC_STORE, /* TODO HARD */IR_PTR_T, dssa, ir_ssa_latest(ctx), NULL);

        ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_end), NULL, NULL);

		/* label false */
        ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_false), NULL, NULL);

        irgen_c_conditional_expression(ctx, this_node->elements[2]);

        ir_emit(ctx, IR_OC_STORE, /* TODO HARD */IR_PTR_T, dssa, ir_ssa_latest(ctx), NULL);

        /* label end */
        ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_end), NULL, NULL);

        ctx->ssa_latest = dssa;
}

static void irgen_c_logical_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		ParseTreeNode_C *lnode;
		ParseTreeNode_C *rnode;
        IRSSAEnt *dssa;

        assert(this_node->num > 0);

        lnode = this_node->elements[0];

        if (this_node->num == 1) {
			irgen_c_logical_and_expression(ctx, lnode);

			return;
		}

		const size_t logical_label_end = ctx->label_tmp++;
        const size_t logical_label_true = ctx->label_tmp++;

        if (lnode->type == PTT_C_LOGICAL_OR_EXPRESSION) {
			irgen_c_logical_or_expression(ctx, lnode);
		} else {
			irgen_c_logical_and_expression(ctx, lnode);
		}

		ir_emit(ctx, IR_OC_JMP_NOT_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, logical_label_true), ir_ssa_latest(ctx), NULL);
		
		rnode = this_node->elements[1];

		irgen_c_logical_and_expression(ctx, rnode);

		ir_emit(ctx, IR_OC_JMP_NOT_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, logical_label_true), ir_ssa_latest(ctx), NULL);

		dssa = ir_ssa_default(ctx);

        ir_emit(ctx, IR_OC_IMM, /* TODO HARD */IR_PTR_T, dssa, ir_ssa_from_literal(ctx, ir_literal_from_lu(0)), NULL);

        ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_end), NULL, NULL);
        
        /* label true */
        ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_true), NULL, NULL);

        ir_emit(ctx, IR_OC_IMM, /* TODO HARD */IR_PTR_T, dssa, ir_ssa_from_literal(ctx, ir_literal_from_lu(1)), NULL);
        
        /* label end */
        ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_end), NULL, NULL);

        ctx->ssa_latest = dssa;
}

static void irgen_c_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		assert(this_node->num >= 1);

		for (size_t i = 0; i < this_node->num; ++i) {
			/*
			 * NOTE:
			 * the last assignment_expression is the result of an expression
			 */
			irgen_c_assignment_expression(ctx, this_node->elements[i]);
		}
}

static void irgen_c_logical_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		ParseTreeNode_C *lnode;
		ParseTreeNode_C *rnode;
        IRSSAEnt *dssa;

        assert(this_node->num > 0);

        lnode = this_node->elements[0];

        if (this_node->num == 1) {
			irgen_c_inclusive_or_expression(ctx, lnode);

			return;
		}

		const size_t logical_label_end = ctx->label_tmp++;
        const size_t logical_label_false = ctx->label_tmp++;

        if (lnode->type == PTT_C_LOGICAL_AND_EXPRESSION) {
			irgen_c_logical_and_expression(ctx, lnode);
		} else {
			irgen_c_inclusive_or_expression(ctx, lnode);
		}

		ir_emit(ctx, IR_OC_JMP_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, logical_label_false), ir_ssa_latest(ctx), NULL);
		
		rnode = this_node->elements[1];

		irgen_c_inclusive_or_expression(ctx, rnode);

		ir_emit(ctx, IR_OC_JMP_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, logical_label_false), ir_ssa_latest(ctx), NULL);

		dssa = ir_ssa_default(ctx);

        ir_emit(ctx, IR_OC_IMM, /* TODO HARD */IR_PTR_T, dssa, ir_ssa_from_literal(ctx, ir_literal_from_lu(1)), NULL);

        ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_end), NULL, NULL);
        
        /* label false */
        ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_false), NULL, NULL);

        ir_emit(ctx, IR_OC_IMM, /* TODO HARD */IR_PTR_T, dssa, ir_ssa_from_literal(ctx, ir_literal_from_lu(0)), NULL);
        
        /* label end */
        ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, logical_label_end), NULL, NULL);

        ctx->ssa_latest = dssa;
}

static void irgen_c_inclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;

	assert(this_node->num > 0);
	
	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_exclusive_or_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_INCLUSIVE_OR_EXPRESSION) {
		irgen_c_inclusive_or_expression(ctx, lnode);
	} else {
		irgen_c_exclusive_or_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);
    
    rnode = this_node->elements[1];

    irgen_c_exclusive_or_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	ir_emit(ctx, IR_OC_OR, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_exclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;

	assert(this_node->num > 0);

	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_and_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_EXCLUSIVE_OR_EXPRESSION) {
		irgen_c_exclusive_or_expression(ctx, lnode);
	} else {
		irgen_c_and_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);

	rnode = this_node->elements[1];

    irgen_c_and_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	ir_emit(ctx, IR_OC_XOR, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;

	assert(this_node->num > 0);

	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_equality_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_AND_EXPRESSION) {
		irgen_c_and_expression(ctx, lnode);
	} else {
		irgen_c_equality_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);

	rnode = this_node->elements[1];

    irgen_c_equality_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	ir_emit(ctx, IR_OC_AND, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_equality_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;
	IROpCode op;

	assert(this_node->num > 0);
	
	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_relational_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_EQUALITY_EXPRESSION) {
		irgen_c_equality_expression(ctx, lnode);
	} else {
		irgen_c_relational_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);
    
    rnode = this_node->elements[1];

    irgen_c_relational_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	switch (this_node->token.type) {
		case T_EQUAL_TO:
			op = IR_OC_EQ;
			break;
		case T_NOT_EQUAL_TO:
			op = IR_OC_NEQ;
			break;
		default:
			assert(0 && "NOT REACHABLE");
    }

	ir_emit(ctx, op, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_relational_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;
	IROpCode op;

	assert(this_node->num > 0);
	
	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_shift_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_RELATIONAL_EXPRESSION) {
		irgen_c_relational_expression(ctx, lnode);
	} else {
		irgen_c_shift_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);
    
    rnode = this_node->elements[1];

    irgen_c_shift_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	switch (this_node->token.type) {
		case T_GREATER_THAN:
			op = IR_OC_GT;
			break;
		case T_LESS_THAN:
			op = IR_OC_LT;
			break;
		case T_GREATER_THAN_OR_EQUAL_TO:
			op = IR_OC_GTE;
			break;
		case T_LESS_THAN_OR_EQUAL_TO:
			op = IR_OC_LTE;
			break;
		default:
			assert(0 && "NOT REACHABLE");
    }

	ir_emit(ctx, op, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_shift_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;
	IROpCode op;

	assert(this_node->num > 0);
	
	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_additive_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_SHIFT_EXPRESSION) {
		irgen_c_shift_expression(ctx, lnode);
	} else {
		irgen_c_additive_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);
    
    rnode = this_node->elements[1];

    irgen_c_additive_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	switch (this_node->token.type) {
		case T_BITWISE_LEFTSHIFT:
			op = IR_OC_SAL;
			break;
		case T_BITWISE_RIGHTSHIFT:
			op = IR_OC_SAR;
			break;
		default:
			assert(0 && "NOT REACHABLE");
    }

	ir_emit(ctx, op, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_additive_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;
	IROpCode op;

	assert(this_node->num > 0);
	
	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_multiplicative_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_ADDITIVE_EXPRESSION) {
		irgen_c_additive_expression(ctx, lnode);
	} else {
		irgen_c_multiplicative_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);
    
    rnode = this_node->elements[1];

    irgen_c_multiplicative_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	switch (this_node->token.type) {
		case T_MINUS:
			op = IR_OC_SUB;
			break;
		case T_PLUS:
			op = IR_OC_ADD;
			break;
		default:
			assert(0 && "NOT REACHABLE");
    }

	ir_emit(ctx, op, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_multiplicative_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{       
	ParseTreeNode_C *lnode;
	ParseTreeNode_C *rnode;
	IRSSAEnt *lssa, *rssa, *dssa;
	IROpCode op;

	assert(this_node->num > 0);
	
	lnode = this_node->elements[0];

	if (this_node->num == 1) {
		irgen_c_cast_expression(ctx, lnode);

		return;
	}

	if (lnode->type == PTT_C_MULTIPLICATIVE_EXPRESSION) {
		irgen_c_multiplicative_expression(ctx, lnode);
	} else {
		irgen_c_cast_expression(ctx, lnode);
    }

    lssa = ir_ssa_latest(ctx);
    
    rnode = this_node->elements[1];

    irgen_c_cast_expression(ctx, rnode);

	rssa = ir_ssa_latest(ctx);

    dssa = ir_ssa_default(ctx);

	switch (this_node->token.type) {
		case T_MULTIPLY:
			op = IR_OC_MUL;
			break;
		case T_DIVIDE:
			op = IR_OC_DIV;
			break;
		case T_MODULUS:
			op = IR_OC_MOD;
			break;
		default:
			assert(0 && "NOT REACHABLE");
    }

	ir_emit(ctx, op, /* TODO HARD */IR_PTR_T, dssa, lssa, rssa);
}

static void irgen_c_cast_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		// TODO will happen because parser is buggy ??
		// TODO this_node ??
		if (node->token.type == T_OPEN_PARENT) {
        	assert(0 && "TODO not implemented: with T_OPEN_PARENT");
        }

        irgen_c_unary_expression(ctx, node);
}

static void irgen_c_unary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		switch (node->token.type) {
			case T_UNKNOWN: {
				irgen_c_postfix_expression(ctx, node);
			} return;
			case T_OPEN_PARENT: {
				size_t argument_number = 0;
				IRSSAEnt *dssa;
				IRSSAEnt **argument_ssas = NULL;

				if (node->num == 2) {
					ParseTreeNode_C *argument_expression_list = node->elements[1];
					
					argument_number = argument_expression_list->num;

					argument_ssas = malloc(sizeof(IRSSAEnt *) * argument_expression_list->num);

					for (size_t i = 0; i < argument_expression_list->num; ++i) {
						irgen_c_assignment_expression(ctx, argument_expression_list->elements[i]);

						argument_ssas[i] = ir_ssa_latest(ctx);
					}
				}

				if (node->elements[0]->type != PTT_C_POSTFIX_EXPRESSION) {
					assert(0 && "PTT_C_UNARY_EXPRESSION (must sementically be PTT_C_PRIMARY_EXPRESSION) ?? FAST HACK");
				}
				
				if (node->elements[0]->elements[0]->type != PTT_C_PRIMARY_EXPRESSION) {
					assert(0 && "PTT_C_UNARY_EXPRESSION (must sementically be PTT_C_PRIMARY_EXPRESSION) ?? FAST HACK");
				}
				
				if (node->elements[0]->elements[0]->elements[0]->type != PTT_C_IDENTIFIER) {
					assert(0 && "PTT_C_UNARY_EXPRESSION (must sementically be PTT_C_IDENTIFIER) ?? FAST HACK");
				}
				
				ParseTreeNode_C *identifier = node->elements[0]->elements[0]->elements[0];

				for (size_t i = 0; i < argument_number; ++i) {
					ir_emit(ctx, IR_OC_PARAM, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, i + 1), argument_ssas[i], NULL);
				}

				if (argument_ssas != NULL) {
					free(argument_ssas);
				}

				dssa = ir_ssa_default(ctx);

				ir_emit(ctx, IR_OC_CALL, /* TODO HARD */IR_PTR_T, dssa, ir_ssa_from_view(ctx, &identifier->token.view), ir_ssa_from_num(ctx, argument_number));
			} return;
			case T_DOT:
				assert(0 && "TODO not implemented: T_DOT");
			case T_ARROW:
				assert(0 && "TODO not implemented: T_ARROW");
			case T_INCREMENT:
				assert(0 && "TODO not implemented: T_INCREMENT");
			case T_DECREMENT:
				assert(0 && "TODO not implemented: T_DECREMENT");
			case T_OPEN_BRACE:
				assert(0 && "TODO not implemented: T_OPEN_BRACE");
			default:
				assert(0 && "NOT REACHABLE");
		}
}

static void irgen_c_type_name(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_postfix_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		if (node->token.type != 0) {
        	assert(0 && "TODO not implemented: with ANY TOKEN");
        }

        irgen_c_primary_expression(ctx, node);
}

static void irgen_c_unary_operator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");
}

static void irgen_c_primary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		switch (node->type) {
			case PTT_C_IDENTIFIER: {
				// TODO probably incomplete but fine for now...
				// TODO need globals etc.
				IRSymTblEnt *ent = ir_symtbl_get(ctx->symtbl, &node->token.view, IR_SYMUSE_LOCAL);
				
				assert(ent != NULL);
				
				ir_emit(ctx, IR_OC_LOAD, ent->type, ir_ssa_default(ctx), ir_ssa_from_addr(ctx, &ent->addr), NULL);
			} break;
			case PTT_C_CONSTANT:
				irgen_c_constant(ctx, node);
				break;
			case PTT_C_STRING:
				irgen_c_string(ctx, node);
				break;
			case PTT_C_EXPRESSION:
				irgen_c_expression(ctx, node);
				break;
			default:
				assert(0 && "NOT REACHABLE");
		}
}

static void irgen_c_assignment_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    ParseTreeNode_C *node = this_node->elements[0];
	
	IRSymTblEnt *entry_id = NULL;
	
	switch (node->type) {
		case PTT_C_CONDITIONAL_EXPRESSION: {
			irgen_c_conditional_expression(ctx, node);
			
			return; // NEEDS TO RETURN
		} break;
		case PTT_C_UNARY_EXPRESSION: {
			/*switch (node->token->type) {
				case T_INCREMENT:
					assert(0 && "TODO not implemented: PTT_C_UNARY_EXPRESSION (T_INCREMENT)");
				case T_DECREMENT:
					assert(0 && "TODO not implemented: PTT_C_UNARY_EXPRESSION (T_DECREMENT)");
				case T_DECREMENT:
					assert(0 && "TODO not implemented: PTT_C_UNARY_EXPRESSION (T_DECREMENT)");
			}
			assert(0 && "TODO not implemented: PTT_C_UNARY_EXPRESSION");*/
			
			// TODO only with PTT_C_IDENTIFIER
			ParseTreeNode_C *expr_node = node->elements[0];
			
			if (expr_node->type != PTT_C_POSTFIX_EXPRESSION) {
				assert(0 && "TODO not implemented: PTT_C_UNARY_EXPRESSION (without PTT_C_POSTFIX_EXPRESSION)");
			}
			
			if (expr_node->token.type != T_UNKNOWN) {
				assert(0 && "TODO not implemented: PTT_C_UNARY_EXPRESSION (without T_UNKNOWN)");
			}
			
			if (expr_node->elements[0]->type != PTT_C_PRIMARY_EXPRESSION) {
				assert(0 && "PTT_C_UNARY_EXPRESSION (must sementically be PTT_C_PRIMARY_EXPRESSION)");
			}
			
			if (expr_node->elements[0]->elements[0]->type != PTT_C_IDENTIFIER) {
				assert(0 && "PTT_C_UNARY_EXPRESSION (must sementically be PTT_C_IDENTIFIER)");
			}
			
			ParseTreeNode_C *identifier = expr_node->elements[0]->elements[0];
			
			entry_id = ir_symtbl_get(ctx->symtbl, &identifier->token.view, IR_SYMUSE_LOCAL);
			
			assert(entry_id != NULL && "SYMTBL entry not found :(!");
		} break;
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	assert(entry_id != NULL);

	IRSSAEnt *lval, *rval, *arg1 = NULL, *arg2 = NULL;

	irgen_c_assignment_expression(ctx, this_node->elements[2]);

	rval = ir_ssa_latest(ctx);

	lval = ir_ssa_from_stack(ctx, &entry_id->addr); // TODO stack is hard currently only LOCAL

	if (this_node->elements[1]->token.type != T_ASSIGNMENT) {
		arg1 = lval;
		arg2 = rval;
	} else {
		arg1 = rval;
	}

	IROpCode op;

	switch (this_node->elements[1]->token.type) {
		case T_MULTIPLY_ASSIGN:
			op = IR_OC_MUL;
			break;
		case T_DIVIDE_ASSIGN:
			op = IR_OC_DIV;
			break;
		case T_MODULUS_ASSIGN:
			op = IR_OC_MOD;
			break;
		case T_PLUS_ASSIGN:
			op = IR_OC_ADD;
			break;
		case T_MINUS_ASSIGN:
			op = IR_OC_SUB;
			break;
		case T_BITWISE_LEFTSHIFT_ASSIGN:
			op = IR_OC_SAL;
			break;
		case T_BITWISE_RIGHTSHIFT_ASSIGN:
			op = IR_OC_SAR;
			break;
		case T_BITWISE_AND_ASSIGN:
			op = IR_OC_AND;
			break;
		case T_BITWISE_XOR_ASSIGN:
			op = IR_OC_XOR;
			break;
		case T_BITWISE_OR_ASSIGN:
			op = IR_OC_OR;
			break;
		case T_ASSIGNMENT:
			op = IR_OC_STORE;
			break;
		default:
			assert(0 && "NOT REACHABLE");
	}
	
	ir_emit(ctx, op, entry_id->type, lval, arg1, arg2);
}

static void irgen_c_constant(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    switch (this_node->token.type) {
		case T_INTEGER_CONSTANT:
			ir_emit(ctx, IR_OC_IMM, /* TODO HARD */IR_PTR_T, ir_ssa_default(ctx), ir_ssa_from_literal(ctx, this_node->token.literal), NULL);
			break;
		case T_FLOATING_CONSTANT:
			ir_emit(ctx, IR_OC_IMM, /* TODO HARD */IR_F64_T, ir_ssa_default(ctx), ir_ssa_from_literal(ctx, this_node->token.literal), NULL);
			break;
		case T_CHARACTER_CONSTANT:
			ir_emit(ctx, IR_OC_IMM, /* TODO HARD */IR_S32_T, ir_ssa_default(ctx), ir_ssa_from_literal(ctx, this_node->token.literal), NULL);
			break;
		case T_IDENTIFIER:
			assert(0 && "TODO not implemented: PTT_C_ENUMERATION_CONSTANT");
			break;
		default:
			assert(0 && "NOT REACHABLE");
	}
}

static void irgen_c_string(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	const size_t label_str = ctx->label_str++;

    ir_emit(ctx, IR_OC_STRING, IR_PTR_T, ir_ssa_from_str(ctx, label_str), ir_ssa_from_literal(ctx, this_node->token.literal) , NULL);

	ir_emit(ctx, IR_OC_LOAD, IR_PTR_T, ir_ssa_default(ctx), ir_ssa_from_str(ctx, label_str), NULL);
}

static void irgen_c_assignment_operator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	(void) ctx;
    (void) this_node;

	assert(0 && "FUNCTION IS DEPRECATED!");
}

static void irgen_c_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    (void) ctx;
    (void) this_node;

    assert(0 && "TODO not implemented");
}

static void irgen_c_parameter_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    (void) ctx;
    (void) this_node;

    assert(0 && "TODO not implemented");
}

static void irgen_c_parameter_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    (void) ctx;
    (void) this_node;

    assert(0 && "TODO not implemented");
}

static void irgen_c_direct_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    (void) ctx;
    (void) this_node;

    assert(0 && "TODO not implemented");
}

static void irgen_c_enumerator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    (void) ctx;
    (void) this_node;

    assert(0 && "TODO not implemented");
}

static void irgen_c_enumerator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    (void) ctx;
    (void) this_node;

    assert(0 && "TODO not implemented");
}

static void irgen_c_init_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	for (size_t i = 0; i < this_node->num; ++i) {
		irgen_c_init_declarator(ctx, this_node->elements[i]);
	}
}

static void irgen_c_init_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	IRSSAEnt *ssa_local = ir_ssa_from_stack(ctx, &this_node->symtblent->addr);

    ir_emit(ctx, IR_OC_LOCAL, this_node->symtblent->type, ssa_local, NULL, NULL);
    
    if (this_node->num == 1) {	
    	return;
    }

	irgen_c_initializer(ctx, this_node->elements[1]);
	
	IRSSAEnt *ssa_init = ir_ssa_latest(ctx);

	ir_emit(ctx, IR_OC_STORE, this_node->symtblent->type, ssa_local, ssa_init, NULL);
}

static void irgen_c_initializer(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    ParseTreeNode_C *node = this_node->elements[0];

	switch (node->type) {
		case PTT_C_ASSIGNMENT_EXPRESSION:
			irgen_c_assignment_expression(ctx, node);
			break;
		case PTT_C_INITIALIZER_LIST:
			assert(0 && "TODO not implemented ({ initializer_list })");
			break;
		default:
			assert(0 && "NOT REACHABLE");
	}
}

static void irgen_c_initializer_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    (void) ctx;
    (void) this_node;

    assert(0 && "TODO not implemented");
}

static void irgen_c_statement(IR_CTX *ctx, ParseTreeNode_C *statement)
{
	ParseTreeNode_C *node = statement->elements[0];
	
	switch (node->type) {
		case PTT_C_LABELED_STATEMENT: {
			irgen_c_labeled_statement(ctx, node);
		} break;
		case PTT_C_EXPRESSION_STATEMENT: {
			irgen_c_expression_statement(ctx, node);
		} break;
		case PTT_C_COMPOUND_STATEMENT: {
			// TODO change symtbl
			irgen_c_compound_statement(ctx, node);
		} break;
		case PTT_C_SELECTION_STATEMENT: {
			irgen_c_selection_statement(ctx, node);
		} break;
		case PTT_C_ITERATION_STATEMENT: {
			irgen_c_iteration_statement(ctx, node);
		} break;
		case PTT_C_JUMP_STATEMENT: {
			irgen_c_jump_statement(ctx, node);
		} break;
		default: {
			assert(0 && "NOT REACHABLE");
		}
	}
}

static void irgen_c_labeled_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	switch (this_node->token.type) {
		case T_IDENTIFIER: {
			IRSymTblEnt *label_entry;
			ParseTreeNode_C *statement = this_node->elements[0];
			
			label_entry = ir_symtbl_get(ctx->symtbl, &this_node->token.view, IR_SYMUSE_LABEL);
			
			assert(label_entry != NULL);
			
			label_entry->addr = ctx->label_tmp++;
			
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, label_entry->addr), NULL, NULL);
			
			irgen_c_statement(ctx, statement);
		} break;
		case T_DEFAULT: {
			ParseTreeNode_C *statement = this_node->elements[0];
			const size_t label = ctx->label_tmp++;
			
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, label), NULL, NULL);
			
			irgen_c_statement(ctx, statement);

			ctx->switch_stmt_section = ctx->code_current;
			
			ctx->code_current = ctx->switch_test_section;
			
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, label), NULL, NULL);
			
			ctx->switch_test_section = list_prev(ctx->code_current);
			
			ctx->code_current = ctx->switch_stmt_section;
		} break;
		case T_CASE: {
			ParseTreeNode_C *constant_expression = this_node->elements[0];
			ParseTreeNode_C *statement = this_node->elements[1];
			const size_t label = ctx->label_tmp++;
			
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, label), NULL, NULL);
			
			irgen_c_statement(ctx, statement);

			ctx->switch_stmt_section = ctx->code_current;
			
			ctx->code_current = ctx->switch_test_section;
			
			irgen_c_constant_expression(ctx, constant_expression);
			
			ir_emit(ctx, IR_OC_EQ, /* TODO HARD */IR_PTR_T, ir_ssa_default(ctx), ir_ssa_latest(ctx), ctx->switch_expression);
			
			ir_emit(ctx, IR_OC_JMP_NOT_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, label), ir_ssa_latest(ctx), NULL);
			
			ctx->switch_test_section = ctx->code_current;
			
			ctx->code_current = ctx->switch_stmt_section;
		} break;
		default: {
			assert(0 && "NOT REACHABLE");
		}
	}
}

static void irgen_c_expression_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
    if (this_node->num == 0) {
		return;
	}

    irgen_c_expression(ctx, this_node->elements[0]);
}

static void irgen_c_selection_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	switch(this_node->token.type) {
    	case T_IF: {
    		const size_t before_label_select_begin = ctx->label_select_begin;
			const size_t before_label_select_end = ctx->label_select_end;

			ctx->label_select_begin = ctx->label_tmp++;
			ctx->label_select_end = ctx->label_tmp++;

    		irgen_c_expression(ctx, this_node->elements[0]);
    		
    		ir_emit(ctx, IR_OC_JMP_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_select_begin), ir_ssa_latest(ctx), NULL);

    		irgen_c_statement(ctx, this_node->elements[1]);

    		/* jmp_if_end */
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_select_end), NULL, NULL);

    		/* label_else */
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_select_begin), NULL, NULL);

    		if (this_node->num == 3) {
    			irgen_c_statement(ctx, this_node->elements[2]);

    			/* jmp_if_end */
				ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_select_end), NULL, NULL);
			}
			
			/* label_end */
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_select_end), NULL, NULL);

			ctx->label_select_begin = before_label_select_begin;
			ctx->label_select_end = before_label_select_end;
    	} break;
    	case T_SWITCH: {
    		IRSSAEnt *before_switch_expression = ctx->switch_expression;
    		const size_t label_switch_test = ctx->label_tmp++;
    		const size_t before_label_iter_end = ctx->label_iter_end;
    		list_node_t *before_switch_stmt_section = ctx->switch_stmt_section;
    		list_node_t *before_switch_test_section = ctx->switch_test_section;
    		list_node_t *switch_end;
    		
    		ctx->label_iter_end = ctx->label_tmp++;
    		
    		irgen_c_expression(ctx, this_node->elements[0]);
    		
    		ctx->switch_expression = ir_ssa_latest(ctx);
    		
    		/* jmp switch_test */
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, label_switch_test), NULL, NULL);
    		
    		/* ...[switch_stmt_section] */
    		ctx->switch_stmt_section = ctx->code_current;
    		
    		/* jmp switch_end */
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_end), NULL, NULL);
			
			/* label switch_test */
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, label_switch_test), NULL, NULL);
    		
    		/* ...[switch_test_section] */
    		ctx->switch_test_section = ctx->code_current;
    		
    		/* label switch_end */
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_end), NULL, NULL);
    		
    		switch_end = ctx->code_current;
    		
    		ctx->code_current = ctx->switch_stmt_section;
    		
    		irgen_c_statement(ctx, this_node->elements[1]);

    		ctx->code_current = switch_end;
    		ctx->switch_expression = before_switch_expression;
    		ctx->label_iter_end = before_label_iter_end;
    		ctx->switch_stmt_section = before_switch_stmt_section;
    		ctx->switch_test_section = before_switch_test_section;
    	} break;
    	default: {
			assert(0 && "NOT REACHABLE");
		}
    }
}

static void irgen_c_iteration_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	const size_t before_label_iter_begin = ctx->label_iter_begin;
	const size_t before_label_iter_end = ctx->label_iter_end;

	ctx->label_iter_begin = ctx->label_tmp++;
	ctx->label_iter_end = ctx->label_tmp++;

    switch(this_node->token.type) {
		case T_WHILE: {
			/* label_begin */
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_begin), NULL, NULL);

			ParseTreeNode_C *expression = this_node->elements[0];

			irgen_c_expression(ctx, expression);

			ir_emit(ctx, IR_OC_JMP_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_end), ir_ssa_latest(ctx), NULL);
			
			ParseTreeNode_C *statement = this_node->elements[1];

			irgen_c_statement(ctx, statement);
			
			/* jmp_begin */
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_begin), NULL, NULL);
		} break;
		case T_FOR: {
			ParseTreeNode_C *expression_init = NULL;
			ParseTreeNode_C *expression_cond = NULL;
			ParseTreeNode_C *expression_last = NULL;
			ParseTreeNode_C *statement = this_node->elements[this_node->num - 1];
			
			for (size_t i = 0; i < this_node->num - 1; ++i) {
				ParseTreeNode_C *expression = this_node->elements[i];
				
				switch (expression->token.type) {
					case T_OPEN_PARENT:
						expression_init = expression;
						break;
					case T_SEMICOLON:
						expression_cond = expression;
						break;
					case T_CLOSING_PARENT:
						expression_last = expression;
						break;
					default:
						assert(0 && "NOT REACHABLE");
				}
			}
			
			if (expression_init != NULL) {
				irgen_c_expression(ctx, expression_init);
			}

			const size_t for_label_begin = ctx->label_iter_begin;

			if (expression_last != NULL) {
				ctx->label_iter_begin = ctx->label_tmp++;
			}

			/* label_begin */
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, for_label_begin), NULL, NULL);
			
			if (expression_cond != NULL) {
				irgen_c_expression(ctx, expression_cond);

				ir_emit(ctx, IR_OC_JMP_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_end), ir_ssa_latest(ctx), NULL);
			}
			
			irgen_c_statement(ctx, statement);
			
			if (expression_last != NULL) {
				/* label_expression_last */
				ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_begin), NULL, NULL);

				irgen_c_expression(ctx, expression_last);
			}

			/* jmp_begin */
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, for_label_begin), NULL, NULL);
		} break;
		case T_DO: {
			/* label_begin */
			ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_begin), NULL, NULL);

			ParseTreeNode_C *statement = this_node->elements[0];

			irgen_c_statement(ctx, statement);
			
			ParseTreeNode_C *expression = this_node->elements[1];

			irgen_c_expression(ctx, expression);
			
			ir_emit(ctx, IR_OC_JMP_NOT_ZERO, /* TODO HARD */IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_begin), ir_ssa_latest(ctx), NULL);
		} break;
		default: {
			assert(0 && "NOT REACHABLE");
		}
    }
    
    /* label_end */
	ir_emit(ctx, IR_OC_LABEL, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_end), NULL, NULL);
	
	ctx->label_iter_begin = before_label_iter_begin;
	ctx->label_iter_end = before_label_iter_end;
}

static void irgen_c_jump_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	switch(this_node->token.type) {
		case T_GOTO: {
			ParseTreeNode_C *identifier;
			IRSymTblEnt *label_entry;
			
			identifier = this_node->elements[0];
			
			assert(identifier != NULL);
			
			label_entry = ir_symtbl_get(ctx->symtbl, &identifier->token.view, IR_SYMUSE_LABEL);
			
			assert(label_entry != NULL);

			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_addr(ctx, &label_entry->addr), NULL, NULL);
		} break;
		case T_CONTINUE: {
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_begin), NULL, NULL);
		} break;
		case T_BREAK: {
			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_iter_end), NULL, NULL);
		} break;
		case T_RETURN: {
			if (this_node->num > 0) {
				irgen_c_expression(ctx, this_node->elements[0]);
				
				ir_emit(ctx, IR_OC_RET, /* TODO HARD */IR_PTR_T, ir_ssa_latest(ctx), NULL, NULL);
			}

			ir_emit(ctx, IR_OC_JMP, IR_PTR_T, ir_ssa_from_num(ctx, ctx->label_func_end), NULL, NULL);
		} break;
		default:
			assert(0 && "NOT REACHABLE");
	}
}
