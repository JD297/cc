#include "lexer_c.h"
#include "parser_c.h"
#include "parse_tree_node_c.h"
#include "parse_tree_type_c.h"
#include "jd297/logger.h"

#include <stddef.h>
#include <stdlib.h>

ParseTreeNode_C *parser_c_parse(Parser_C_CTX *ctx)
{
    return parser_c_parse_translation_unit(ctx);
}

ParseTreeNode_C *parser_c_parse_translation_unit(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TRANSLATION_UNIT, NULL);

    ParseTreeNode_C *external_declaration;

    parser_c_parse_list_opt(ctx, this_node, external_declaration);

    return this_node;
}

ParseTreeNode_C *parser_c_parse_external_declaration(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXTERNAL_DECLARATION, NULL);

    ParseTreeNode_C *function_definition;
    ParseTreeNode_C *declaration;

    parser_c_parse_opt(ctx, this_node, function_definition, ret);

    parser_c_parse_opt(ctx, this_node, declaration, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_function_definition(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_FUNCTION_DEFINITION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *declaration;
    ParseTreeNode_C *compound_statement;

    Lexer_C lexer_saved = *ctx->lexer;

	// TODO not push to this_node instead
	// workaround with a stack_node to reduce mem
	// of this_node
	// also it is easier to later traverse the tree ??
    parser_c_parse_list_opt(ctx, this_node, declaration_specifier);
    
    parser_c_parse_required(ctx, this_node, declarator, error);

    parser_c_parse_list_opt(ctx, this_node, declaration);
    
    // TODO create new symtbl and add it with the identifier
    // TODO add .. to symtbl
    // TODO maybe function for this ??
    lmap_t *newsymtbl;
    
    parser_c_parse_required(ctx, this_node, compound_statement, error);

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *init_declarator;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_list_required(ctx, this_node, declaration_specifier, error);

    parser_c_parse_list_opt(ctx, this_node, init_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_SEMICOLON) == 0) {
        goto error;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration_specifier(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION_SPECIFIER, NULL);

    ParseTreeNode_C *storage_class_specifier;
    ParseTreeNode_C *type_specifier;
    ParseTreeNode_C *type_qualifier;

    next: {
        parser_c_parse_opt(ctx, this_node, storage_class_specifier, next);

        parser_c_parse_opt(ctx, this_node, type_specifier, next);

        parser_c_parse_opt(ctx, this_node, type_qualifier, next);
    }

    if (this_node->num == 0) {
        goto error;
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declarator(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATOR, NULL);

    ParseTreeNode_C *pointer;
    ParseTreeNode_C *direct_declarator;

    parser_c_parse_opt(ctx, this_node, pointer, next_direct_declarator);
    
    next_direct_declarator: {
        parser_c_parse_required(ctx, this_node, direct_declarator, error);
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_compound_statement(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_COMPOUND_STATEMENT, NULL);

    ParseTreeNode_C *declaration;
    ParseTreeNode_C *statement;

    Lexer_C lexer_saved = *ctx->lexer;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_BRACE) == 0) {
        goto error;
    }

	// TODO this_node could be stack_node
    parser_c_parse_list_opt(ctx, this_node, declaration);

    // TODO add symtblent's from declaration_list
    
    parser_c_parse_list_opt(ctx, this_node, statement);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_storage_class_specifier(Parser_C_CTX *ctx)
{
    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_storage_class_specifier;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_storage_class_specifier) == -1) {
        goto error;
    }

    switch (token_storage_class_specifier.type) {
        case T_AUTO:
        case T_REGISTER:
        case T_STATIC:
        case T_EXTERN:
        case T_TYPEDEF: {
            return parse_tree_node_c_create(PTT_C_STORAGE_CLASS_SPECIFIER, &token_storage_class_specifier);
        }
        default: break;
    }

    error: {
        *ctx->lexer = lexer_saved;

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_type_specifier(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_SPECIFIER, NULL);

    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_type_specifier;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_type_specifier) == -1) {
        goto error;
    }

    switch (token_type_specifier.type) {
        case T_VOID:
        case T_CHAR:
        case T_SHORT:
        case T_INT:
        case T_LONG:
        case T_FLOAT:
        case T_DOUBLE:
        case T_SIGNED:
        case T_UNSIGNED: {
            this_node->token = token_type_specifier;
    
            return this_node;
        }
        default: break;
    }

    *ctx->lexer = lexer_saved;

    ParseTreeNode_C *struct_or_union_specifier;
    ParseTreeNode_C *enum_specifier;
    ParseTreeNode_C *typedef_name;

    parser_c_parse_opt(ctx, this_node, struct_or_union_specifier, ret);

    parser_c_parse_opt(ctx, this_node, enum_specifier, ret);

    parser_c_parse_opt(ctx, this_node, typedef_name, ret);

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_type_qualifier(Parser_C_CTX *ctx)
{
    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_type_qualifier;
    
    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_type_qualifier) == -1) {
        return NULL;
    }

    switch (token_type_qualifier.type) {
        case T_CONST:
        case T_VOLATILE: {
            return parse_tree_node_c_create(PTT_C_TYPE_QUALIFIER, &token_type_qualifier);
        } break;
        default: {
            *ctx->lexer = lexer_saved;

            return NULL;
        }       
    }
}

