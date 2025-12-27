#include <assert.h>
#include <stdlib.h>

#include "irgen_c.h"
#include "ir.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

static int irgen_c_translation_unit(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_external_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_function_definition(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_declaration_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_compound_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_storage_class_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_type_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_type_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_struct_or_union_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_enum_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_typedef_name(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_struct_or_union(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_identifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_struct_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_specifier_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_struct_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_struct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_constant_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_pointer(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_direct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_parameter_type_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_conditional_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_logical_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_logical_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_inclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_exclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_equality_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_relational_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_shift_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_additive_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_multiplicative_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_cast_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_unary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_type_name(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_postfix_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_unary_operator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_primary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_assignment_expression(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_constant(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_string(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_assignment_operator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_parameter_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_parameter_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_direct_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_enumerator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_enumerator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_init_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_init_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_initializer(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_initializer_list(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_labeled_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_expression_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_selection_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_iteration_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);
static int irgen_c_jump_statement(IR_CTX *ctx, ParseTreeNode_C *this_node);

int irgen_c_run(IR_CTX *ctx, ParseTreeNode_C *translation_unit)
{
	if (irgen_c_translation_unit(ctx, translation_unit) != 0) {
		return -1;
	}

	return 0;
}

static int irgen_c_translation_unit(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	for (size_t i = 0; i < this_node->num; ++i) {
		if (irgen_c_external_declaration(ctx, this_node->elements[i]) != 0) {
			return -1;
		}
	}

	return 0;
}

static int irgen_c_external_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	for (size_t j = 0; j < this_node->num; ++j) {
		ParseTreeNode_C *node = this_node->elements[j];

		switch (node->type) {
			case PTT_C_FUNCTION_DEFINITION: {
				if (irgen_c_function_definition(ctx, node) != 0) {
					return -1;
				}
			} break;
			case PTT_C_DECLARATION: {
				if (irgen_c_declaration(ctx, node) != 0) {
					return -1;
				}
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			}
		}
	}

	return 0;
}

static int irgen_c_function_definition(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	ctx->symtbl = this_node->symtbl;

	ir_emit_func_begin(ctx, this_node->symtbl->id);

	for (size_t i = 0; i < this_node->num; ++i) {
		ParseTreeNode_C *node = this_node->elements[i];

		switch (node->type) {
			case PTT_C_DECLARATION_SPECIFIER: break;

			case PTT_C_DECLARATOR: break; // TODO IGNORE ??

			// TODO IGNORE
			case PTT_C_DECLARATION: break; // TODO save param count

			case PTT_C_COMPOUND_STATEMENT: {
				if (irgen_c_compound_statement(ctx, node) != 0) {
					return -1;
				}
			} break;

			default:
				assert(0 && "NOT REACHABLE");
		}
	}

	ir_emit_func_end(ctx);
	
	ctx->symtbl = ctx->symtbl->parent;

	return 0;
}

static int irgen_c_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	if (this_node->num != 2) {
		// TODO: fprintf(stderr, "<file>:<line>:<col>: warning: declaration does not declare anything [-Wmissing-declarations]\n");
		return 0;
	}
	
	return irgen_c_init_declarator_list(ctx, this_node->elements[1]);
}

static int irgen_c_declaration_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_compound_statement(IR_CTX *ctx, ParseTreeNode_C *compound_statement)
{
	for (size_t i = 0; i < compound_statement->num; ++i) {
		ParseTreeNode_C *node = compound_statement->elements[i];
		
		switch (node->type) {
			case PTT_C_DECLARATION: {
				if (irgen_c_declaration(ctx, node) != 0) {
					return -1;
				}
			} break;
			case PTT_C_STATEMENT: {
				if (irgen_c_statement(ctx, node) != 0) {
					return -1;
				}
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			}
		}
	}

	return 0;
}

static int irgen_c_storage_class_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_type_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_type_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_struct_or_union_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_enum_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_typedef_name(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_struct_or_union(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_identifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_struct_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_specifier_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_struct_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_struct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_constant_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_pointer(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_direct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_parameter_type_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_conditional_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_TERNARY) {
        	assert(0 && "TODO not implemented: with T_TERNARY");
        }
        
        return irgen_c_logical_or_expression(ctx, node);
}

static int irgen_c_logical_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_LOGICAL_OR) {
        	assert(0 && "TODO not implemented: with T_LOGICAL_OR");
        }
        
        return irgen_c_logical_and_expression(ctx, node);
}

static int irgen_c_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		for (size_t i = 0; i < this_node->num; ++i) {
			/*
			 * NOTE:
			 * the last assignment_expression is the result of an expression
			 */
			if (irgen_c_assignment_expression(ctx, this_node->elements[i]) == -1) {
				return -1;
			}
		}

        return 0;
}

static int irgen_c_logical_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_LOGICAL_AND) {
        	assert(0 && "TODO not implemented: with T_LOGICAL_AND");
        }
        
        return irgen_c_inclusive_or_expression(ctx, node);
}

static int irgen_c_inclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_exclusive_or_expression(ctx, node);
		}
		
		ParseTreeNode_C *right = this_node->elements[1];
        
        if (irgen_c_exclusive_or_expression(ctx, right) != 0) {
        	return -1;
        }
		
		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_INCLUSIVE_OR_EXPRESSION) {
			if (irgen_c_inclusive_or_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_exclusive_or_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }

		ir_emit_pop(ctx, IR_PTR_T, IR_REG2);
		
		ir_emit_or(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

		return 0;
}

static int irgen_c_exclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_and_expression(ctx, node);
		}
		
		ParseTreeNode_C *right = this_node->elements[1];
        
        if (irgen_c_and_expression(ctx, right) != 0) {
        	return -1;
        }
		
		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_EXCLUSIVE_OR_EXPRESSION) {
			if (irgen_c_exclusive_or_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_and_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }
		
        ir_emit_pop(ctx, IR_PTR_T, IR_REG2);

		ir_emit_xor(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
		
		return 0;
}

static int irgen_c_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_equality_expression(ctx, node);
		}
		
		ParseTreeNode_C *right = this_node->elements[1];
        
        if (irgen_c_equality_expression(ctx, right) != 0) {
        	return -1;
        }
		
		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_AND_EXPRESSION) {
			if (irgen_c_and_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_equality_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }
		
        ir_emit_pop(ctx, IR_PTR_T, IR_REG2);
		
		ir_emit_and(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
		
		return 0;
}

static int irgen_c_equality_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_relational_expression(ctx, node);
		}

        ParseTreeNode_C *right = this_node->elements[1];
        
        if (irgen_c_relational_expression(ctx, right) != 0) {
        	return -1;
        }
		
		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_EQUALITY_EXPRESSION) {
			if (irgen_c_equality_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_relational_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }
		
        ir_emit_pop(ctx, IR_PTR_T, IR_REG2);

        switch (this_node->token.type) {
        	case T_EQUAL_TO: {
        		ir_emit_eq(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

        		return 0;
    		}
        	case T_NOT_EQUAL_TO: {
	        	ir_emit_neq(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
				
        		return 0;
        	}
        	default:
        		assert(0 && "NOT REACHABLE");
        }
}

static int irgen_c_relational_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_shift_expression(ctx, node);
		}

        ParseTreeNode_C *right = this_node->elements[1];
        
        if (irgen_c_shift_expression(ctx, right) != 0) {
        	return -1;
        }
		
		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_RELATIONAL_EXPRESSION) {
			if (irgen_c_relational_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_shift_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }
		
        ir_emit_pop(ctx, IR_PTR_T, IR_REG2);

        switch (this_node->token.type) {
        	case T_GREATER_THAN: {
        		ir_emit_gt(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

        		return 0;
    		}
        	case T_LESS_THAN: {
	        	ir_emit_lt(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
				
        		return 0;
        	}
        	case T_GREATER_THAN_OR_EQUAL_TO: {
	        	ir_emit_gte(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
				
        		return 0;
        	}
        	case T_LESS_THAN_OR_EQUAL_TO: {
	        	ir_emit_lte(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
				
        		return 0;
        	}
        	default:
        		assert(0 && "NOT REACHABLE");
        }
}

static int irgen_c_shift_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_additive_expression(ctx, node);
		}

        ParseTreeNode_C *right = this_node->elements[1];
        
        if (irgen_c_additive_expression(ctx, right) != 0) {
        	return -1;
        }
		
		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_SHIFT_EXPRESSION) {
			if (irgen_c_shift_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_additive_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }

        ir_emit_pop(ctx, IR_PTR_T, IR_REG2);

        switch (this_node->token.type) {
        	case T_BITWISE_LEFTSHIFT: {
        		ir_emit_sal(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

        		return 0;
    		}
        	case T_BITWISE_RIGHTSHIFT: {
	        	ir_emit_sar(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
				
        		return 0;
        	}
        	default:
        		assert(0 && "NOT REACHABLE");
        }
}

static int irgen_c_additive_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_multiplicative_expression(ctx, node);
		}

        ParseTreeNode_C *right = this_node->elements[1];
        
        if (irgen_c_multiplicative_expression(ctx, right) != 0) {
        	return -1;
        }
		
		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_ADDITIVE_EXPRESSION) {
			if (irgen_c_additive_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_multiplicative_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }

        ir_emit_pop(ctx, IR_PTR_T, IR_REG2);

        switch (this_node->token.type) {
        	case T_MINUS: {
        		ir_emit_sub(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

        		return 0;
    		}
        	case T_PLUS: {
	        	ir_emit_add(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
				
        		return 0;
        	}
        	default:
        		assert(0 && "NOT REACHABLE");
        }
}

static int irgen_c_multiplicative_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{       
        if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return irgen_c_cast_expression(ctx, node);
		}

        ParseTreeNode_C *right = this_node->elements[1];

        if (irgen_c_cast_expression(ctx, right) != 0) {
        	return -1;
        }

		ir_emit_push(ctx, IR_PTR_T, IR_ATYPE_REG, IR_REG1);

		ParseTreeNode_C *left = this_node->elements[0];
		
		if (left->type == PTT_C_MULTIPLICATIVE_EXPRESSION) {
			if (irgen_c_multiplicative_expression(ctx, left) != 0) {
				return -1;
			}
		} else {
		    if (irgen_c_cast_expression(ctx, left) != 0) {
		    	return -1;
		    }
        }

		ir_emit_pop(ctx, IR_PTR_T, IR_REG2);

        switch (this_node->token.type) {
        	case T_MULTIPLY: {
        		ir_emit_mul(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

        		return 0;
        	}
        	case T_DIVIDE: {
        		ir_emit_div(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

        		return 0;
        	}
        	case T_MODULUS: {
        		ir_emit_mod(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);

        		return 0;
        	}
        	default:
        		assert(0 && "NOT REACHABLE");
        }
}

static int irgen_c_cast_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		// TODO will happen because parser is buggy ??
		// TODO this_node ??
		if (node->token.type == T_OPEN_PARENT) {
        	assert(0 && "TODO not implemented: with T_OPEN_PARENT");
        }

        return irgen_c_unary_expression(ctx, node);
}

static int irgen_c_unary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		switch (node->token.type) {
			case T_UNKNOWN: return irgen_c_postfix_expression(ctx, node);
			case T_OPEN_PARENT: {
				size_t argument_number = 0;
			
				// TODO ADD BASIC BLOCK BEGIN
				IRPrimitiveType *argument_ptypes = NULL;

				if (node->num == 2) {
					ParseTreeNode_C *argument_expression_list = node->elements[1];
					
					argument_number = argument_expression_list->num;

					argument_ptypes = malloc(sizeof(IRPrimitiveType) * argument_expression_list->num);

					for (size_t i = 0; i < argument_expression_list->num; ++i) {
						// TODO ADD BASIC BLOCK BEGIN ??
						if (irgen_c_assignment_expression(ctx, argument_expression_list->elements[i]) != 0) {
							return -1;
						}
						
						argument_ptypes[i] = /* TODO HARD */IR_PTR_T;
						
						ir_emit_param_push(ctx, i + 1, argument_ptypes[i], IR_ATYPE_REG, IR_REG1);
						
						// TODO ADD BASIC BLOCK END ??
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
				
				// TODO ADD BASIC BLOCK END
				for (size_t i = 0; i < argument_number; ++i) {
					assert(argument_ptypes != NULL);

					ir_emit_param_pop(ctx, argument_ptypes[i], i + 1);
				}
				
				if (argument_ptypes != NULL) {
					free(argument_ptypes);
				}
				
				ir_emit_call(ctx, argument_number, &identifier->token.view);
			} return 0;
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
			default: assert(0 && "NOT REACHABLE");
		}
}

static int irgen_c_type_name(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_postfix_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		if (node->token.type != 0) {
        	assert(0 && "TODO not implemented: with ANY TOKEN");
        }

        return irgen_c_primary_expression(ctx, node);
}

static int irgen_c_unary_operator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_primary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		switch (node->type) {
			case PTT_C_IDENTIFIER: {
				// TODO probably incomplete but fine for now...
				IRSymTblEnt *ent = ir_symtbl_get(ctx->symtbl, &node->token.view, IR_SYMUSE_LOCAL);
				
				assert(ent != NULL);
				
				ir_emit_load(ctx, /* TODO HARD */IR_PTR_T, &ent->addr, IR_REG1);
			} break;
			case PTT_C_CONSTANT: {
				return irgen_c_constant(ctx, node);
			} break;
			case PTT_C_STRING: {
				return irgen_c_string(ctx, node);
			} break;
			case PTT_C_EXPRESSION: {
				return irgen_c_expression(ctx, node);
			} break;
			default:
				assert(0 && "NOT REACHABLE");
		}

        return 0;
}

static int irgen_c_assignment_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
		
		IRSymTblEnt *entry_id = NULL;
		
		switch (node->type) {
			case PTT_C_CONDITIONAL_EXPRESSION: {
				return irgen_c_conditional_expression(ctx, node);
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

		if (irgen_c_assignment_expression(ctx, this_node->elements[2]) != 0) {
			return -1;
		}

		if (this_node->elements[1]->token.type != T_ASSIGNMENT) {
			/* TODO DEBUG */
			assert(
				this_node->elements[1]->token.type == T_MULTIPLY_ASSIGN ||
				this_node->elements[1]->token.type == T_DIVIDE_ASSIGN ||
				this_node->elements[1]->token.type == T_MODULUS_ASSIGN ||
				this_node->elements[1]->token.type == T_PLUS_ASSIGN ||
				this_node->elements[1]->token.type == T_MINUS_ASSIGN ||
				this_node->elements[1]->token.type == T_BITWISE_LEFTSHIFT_ASSIGN ||
				this_node->elements[1]->token.type == T_BITWISE_RIGHTSHIFT_ASSIGN ||
				this_node->elements[1]->token.type == T_BITWISE_AND_ASSIGN ||
				this_node->elements[1]->token.type == T_BITWISE_XOR_ASSIGN ||
				this_node->elements[1]->token.type == T_BITWISE_OR_ASSIGN
			);
			
			ir_emit_push(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_REG1);

			ir_emit_load(ctx, /* TODO HARD */IR_PTR_T, &entry_id->addr, IR_REG1);
			
			if (irgen_c_assignment_operator(ctx, this_node->elements[1]) != 0) {
				return -1;
			}
		}

		// TODO only works with identifiers aka a symtbl entry
		
		ir_emit_store(ctx, /*TODO HARD*/IR_PTR_T, IR_REG1, &entry_id->addr);

        return 0;
}

static int irgen_c_constant(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        switch (this_node->token.type) {
			case T_INTEGER_CONSTANT: {
				ir_emit_imm(ctx, /* TODO HARD */IR_PTR_T, this_node->token.literal, IR_REG1);

				return 0;
			} break;
			case T_FLOATING_CONSTANT: {
				ir_emit_imm(ctx, /* TODO HARD */IR_F64_T, this_node->token.literal, IR_REG1);
			} break;
			case T_CHARACTER_CONSTANT: {
				ir_emit_imm(ctx, /* TODO HARD */IR_S32_T, this_node->token.literal, IR_REG1);

				return 0;
			} break;
			case T_IDENTIFIER: {
				assert(0 && "TODO not implemented: PTT_C_ENUMERATION_CONSTANT");
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			} break;
		}

        return 0;
}

static int irgen_c_string(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		const size_t label_str = ctx->label_str;

        ir_emit_string(ctx, this_node->token.literal, 1 /* TODO HARD, 1 means NULL TERMINATED STRING YES */, ctx->label_str++); // TODO move label++ back ??

		ir_emit_load_string(ctx, label_str, IR_REG1);

        return 0;
}

static int irgen_c_assignment_operator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		// TODO NOT HERE (POP)
        ir_emit_pop(ctx, IR_PTR_T, IR_REG2);

		switch (this_node->token.type) {
			case T_MULTIPLY_ASSIGN: {
				ir_emit_mul(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_DIVIDE_ASSIGN: {
				ir_emit_div(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_MODULUS_ASSIGN: {
				ir_emit_mod(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_PLUS_ASSIGN: {
				ir_emit_add(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_MINUS_ASSIGN: {
				ir_emit_sub(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_BITWISE_LEFTSHIFT_ASSIGN: {
				ir_emit_sal(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_BITWISE_RIGHTSHIFT_ASSIGN: {
				ir_emit_sar(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_BITWISE_AND_ASSIGN: {
				ir_emit_and(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_BITWISE_XOR_ASSIGN: {
				ir_emit_xor(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
		    case T_BITWISE_OR_ASSIGN: {
				ir_emit_or(ctx, /* TODO HARD */IR_PTR_T, IR_ATYPE_REG, IR_ATYPE_REG, IR_REG1, IR_REG1, IR_REG2);
	    	} break;
			default:
				assert(0 && "NOT REACHABLE, hint: T_ASSIGNMENT is not allowed");
		}

        return 0;
}

static int irgen_c_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_parameter_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_parameter_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_direct_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_enumerator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_enumerator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_init_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	for (size_t i = 0; i < this_node->num; ++i) {
		if (irgen_c_init_declarator(ctx, this_node->elements[i]) != 0) {
			return -1;
		}
	}
	
	return 0;
}

static int irgen_c_init_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ir_emit_local(ctx, /*TODO HARD*/IR_PTR_T, &this_node->symtblent->addr);
        
        if (this_node->num == 1) {	
        	return 0;
        }
		if (irgen_c_initializer(ctx, this_node->elements[1]) != 0) {
			return -1;
		}

		ir_emit_store(ctx, /*TODO HARD*/IR_PTR_T, IR_REG1, &this_node->symtblent->addr);

        return 0;
}

static int irgen_c_initializer(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		switch (node->type) {
			case PTT_C_ASSIGNMENT_EXPRESSION:
				return irgen_c_assignment_expression(ctx, node);
			case PTT_C_INITIALIZER_LIST:
				assert(0 && "TODO not implemented ({ initializer_list })");
			default:
				assert(0 && "NOT REACHABLE");
		}
}

static int irgen_c_initializer_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

static int irgen_c_statement(IR_CTX *ctx, ParseTreeNode_C *statement)
{
	ParseTreeNode_C *node = statement->elements[0];
	
	switch (node->type) {
		case PTT_C_LABELED_STATEMENT: {
			if (irgen_c_labeled_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_EXPRESSION_STATEMENT: {
			if (irgen_c_expression_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_COMPOUND_STATEMENT: {
			// TODO change symtbl
			if (irgen_c_compound_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_SELECTION_STATEMENT: {
			if (irgen_c_selection_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_ITERATION_STATEMENT: {
			if (irgen_c_iteration_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_JUMP_STATEMENT: {
			if (irgen_c_jump_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		default: {
			assert(0 && "NOT REACHABLE");
		}
	}

	return 0;
}

static int irgen_c_labeled_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	switch (this_node->token.type) {
		case T_IDENTIFIER: {
			IRSymTblEnt *label_entry;
			ParseTreeNode_C *statement = this_node->elements[0];
			
			label_entry = ir_symtbl_get(ctx->symtbl, &this_node->token.view, IR_SYMUSE_LABEL);
			
			assert(label_entry != NULL);
			
			label_entry->addr = ctx->label_tmp++;
			
			ir_emit_label(ctx, IR_ATYPE_NUM, label_entry->addr);
			
			if (irgen_c_statement(ctx, statement) != 0) {
				return -1;
			}
		} break;
		case T_DEFAULT: {
			assert(0 && "TODO not implemented (default)");
			// ParseTreeNode_C *statement = this_node->elements[0];
		} break;
		case T_CASE: {
			assert(0 && "TODO not implemented (case)");
			// TODO requires a constant_expression evaluater ??
			// ParseTreeNode_C *constant_expression = this_node->elements[0];
			// ParseTreeNode_C *statement = this_node->elements[1];
		}
		default: {
			assert(0 && "NOT REACHABLE");
		}
	}
	
	return 0;
}

static int irgen_c_expression_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        if (this_node->num == 0) {
			return 0;
		}

        return irgen_c_expression(ctx, this_node->elements[0]);
}

static int irgen_c_selection_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		const size_t before_label_select_begin = ctx->label_select_begin;
		const size_t before_label_select_end = ctx->label_select_end;

		ctx->label_select_begin = ctx->label_tmp++;
		ctx->label_select_end = ctx->label_tmp++;

		switch(this_node->token.type) {
        	case T_IF: {
        		if (irgen_c_expression(ctx, this_node->elements[0]) == -1) {
        			return -1;
        		}
        		
        		ir_emit_jmp_zero(ctx, /* TODO HARD */IR_PTR_T, IR_REG1, ctx->label_select_begin);

        		if (irgen_c_statement(ctx, this_node->elements[1]) == -1) {
        			return -1;
        		}

        		/* jmp_if_end */
				ir_emit_jmp(ctx, IR_ATYPE_NUM, ctx->label_select_end);

        		/* label_else */
				ir_emit_label(ctx, IR_ATYPE_NUM, ctx->label_select_begin);

        		if (this_node->num == 3) {
        			if (irgen_c_statement(ctx, this_node->elements[2]) == -1) {
		    			return -1;
		    		}

        			/* jmp_if_end */
					ir_emit_jmp(ctx, IR_ATYPE_NUM, ctx->label_select_end);
    			}
        	} break;
        	case T_SWITCH: {
        		assert(0 && "TODO: not implemented: ir_selection_statement with (switch)");
        	} break;
        	default: {
				assert(0 && "NOT REACHABLE");
			}
        }

		/* label_end */
		ir_emit_label(ctx, IR_ATYPE_NUM, ctx->label_select_end);

		ctx->label_select_begin = before_label_select_begin;
		ctx->label_select_end = before_label_select_end;

        return 0;
}

static int irgen_c_iteration_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		const size_t before_label_iter_begin = ctx->label_iter_begin;
		const size_t before_label_iter_end = ctx->label_iter_end;

		ctx->label_iter_begin = ctx->label_tmp++;
		ctx->label_iter_end = ctx->label_tmp++;

        switch(this_node->token.type) {
			case T_WHILE: {
				/* label_begin */
				ir_emit_label(ctx, IR_ATYPE_NUM, ctx->label_iter_begin);

				ParseTreeNode_C *expression = this_node->elements[0];

				if (irgen_c_expression(ctx, expression) != 0) {
					return -1;
				}

				ir_emit_jmp_zero(ctx, /* TODO HARD */IR_PTR_T, IR_REG1, ctx->label_iter_end);
				
				ParseTreeNode_C *statement = this_node->elements[1];

				if (irgen_c_statement(ctx, statement) != 0) {
					return -1;
				}
				
				/* jmp_begin */
				ir_emit_jmp(ctx, IR_ATYPE_NUM, ctx->label_iter_begin);
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
					if (irgen_c_expression(ctx, expression_init) != 0) {
						return -1;
					}
				}

				const size_t for_label_begin = ctx->label_iter_begin;

				if (expression_last != NULL) {
					ctx->label_iter_begin = ctx->label_tmp++;
				}

				/* label_begin */
				ir_emit_label(ctx, IR_ATYPE_NUM, for_label_begin);
				
				if (expression_cond != NULL) {
					if (irgen_c_expression(ctx, expression_cond) != 0) {
						return -1;
					}

					ir_emit_jmp_zero(ctx, /* TODO HARD */IR_PTR_T, IR_REG1, ctx->label_iter_end);
				}
				
				if (irgen_c_statement(ctx, statement) != 0) {
					return -1;
				}
				
				if (expression_last != NULL) {
					/* label_expression_last */
					ir_emit_label(ctx, IR_ATYPE_NUM, ctx->label_iter_begin);

					if (irgen_c_expression(ctx, expression_last) != 0) {
						return -1;
					}
				}

				/* jmp_begin */
				ir_emit_jmp(ctx, IR_ATYPE_NUM, for_label_begin);
			} break;
			case T_DO: {
				/* label_begin */
				ir_emit_label(ctx, IR_ATYPE_NUM, ctx->label_iter_begin);

				ParseTreeNode_C *statement = this_node->elements[0];

				if (irgen_c_statement(ctx, statement) != 0) {
					return -1;
				}
				
				ParseTreeNode_C *expression = this_node->elements[1];

				if (irgen_c_expression(ctx, expression) != 0) {
					return -1;
				}
				
				ir_emit_jmp_not_zero(ctx, /* TODO HARD */IR_PTR_T, IR_REG1, ctx->label_iter_begin);
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			}
        }
        
        /* label_end */
		ir_emit_label(ctx, IR_ATYPE_NUM, ctx->label_iter_end);
		
		ctx->label_iter_begin = before_label_iter_begin;
		ctx->label_iter_end = before_label_iter_end;
		
		return 0;
}

static int irgen_c_jump_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		switch(this_node->token.type) {
			case T_GOTO: {
				ParseTreeNode_C *identifier;
				IRSymTblEnt *label_entry;
				
				identifier = this_node->elements[0];
				
				assert(identifier != NULL);
				
				label_entry = ir_symtbl_get(ctx->symtbl, &identifier->token.view, IR_SYMUSE_LABEL);
				
				assert(label_entry != NULL);
				
				ir_emit_jmp(ctx, IR_ATYPE_ADDR, &label_entry->addr);
			} return 0;
			case T_CONTINUE: {
				ir_emit_jmp(ctx, IR_ATYPE_NUM, ctx->label_iter_begin);
			} return 0;
			case T_BREAK: {
				ir_emit_jmp(ctx, IR_ATYPE_NUM, ctx->label_iter_end);
			} return 0;
			case T_RETURN: {
				if (this_node->num > 0) {
					if (irgen_c_expression(ctx, this_node->elements[0]) != 0) {
						return -1;
					}
				}

				ir_emit_jmp(ctx, IR_ATYPE_NUM, ctx->label_func_end);
			} return 0;
			default:
				assert(0 && "NOT REACHABLE");
		}
}
