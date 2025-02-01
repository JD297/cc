#include "lexer_c.h"
#include "parser_c.h"
#include "parse_tree_node_c.h"
#include "parse_tree_type_c.h"

#include <assert.h>

#include <stddef.h>
#include <stdlib.h>

ParseTreeNode_C *parser_c_parse(Lexer_C *lexer)
{
    return parser_c_parse_translation_unit(lexer);
}

ParseTreeNode_C *parser_c_parse_translation_unit(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TRANSLATION_UNIT, NULL);

    ParseTreeNode_C *external_declaration;

    parser_c_parse_list_opt(lexer, this_node, external_declaration);

    return this_node;
}

ParseTreeNode_C *parser_c_parse_external_declaration(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXTERNAL_DECLARATION, NULL);

    ParseTreeNode_C *function_definition;
    ParseTreeNode_C *declaration;

    parser_c_parse_opt(lexer, this_node, function_definition, ret);

    parser_c_parse_opt(lexer, this_node, declaration, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_function_definition(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_FUNCTION_DEFINITION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *declaration;
    ParseTreeNode_C *compound_statement;

    parser_c_parse_list_opt(lexer, this_node, declaration_specifier);
    
    parser_c_parse_required(lexer, this_node, declarator, error);

    parser_c_parse_list_opt(lexer, this_node, declaration);
    
    parser_c_parse_required(lexer, this_node, compound_statement, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *init_declarator;

    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_list_required(lexer, this_node, declaration_specifier, error);

    parser_c_parse_list_opt(lexer, this_node, init_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_SEMICOLON) == 0) {
        goto error;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration_specifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION_SPECIFIER, NULL);

    ParseTreeNode_C *storage_class_specifier;
    ParseTreeNode_C *type_specifier;
    ParseTreeNode_C *type_qualifier;

    next: {
        parser_c_parse_opt(lexer, this_node, storage_class_specifier, next);

        parser_c_parse_opt(lexer, this_node, type_specifier, next);

        parser_c_parse_opt(lexer, this_node, type_qualifier, next);
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

ParseTreeNode_C *parser_c_parse_declarator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATOR, NULL);

    ParseTreeNode_C *pointer;
    ParseTreeNode_C *direct_declarator;

    parser_c_parse_opt(lexer, this_node, pointer, next_direct_declarator);
    
    next_direct_declarator: {
        parser_c_parse_required(lexer, this_node, direct_declarator, error);
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_compound_statement(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_COMPOUND_STATEMENT, NULL);

    ParseTreeNode_C *declaration;
    ParseTreeNode_C *statement;

    const char* lexer_saved = lexer->pbuf;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }

    parser_c_parse_list_opt(lexer, this_node, declaration);

    parser_c_parse_list_opt(lexer, this_node, statement);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_storage_class_specifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STORAGE_CLASS_SPECIFIER, NULL);

    const char* lexer_saved = lexer->pbuf;

    Token_C *token_storage_class_specifier = lexer_c_next_skip_whitespace(lexer);

    if (token_storage_class_specifier->type == T_AUTO || 
        token_storage_class_specifier->type == T_REGISTER || 
        token_storage_class_specifier->type == T_STATIC || 
        token_storage_class_specifier->type == T_EXTERN || 
        token_storage_class_specifier->type == T_TYPEDEF) {
        this_node->token = token_storage_class_specifier;
    
        return this_node;
    }

    lexer->pbuf = lexer_saved;

    token_c_destroy(token_storage_class_specifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_type_specifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_SPECIFIER, NULL);

    const char* lexer_saved = lexer->pbuf;

    Token_C *token_type_specifier = lexer_c_next_skip_whitespace(lexer);

    if (token_type_specifier->type == T_VOID || 
        token_type_specifier->type == T_CHAR || 
        token_type_specifier->type == T_SHORT || 
        token_type_specifier->type == T_INT || 
        token_type_specifier->type == T_LONG ||
        token_type_specifier->type == T_FLOAT ||
        token_type_specifier->type == T_DOUBLE ||
        token_type_specifier->type == T_SIGNED ||
        token_type_specifier->type == T_UNSIGNED) {
        this_node->token = token_type_specifier;
    
        return this_node;
    }

    lexer->pbuf = lexer_saved;

    ParseTreeNode_C *struct_or_union_specifier;
    ParseTreeNode_C *enum_specifier;
    ParseTreeNode_C *typedef_name;

    parser_c_parse_opt(lexer, this_node, struct_or_union_specifier, ret);

    parser_c_parse_opt(lexer, this_node, enum_specifier, ret);

    parser_c_parse_opt(lexer, this_node, typedef_name, ret);

    lexer->pbuf = lexer_saved;

    token_c_destroy(token_type_specifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_type_qualifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_QUALIFIER, NULL);

    const char* lexer_saved = lexer->pbuf;

    Token_C *token_type_qualifier = lexer_c_next_skip_whitespace(lexer);

    if (token_type_qualifier->type == T_CONST || token_type_qualifier->type == T_VOLATILE) {
        this_node->token = token_type_qualifier;
    
        return this_node;
    }

    lexer->pbuf = lexer_saved;

    token_c_destroy(token_type_qualifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_struct_or_union_specifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION_SPECIFIER, NULL);

    ParseTreeNode_C *struct_or_union;
    ParseTreeNode_C *identifier;
    ParseTreeNode_C *struct_declaration;

    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_required(lexer, this_node, struct_or_union, error);

    parser_c_parse_opt(lexer, this_node, identifier, has_identifier);
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }

    parser_c_parse_list_required(lexer, this_node, struct_declaration, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    has_identifier: {
        const char* lexer_saved_brace = lexer->pbuf;

        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_BRACE) == 0) {
            lexer->pbuf = lexer_saved_brace;

            goto ret;
        }
        
        parser_c_parse_list_required(lexer, this_node, struct_declaration, error);
    
        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACE) == 0) {
            goto error;
        }
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enum_specifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUM_SPECIFIER, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *enumerator;

    const char* lexer_saved = lexer->pbuf;
    
    Token_C *token_enum = lexer_c_next_skip_whitespace(lexer);
    
    if (token_enum->type == T_STRUCT) {
        goto error;
    }
    
    this_node->token = token_enum;
    
    parser_c_parse_opt(lexer, this_node, identifier, has_identifier);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }
    
    parser_c_parse_list_required(lexer, this_node, enumerator, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }
    
    goto ret;

    has_identifier: {
        const char* lexer_saved_brace = lexer->pbuf;

        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_BRACE) == 0) {
            lexer->pbuf = lexer_saved_brace;

            goto ret;
        }
        
        parser_c_parse_list_required(lexer, this_node, enumerator, error);
    
        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACE) == 0) {
            goto error;
        }
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_typedef_name(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPEDEF_NAME, NULL);

    ParseTreeNode_C *identifier;

    parser_c_parse_required(lexer, this_node, identifier, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_or_union(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION, NULL);
    
    const char* lexer_saved = lexer->pbuf;

    Token_C *token_struct_or_union = lexer_c_next_skip_whitespace(lexer);

    if (token_struct_or_union->type == T_STRUCT || token_struct_or_union->type == T_UNION) {
        this_node->token = token_struct_or_union;
    
        return this_node;
    }

    lexer->pbuf = lexer_saved;

    token_c_destroy(token_struct_or_union);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_identifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_IDENTIFIER, NULL);

    const char* lexer_saved = lexer->pbuf;

    Token_C *token_identifier = lexer_c_next_skip_whitespace(lexer);

    if (token_identifier->type == T_IDENTIFIER) {
        this_node->token = token_identifier;
    
        return this_node;
    }

    lexer->pbuf = lexer_saved;

    token_c_destroy(token_identifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_struct_declaration(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATION, NULL);

    ParseTreeNode_C *specifier_qualifier;
    ParseTreeNode_C *struct_declarator;

    parser_c_parse_list_required(lexer, this_node, specifier_qualifier, error);

    parser_c_parse_list_required(lexer, this_node, struct_declarator, error);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_SEMICOLON) == 0) {
        goto error;
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_specifier_qualifier(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SPECIFIER_QUALIFIER, NULL);

    ParseTreeNode_C *type_specifier;
    ParseTreeNode_C *type_qualifier;

    parser_c_parse_opt(lexer, this_node, type_specifier, ret);

    parser_c_parse_opt(lexer, this_node, type_qualifier, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator_list(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR_LIST, NULL);

    ParseTreeNode_C *struct_declarator;
    
    const char* lexer_saved = lexer->pbuf;

    next_struct_declarator: {
        parser_c_parse_required(lexer, this_node, struct_declarator, check_error);
        
        const char* lexer_saved_comma = lexer->pbuf;
        
        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COMMA) == 1) {
            goto next_struct_declarator;
        }
        
        lexer->pbuf = lexer_saved_comma;
    }
    
    check_error: {
        if (this_node->num == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR, NULL);

    ParseTreeNode_C *declarator;
    ParseTreeNode_C *constant_expression;
    
    const char* lexer_saved = lexer->pbuf;
    
    parser_c_parse_opt(lexer, this_node, declarator, has_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COLON) == 0) {
        goto error;
    }
    
    parser_c_parse_required(lexer, this_node, constant_expression, error);
    
    goto ret;

    has_declarator: {
        const char* lexer_saved_colon = lexer->pbuf;

        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COLON) == 0) {
            lexer->pbuf = lexer_saved_colon;

            goto ret;
        }
        
        parser_c_parse_required(lexer, this_node, constant_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_constant_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONSTANT_EXPRESSION, NULL);

    ParseTreeNode_C *conditional_expression;

    parser_c_parse_required(lexer, this_node, conditional_expression, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_pointer(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_POINTER, NULL);

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *type_qualifier;
    ParseTreeNode_C *pointer;
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_MULTIPLY) == 0) {
        goto error;
    }
    
    parser_c_parse_list_opt(lexer, this_node, type_qualifier);
    
    parser_c_parse_opt(lexer, this_node, pointer, ret);

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_direct_declarator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DIRECT_DECLARATOR, NULL);

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *constant_expression;
    ParseTreeNode_C *parameter_type_list;

    Token_C *token_after_direct_declarator;
    
    parser_c_parse_opt(lexer, this_node, identifier, after_direct_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(lexer, this_node, declarator, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
        goto error;
    }

    after_direct_declarator: {
        token_after_direct_declarator = lexer_c_next_skip_whitespace(lexer);
        
        if (token_after_direct_declarator == NULL) {
            goto error;
        }
        
        switch(token_after_direct_declarator->type) {
            case T_OPEN_PARENT: {
                parser_c_parse_list_required(lexer, this_node, identifier, next_after_direct_declarator_check_parameter_type);
                
                goto next_after_direct_declarator_parent;
                
                next_after_direct_declarator_check_parameter_type: {
            
                    parser_c_parse_required(lexer, this_node, parameter_type_list, error);
                    
                    next_after_direct_declarator_parent: {
                        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
                            goto error;
                        }
                    }
                }
                
                break;
            }
            case T_OPEN_BRACKET: {
                parser_c_parse_opt(lexer, this_node, constant_expression, next_after_direct_declarator_bracket);
                
                next_after_direct_declarator_bracket:

                if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACKET) == 0) {
                    goto error;
                }
                
                break;
            }
            default: {
                goto error;
            }
        }
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;

        token_c_destroy(token_after_direct_declarator);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_type_list(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_TYPE_LIST, NULL);

    ParseTreeNode_C *parameter_list;
    
    Token_C *token_dot_dot_dot;
    
    const char *lexer_saved = lexer->pbuf;
    
    parser_c_parse_required(lexer, this_node, parameter_list, error);

    const char* lexer_saved_comma = lexer->pbuf;
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COMMA) == 1) {
        if ((token_dot_dot_dot = lexer_c_next_skip_whitespace(lexer)) == NULL) {
            goto error;
        }
        
        if (token_dot_dot_dot->type != T_DOT_DOT_DOT) {
            goto error;
        }
        
        this_node->token = token_dot_dot_dot;
    } else {
        lexer->pbuf = lexer_saved_comma;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(token_dot_dot_dot);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_conditional_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONDITIONAL_EXPRESSION, NULL);

    ParseTreeNode_C *logical_or_expression;
    ParseTreeNode_C *expression;
    ParseTreeNode_C *conditional_expression;
    
    const char *lexer_saved = lexer->pbuf;

    parser_c_parse_required(lexer, this_node, logical_or_expression, error);

    const char *lexer_saved_ternary = lexer->pbuf;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_TERNARY) == 0) {
        lexer->pbuf = lexer_saved_ternary;

        goto ret;
    }

    parser_c_parse_required(lexer, this_node, expression, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_TERNARY) == 0) {
        goto error;
    }
    
    parser_c_parse_required(lexer, this_node, conditional_expression, error);

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_logical_or_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_LOGICAL_OR_EXPRESSION
    #define TOKEN_TYPE T_LOGICAL_OR

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *logical_and_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, logical_and_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, logical_and_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, logical_and_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXPRESSION, NULL);

    ParseTreeNode_C *assignment_expression;

    const char *lexer_saved = lexer->pbuf;

    next_assignment_expression: {
        parser_c_parse_required(lexer, this_node, assignment_expression, error);

        const char *lexer_saved_token = lexer->pbuf;

        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COMMA) == 1) {
            goto next_assignment_expression;
        }
        
        lexer->pbuf = lexer_saved_token;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_logical_and_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_LOGICAL_AND_EXPRESSION
    #define TOKEN_TYPE T_LOGICAL_AND

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *inclusive_or_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, inclusive_or_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, inclusive_or_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, inclusive_or_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_inclusive_or_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_INCLUSIVE_OR_EXPRESSION
    #define TOKEN_TYPE T_BITWISE_OR

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *exclusive_or_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, exclusive_or_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, exclusive_or_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, exclusive_or_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_exclusive_or_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_EXCLUSIVE_OR_EXPRESSION
    #define TOKEN_TYPE T_BITWISE_XOR

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *and_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, and_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, and_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, and_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_and_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_AND_EXPRESSION
    #define TOKEN_TYPE T_BITWISE_AND

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *equality_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, equality_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, equality_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, equality_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_equality_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_EQUALITY_EXPRESSION
    #define TOKEN_TYPE_1 T_EQUAL_TO
    #define TOKEN_TYPE_2 T_NOT_EQUAL_TO

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *relational_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, relational_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2)) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, relational_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2)) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, relational_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE_1
    #undef TOKEN_TYPE_2
}