ParseTreeNode_C *parser_c_parse_struct_or_union_specifier(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION_SPECIFIER, NULL);

    ParseTreeNode_C *struct_or_union;
    ParseTreeNode_C *identifier;
    ParseTreeNode_C *struct_declaration;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_required(ctx, this_node, struct_or_union, error);

    parser_c_parse_opt(ctx, this_node, identifier, has_identifier);
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_BRACE) == 0) {
        goto error;
    }

    parser_c_parse_list_required(ctx, this_node, struct_declaration, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    has_identifier: {
        Lexer_C lexer_saved_brace = *ctx->lexer;

        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_BRACE) == 0) {
            *ctx->lexer = lexer_saved_brace;

            goto ret;
        }
        
        parser_c_parse_list_required(ctx, this_node, struct_declaration, error);
    
        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACE) == 0) {
            goto error;
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enum_specifier(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *identifier;
    ParseTreeNode_C *enumerator;

    Lexer_C lexer_saved = *ctx->lexer;
    
    Token_C token_enum;
    
    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_enum) == -1) {
        return NULL;
    }
    
    if (token_enum.type == T_STRUCT) {
        *ctx->lexer = lexer_saved;

        return NULL;
    }

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUM_SPECIFIER, &token_enum);

    parser_c_parse_opt(ctx, this_node, identifier, has_identifier);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_BRACE) == 0) {
        goto error;
    }
    
    parser_c_parse_list_required(ctx, this_node, enumerator, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACE) == 0) {
        goto error;
    }
    
    goto ret;

    has_identifier: {
        Lexer_C lexer_saved_brace = *ctx->lexer;

        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_BRACE) == 0) {
            *ctx->lexer = lexer_saved_brace;

            goto ret;
        }
        
        parser_c_parse_list_required(ctx, this_node, enumerator, error);
    
        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACE) == 0) {
            goto error;
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_typedef_name(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPEDEF_NAME, NULL);

    ParseTreeNode_C *identifier;

    goto error; // TODO check if identifier is registered as a typedef use symtbl

    parser_c_parse_required(ctx, this_node, identifier, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_or_union(Parser_C_CTX *ctx)
{
    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_struct_or_union;
    
    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_struct_or_union) == -1) {
        return NULL;
    }

    switch (token_struct_or_union.type) {
        case T_STRUCT:
        case T_UNION: {
            return parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION, &token_struct_or_union);
        } break;
        default: {
            *ctx->lexer = lexer_saved;

            return NULL;   
        }
    }
}

ParseTreeNode_C *parser_c_parse_identifier(Parser_C_CTX *ctx)
{
    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_identifier;
    
    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_identifier) == -1) {
        return NULL;
    }

    if (token_identifier.type == T_IDENTIFIER) {
        return parse_tree_node_c_create(PTT_C_IDENTIFIER, &token_identifier);
    }

    *ctx->lexer = lexer_saved;

    return NULL;
}

