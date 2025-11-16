#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ir.h"
#include "jd297/lmap_sv.h"

int ir_run(IR_CTX *ctx, ParseTreeNode_C *translation_unit)
{
	for (size_t i = 0; i < translation_unit->num; ++i) {
		if (ir_external_declaration(ctx, translation_unit->elements[i]) != 0) {
			return -1;
		}
	}

	return 0;
}

int ir_external_declaration(IR_CTX *ctx, ParseTreeNode_C *external_declaration)
{
	for (size_t j = 0; j < external_declaration->num; ++j) {
		ParseTreeNode_C *node = external_declaration->elements[j];

		switch (node->type) {
			case PTT_C_FUNCTION_DEFINITION: {
				if (ir_function_definition(ctx, node) != 0) {
					return -1;
				}
			} break;
			case PTT_C_DECLARATION: {
				if (ir_declaration(ctx, node) != 0) {
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

int ir_function_definition(IR_CTX *ctx, ParseTreeNode_C *function_definition)
{
	ctx->symtbl = function_definition->symtbl;

	assert(ctx->symtbl != NULL);

	IRCode *func_begin_code = malloc(sizeof(IRCode));

	assert(func_begin_code != NULL);

	*func_begin_code = (IRCode) {
		.op = IR_OC_FUNC_BEGIN,
		.result.ptr = symtbl_get(ctx->symtbl, ctx->symtbl->id)
	};

	list_insert(ctx->code, list_end(ctx->code), func_begin_code);

	for (size_t i = 0; i < function_definition->num; ++i) {
		ParseTreeNode_C *node = function_definition->elements[i];

		switch (node->type) {
			case PTT_C_DECLARATION_SPECIFIER: break;

			case PTT_C_DECLARATOR: break; // TODO IGNORE ??

			// TODO IGNORE
			case PTT_C_DECLARATION: break; // TODO save param count

			case PTT_C_COMPOUND_STATEMENT: {
				if (ir_compound_statement(ctx, node) != 0) {
					return -1;
				}
			} break;

			default: assert(0 && "NOT REACHABLE");
		}
	}

	IRCode *func_end_code = malloc(sizeof(IRCode));

	assert(func_end_code != NULL);

	*func_end_code = (IRCode){
		.op = IR_OC_FUNC_END,
		.result.ptr = symtbl_get(ctx->symtbl, ctx->symtbl->id)
	};

	list_insert(ctx->code, list_end(ctx->code), func_end_code);

	ctx->symtbl = ctx->symtbl->parent;

	return 0;
}

int ir_declaration(IR_CTX *ctx, ParseTreeNode_C *declaration)
{
	(void) ctx;
	(void) declaration;

	assert(0 && "TODO ir_declaration not implemented");
}

int ir_declaration_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_compound_statement(IR_CTX *ctx, ParseTreeNode_C *compound_statement)
{
	for (size_t i = 0; i < compound_statement->num; ++i) {
		ParseTreeNode_C *node = compound_statement->elements[i];
		
		switch (node->type) {
			case PTT_C_DECLARATION: {
				if (ir_declaration(ctx, node) != 0) {
					return -1;
				}
			} break;
			case PTT_C_STATEMENT: {
				if (ir_statement(ctx, node) != 0) {
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

int ir_storage_class_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_type_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_type_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_struct_or_union_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_enum_specifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_typedef_name(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_struct_or_union(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_identifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_struct_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_specifier_qualifier(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_struct_declarator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_struct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_constant_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_pointer(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_direct_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_parameter_type_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_conditional_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_TERNARY) {
        	assert(0 && "TODO not implemented: with T_TERNARY");
        }
        
        return ir_logical_or_expression(ctx, node);
}

int ir_logical_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_LOGICAL_OR) {
        	assert(0 && "TODO not implemented: with T_LOGICAL_OR");
        }
        
        return ir_logical_and_expression(ctx, node);
}

int ir_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		for (size_t i = 0; i < this_node->num; ++i) {
			// TODO NOTE: the last assignment_expression
			// is the result of an expression
			if (ir_assignment_expression(ctx, this_node->elements[i]) == -1) {
				return -1;
			}
		}

        return 0;
}

int ir_logical_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_LOGICAL_AND) {
        	assert(0 && "TODO not implemented: with T_LOGICAL_AND");
        }
        
        return ir_inclusive_or_expression(ctx, node);
}

int ir_inclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;

        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_BITWISE_OR) {
        	assert(0 && "TODO not implemented: with T_BITWISE_OR");
        }
        
        return ir_exclusive_or_expression(ctx, node);
}

int ir_exclusive_or_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_BITWISE_XOR) {
        	assert(0 && "TODO not implemented: with T_BITWISE_XOR");
        }
        
        return ir_and_expression(ctx, node);
}

int ir_and_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_BITWISE_AND) {
        	assert(0 && "TODO not implemented: with T_BITWISE_AND");
        }
        
        return ir_equality_expression(ctx, node);
}

int ir_equality_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_EQUAL_TO) {
        	assert(0 && "TODO not implemented: with T_EQUAL_TO");
        }
        
        if (node->token.type == T_NOT_EQUAL_TO) {
        	assert(0 && "TODO not implemented: with T_NOT_EQUAL_TO");
        }
        
        return ir_relational_expression(ctx, node);
}