ParseTreeNode_C *parser_c_parse_relational_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_RELATIONAL_EXPRESSION
    #define TOKEN_TYPE_1 T_LESS_THAN
    #define TOKEN_TYPE_2 T_GREATER_THAN
    #define TOKEN_TYPE_3 T_LESS_THAN_OR_EQUAL_TO
    #define TOKEN_TYPE_4 T_GREATER_THAN_OR_EQUAL_TO

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *shift_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, shift_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2 &&
                                                                                    this_node_token->type != TOKEN_TYPE_3 &&
                                                                                    this_node_token->type != TOKEN_TYPE_4)) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, shift_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2 &&
                                                                                        this_node_token->type != TOKEN_TYPE_3 &&
                                                                                        this_node_token->type != TOKEN_TYPE_4)) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, shift_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE_1
    #undef TOKEN_TYPE_2
    #undef TOKEN_TYPE_3
    #undef TOKEN_TYPE_4
}

ParseTreeNode_C *parser_c_parse_shift_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_SHIFT_EXPRESSION
    #define TOKEN_TYPE_1 T_BITWISE_LEFTSHIFT
    #define TOKEN_TYPE_2 T_BITWISE_RIGHTSHIFT

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *additive_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, additive_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2)) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, additive_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2)) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, additive_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE_1
    #undef TOKEN_TYPE_2
}