ParseTreeNode_C *parser_c_parse_struct_declaration(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATION, NULL);

    ParseTreeNode_C *specifier_qualifier;
    ParseTreeNode_C *struct_declarator;

    parser_c_parse_list_required(ctx, this_node, specifier_qualifier, error);

    parser_c_parse_list_required(ctx, this_node, struct_declarator, error);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_SEMICOLON) == 0) {
        goto error;
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_specifier_qualifier(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SPECIFIER_QUALIFIER, NULL);

    ParseTreeNode_C *type_specifier;
    ParseTreeNode_C *type_qualifier;

    parser_c_parse_opt(ctx, this_node, type_specifier, ret);

    parser_c_parse_opt(ctx, this_node, type_qualifier, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator_list(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR_LIST, NULL);

    ParseTreeNode_C *struct_declarator;
    
    Lexer_C lexer_saved = *ctx->lexer;

    next_struct_declarator: {
        parser_c_parse_required(ctx, this_node, struct_declarator, check_error);
        
        Lexer_C lexer_saved_comma = *ctx->lexer;
        
        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COMMA) == 1) {
            goto next_struct_declarator;
        }
        
        *ctx->lexer = lexer_saved_comma;
    }
    
    check_error: {
        if (this_node->num == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR, NULL);

    ParseTreeNode_C *declarator;
    ParseTreeNode_C *constant_expression;
    
    Lexer_C lexer_saved = *ctx->lexer;
    
    parser_c_parse_opt(ctx, this_node, declarator, has_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COLON) == 0) {
        goto error;
    }
    
    parser_c_parse_required(ctx, this_node, constant_expression, error);
    
    goto ret;

    has_declarator: {
        Lexer_C lexer_saved_colon = *ctx->lexer;

        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COLON) == 0) {
            *ctx->lexer = lexer_saved_colon;

            goto ret;
        }
        
        parser_c_parse_required(ctx, this_node, constant_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_constant_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONSTANT_EXPRESSION, NULL);

    ParseTreeNode_C *conditional_expression;

    parser_c_parse_required(ctx, this_node, conditional_expression, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_pointer(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_POINTER, NULL);

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *type_qualifier;
    ParseTreeNode_C *pointer;
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_MULTIPLY) == 0) {
        goto error;
    }
    
    parser_c_parse_list_opt(ctx, this_node, type_qualifier);
    
    parser_c_parse_opt(ctx, this_node, pointer, ret);

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_direct_declarator(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DIRECT_DECLARATOR, NULL);

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *constant_expression;
    ParseTreeNode_C *parameter_type_list;
    
    parser_c_parse_opt(ctx, this_node, identifier, after_direct_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(ctx, this_node, declarator, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
        goto error;
    }

    after_direct_declarator: {
        Lexer_C lexer_saved_after_direct_declarator = *ctx->lexer;

        Token_C token_after_direct_declarator;
        
        if (lexer_c_next_skip_whitespace(ctx->lexer, &token_after_direct_declarator) == -1) {
            goto error;
        }

        switch(token_after_direct_declarator.type) {
            case T_OPEN_PARENT: {
                parser_c_parse_required(ctx, this_node, parameter_type_list, next_after_direct_declarator_check_identifier_list);
                
                goto next_after_direct_declarator_parent;
                
                next_after_direct_declarator_check_identifier_list: {
                    parser_c_parse_list_opt(ctx, this_node, identifier);

                    next_after_direct_declarator_parent: {
                        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
                            goto error;
                        }
                    }
                }
                
                break;
            }
            case T_OPEN_BRACKET: {
                parser_c_parse_opt(ctx, this_node, constant_expression, next_after_direct_declarator_bracket);
                
                next_after_direct_declarator_bracket:

                if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACKET) == 0) {
                    goto error;
                }
                
                break;
            }
            default: {
                if (this_node->num == 1) {
                    *ctx->lexer = lexer_saved_after_direct_declarator;

                    break;
                }

                goto error;
            }
        }
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_type_list(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_TYPE_LIST, NULL);

    ParseTreeNode_C *parameter_list;
    
    Lexer_C lexer_saved = *ctx->lexer;
    
    parser_c_parse_required(ctx, this_node, parameter_list, error);

    Lexer_C lexer_saved_comma = *ctx->lexer;
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COMMA) == 1) {
        Token_C token_dot_dot_dot;
    
        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, &token_dot_dot_dot, T_DOT_DOT_DOT) == 0) {
            goto error;
        }
        
        this_node->token = token_dot_dot_dot;
    } else {
        *ctx->lexer = lexer_saved_comma;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_conditional_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONDITIONAL_EXPRESSION, NULL);

    ParseTreeNode_C *logical_or_expression;
    ParseTreeNode_C *expression;
    ParseTreeNode_C *conditional_expression;
    
    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_required(ctx, this_node, logical_or_expression, error);

    Lexer_C lexer_saved_ternary = *ctx->lexer;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_TERNARY) == 0) {
        *ctx->lexer = lexer_saved_ternary;

        goto ret;
    }

    parser_c_parse_required(ctx, this_node, expression, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_TERNARY) == 0) {
        goto error;
    }
    
    parser_c_parse_required(ctx, this_node, conditional_expression, error);

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_logical_or_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_LOGICAL_OR_EXPRESSION, NULL);

    ParseTreeNode_C *logical_and_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;

    parser_c_parse_required(ctx, this_node, logical_and_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;
    
    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_LOGICAL_OR: {
            parser_c_parse_required(ctx, this_node, logical_and_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }

        switch (this_node_token.type) {
            case T_LOGICAL_OR: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_LOGICAL_OR_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, logical_and_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXPRESSION, NULL);

    ParseTreeNode_C *assignment_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    next_assignment_expression: {
        parser_c_parse_required(ctx, this_node, assignment_expression, error);

        Lexer_C lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COMMA) == 1) {
            goto next_assignment_expression;
        }
        
        *ctx->lexer = lexer_saved_token;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_logical_and_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_LOGICAL_AND_EXPRESSION, NULL);

    ParseTreeNode_C *inclusive_or_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;

    parser_c_parse_required(ctx, this_node, inclusive_or_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_LOGICAL_AND: {
            parser_c_parse_required(ctx, this_node, inclusive_or_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }

        switch (this_node_token.type) {
            case T_LOGICAL_AND: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_LOGICAL_AND_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, inclusive_or_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_inclusive_or_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INCLUSIVE_OR_EXPRESSION, NULL);

    ParseTreeNode_C *exclusive_or_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;
    
    parser_c_parse_required(ctx, this_node, exclusive_or_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_BITWISE_OR: {
            parser_c_parse_required(ctx, this_node, exclusive_or_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }

        switch (this_node_token.type) {
            case T_BITWISE_OR: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_INCLUSIVE_OR_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, exclusive_or_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_exclusive_or_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXCLUSIVE_OR_EXPRESSION, NULL);

    ParseTreeNode_C *and_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;

    parser_c_parse_required(ctx, this_node, and_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_BITWISE_XOR: {
            parser_c_parse_required(ctx, this_node, and_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }

        switch (this_node_token.type) {
            case T_BITWISE_XOR: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_EXCLUSIVE_OR_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, and_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_and_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_AND_EXPRESSION, NULL);

    ParseTreeNode_C *equality_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;

    parser_c_parse_required(ctx, this_node, equality_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case PTT_C_AND_EXPRESSION: {
            parser_c_parse_required(ctx, this_node, equality_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }

        switch (this_node_token.type) {
            case PTT_C_AND_EXPRESSION: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_AND_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, equality_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_equality_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EQUALITY_EXPRESSION, NULL);

    ParseTreeNode_C *relational_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;
    
    parser_c_parse_required(ctx, this_node, relational_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_EQUAL_TO:
        case T_NOT_EQUAL_TO: {
            parser_c_parse_required(ctx, this_node, relational_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }

        switch (this_node_token.type) {
            case T_EQUAL_TO:
            case T_NOT_EQUAL_TO: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_EQUALITY_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, relational_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;
        
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_relational_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_RELATIONAL_EXPRESSION, NULL);

    ParseTreeNode_C *shift_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;

    parser_c_parse_required(ctx, this_node, shift_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_LESS_THAN:
        case T_GREATER_THAN:
        case T_LESS_THAN_OR_EQUAL_TO:
        case T_GREATER_THAN_OR_EQUAL_TO: {
            parser_c_parse_required(ctx, this_node, shift_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }

        switch (this_node_token.type) {
            case T_LESS_THAN:
            case T_GREATER_THAN:
            case T_LESS_THAN_OR_EQUAL_TO:
            case T_GREATER_THAN_OR_EQUAL_TO: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_RELATIONAL_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, shift_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;
        
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_shift_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SHIFT_EXPRESSION, NULL);

    ParseTreeNode_C *additive_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;
    
    parser_c_parse_required(ctx, this_node, additive_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_BITWISE_LEFTSHIFT:
        case T_BITWISE_RIGHTSHIFT: {
            parser_c_parse_required(ctx, this_node, additive_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;    
        }

        switch (this_node_token.type) {
            case T_BITWISE_LEFTSHIFT:
            case T_BITWISE_RIGHTSHIFT: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_SHIFT_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, additive_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;
        
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_additive_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ADDITIVE_EXPRESSION, NULL);

    ParseTreeNode_C *multiplicative_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;
    
    parser_c_parse_required(ctx, this_node, multiplicative_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;
    
    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }

    switch (this_node_token.type) {
        case T_PLUS:
        case T_MINUS: {
            parser_c_parse_required(ctx, this_node, multiplicative_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            *ctx->lexer = lexer_saved_token;

            goto error;
        }

        switch (this_node_token.type) {
            case T_PLUS:
            case T_MINUS: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_ADDITIVE_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, multiplicative_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;
        
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_multiplicative_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_MULTIPLICATIVE_EXPRESSION, NULL);

    ParseTreeNode_C *cast_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;
    
    parser_c_parse_required(ctx, this_node, cast_expression, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    Token_C this_node_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
        goto error;
    }
    
    switch (this_node_token.type) {
        case T_MULTIPLY:
        case T_DIVIDE:
        case T_MODULUS: {
            parser_c_parse_required(ctx, this_node, cast_expression, error);

            this_node->token = this_node_token;
        } break;
        default: {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }
    }

    while (1) {
        lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            goto error;
        }
    
        switch (this_node_token.type) {
            case T_MULTIPLY:
            case T_DIVIDE:
            case T_MODULUS: {
                this_node->token = this_node_token;

                left_node = this_node;

                this_node = parse_tree_node_c_create(PTT_C_MULTIPLICATIVE_EXPRESSION, NULL);

                parse_tree_node_c_add(this_node, left_node);

                parser_c_parse_required(ctx, this_node, cast_expression, error);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;

                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;
        
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_cast_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CAST_EXPRESSION, NULL);

    ParseTreeNode_C *unary_expression;
    ParseTreeNode_C *type_name;
    ParseTreeNode_C *cast_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_opt(ctx, this_node, unary_expression, ret);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_PARENT) == 0) {
        goto error;
    }

    parser_c_parse_required(ctx, this_node, type_name, error);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(ctx, this_node, cast_expression, error);

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_unary_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_UNARY_EXPRESSION, NULL);

    ParseTreeNode_C *postfix_expression;
    ParseTreeNode_C *unary_expression;
    ParseTreeNode_C *unary_operator;
    ParseTreeNode_C *cast_expression;
    ParseTreeNode_C *type_name;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_opt(ctx, this_node, postfix_expression, ret);

    parser_c_parse_required(ctx, this_node, unary_operator, next_tokens);
    
    parser_c_parse_required(ctx, this_node, cast_expression, error);
    
    next_tokens: ;
    
    Token_C token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &token) == -1) {
        goto error;
    }

    switch(token.type) {
        case T_INCREMENT:
        case T_DECREMENT: {
            parser_c_parse_required(ctx, this_node, unary_expression, error);
        } break;
        case T_SIZEOF: {
            parser_c_parse_opt(ctx, this_node, unary_expression, out);
            
            parser_c_parse_required(ctx, this_node, type_name, error);
        } break;
        default: goto error;
    }

    out:
    this_node->token = token;

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_type_name(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_NAME, NULL);

    ParseTreeNode_C *specifier_qualifier;
    ParseTreeNode_C *abstract_declarator;

    next_specifier_qualifier: {
        parser_c_parse_opt(ctx, this_node, specifier_qualifier, next_specifier_qualifier);
    }

    if (this_node->num == 0) {
        goto error;
    }

    parser_c_parse_opt(ctx, this_node, abstract_declarator, ret);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_postfix_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, NULL);

    ParseTreeNode_C *primary_expression;
    ParseTreeNode_C *expression;
    ParseTreeNode_C *assignment_expression;
    ParseTreeNode_C *identifier;

    Lexer_C lexer_saved = *ctx->lexer;

    ParseTreeNode_C *left_node;
    
    Token_C this_node_token;

    parser_c_parse_required(ctx, this_node, primary_expression, error);

    while (1) {
        Lexer_C lexer_saved_token = *ctx->lexer;

        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            *ctx->lexer = lexer_saved_token;

            goto ret;
        }

        switch(this_node_token.type) {
            case T_OPEN_BRACKET: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, &this_node_token);

                parse_tree_node_c_add(this_node, left_node);
                
                parser_c_parse_required(ctx, this_node, expression, error);
                
                if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACKET) == 0) {
                    goto error;
                }
            } break;
            case T_OPEN_PARENT: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, &this_node_token);

                parse_tree_node_c_add(this_node, left_node);
                
                parser_c_parse_list_opt(ctx, this_node, assignment_expression);
                
                if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
                    goto error;
                }
            } break;
            case T_DOT:
            case T_ARROW: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, &this_node_token);

                parse_tree_node_c_add(this_node, left_node);
                
                parser_c_parse_required(ctx, this_node, identifier, error);
            } break;
            case T_INCREMENT:
            case T_DECREMENT: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, &this_node_token);

                parse_tree_node_c_add(this_node, left_node);
            } break;
            default: {
                *ctx->lexer = lexer_saved_token;
                
                goto ret;
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved; 

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_unary_operator(Parser_C_CTX *ctx)
{
    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_unary_operator;
    
    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_unary_operator) == -1) {
        return NULL;
    }

    switch (token_unary_operator.type) {
        case T_BITWISE_AND:
        case T_MULTIPLY:
        case T_PLUS:
        case T_MINUS:
        case T_TILDE:
        case T_BITWISE_OR: {
            return parse_tree_node_c_create(PTT_C_UNARY_OPERATOR, &token_unary_operator);
        }
        default: break;
    }

    *ctx->lexer = lexer_saved;

    return NULL;
}

