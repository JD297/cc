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

ParseTreeNode_C *parser_c_parse_translation_unit(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TRANSLATION_UNIT, NULL);

    ParseTreeNode_C *external_declaration;
    ParseTreeNode_C *translation_unit;

    if ((external_declaration = parser_c_parse_external_declaration(parser)) == NULL) {
        goto error;
    }

    parse_tree_node_c_add(this_node, external_declaration);

    if ((translation_unit = parser_c_parse_translation_unit(parser)) != NULL) {
        parse_tree_node_c_add(this_node, translation_unit);
    }

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_external_declaration(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXTERNAL_DECLARATION, NULL);

    ParseTreeNode_C *function_declaration;
    ParseTreeNode_C *declaration;

    if ((function_declaration = parser_c_parse_function_definition(parser)) != NULL) {
        parse_tree_node_c_add(this_node, function_declaration);

        return this_node;
    }

    if ((declaration = parser_c_parse_declaration(parser)) != NULL) {
        parse_tree_node_c_add(this_node, declaration);

        return this_node;
    }

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_function_definition(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_function_definition");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_FUNCTION_DEFINITION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_function_definition");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declaration_specifier(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_declaration_specifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATION_SPECIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_declarator(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_declarator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DECLARATOR, NULL);

    goto error;

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

    while ((declaration = parser_c_parse_declaration(parser)) != NULL) {
        parse_tree_node_c_add(this_node, declaration);
    }

    while ((statement = parser_c_parse_statement(parser)) != NULL) {
        parse_tree_node_c_add(this_node, statement);
    }

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

    if ((struct_or_union_specifier = parser_c_parse_struct_or_union_specifier(parser)) != NULL) {
        parse_tree_node_c_add(this_node, struct_or_union_specifier);

        return this_node;
    }

    if ((enum_specifier = parser_c_parse_enum_specifier(parser)) != NULL) {
        parse_tree_node_c_add(this_node, enum_specifier);

        return this_node;
    }

    if ((typedef_name = parser_c_parse_typedef_name(parser)) != NULL) {
        parse_tree_node_c_add(this_node, typedef_name);

        return this_node;
    }

    parser->lexer->pbuf = lexer_saved;

    token_c_destroy(token_type_specifier);

    parse_tree_node_c_destroy(this_node);

    return NULL;
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

ParseTreeNode_C *parser_c_parse_enum_specifier(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_enum_specifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ENUM_SPECIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_typedef_name(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPEDEF_NAME, NULL);

    ParseTreeNode_C *identifier;

    if ((identifier = parser_c_parse_identifier(parser)) != NULL) {
        parse_tree_node_c_add(this_node, identifier);

        return this_node;
    }

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_struct_or_union(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_struct_or_union");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_struct_declaration");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_specifier_qualifier(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_specifier_qualifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SPECIFIER_QUALIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator_list(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_struct_declarator_list");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR_LIST, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_declarator(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_struct_declarator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_DECLARATOR, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_constant_expression(Parser_C *parser)
{
    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONSTANT_EXPRESSION, NULL);

    ParseTreeNode_C *conditional_expression;

    if ((conditional_expression = parser_c_parse_conditional_expression(parser)) != NULL) {
        parse_tree_node_c_add(this_node, conditional_expression);

        return this_node;
    }

    parse_tree_node_c_destroy(this_node);

    return NULL;
}

ParseTreeNode_C *parser_c_parse_pointer(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_pointer");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_POINTER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_direct_declarator(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_direct_declarator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_DIRECT_DECLARATOR, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_parameter_type_list(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_parameter_type_list");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PARAMETER_TYPE_LIST, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_conditional_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_conditional_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONDITIONAL_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_logical_or_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_logical_or_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_LOGICAL_OR_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_logical_and_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_LOGICAL_AND_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_inclusive_or_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_inclusive_or_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INCLUSIVE_OR_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_exclusive_or_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_exclusive_or_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EXCLUSIVE_OR_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_and_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_and_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_AND_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_equality_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_equality_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_EQUALITY_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_relational_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_relational_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_RELATIONAL_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_shift_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_shift_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_SHIFT_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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

    if ((identifier = parser_c_parse_identifier(parser)) != NULL) {
        parse_tree_node_c_add(this_node, identifier);

        return this_node;
    }

    if ((constant = parser_c_parse_constant(parser)) != NULL) {
        parse_tree_node_c_add(this_node, constant);

        return this_node;
    }

    if ((string = parser_c_parse_string(parser)) != NULL) {
        parse_tree_node_c_add(this_node, string);

        return this_node;
    }

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_OPEN_PARENT) == 0) {
        goto error;
    }

    if ((expression = parser_c_parse_expression(parser)) == NULL) {
        goto error;
    }

    parse_tree_node_c_add(this_node, expression);

    if (lexer_c_next_skip_whitespace_token_is_type(parser->lexer, T_CLOSING_PARENT) == 0) {
        goto error;
    }

    return this_node;

    error: {
        parser->lexer->pbuf = lexer_saved;

        parse_tree_node_c_destroy(this_node);

        return NULL;
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

    if ((integer_constant = parser_c_parse_integer_constant(parser)) != NULL) {
        parse_tree_node_c_add(this_node, integer_constant);

        return this_node;
    }

    if ((character_constant = parser_c_parse_character_constant(parser)) != NULL) {
        parse_tree_node_c_add(this_node, character_constant);

        return this_node;
    }

    if ((floating_constant = parser_c_parse_floating_constant(parser)) != NULL) {
        parse_tree_node_c_add(this_node, floating_constant);

        return this_node;
    }

    if ((enumeration_constant = parser_c_parse_enumeration_constant(parser)) != NULL) {
        parse_tree_node_c_add(this_node, enumeration_constant);

        return this_node;
    }

    parse_tree_node_c_destroy(this_node);

    return NULL;
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_init_declarator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_INIT_DECLARATOR, NULL);

    goto error;

    return this_node;

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

    if ((labeled_statement = parser_c_parse_labeled_statement(parser)) != NULL) {
        parse_tree_node_c_add(this_node, labeled_statement);

        return this_node;
    }

    if ((expression_statement = parser_c_parse_expression_statement(parser)) != NULL) {
        parse_tree_node_c_add(this_node, expression_statement);

        return this_node;
    }

    if ((compound_statement = parser_c_parse_compound_statement(parser)) != NULL) {
        parse_tree_node_c_add(this_node, compound_statement);

        return this_node;
    }

    if ((selection_statement = parser_c_parse_selection_statement(parser)) != NULL) {
        parse_tree_node_c_add(this_node, selection_statement);

        return this_node;
    }

    if ((iteration_statement = parser_c_parse_iteration_statement(parser)) != NULL) {
        parse_tree_node_c_add(this_node, iteration_statement);

        return this_node;
    }

    if ((jump_statement = parser_c_parse_jump_statement(parser)) != NULL) {
        parse_tree_node_c_add(this_node, jump_statement);

        return this_node;
    }

    parse_tree_node_c_destroy(this_node);

    return NULL;
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

int parser_c_parse(Parser_C *parser)
{
    ParseTreeNode_C *node;

    if ((node = parser_c_parse_translation_unit(parser)) != NULL) {
        parse_tree_node_c_add(parser->parse_tree, node);
        
        return 0;
    }

    return -1;
}
