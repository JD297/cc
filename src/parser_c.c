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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_translation_unit");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TRANSLATION_UNIT, NULL);

    goto error;

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

    assert(0 && "Not implemented parser_c_parse_declaration");

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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_compound_statement");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_COMPOUND_STATEMENT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_storage_class_specifier(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_storage_class_specifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STORAGE_CLASS_SPECIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_type_specifier(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_type_specifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_SPECIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_type_qualifier(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_type_qualifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_TYPE_QUALIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_struct_or_union_specifier(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_struct_or_union_specifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRUCT_OR_UNION_SPECIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_identifier");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_IDENTIFIER, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_constant_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_CONSTANT_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_unary_operator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_UNARY_OPERATOR, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
}

ParseTreeNode_C *parser_c_parse_primary_expression(Parser_C *parser)
{
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_primary_expression");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_PRIMARY_EXPRESSION, NULL);

    goto error;

    return this_node;

    error: {
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_string");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STRING, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_assignment_operator");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_ASSIGNMENT_OPERATOR, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
    }
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
    // TODO
    (void)parser;

    assert(0 && "Not implemented parser_c_parse_statement");

    ParseTreeNode_C *this_node = parse_tree_node_c_create(PTT_C_STATEMENT, NULL);

    goto error;

    return this_node;

    error: {
        parse_tree_node_c_destroy(this_node);

        return NULL;
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

int parser_c_parse(Parser_C *parser)
{
    ParseTreeNode_C *node;

    if ((node = parser_c_parse_translation_unit(parser)) != NULL) {
        parse_tree_node_c_add(parser->parse_tree, node);
        
        return 0;
    }

    return -1;
}