ParseTreeNode_C *parser_c_parse_primary_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PRIMARY_EXPRESSION, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *constant;
    ParseTreeNode_C *string;
    ParseTreeNode_C *expression;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_opt(ctx, this_node, identifier, ret);

    parser_c_parse_opt(ctx, this_node, constant, ret);

    parser_c_parse_opt(ctx, this_node, string, ret);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_PARENT) == 0) {
        goto error;
    }

    parser_c_parse_required(ctx, this_node, expression, error);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
        goto error;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_assignment_expression(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ASSIGNMENT_EXPRESSION, NULL);

    ParseTreeNode_C *conditional_expression;
    ParseTreeNode_C *unary_expression;
    ParseTreeNode_C *assignment_operator;
    ParseTreeNode_C *assignment_expression;

    parser_c_parse_opt(ctx, this_node, conditional_expression, ret);

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_required(ctx, this_node, unary_expression, error);

    parser_c_parse_required(ctx, this_node, assignment_operator, error);

    parser_c_parse_required(ctx, this_node, assignment_expression, error);

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_constant(Parser_C_CTX *ctx)
{
    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &token) == -1) {
        return NULL;
    }
    
    switch(token.type) {
        case T_CHARACTER:
        case T_NUMBER:
        case T_STRING: {
            return parse_tree_node_c_create(PTT_C_CONSTANT, &token);
        } break;
        default: {
            *ctx->lexer = lexer_saved;

            return NULL;
        }
    }
}