int ir_relational_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (node->token.type == T_GREATER_THAN) {
        	assert(0 && "TODO not implemented: with T_GREATER_THAN");
        }
        
        if (node->token.type == T_LESS_THAN) {
        	assert(0 && "TODO not implemented: with T_LESS_THAN");
        }
        
        if (node->token.type == T_GREATER_THAN_OR_EQUAL_TO) {
        	assert(0 && "TODO not implemented: with T_GREATER_THAN_OR_EQUAL_TO");
        }
        
        if (node->token.type == T_LESS_THAN_OR_EQUAL_TO) {
        	assert(0 && "TODO not implemented: with T_LESS_THAN_OR_EQUAL_TO");
        }
        
        return ir_shift_expression(ctx, node);
}

int ir_shift_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
        
        if (this_node->token.type == T_BITWISE_LEFTSHIFT) {
        	assert(0 && "TODO not implemented: with T_BITWISE_LEFTSHIFT");
        }
        
        if (this_node->token.type == T_BITWISE_RIGHTSHIFT) {
        	assert(0 && "TODO not implemented: with T_BITWISE_RIGHTSHIFT");
        }
        
        return ir_additive_expression(ctx, node);
}

int ir_additive_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return ir_multiplicative_expression(ctx, node);
		}

        ParseTreeNode_C *right = this_node->elements[1];
        
        if (ir_multiplicative_expression(ctx, right) != 0) {
        	return -1;
        }
		
		IRCode *push = malloc(sizeof(IRCode));
		*push = (IRCode) {
			.op = IR_OC_PUSH,
			// .result.ptr = &r1 // TODO set a register probably R1 (return register)
		};
		list_insert(ctx->code, list_end(ctx->code), push);

		ParseTreeNode_C *left = this_node->elements[0];
        
        if (ir_multiplicative_expression(ctx, left) != 0) {
        	return -1;
        }
		
        IRCode *pop = malloc(sizeof(IRCode));
		*pop = (IRCode) {
			.op = IR_OC_POP,
			// .result.ptr = &r2 // TODO set a register probably R2 (default accumulate register)
		};
		list_insert(ctx->code, list_end(ctx->code), pop);

        switch (this_node->token.type) {
        	case T_MINUS: {
        		IRCode *sub = malloc(sizeof(IRCode));
				*sub = (IRCode) {
					.op = IR_OC_SUB,
					// .result.ptr = &r1 // TODO set a register probably R1 (return register)
					// .arg1.ptr = &r1 // TODO set a register probably R1 (return register)
					// .arg2.ptr = &r2 // TODO set a register probably R2 (default accumulate register)
				};
				list_insert(ctx->code, list_end(ctx->code), sub);

        		return 0;
    		}
        	case T_PLUS: {
	        	IRCode *add = malloc(sizeof(IRCode));
				*add = (IRCode) {
					.op = IR_OC_ADD,
					// .result.ptr = &r1 // TODO set a register probably R1 (return register)
					// .arg1.ptr = &r1 // TODO set a register probably R1 (return register)
					// .arg2.ptr = &r2 // TODO set a register probably R2 (default accumulate register)
				};
				list_insert(ctx->code, list_end(ctx->code), add);
				
        		return 0;
        	}
        	default:
        		assert(0 && "not reachable");
        }
}

int ir_multiplicative_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{       
        if (this_node->num == 1) {
			ParseTreeNode_C *node = this_node->elements[0];

			return ir_cast_expression(ctx, node);
		}
        
        switch (this_node->token.type) {
        	case T_MULTIPLY: {
        		assert(0 && "TODO not implemented: with T_MULTIPLY");
        	
        		return 0;
        	}
        	case T_DIVIDE: {
        		assert(0 && "TODO not implemented: with T_DIVIDE");
        	
        		return 0;
        	}
        	case T_MODULUS: {
        		assert(0 && "TODO not implemented: with T_MODULUS");
        	
        		return 0;
        	}
        	default:
        		assert(0 && "not reachable");
        }
}

int ir_cast_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		// TODO will happen because parser is buggy ??
		// TODO this_node ??
		if (node->token.type == T_OPEN_PARENT) {
        	assert(0 && "TODO not implemented: with T_OPEN_PARENT");
        }

        return ir_unary_expression(ctx, node);
}