ParseTreeNode_C *parser_c_parse_additive_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_ADDITIVE_EXPRESSION
    #define TOKEN_TYPE_1 T_PLUS
    #define TOKEN_TYPE_2 T_MINUS

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *multiplicative_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, multiplicative_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2)) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, multiplicative_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2)) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, multiplicative_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE_1
    #undef TOKEN_TYPE_2
}

ParseTreeNode_C *parser_c_parse_multiplicative_expression(Lexer_C *lexer)
{
    #define PTT_C_TYPE PTT_C_MULTIPLICATIVE_EXPRESSION
    #define TOKEN_TYPE_1 T_MULTIPLY
    #define TOKEN_TYPE_2 T_DIVIDE
    #define TOKEN_TYPE_3 T_MODULUS

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *cast_expression;

    const char *lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, cast_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2 &&
                                                                                    this_node_token->type != TOKEN_TYPE_3)) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, cast_expression, error);

    this_node->token = this_node_token;

    while (1) {
        const char *lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2 &&
                                                                                        this_node_token->type != TOKEN_TYPE_3)) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(lexer, this_node, cast_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE_1
    #undef TOKEN_TYPE_2
    #undef TOKEN_TYPE_3
}

ParseTreeNode_C *parser_c_parse_cast_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CAST_EXPRESSION, NULL);

    ParseTreeNode_C *unary_expression;
    ParseTreeNode_C *type_name;
    ParseTreeNode_C *cast_expression;

    const char *lexer_saved = lexer->pbuf;

    parser_c_parse_opt(lexer, this_node, unary_expression, ret);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_PARENT) == 0) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, type_name, error);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(lexer, this_node, cast_expression, error);

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_unary_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_UNARY_EXPRESSION, NULL);

    ParseTreeNode_C *postfix_expression;
    ParseTreeNode_C *unary_expression;
    ParseTreeNode_C *unary_operator;
    ParseTreeNode_C *cast_expression;
    ParseTreeNode_C *type_name;

    const char *lexer_saved = lexer->pbuf;

    parser_c_parse_opt(lexer, this_node, postfix_expression, ret);

    parser_c_parse_required(lexer, this_node, unary_operator, next_tokens);
    
    parser_c_parse_required(lexer, this_node, cast_expression, error);
    
    next_tokens: ;
    
    Token_C *token = lexer_c_next_skip_whitespace(lexer);

    if (token == NULL) {
        goto error;
    }

    switch(token->type) {
        case T_INCREMENT:
        case T_DECREMENT: {
            parser_c_parse_required(lexer, this_node, unary_expression, error);
        } break;
        case T_SIZEOF: {
            parser_c_parse_opt(lexer, this_node, unary_expression, ret);
            
            parser_c_parse_required(lexer, this_node, type_name, error);
        } break;
        default: goto error;
    }

    ret: {
        this_node->token = token;
    
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        token_c_destroy(token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_type_name(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_NAME, NULL);

    ParseTreeNode_C *specifier_qualifier;
    ParseTreeNode_C *abstract_declarator;

    next_specifier_qualifier: {
        parser_c_parse_opt(lexer, this_node, specifier_qualifier, next_specifier_qualifier);
    }

    if (this_node->num == 0) {
        goto error;
    }

    parser_c_parse_opt(lexer, this_node, abstract_declarator, ret);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_postfix_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, NULL);

    ParseTreeNode_C *primary_expression;
    ParseTreeNode_C *expression;
    ParseTreeNode_C *assignment_expression;
    ParseTreeNode_C *identifier;

    const char* lexer_saved = lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(lexer, this_node, primary_expression, error);

    while (1) {
        const char* lexer_saved_token = lexer->pbuf;

        if ((this_node_token = lexer_c_next_skip_whitespace(lexer)) == NULL) {
            lexer->pbuf = lexer_saved_token;

            goto ret;
        }

        switch(this_node_token->type) {
            case T_OPEN_BRACKET: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, this_node_token);

                parse_tree_node_c_add(this_node, left_node);
                
                parser_c_parse_required(lexer, this_node, expression, error);
                
                if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACKET) == 0) {
                    goto error;
                }
            } break;
            case T_OPEN_PARENT: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, this_node_token);

                parse_tree_node_c_add(this_node, left_node);
                
                parser_c_parse_list_opt(lexer, this_node, assignment_expression);
                
                if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
                    goto error;
                }
            } break;
            case T_DOT:
            case T_ARROW: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, this_node_token);

                parse_tree_node_c_add(this_node, left_node);
                
                parser_c_parse_required(lexer, this_node, identifier, error);
            } break;
            case T_INCREMENT:
            case T_DECREMENT: {
                left_node = this_node;
                
                this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, this_node_token);

                parse_tree_node_c_add(this_node, left_node);
            } break;
            default: {
                lexer->pbuf = lexer_saved_token;
                
                token_c_destroy(this_node_token);
            }
        }
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved; 

        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_unary_operator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_UNARY_OPERATOR, NULL);

    const char* lexer_saved = lexer->pbuf;

    Token_C *token_unary_operator = lexer_c_next_skip_whitespace(lexer);

    if (token_unary_operator->type == T_BITWISE_AND || 
        token_unary_operator->type == T_MULTIPLY || 
        token_unary_operator->type == T_MULTIPLY || 
        token_unary_operator->type == T_MINUS || 
        token_unary_operator->type == T_TILDE ||
        token_unary_operator->type == T_BITWISE_OR) {
        this_node->token = token_unary_operator;
    
        return this_node;
    }

    lexer->pbuf = lexer_saved;

    token_c_destroy(token_unary_operator);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_primary_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PRIMARY_EXPRESSION, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *constant;
    ParseTreeNode_C *string;
    ParseTreeNode_C *expression;

    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_opt(lexer, this_node, identifier, ret);

    parser_c_parse_opt(lexer, this_node, constant, ret);

    parser_c_parse_opt(lexer, this_node, string, ret);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_PARENT) == 0) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, expression, error);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
        goto error;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_assignment_expression(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ASSIGNMENT_EXPRESSION, NULL);

    ParseTreeNode_C *conditional_expression;
    ParseTreeNode_C *unary_expression;
    ParseTreeNode_C *assignment_operator;
    ParseTreeNode_C *assignment_expression;

    parser_c_parse_opt(lexer, this_node, conditional_expression, ret);

    parser_c_parse_required(lexer, this_node, unary_expression, error);

    parser_c_parse_required(lexer, this_node, assignment_operator, error);

    parser_c_parse_required(lexer, this_node, assignment_expression, error);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_constant(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONSTANT, NULL);
    
    const char* lexer_saved = lexer->pbuf;

    Token_C *token = lexer_c_next_skip_whitespace(lexer);

    if (token == NULL) {
        goto error;
    }
    
    switch(token->type) {
        case T_CHARACTER:
        case T_NUMBER:
        case T_STRING: {
            this_node->token = token;
        } break;
        default: goto error;
    }    
   
    return this_node;

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_string(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRING, NULL);

    const char* lexer_saved = lexer->pbuf;

    Token_C *token_string = lexer_c_next_skip_whitespace(lexer);

    if (token_string == NULL) {
        goto error;
    }

    if (token_string->type == T_STRING) {
        this_node->token = token_string;

        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        token_c_destroy(token_string);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_assignment_operator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ASSIGNMENT_OPERATOR, NULL);

    const char* lexer_saved = lexer->pbuf;

    Token_C *token_assignment_operator = lexer_c_next_skip_whitespace(lexer);

    if (token_assignment_operator == NULL) {
        goto error;
    }

    if (token_assignment_operator->type == T_ASSIGNMENT || 
        token_assignment_operator->type == T_MULTIPLY_ASSIGN || 
        token_assignment_operator->type == T_DIVIDE_ASSIGN || 
        token_assignment_operator->type == T_MODULUS_ASSIGN || 
        token_assignment_operator->type == T_PLUS_ASSIGN ||
        token_assignment_operator->type == T_MINUS_ASSIGN ||
        token_assignment_operator->type == T_BITWISE_LEFTSHIFT_ASSIGN ||
        token_assignment_operator->type == T_BITWISE_RIGHTSHIFT_ASSIGN ||
        token_assignment_operator->type == T_BITWISE_AND_ASSIGN ||
        token_assignment_operator->type == T_BITWISE_XOR_ASSIGN ||
        token_assignment_operator->type == T_BITWISE_OR_ASSIGN) {
        this_node->token = token_assignment_operator;
    
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        token_c_destroy(token_assignment_operator);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_abstract_declarator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ABSTRACT_DECLARATOR, NULL);

    ParseTreeNode_C *pointer;
    ParseTreeNode_C *direct_abstract_declarator;

    parser_c_parse_opt(lexer, this_node, pointer, next_after_pointer);

    parser_c_parse_required(lexer, this_node, direct_abstract_declarator, error);

    goto ret;

    next_after_pointer:

    parser_c_parse_opt(lexer, this_node, direct_abstract_declarator, ret);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_list(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_LIST, NULL);

    ParseTreeNode_C *parameter_declaration;

    const char* lexer_saved = lexer->pbuf;

    next_parameter_declaration_list: {
        parser_c_parse_required(lexer, this_node, parameter_declaration, error);

        const char* lexer_saved_comma = lexer->pbuf;

        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COMMA) == 1) {
            goto next_parameter_declaration_list;
        }
        
        lexer->pbuf = lexer_saved_comma;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_declaration(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_DECLARATION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *abstract_declarator;

    parser_c_parse_list_required(lexer, this_node, declaration_specifier, error);
    
    parser_c_parse_opt(lexer, this_node, declarator, ret);
    
    parser_c_parse_opt(lexer, this_node, abstract_declarator, ret);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_direct_abstract_declarator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DIRECT_ABSTRACT_DECLARATOR, NULL);
    ParseTreeNode_C *left_node = NULL;

    const char* lexer_saved;

    ParseTreeNode_C *abstract_declarator;
    ParseTreeNode_C *constant_expression;
    ParseTreeNode_C *parameter_type_list;

    Token_C *this_node_token;

    while (1) {
        lexer_saved = lexer->pbuf;
    
        this_node_token = lexer_c_next_skip_whitespace(lexer);
        
        if (this_node_token == NULL) {
            break;
        }
        
        if (this_node_token->type == T_OPEN_PARENT) {
            if (this_node->num == 0) {
                parser_c_parse_opt(lexer, this_node, abstract_declarator, while_end);
            }
            
            parser_c_parse_opt(lexer, this_node, parameter_type_list, next_token);
            
            next_token: ;

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
                break;
            }
            
            this_node->token = this_node_token;
            
            goto while_end;
        }
        
        if (this_node_token->type == T_OPEN_BRACKET) {
            parser_c_parse_opt(lexer, this_node, constant_expression, next);
            
            next: ;

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACKET) == 0) {
                break;
            }
            
            this_node->token = this_node_token;
            
            goto while_end;
        } break;
        
        break;

        while_end: {
            left_node = this_node;
            this_node = parse_tree_node_c_create(PTT_C_DIRECT_ABSTRACT_DECLARATOR, NULL);
            parse_tree_node_c_add(this_node, left_node);

            continue;
        }
    }
    
    lexer->pbuf = lexer_saved;
    
    token_c_destroy(this_node_token);
    
    if (this_node->num != 0) {
        this_node->elements[0] = NULL;
    }
    
    parse_tree_node_c_destroy(this_node);
    
    return left_node;
}