ParseTreeNode_C *parser_c_parse_string(Parser_C_CTX *ctx)
{
    Token_C token_string;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, &token_string, T_STRING) == 0) {
        return NULL;
    }

    return parse_tree_node_c_create(PTT_C_STRING, &token_string);
}

ParseTreeNode_C *parser_c_parse_assignment_operator(Parser_C_CTX *ctx)
{
    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_assignment_operator;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_assignment_operator) == -1) {
        goto error;
    }

    switch(token_assignment_operator.type) {
        case T_ASSIGNMENT:
        case T_MULTIPLY_ASSIGN:
        case T_DIVIDE_ASSIGN:
        case T_MODULUS_ASSIGN:
        case T_PLUS_ASSIGN:
        case T_MINUS_ASSIGN:
        case T_BITWISE_LEFTSHIFT_ASSIGN:
        case T_BITWISE_RIGHTSHIFT_ASSIGN:
        case T_BITWISE_AND_ASSIGN:
        case T_BITWISE_XOR_ASSIGN:
        case T_BITWISE_OR_ASSIGN: {
            return parse_tree_node_c_create(PTT_C_ASSIGNMENT_OPERATOR, &token_assignment_operator);
        }
        default: break;
    }

    error: {
        *ctx->lexer = lexer_saved;

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_abstract_declarator(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ABSTRACT_DECLARATOR, NULL);

    ParseTreeNode_C *pointer;
    ParseTreeNode_C *direct_abstract_declarator;

    parser_c_parse_opt(ctx, this_node, pointer, next_after_pointer);

    parser_c_parse_required(ctx, this_node, direct_abstract_declarator, error);

    goto ret;

    next_after_pointer:

    parser_c_parse_opt(ctx, this_node, direct_abstract_declarator, ret);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_list(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_LIST, NULL);

    ParseTreeNode_C *parameter_declaration;

    Lexer_C lexer_saved;
    Lexer_C lexer_saved_comma;

    next_parameter_declaration_list: {
        lexer_saved = *ctx->lexer;

        parser_c_parse_required(ctx, this_node, parameter_declaration, next_parameter_declaration_list_after);

        lexer_saved_comma = *ctx->lexer;

        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COMMA) == 1) {
            goto next_parameter_declaration_list;
        }
        
        lexer_saved = lexer_saved_comma;
    }
    
    next_parameter_declaration_list_after: {
        if (this_node->num == 0) {
            *ctx->lexer = lexer_saved;

            goto error;
        }

        *ctx->lexer = lexer_saved_comma;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_declaration(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_DECLARATION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *abstract_declarator;

    parser_c_parse_list_required(ctx, this_node, declaration_specifier, error);
    
    parser_c_parse_opt(ctx, this_node, declarator, ret);
    
    parser_c_parse_opt(ctx, this_node, abstract_declarator, ret);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_direct_abstract_declarator(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DIRECT_ABSTRACT_DECLARATOR, NULL);
    ParseTreeNode_C *left_node = NULL;

    Lexer_C lexer_saved;

    ParseTreeNode_C *abstract_declarator;
    ParseTreeNode_C *constant_expression;
    ParseTreeNode_C *parameter_type_list;

    Token_C this_node_token;

    while (1) {
        lexer_saved = *ctx->lexer;
    
        if (lexer_c_next_skip_whitespace(ctx->lexer, &this_node_token) == -1) {
            break;
        }
        
        if (this_node_token.type == T_OPEN_PARENT) {
            if (this_node->num == 0) {
                parser_c_parse_opt(ctx, this_node, abstract_declarator, while_end);
            }
            
            parser_c_parse_opt(ctx, this_node, parameter_type_list, next_token);
            
            next_token: ;

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
                break;
            }
            
            this_node->token = this_node_token;
            
            goto while_end;
        }
        
        if (this_node_token.type == T_OPEN_BRACKET) {
            parser_c_parse_opt(ctx, this_node, constant_expression, next);
            
            next: ;

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACKET) == 0) {
                break;
            }
            
            this_node->token = this_node_token;
            
            goto while_end;
        }
        
        break;

        while_end: {
            left_node = this_node;
            this_node = parse_tree_node_c_create(PTT_C_DIRECT_ABSTRACT_DECLARATOR, NULL);
            parse_tree_node_c_add(this_node, left_node);

            continue;
        }
    }
    
    *ctx->lexer = lexer_saved;
    
    if (this_node->num != 0) {
        this_node->elements[0] = NULL;
    }
    
    parse_tree_node_c_destroy(this_node);
    
    return left_node;
}

