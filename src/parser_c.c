#include "lexer_c.h"
#include "parser_c.h"
#include "parse_tree_node_c.h"
#include "parse_tree_type_c.h"

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

ParseTreeNode_C *parser_c_parse_constant(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;
    
    Token_C *current_token = lexer_c_next_skip_whitespace(parser->lexer);
    
    if (current_token == NULL) {
        goto error;
    }

    if (current_token->type == T_NUMBER) {
        return parse_tree_node_c_create(PTTC_PRIMARY_EXPRESSION, current_token);
    }

    // TODO ...

    error: {
        parser->lexer->pbuf = pbuf_backup;

        token_c_destroy(current_token);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_primary_expression(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;

    ParseTreeNode_C *primary_expression;

    if ((primary_expression = parser_c_parse_constant(parser)) != NULL) {
        return primary_expression;
    }
    
    // TODO ...

    parser->lexer->pbuf = pbuf_backup;

    return NULL;
}

ParseTreeNode_C *parser_c_parse_expression(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;

    ParseTreeNode_C *expression;

    // TODO this is not the correct definition but it is easier to implement right now
    //      it can be correct when we can generate this kind of function patterns form the bnf grammer file.
    if ((expression = parser_c_parse_primary_expression(parser)) != NULL) {
        return expression;
    }

    parser->lexer->pbuf = pbuf_backup;

    return NULL;
}

ParseTreeNode_C *parser_c_parse_jump_statement(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;

    ParseTreeNode_C *jump_statement = parse_tree_node_c_create(PTTC_JUMP_STATEMENT, NULL);
    
    Token_C *current_token = lexer_c_next_skip_whitespace(parser->lexer);

    if (current_token == NULL) {
        goto error;
    }

    if (current_token->type == T_RETURN) {
        jump_statement->token = current_token;

        {
            ParseTreeNode_C *expression;

            if ((expression = parser_c_parse_expression(parser)) == NULL) {
                goto error;
            }

            parse_tree_node_c_add(jump_statement, expression);
        }

        if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_SEMICOLON) == 0) {
            goto error;
        }

        return jump_statement;
    }

    // TODO ...

    error: {
        parse_tree_node_c_destroy(jump_statement);

        token_c_destroy(current_token);
        
        parser->lexer->pbuf = pbuf_backup;

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_statement(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;

    ParseTreeNode_C *statement;

    if ((statement = parser_c_parse_jump_statement(parser)) != NULL) {
        return statement;
    }
    
    // TODO ...

    parser->lexer->pbuf = pbuf_backup;

    return NULL;
}

ParseTreeNode_C *parser_c_parse_statement_list(Parser_C *parser)
{
    ParseTreeNode_C *statement_list = parse_tree_node_c_create(PTTC_STATEMENT, NULL);

    ParseTreeNode_C *statement;

    while ((statement = parser_c_parse_statement(parser)) != NULL) {
        parse_tree_node_c_add(statement_list, statement);
    }
    
    if (statement_list->num == 0) {
        goto error;
    }

    return statement_list;
    
    error: {
        parse_tree_node_c_destroy(statement_list);
        
        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration_specifier(Parser_C *parser)
{
    (void)parser;

    return NULL; // TODO
}

ParseTreeNode_C *parser_c_parse_pointer(Parser_C *parser)
{
    (void)parser;

    return NULL; // TODO
}

ParseTreeNode_C *parser_c_direct_declarator(Parser_C *parser)
{
    ParseTreeNode_C *direct_declarator;

    const char *pbuf_backup = parser->lexer->pbuf;
    
    Token_C *current_token;

    {
        if ((current_token = lexer_c_next_skip_whitespace(parser->lexer)) == NULL || current_token->type != T_IDENTIFIER) {
            goto error;
        }
        
        direct_declarator = parse_tree_node_c_create(PTTC_DIRECT_DECLARATOR, current_token);
        
        {
            if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_PARENT)) {
                // TODO ...
                
                if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_PARENT) == 0) {
                    goto error;
                }
            }
        }
    }

    return direct_declarator;
    
    error: {
        parse_tree_node_c_destroy(direct_declarator);

        token_c_destroy(current_token);

        parser->lexer->pbuf = pbuf_backup;

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declarator(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;

    ParseTreeNode_C *declarator = parse_tree_node_c_create(PTTC_DECLARATOR, NULL);

    {
        ParseTreeNode_C *pointer;

        if ((pointer = parser_c_parse_pointer(parser)) != NULL) { // opt
            parse_tree_node_c_add(declarator, pointer);
        }
    }

    {
        ParseTreeNode_C *direct_declarator;

        if ((direct_declarator = parser_c_direct_declarator(parser)) == NULL) {
            goto error;
        }

        parse_tree_node_c_add(declarator, direct_declarator);
    }

    return declarator;

    error: {
        parse_tree_node_c_destroy(declarator);

        parser->lexer->pbuf = pbuf_backup;

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration_list(Parser_C *parser)
{
    (void)parser;

    return NULL; // TODO
}

ParseTreeNode_C *parser_c_parse_compound_statement(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;

    ParseTreeNode_C *compound_statement = parse_tree_node_c_create(PTTC_COMPOUND_STATEMENT, NULL);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_BRACE) == 0) {
        goto error;
    }

    {
        ParseTreeNode_C *declaration_list;

        if ((declaration_list = parser_c_parse_declaration_list(parser)) != NULL) { // opt
            parse_tree_node_c_add(compound_statement, declaration_list);
        }
    }
    
    {
        ParseTreeNode_C *statement_list;
    
        if ((statement_list = parser_c_parse_statement_list(parser)) != NULL) { // opt
            parse_tree_node_c_add(compound_statement, statement_list);
        }
    }

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_BRACE) == 0) {
        goto error;
    }

    return compound_statement;
    
    error: {
        parse_tree_node_c_destroy(compound_statement);
    
        parser->lexer->pbuf = pbuf_backup;
    
        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_function_definition(Parser_C *parser)
{
    const char *pbuf_backup = parser->lexer->pbuf;

    ParseTreeNode_C *function_definition = parse_tree_node_c_create(PTTC_FUNCTION_DEFINITION, NULL);

    {
        ParseTreeNode_C *declaration_specifier;

        if ((declaration_specifier = parser_c_parse_declaration_specifier(parser)) != NULL) { // opt
            parse_tree_node_c_add(function_definition, declaration_specifier);
        }
    }
    
    {
        ParseTreeNode_C *declarator;
        
        if ((declarator = parser_c_parse_declarator(parser)) == NULL) {
            goto error;
        }
        
        parse_tree_node_c_add(function_definition, declarator);
    }
    
    {
        ParseTreeNode_C *declaration_list;

        if ((declaration_list = parser_c_parse_declaration_list(parser)) != NULL) { // opt
            parse_tree_node_c_add(function_definition, declaration_list);
        }
    }
    
    {
        ParseTreeNode_C *compound_statement;

        if ((compound_statement = parser_c_parse_compound_statement(parser)) == NULL) {
            goto error;
        }
        
        parse_tree_node_c_add(function_definition, compound_statement);
    }

    return function_definition;
    
    error: {
        parse_tree_node_c_destroy(function_definition);

        parser->lexer->pbuf = pbuf_backup;

        return NULL;
    }
}

int parser_c_parse(Parser_C *parser)
{
    ParseTreeNode_C *node;

    if ((node = parser_c_parse_function_definition(parser)) != NULL) {
        parse_tree_node_c_add(parser->parse_tree, node);
    
        return 0;
    }
    
    parse_tree_node_c_destroy(node);

    return -1;
}
