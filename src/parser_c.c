#include "lexer_c.h"
#include "parser_c.h"
#include "parse_tree_node_c.h"
#include "parse_tree_type_c.h"

#include <assert.h>

#include <stddef.h>
#include <stdlib.h>

void *parser_c_create(const char *src)
{
    Parser_C *parser = (Parser_C *)malloc(sizeof(Parser_C)); 
    
    if (parser == NULL) {
        return NULL;
    }
    
    parser->lexer = lexer_c_create(src);
    
    if (parser->lexer == NULL) {
        return NULL;
    }
    
    parser->parse_tree = parse_tree_node_c_create(0, NULL);

    if (parser->parse_tree == NULL) {
        return NULL;
    }
    
    return parser;
}

void parser_c_destroy(Parser_C *parser)
{
    lexer_c_destroy(parser->lexer);

    free(parser);
}

ParseTreeNode_C *parser_c_parse(Parser_C *parser)
{
    return parser_c_parse_translation_unit(parser);
}

ParseTreeNode_C *parser_c_parse_translation_unit(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TRANSLATION_UNIT, NULL);

    ParseTreeNode_C *external_declaration;

    parser_c_parse_list_opt(parser, this_node, external_declaration);

    return this_node;
}

ParseTreeNode_C *parser_c_parse_external_declaration(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXTERNAL_DECLARATION, NULL);

    ParseTreeNode_C *function_definition;
    ParseTreeNode_C *declaration;

    parser_c_parse_opt(parser, this_node, function_definition, ret);

    parser_c_parse_opt(parser, this_node, declaration, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_function_definition(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_FUNCTION_DEFINITION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *declaration;
    ParseTreeNode_C *compound_statement;

    const char* lexer_saved = parser->lexer->pbuf;

    parser_c_parse_list_opt(parser, this_node, declaration_specifier);
    
    parser_c_parse_required(parser, this_node, declarator, error);

    parser_c_parse_list_opt(parser, this_node, declaration);
    
    parser_c_parse_required(parser, this_node, compound_statement, error);

    return this_node;

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION, NULL);

    ParseTreeNode_C *declaration_specifier;
    ParseTreeNode_C *init_declarator;

    const char* lexer_saved = parser->lexer->pbuf;

    parser_c_parse_list_required(parser, this_node, declaration_specifier, error);

    parser_c_parse_list_opt(parser, this_node, init_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_SEMICOLON) == 0) {
        goto error;
    }

    return this_node;

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration_specifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION_SPECIFIER, NULL);

    ParseTreeNode_C *storage_class_specifier;
    ParseTreeNode_C *type_specifier;
    ParseTreeNode_C *type_qualifier;

    next: {
        parser_c_parse_opt(parser, this_node, storage_class_specifier, next);

        parser_c_parse_opt(parser, this_node, type_specifier, next);

        parser_c_parse_opt(parser, this_node, type_qualifier, next);
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

ParseTreeNode_C *parser_c_parse_declarator(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATOR, NULL);

    ParseTreeNode_C *pointer;
    ParseTreeNode_C *direct_declarator;

    parser_c_parse_opt(parser, this_node, pointer, next_direct_declarator);
    
    next_direct_declarator: {
        parser_c_parse_required(parser, this_node, direct_declarator, error);
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_compound_statement(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_COMPOUND_STATEMENT, NULL);

    ParseTreeNode_C *declaration;
    ParseTreeNode_C *statement;

    const char* lexer_saved = parser->lexer->pbuf;

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }

    parser_c_parse_list_opt(parser, this_node, declaration);

    parser_c_parse_list_opt(parser, this_node, statement);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    return this_node;

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_storage_class_specifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STORAGE_CLASS_SPECIFIER, NULL);

    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_storage_class_specifier = lexer_c_next_skip_whitespace(parser->lexer);

    if (token_storage_class_specifier->type == T_AUTO || 
        token_storage_class_specifier->type == T_REGISTER || 
        token_storage_class_specifier->type == T_STATIC || 
        token_storage_class_specifier->type == T_EXTERN || 
        token_storage_class_specifier->type == T_TYPEDEF) {
        this_node->token = token_storage_class_specifier;
    
        return this_node;
    }

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_storage_class_specifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_type_specifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_SPECIFIER, NULL);

    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_type_specifier = lexer_c_next_skip_whitespace(parser->lexer);

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

    ParseTreeNode_C *struct_or_union_specifier;
    ParseTreeNode_C *enum_specifier;
    ParseTreeNode_C *typedef_name;

    parser_c_parse_opt(parser, this_node, struct_or_union_specifier, ret);

    parser_c_parse_opt(parser, this_node, enum_specifier, ret);

    parser_c_parse_opt(parser, this_node, typedef_name, ret);

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_type_specifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_type_qualifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_QUALIFIER, NULL);

    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_type_qualifier = lexer_c_next_skip_whitespace(parser->lexer);

    if (token_type_qualifier->type == T_CONST || token_type_qualifier->type == T_VOLATILE) {
        this_node->token = token_type_qualifier;
    
        return this_node;
    }

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_type_qualifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_struct_or_union_specifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION_SPECIFIER, NULL);

    ParseTreeNode_C *struct_or_union;
    ParseTreeNode_C *identifier;
    ParseTreeNode_C *struct_declaration;

    const char* lexer_saved = parser->lexer->pbuf;

    parser_c_parse_required(parser, this_node, struct_or_union, error);

    parser_c_parse_opt(parser, this_node, identifier, has_identifier);
    
    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }

    parser_c_parse_list_required(parser, this_node, struct_declaration, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    has_identifier: {
        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_BRACE) == 0) {
            goto ret;
        }
        
        parser_c_parse_list_required(parser, this_node, struct_declaration, error);
    
        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_BRACE) == 0) {
            goto error;
        }
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enum_specifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUM_SPECIFIER, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *enumerator;

    const char* lexer_saved = parser->lexer->pbuf;
    
    Token_C *token_enum = lexer_c_next_skip_whitespace(parser->lexer);
    
    if (token_enum->type == T_STRUCT) {
        goto error;
    }
    
    this_node->token = token_enum;
    
    parser_c_parse_opt(parser, this_node, identifier, has_identifier);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }
    
    parser_c_parse_list_required(parser, this_node, enumerator, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }
    
    goto ret;

    has_identifier: {
        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_BRACE) == 0) {
            goto ret;
        }
        
        parser_c_parse_list_required(parser, this_node, enumerator, error);
    
        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_BRACE) == 0) {
            goto error;
        }
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_typedef_name(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPEDEF_NAME, NULL);

    ParseTreeNode_C *identifier;

    parser_c_parse_required(parser, this_node, identifier, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_or_union(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION, NULL);
    
    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_struct_or_union = lexer_c_next_skip_whitespace(parser->lexer);

    if (token_struct_or_union->type == T_STRUCT || token_struct_or_union->type == T_UNION) {
        this_node->token = token_struct_or_union;
    
        return this_node;
    }

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_struct_or_union);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_identifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_IDENTIFIER, NULL);

    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_identifier = lexer_c_next_skip_whitespace(parser->lexer);

    if (token_identifier->type == T_IDENTIFIER) {
        this_node->token = token_identifier;
    
        return this_node;
    }

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_identifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_struct_declaration(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATION, NULL);

    ParseTreeNode_C *specifier_qualifier;
    ParseTreeNode_C *struct_declarator;

    parser_c_parse_list_required(parser, this_node, specifier_qualifier, error);

    parser_c_parse_list_required(parser, this_node, struct_declarator, error);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_SEMICOLON) == 0) {
        goto error;
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_specifier_qualifier(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SPECIFIER_QUALIFIER, NULL);

    ParseTreeNode_C *type_specifier;
    ParseTreeNode_C *type_qualifier;

    parser_c_parse_opt(parser, this_node, type_specifier, ret);

    parser_c_parse_opt(parser, this_node, type_qualifier, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator_list(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR_LIST, NULL);

    ParseTreeNode_C *struct_declarator;
    
    const char* lexer_saved = parser->lexer->pbuf;

    next_struct_declarator: {
        parser_c_parse_required(parser, this_node, struct_declarator, check_error);
        
        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_COMMA) != 0) {
            goto next_struct_declarator;
        }
    }
    
    check_error: {
        if (this_node->num == 0) {
            goto error;
        }
    }

    return this_node;

    error: {
        parser->lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR, NULL);

    ParseTreeNode_C *declarator;
    ParseTreeNode_C *constant_expression;
    
    const char* lexer_saved = parser->lexer->pbuf;
    
    parser_c_parse_opt(parser, this_node, declarator, has_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_COLON) == 0) {
        goto error;
    }
    
    parser_c_parse_required(parser, this_node, constant_expression, error);
    
    goto ret;

    has_declarator: {
        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_COLON) == 0) {
            goto ret;
        }
        
        parser_c_parse_required(parser, this_node, constant_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_constant_expression(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONSTANT_EXPRESSION, NULL);

    ParseTreeNode_C *conditional_expression;

    parser_c_parse_required(parser, this_node, conditional_expression, error);

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_pointer(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_POINTER, NULL);

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *type_qualifier;
    ParseTreeNode_C *pointer;
    
    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_MULTIPLY) == 0) {
        goto error;
    }
    
    parser_c_parse_list_opt(parser, this_node, type_qualifier);
    
    parser_c_parse_opt(parser, this_node, pointer, ret);

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_direct_declarator(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DIRECT_DECLARATOR, NULL);

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *declarator;
    ParseTreeNode_C *constant_expression;
    ParseTreeNode_C *parameter_type_list;

    Token_C *token_after_direct_declarator;
    
    parser_c_parse_opt(parser, this_node, identifier, after_direct_declarator);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_PARENT) == 0) {
        goto error;
    }
    
    parser_c_parse_required(parser, this_node, declarator, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_PARENT) == 0) {
        goto error;
    }

    after_direct_declarator: {
        token_after_direct_declarator = lexer_c_next_skip_whitespace(parser->lexer);
        
        if (token_after_direct_declarator == NULL) {
            goto error;
        }
        
        switch(token_after_direct_declarator->type) {
            case T_OPEN_PARENT: {
                parser_c_parse_list_required(parser, this_node, identifier, next_after_direct_declarator_check_parameter_type);
                
                goto next_after_direct_declarator_parent;
                
                next_after_direct_declarator_check_parameter_type: {
            
                    parser_c_parse_required(parser, this_node, parameter_type_list, error);
                    
                    next_after_direct_declarator_parent: {
                        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_PARENT) == 0) {
                            goto error;
                        }
                    }
                }
                
                break;
            }
            case T_OPEN_BRACKET: {
                parser_c_parse_opt(parser, this_node, constant_expression, next_after_direct_declarator_bracket);
                
                next_after_direct_declarator_bracket:

                if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_BRACKET) == 0) {
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
        parser->lexer->pbuf = lexer_saved;

        token_c_destroy(token_after_direct_declarator);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_type_list(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_TYPE_LIST, NULL);

    ParseTreeNode_C *parameter_list;
    
    Token_C *token_dot_dot_dot;
    
    const char *lexer_saved = parser->lexer->pbuf;
    
    parser_c_parse_required(parser, this_node, parameter_list, error);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_COMMA) == 1) {
        if ((token_dot_dot_dot = lexer_c_next_skip_whitespace(parser->lexer)) == NULL) {
            goto error;
        }
        
        if (token_dot_dot_dot->type != T_DOT_DOT_DOT) {
            goto error;
        }
        
        this_node->token = token_dot_dot_dot;
    }

    return this_node;

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(token_dot_dot_dot);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_conditional_expression(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONDITIONAL_EXPRESSION, NULL);

    ParseTreeNode_C *logical_or_expression;
    ParseTreeNode_C *expression;
    ParseTreeNode_C *conditional_expression;
    
    const char *lexer_saved = parser->lexer->pbuf;

    parser_c_parse_required(parser, this_node, logical_or_expression, error);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_TERNARY) == 0) {
        goto ret;
    }

    parser_c_parse_required(parser, this_node, expression, error);
    
    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_TERNARY) == 0) {
        goto error;
    }
    
    parser_c_parse_required(parser, this_node, conditional_expression, error);

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
    
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_logical_or_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_LOGICAL_OR_EXPRESSION
    #define TOKEN_TYPE T_LOGICAL_OR

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *logical_and_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, logical_and_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, logical_and_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, logical_and_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_logical_and_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_LOGICAL_AND_EXPRESSION
    #define TOKEN_TYPE T_LOGICAL_AND

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *inclusive_or_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, inclusive_or_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, inclusive_or_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, inclusive_or_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_inclusive_or_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_INCLUSIVE_OR_EXPRESSION
    #define TOKEN_TYPE T_BITWISE_OR

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *exclusive_or_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, exclusive_or_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, exclusive_or_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, exclusive_or_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_exclusive_or_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_EXCLUSIVE_OR_EXPRESSION
    #define TOKEN_TYPE T_BITWISE_XOR

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *and_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, and_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, and_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, and_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_and_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_AND_EXPRESSION
    #define TOKEN_TYPE T_BITWISE_AND

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *equality_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, equality_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, equality_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || this_node_token->type != TOKEN_TYPE) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, equality_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE
}