int ir_unary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		if (node->token.type != 0) { // TODO this_node ??
        	assert(0 && "TODO not implemented: with ANY TOKEN");
        }

        return ir_postfix_expression(ctx, node);
}

int ir_type_name(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_postfix_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		if (node->token.type != 0) {
        	assert(0 && "TODO not implemented: with ANY TOKEN");
        }

        return ir_primary_expression(ctx, node);
}

int ir_unary_operator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_primary_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];

		switch (node->type) {
			case PTT_C_IDENTIFIER: {
				assert(0 && "TODO not implemented: PTT_C_IDENTIFIER");
			} break;
			case PTT_C_CONSTANT: {
				return ir_constant(ctx, node);
			} break;
			case PTT_C_STRING: {
				assert(0 && "TODO not implemented: PTT_C_STRING");
			} break;
			case PTT_C_EXPRESSION: {
				assert(0 && "TODO not implemented: PTT_C_EXPRESSION");
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			} break;
		}

        return 0;
}

int ir_assignment_expression(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        ParseTreeNode_C *node = this_node->elements[0];
		
		switch (node->type) {
			case PTT_C_CONDITIONAL_EXPRESSION: {
				return ir_conditional_expression(ctx, node);
			} break;
			case PTT_C_UNARY_EXPRESSION: {
				assert(0 && "TODO not implemented: PTT_C_UNARY_EXPRESSION");
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			} break;
		}

        return 0;
}

int ir_constant(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        switch (this_node->token.type) {
			case T_NUMBER: {
				
				IRCode *imm = malloc(sizeof(IRCode));
				*imm = (IRCode) {
					.op = IR_OC_IMM,
					// .result.ptr = &r1, // TODO set a register probably R1 (return register)
					.arg1.sv = &this_node->token.view
				};
				list_insert(ctx->code, list_end(ctx->code), imm);

				return 0;
			} break;
			case T_CHARACTER: {
				assert(0 && "TODO not implemented: T_CHARACTER");
			} break;
			/*case PTT_C_FLOATING_CONSTANT: { // TODO also T_NUMBER ??
				assert(0 && "TODO not implemented: PTT_C_FLOATING_CONSTANT");
			} break;*/
			case T_IDENTIFIER: {
				assert(0 && "TODO not implemented: PTT_C_ENUMERATION_CONSTANT");
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			} break;
		}

        return 0;
}

int ir_string(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_assignment_operator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_parameter_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_parameter_declaration(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_direct_abstract_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_enumerator_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_enumerator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_init_declarator(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_initializer(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_initializer_list(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_statement(IR_CTX *ctx, ParseTreeNode_C *statement)
{
	ParseTreeNode_C *node = statement->elements[0];
	
	switch (node->type) {
		case PTT_C_LABELED_STATEMENT: {
			if (ir_labeled_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_EXPRESSION_STATEMENT: {
			if (ir_jump_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_COMPOUND_STATEMENT: {
			// TODO change symtbl
			if (ir_compound_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_SELECTION_STATEMENT: {
			if (ir_selection_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_ITERATION_STATEMENT: {
			if (ir_iteration_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		case PTT_C_JUMP_STATEMENT: {
			if (ir_jump_statement(ctx, node) != 0) {
				return -1;
			}
		} break;
		default: {
			assert(0 && "NOT REACHABLE");
		}
	}

	return 0;
}

int ir_labeled_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
	(void) ctx;
	(void) this_node;

	assert(0 && "TODO ir_labeled_statement not implemented");
}

int ir_expression_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_selection_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_iteration_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
        (void) ctx;
        (void) this_node;

        assert(0 && "TODO not implemented");

        return 0;
}

int ir_jump_statement(IR_CTX *ctx, ParseTreeNode_C *this_node)
{
		switch(this_node->token.type) {
			case T_GOTO: {
				assert(0 && "TODO not implemented (goto)");
			} break;
			case T_CONTINUE: {
				assert(0 && "TODO not implemented (continue)");
			} break;
			case T_BREAK: {
				assert(0 && "TODO not implemented (break)");
			} break;
			case T_RETURN: {
				if (this_node->num > 0) {
					if (ir_expression(ctx, this_node->elements[0]) != 0) {
						return -1;
					}
				}
			
				IRCode *code = malloc(sizeof(IRCode));
				
				assert(code != NULL);
				
				*code = (IRCode){
					.op = IR_OC_JMP_FUNC_END,
					.result.ptr = symtbl_function(ctx->symtbl)
				};

				list_insert(ctx->code, list_end(ctx->code), code);

				return 0;
			} break;
			default: {
				assert(0 && "NOT REACHABLE");
			}
		}

        return 0;
}