ParseTreeNode_C *parser_c_parse_enumerator_list(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUMERATOR_LIST, NULL);

    ParseTreeNode_C *enumerator;

    const char* lexer_saved = lexer->pbuf;

    next_enumerator_list: {
        parser_c_parse_required(lexer, this_node, enumerator, error);

        const char* lexer_saved_comma = lexer->pbuf;

        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COMMA) == 1) {
            goto next_enumerator_list;
        }
        
        lexer->pbuf = lexer_saved_comma;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enumerator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUMERATOR, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *constant_expression;

    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_required(lexer, this_node, identifier, error);

    const char* lexer_saved_assignment = lexer->pbuf;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_ASSIGNMENT) == 0) {
        lexer->pbuf = lexer_saved_assignment;

        goto ret;
    }

    parser_c_parse_required(lexer, this_node, constant_expression, error);

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_init_declarator(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INIT_DECLARATOR, NULL);

    ParseTreeNode_C *declarator;
    ParseTreeNode_C *initializer;

    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_required(lexer, this_node, declarator, error);

    const char* lexer_saved_token = lexer->pbuf;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_ASSIGNMENT) == 0) {
        lexer->pbuf = lexer_saved_token;

        goto ret;
    }

    parser_c_parse_required(lexer, this_node, initializer, error);

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_initializer(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INITIALIZER, NULL);

    ParseTreeNode_C *assignment_expression;
    ParseTreeNode_C *initializer_list;

    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_opt(lexer, this_node, assignment_expression, ret);

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }

    parser_c_parse_required(lexer, this_node, initializer_list, error);

    const char* lexer_saved_comma = lexer->pbuf;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COMMA) == 0) {
        lexer->pbuf = lexer_saved_comma;
    }

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_initializer_list(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INITIALIZER_LIST, NULL);

    ParseTreeNode_C *initializer;

    const char* lexer_saved = lexer->pbuf;

    next_initializer_list: {
        parser_c_parse_required(lexer, this_node, initializer, error);

        const char* lexer_saved_comma = lexer->pbuf;

        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COMMA) == 1) {
            goto next_initializer_list;
        }
        
        lexer->pbuf = lexer_saved_comma;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_statement(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STATEMENT, NULL);

    ParseTreeNode_C *labeled_statement;
    ParseTreeNode_C *expression_statement;
    ParseTreeNode_C *compound_statement;
    ParseTreeNode_C *selection_statement;
    ParseTreeNode_C *iteration_statement;
    ParseTreeNode_C *jump_statement;

    parser_c_parse_opt(lexer, this_node, labeled_statement, ret);

    parser_c_parse_opt(lexer, this_node, expression_statement, ret);

    parser_c_parse_opt(lexer, this_node, compound_statement, ret);

    parser_c_parse_opt(lexer, this_node, selection_statement, ret);

    parser_c_parse_opt(lexer, this_node, iteration_statement, ret);

    parser_c_parse_opt(lexer, this_node, jump_statement, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_labeled_statement(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_LABELED_STATEMENT, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *statement;
    ParseTreeNode_C *constant_expression;

    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_opt(lexer, this_node, identifier, rest);

    Token_C *token = lexer_c_next_skip_whitespace(lexer);
    
    if (token == NULL) {
        goto error;
    }

    if (token->type == T_DEFAULT) {
        this_node->token = token;
    
        goto rest;
    }
    
    if (token->type == T_CASE) {
        this_node->token = token;
        
        parser_c_parse_required(lexer, this_node, constant_expression, error);
    
        goto rest;
    }
    
    goto error;

    rest: {
        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_COLON) == 0) {
            goto error;
        }

        parser_c_parse_required(lexer, this_node, statement, error);
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;

        token_c_destroy(token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_expression_statement(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXPRESSION_STATEMENT, NULL);

    ParseTreeNode_C *expression;
    
    const char* lexer_saved = lexer->pbuf;

    parser_c_parse_opt(lexer, this_node, expression, next);

    next: {
        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_SEMICOLON) == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;    

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_selection_statement(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SELECTION_STATEMENT, NULL);

    ParseTreeNode_C *expression;
    ParseTreeNode_C *statement;

    const char* lexer_saved = lexer->pbuf;

    Token_C *token = lexer_c_next_skip_whitespace(lexer);

    if (token == NULL) {
        goto error;
    }
    
    if (token->type != T_IF && token->type != T_SWITCH) {
        goto error;
    }

    this_node->token = token;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(lexer, this_node, expression, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(lexer, this_node, statement, error);

    if (token->type == T_SWITCH) {
        goto ret;
    }

    const char* lexer_saved_else = lexer->pbuf;
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_ELSE) == 0) {
        lexer->pbuf = lexer_saved_else;
    
        goto ret;
    }

    parser_c_parse_required(lexer, this_node, statement, error);

    ret: {
        return this_node;
    }

    error: {
        lexer->pbuf = lexer_saved;

        token_c_destroy(token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_iteration_statement(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ITERATION_STATEMENT, NULL);

    ParseTreeNode_C *expression;
    ParseTreeNode_C *statement;

    const char* lexer_saved = lexer->pbuf;

    Token_C *iteration_statement_token = lexer_c_next_skip_whitespace(lexer);

    if (iteration_statement_token == NULL) {
        goto error;
    }

    this_node->token = iteration_statement_token;

    switch(iteration_statement_token->type) {
        case T_WHILE: {
            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(lexer, this_node, expression, error);

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(lexer, this_node, statement, error);
        } break;
        case T_DO: {
            parser_c_parse_required(lexer, this_node, statement, error);

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_WHILE) == 0) {
                goto error;
            }

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(lexer, this_node, expression, error);

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
                goto error;
            }

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_SEMICOLON) == 0) {
                goto error;
            }
        } break;
        case T_FOR: {
            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_OPEN_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_opt(lexer, this_node, expression, next_for_1);

            next_for_1:

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_SEMICOLON) == 0) {
                goto error;
            }

            parser_c_parse_opt(lexer, this_node, expression, next_for_2);

            next_for_2:

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_SEMICOLON) == 0) {
                goto error;
            }

            parser_c_parse_opt(lexer, this_node, expression, next_for_3);

            next_for_3:

            if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_CLOSING_PARENT) == 0) {
                goto error;
            }

            parser_c_parse_required(lexer, this_node, statement, error);
        } break;
        default: goto error;
    }

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;
    
        token_c_destroy(iteration_statement_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_jump_statement(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_JUMP_STATEMENT, NULL);
    
    ParseTreeNode_C *identifier;
    ParseTreeNode_C *expression;
    
    const char* lexer_saved = lexer->pbuf;
    
    Token_C *jump_statement_token = lexer_c_next_skip_whitespace(lexer);

    if (jump_statement_token == NULL) {
        goto error;
    }

    switch(jump_statement_token->type) {
        case T_GOTO: {
            parser_c_parse_required(lexer, this_node, identifier, next_token_semicolon);
        } break;
        case T_RETURN: {
            parser_c_parse_opt(lexer, this_node, expression, next_token_semicolon);
        } break;
        case T_CONTINUE:
        case T_BREAK: break;
        default: goto error;
    }
    
    next_token_semicolon:
    
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_SEMICOLON) == 0) {
        goto error;
    }
    
    this_node->token = jump_statement_token;

    return this_node;

    error: {
        lexer->pbuf = lexer_saved;

        token_c_destroy(jump_statement_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_conditional(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_CONDITIONAL, NULL);

    ParseTreeNode_C *preprocessor_if_line;
    ParseTreeNode_C *preprocessor_text;
    ParseTreeNode_C *preprocessor_elif_parts;
    ParseTreeNode_C *preprocessor_else_part;

    parser_c_parse_required(lexer, this_node, preprocessor_if_line, error);

    parser_c_parse_required(lexer, this_node, preprocessor_text, error);

    parser_c_parse_required(lexer, this_node, preprocessor_elif_parts, error);

    parser_c_parse_opt(lexer, this_node, preprocessor_else_part, next_endif);

    next_endif: {
        if (lexer_c_next_skip_whitespace_token_is_type(lexer, T_MACRO_ENDIF) == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_if_line(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_IF_LINE, NULL);

    ParseTreeNode_C *constant_expression;
    ParseTreeNode_C *identifier;

    Token_C *token_if = lexer_c_next_skip_whitespace(lexer);

    if (token_if == NULL) {
        goto error;
    }

    switch (token_if->type) {
        case T_MACRO_IF: {
            parser_c_parse_required(lexer, this_node, constant_expression, error);
        } break;
        case T_MACRO_IFDEF: {
            parser_c_parse_required(lexer, this_node, identifier, error);
        } break;
        case T_MACRO_IFNDEF: {
            parser_c_parse_required(lexer, this_node, identifier, error);
        } break;
        default: {
            goto error;
        }
    }

    this_node->token = token_if;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_preprocessor_elif_parts(Lexer_C *lexer)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PREPROCESSOR_ELIF_PARTS, NULL);

    ParseTreeNode_C *preprocessor_elif_line;
    ParseTreeNode_C *preprocessor_text;

    while (1) {
        parser_c_parse_required(lexer, this_node, preprocessor_elif_line, while_end);

        parser_c_parse_required(lexer, this_node, preprocessor_text, error);
    }

    while_end: {
        if (this_node->num == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}