ParseTreeNode_C *parser_c_parse_equality_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_EQUALITY_EXPRESSION
    #define TOKEN_TYPE_1 T_EQUAL_TO
    #define TOKEN_TYPE_2 T_NOT_EQUAL_TO

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *relational_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, relational_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2)) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, relational_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2)) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, relational_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE_1
    #undef TOKEN_TYPE_2
}

ParseTreeNode_C *parser_c_parse_relational_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_RELATIONAL_EXPRESSION
    #define TOKEN_TYPE_1 T_LESS_THAN
    #define TOKEN_TYPE_2 T_GREATER_THAN
    #define TOKEN_TYPE_3 T_LESS_THAN_OR_EQUAL_TO
    #define TOKEN_TYPE_4 T_GREATER_THAN_OR_EQUAL_TO

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *shift_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, shift_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2 &&
                                                                                    this_node_token->type != TOKEN_TYPE_3 &&
                                                                                    this_node_token->type != TOKEN_TYPE_4)) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, shift_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2 &&
                                                                                        this_node_token->type != TOKEN_TYPE_3 &&
                                                                                        this_node_token->type != TOKEN_TYPE_4)) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, shift_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
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

ParseTreeNode_C *parser_c_parse_shift_expression(Parser_C *parser)
{
    #define PTT_C_TYPE PTT_C_SHIFT_EXPRESSION
    #define TOKEN_TYPE_1 T_BITWISE_LEFTSHIFT
    #define TOKEN_TYPE_2 T_BITWISE_RIGHTSHIFT

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

    ParseTreeNode_C *additive_expression;

    const char *lexer_saved = parser->lexer->pbuf;

    ParseTreeNode_C *left_node;
    
    Token_C *this_node_token;

    parser_c_parse_required(parser, this_node, additive_expression, error);

    if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                    this_node_token->type != TOKEN_TYPE_2)) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, additive_expression, error);

    this_node->token = this_node_token;

    while (1) {
        if ((this_node_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || (this_node_token->type != TOKEN_TYPE_1 && 
                                                                                        this_node_token->type != TOKEN_TYPE_2)) {
            goto ret;
        }

        this_node->token = this_node_token;

        left_node = this_node;

        this_node = parse_tree_node_c_create(PTT_C_TYPE, NULL);

        parse_tree_node_c_add(this_node, left_node);

        parser_c_parse_required(parser, this_node, additive_expression, error);
    }

    ret: {
        return this_node;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;
        
        token_c_destroy(this_node_token);

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
    
    #undef PTT_C_TYPE
    #undef TOKEN_TYPE_1
    #undef TOKEN_TYPE_2
}

ParseTreeNode_C *parser_c_parse_additive_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_additive_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ADDITIVE_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_multiplicative_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_multiplicative_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_MULTIPLICATIVE_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_cast_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_cast_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CAST_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_unary_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_unary_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_UNARY_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_type_name(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_type_name");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_NAME, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_postfix_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_postfix_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_POSTFIX_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_unary_operator(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_UNARY_OPERATOR, NULL);

    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_unary_operator = lexer_c_next_skip_whitespace(parser->lexer);

    if (token_unary_operator->type == T_BITWISE_AND || 
        token_unary_operator->type == T_MULTIPLY || 
        token_unary_operator->type == T_MULTIPLY || 
        token_unary_operator->type == T_MINUS || 
        token_unary_operator->type == T_TILDE ||
        token_unary_operator->type == T_BITWISE_OR) {
        this_node->token = token_unary_operator;
    
        return this_node;
    }

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_unary_operator);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_primary_expression(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PRIMARY_EXPRESSION, NULL);

    ParseTreeNode_C *identifier;
    ParseTreeNode_C *constant;
    ParseTreeNode_C *string;
    ParseTreeNode_C *expression;

    const char* lexer_saved = parser->lexer->pbuf;

    parser_c_parse_opt(parser, this_node, identifier, ret);

    parser_c_parse_opt(parser, this_node, constant, ret);

    parser_c_parse_opt(parser, this_node, string, ret);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_PARENT) == 0) {
        goto error;
    }

    parser_c_parse_required(parser, this_node, expression, error);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_PARENT) == 0) {
        goto error;
    }

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
    }

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_assignment_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_assignment_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ASSIGNMENT_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_constant(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONSTANT, NULL);

    ParseTreeNode_C *integer_constant;
    ParseTreeNode_C *character_constant;
    ParseTreeNode_C *floating_constant;
    ParseTreeNode_C *enumeration_constant;

    parser_c_parse_opt(parser, this_node, integer_constant, ret);

    parser_c_parse_opt(parser, this_node, character_constant, ret);

    parser_c_parse_opt(parser, this_node, floating_constant, ret);

    parser_c_parse_opt(parser, this_node, enumeration_constant, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_string(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRING, NULL);

    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_string = lexer_c_next_skip_whitespace(parser->lexer);

    if (token_string->type == T_STRING) {
        this_node->token = token_string;
    
        return this_node;
    }

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_string);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_integer_constant(Parser_C *parser)
{
    // TODO
    (void)parser;

    //assert(0 && "Not implemented parser_c_parse_integer_constant");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INTEGER_CONSTANT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_character_constant(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_character_constant");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CHARACTER_CONSTANT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_floating_constant(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_floating_constant");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_FLOATING_CONSTANT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enumeration_constant(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_enumeration_constant");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUMERATION_CONSTANT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_assignment_operator(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ASSIGNMENT_OPERATOR, NULL);

    const char* lexer_saved = parser->lexer->pbuf;

    Token_C *token_assignment_operator = lexer_c_next_skip_whitespace(parser->lexer);

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

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_assignment_operator);

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_abstract_declarator(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_abstract_declarator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ABSTRACT_DECLARATOR, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_list(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_parameter_list");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_LIST, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_declaration(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_parameter_declaration");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_DECLARATION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_direct_abstract_declarator(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_direct_abstract_declarator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DIRECT_ABSTRACT_DECLARATOR, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enumerator_list(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_enumerator_list");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUMERATOR_LIST, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_enumerator(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_enumerator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUMERATOR, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_init_declarator(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INIT_DECLARATOR, NULL);

    ParseTreeNode_C *declarator;
    ParseTreeNode_C *initializer;

    parser_c_parse_required(parser, this_node, declarator, error);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_ASSIGNMENT) == 0) {
        goto ret;
    }

    parser_c_parse_required(parser, this_node, initializer, error);

    ret: {
        return this_node;
    }

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_initializer(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_initializer");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INITIALIZER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_initializer_list(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_initializer_list");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INITIALIZER_LIST, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_statement(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STATEMENT, NULL);

    ParseTreeNode_C *labeled_statement;
    ParseTreeNode_C *expression_statement;
    ParseTreeNode_C *compound_statement;
    ParseTreeNode_C *selection_statement;
    ParseTreeNode_C *iteration_statement;
    ParseTreeNode_C *jump_statement;

    parser_c_parse_opt(parser, this_node, labeled_statement, ret);

    parser_c_parse_opt(parser, this_node, expression_statement, ret);

    parser_c_parse_opt(parser, this_node, compound_statement, ret);

    parser_c_parse_opt(parser, this_node, selection_statement, ret);

    parser_c_parse_opt(parser, this_node, iteration_statement, ret);

    parser_c_parse_opt(parser, this_node, jump_statement, ret);

    parse_tree_node_c_destroy(this_node);

    return NULL;

    ret: {
        return this_node;
    }
}

ParseTreeNode_C *parser_c_parse_labeled_statement(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_labeled_statement");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_LABELED_STATEMENT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_expression_statement(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_expression_statement");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXPRESSION_STATEMENT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_selection_statement(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_selection_statement");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SELECTION_STATEMENT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_iteration_statement(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_iteration_statement");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ITERATION_STATEMENT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_jump_statement(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_jump_statement");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_JUMP_STATEMENT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_control_line(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_control_line");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONTROL_LINE, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_macro_include(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_macro_include");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_MACRO_INCLUDE, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_macro_undef(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_macro_undef");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_MACRO_UNDEF, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}