ParseTreeNode_C *parser_c_parse_enumerator_list(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUMERATOR_LIST, NULL);

    ParseTreeNode_C *enumerator;

    Lexer_C lexer_saved = *ctx->lexer;

    next_enumerator_list: {
        parser_c_parse_required(ctx, this_node, enumerator, error);

        Lexer_C lexer_saved_comma = *ctx->lexer;

        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COMMA) == 1) {
            goto next_enumerator_list;
        }
        
        *ctx->lexer = lexer_saved_comma;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enumerator(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUMERATOR, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *constant_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_required(ctx, this_node, identifier, error);

    Lexer_C lexer_saved_assignment = *ctx->lexer;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_ASSIGNMENT) == 0) {
        *ctx->lexer = lexer_saved_assignment;

        goto ret;
    }

    parser_c_parse_required(ctx, this_node, constant_expression, error);

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_init_declarator(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INIT_DECLARATOR, NULL);

    ParseTreeNode_C *declarator;
    ParseTreeNode_C *initializer;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_required(ctx, this_node, declarator, error);

    Lexer_C lexer_saved_token = *ctx->lexer;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_ASSIGNMENT) == 0) {
        *ctx->lexer = lexer_saved_token;

        goto ret;
    }

    parser_c_parse_required(ctx, this_node, initializer, error);

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_initializer(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INITIALIZER, NULL);

    ParseTreeNode_C *assignment_expression;
    ParseTreeNode_C *initializer_list;

    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_opt(ctx, this_node, assignment_expression, ret);

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_BRACE) == 0) {
        goto error;
    }

    parser_c_parse_required(ctx, this_node, initializer_list, error);

    Lexer_C lexer_saved_comma = *ctx->lexer;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COMMA) == 0) {
        *ctx->lexer = lexer_saved_comma;
    }

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_initializer_list(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INITIALIZER_LIST, NULL);

    ParseTreeNode_C *initializer;

    Lexer_C lexer_saved = *ctx->lexer;

    next_initializer_list: {
        parser_c_parse_required(ctx, this_node, initializer, error);

        Lexer_C lexer_saved_comma = *ctx->lexer;

        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COMMA) == 1) {
            goto next_initializer_list;
        }
        
        *ctx->lexer = lexer_saved_comma;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_statement(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STATEMENT, NULL);

    ParseTreeNode_C *labeled_statement;
    ParseTreeNode_C *expression_statement;
    ParseTreeNode_C *compound_statement;
    ParseTreeNode_C *selection_statement;
    ParseTreeNode_C *iteration_statement;
    ParseTreeNode_C *jump_statement;

    parser_c_parse_opt(ctx, this_node, labeled_statement, ret);

    parser_c_parse_opt(ctx, this_node, expression_statement, ret);

    parser_c_parse_opt(ctx, this_node, compound_statement, ret);

    parser_c_parse_opt(ctx, this_node, selection_statement, ret);

    parser_c_parse_opt(ctx, this_node, iteration_statement, ret);

    parser_c_parse_opt(ctx, this_node, jump_statement, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_labeled_statement(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_LABELED_STATEMENT, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *statement;
    ParseTreeNode_C *constant_expression;

    Lexer_C lexer_saved = *ctx->lexer;    

    parser_c_parse_opt(ctx, this_node, identifier, rest);

    Token_C token;
    
    if (lexer_c_next_skip_whitespace(ctx->lexer, &token) == -1) {
        goto error;
    }

    if (token.type == T_DEFAULT) {
        this_node->token = token;
    
        goto rest;
    }
    
    if (token.type == T_CASE) {
        this_node->token = token;
        
        parser_c_parse_required(ctx, this_node, constant_expression, error);
    
        goto rest;
    }
    
    goto error;

    rest: {
        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_COLON) == 0) {
            goto error;
        }

        parser_c_parse_required(ctx, this_node, statement, error);
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_expression_statement(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXPRESSION_STATEMENT, NULL);

    ParseTreeNode_C *expression;
    
    Lexer_C lexer_saved = *ctx->lexer;

    parser_c_parse_opt(ctx, this_node, expression, next);

    next: {
        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_SEMICOLON) == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;    

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_selection_statement(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SELECTION_STATEMENT, NULL);

    ParseTreeNode_C *expression;
    ParseTreeNode_C *statement;

    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &token) == -1) {
        goto error;
    }

    switch (token.type) {
        case T_IF:
        case T_SWITCH: {
            this_node->token = token;
        } break;
        default: goto error;
    }

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(ctx, this_node, expression, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(ctx, this_node, statement, error);

    if (token.type == T_SWITCH) {
        goto ret;
    }

    Lexer_C lexer_saved_else = *ctx->lexer;
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_ELSE) == 0) {
        *ctx->lexer = lexer_saved_else;
    
        goto ret;
    }

    parser_c_parse_required(ctx, this_node, statement, error);

    ret: {
        return this_node;
    }

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_iteration_statement(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *expression;
    ParseTreeNode_C *statement;

    Lexer_C lexer_saved = *ctx->lexer;

    Token_C iteration_statement_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &iteration_statement_token) == -1) {
        return NULL;
    }

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ITERATION_STATEMENT, &iteration_statement_token);

    switch(iteration_statement_token.type) {
        case T_WHILE: {
            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(ctx, this_node, expression, error);

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(ctx, this_node, statement, error);
        } break;
        case T_DO: {
            parser_c_parse_required(ctx, this_node, statement, error);

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_WHILE) == 0) {
                goto error;
            }

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(ctx, this_node, expression, error);

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
                goto error;
            }

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_SEMICOLON) == 0) {
                goto error;
            }
        } break;
        case T_FOR: {
            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_OPEN_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_opt(ctx, this_node, expression, next_for_1);

            next_for_1:

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_SEMICOLON) == 0) {
                goto error;
            }

            parser_c_parse_opt(ctx, this_node, expression, next_for_2);

            next_for_2:

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_SEMICOLON) == 0) {
                goto error;
            }

            parser_c_parse_opt(ctx, this_node, expression, next_for_3);

            next_for_3:

            if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_CLOSING_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(ctx, this_node, statement, error);
        } break;
        default: goto error;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_jump_statement(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_JUMP_STATEMENT, NULL);
    
    ParseTreeNode_C *identifier;
    ParseTreeNode_C *expression;
    
    Lexer_C lexer_saved = *ctx->lexer;
    
    Token_C jump_statement_token;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &jump_statement_token) == -1) {
        goto error;
    }

    switch(jump_statement_token.type) {
        case T_GOTO: {
            parser_c_parse_required(ctx, this_node, identifier, next_token_semicolon);
        } break;
        case T_RETURN: {
            parser_c_parse_opt(ctx, this_node, expression, next_token_semicolon);
        } break;
        case T_CONTINUE:
        case T_BREAK: break;
        default: goto error;
    }
    
    next_token_semicolon:
    
    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_SEMICOLON) == 0) {
    	lexer_c_log_at(L_ERROR, ctx->lexer, &jump_statement_token, "expected ';' after return statement");
        goto error;
    }
    
    this_node->token = jump_statement_token;

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_conditional(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_CONDITIONAL, NULL);

    ParseTreeNode_C *preprocessor_if_line;
    ParseTreeNode_C *preprocessor_text;
    ParseTreeNode_C *preprocessor_elif_parts;
    ParseTreeNode_C *preprocessor_else_part;

    parser_c_parse_required(ctx, this_node, preprocessor_if_line, error);

    parser_c_parse_required(ctx, this_node, preprocessor_text, error);

    parser_c_parse_required(ctx, this_node, preprocessor_elif_parts, error);

    parser_c_parse_opt(ctx, this_node, preprocessor_else_part, next_endif);

    next_endif: {
        if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_MACRO_ENDIF) == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_if_line(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *constant_expression;
    ParseTreeNode_C *identifier;

    Lexer_C lexer_saved = *ctx->lexer;

    Token_C token_if;

    if (lexer_c_next_skip_whitespace(ctx->lexer, &token_if) == -1) {
        return NULL;
    }
    
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_IF_LINE, NULL);

    switch (token_if.type) {
        case T_MACRO_IF: {
            parser_c_parse_required(ctx, this_node, constant_expression, error);
        } break;
        case T_MACRO_IFDEF: {
            parser_c_parse_required(ctx, this_node, identifier, error);
        } break;
        case T_MACRO_IFNDEF: {
            parser_c_parse_required(ctx, this_node, identifier, error);
        } break;
        default: {
            goto error;
        }
    }

    this_node->token = token_if;

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_elif_parts(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_ELIF_PARTS, NULL);

    ParseTreeNode_C *preprocessor_elif_line;
    ParseTreeNode_C *preprocessor_text;

    while (1) {
        parser_c_parse_required(ctx, this_node, preprocessor_elif_line, ret);

        parser_c_parse_required(ctx, this_node, preprocessor_text, error);
    }

    ret : {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_elif_line(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_ELIF_LINE, NULL);

    ParseTreeNode_C *constant_expression;

    Lexer_C lexer_saved = *ctx->lexer;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_MACRO_ELIF) == 0) {
        goto error;
    }

    parser_c_parse_required(ctx, this_node, constant_expression, error);

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_else_part(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_ELSE_PART, NULL);

    ParseTreeNode_C *preprocessor_else_line;
    ParseTreeNode_C *preprocessor_text;

    parser_c_parse_required(ctx, this_node, preprocessor_else_line, error);

    parser_c_parse_required(ctx, this_node, preprocessor_text, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_else_line(Parser_C_CTX *ctx)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_ELSE_LINE, NULL);

    Lexer_C lexer_saved = *ctx->lexer;

    if (lexer_c_next_skip_whitespace_token_is_type(ctx->lexer, NULL, T_MACRO_ELSE) == 0) {
        goto error;
    }

    return this_node;

    error: {
        *ctx->lexer = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_text(Parser_C_CTX *ctx)
{
    const char *token_text_begin = NULL;
    size_t token_text_len = 0;

    Lexer_C lexer_saved;

    while (1) {
        lexer_saved = *ctx->lexer;

        Token_C token_text;

        if (lexer_c_next(ctx->lexer, &token_text) == -1) {
            *ctx->lexer = lexer_saved;

            break;
        }

        switch (token_text.type) {
            case T_MACRO_IF:
            case T_MACRO_IFDEF:
            case T_MACRO_IFNDEF:
            case T_MACRO_ELIF:
            case T_MACRO_ELSE:
            case T_MACRO_ENDIF:
                *ctx->lexer = lexer_saved;

                goto while_end;
            break;
            default: break;
        }

        if (token_text_begin == NULL) {
            token_text_begin = token_text.value;
        }

        token_text_len += token_text.len;
    }
    
    while_end:

    if (token_text_begin == NULL) {
        return NULL;
    }

    Token_C token_text_build = {
        .type = T_MACRO_TOKEN_SEQUENZE,
        .value = token_text_begin,
        .len = token_text_len
    };

    return parse_tree_node_c_create(PTT_C_PREPROCESSOR_TEXT, &token_text_build);
}
