//#include "lexer_c.h"
#include "parser_c.h"
#include "parse_tree_node_c.h"
#include "parse_tree_type_c.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

int main(void)
{
    /*
    PTTC_FUNCTION_DEFINITION:
        PTTC_DECLARATOR
            PTTC_DIRECT_DECLARATOR(T_IDENTIFIER: "_start"
        PTTC_COMPOUND_STATEMENT
            PTTC_STATEMENT
                PTTC_JUMP_STATEMENT(T_RETURN)
                    PTTC_PRIMARY_EXPRESSION(T_NUMBER: "0")
    */
    const char *src = "_start()\n"
                      "{\n"
                      "    return 0;\n"
                      "}\n";

    Parser_C *parser = parser_c_create(src);

    assert(token_type_c_regex_create() == 0);

    assert(parser != NULL);

    assert(parser_c_parse(parser) != -1);

    assert(parser->parse_tree->elements != NULL);
    assert(parser->parse_tree->num == 1);
    assert(parser->parse_tree->elements[0]->type == PTTC_FUNCTION_DEFINITION);
    
    assert(parser->parse_tree->elements[0]->elements[0]->type == PTTC_DECLARATOR);

    assert(parser->parse_tree->elements[0]->elements[0]->elements[0]->type == PTTC_DIRECT_DECLARATOR);
    assert(parser->parse_tree->elements[0]->elements[0]->elements[0]->token->type == T_IDENTIFIER);
    assert(strncmp(parser->parse_tree->elements[0]->elements[0]->elements[0]->token->value, "_start", 6) == 0);
    
    assert(parser->parse_tree->elements[0]->elements[1]->type == PTTC_COMPOUND_STATEMENT);
    assert(parser->parse_tree->elements[0]->elements[1]->elements[0]->type == PTTC_STATEMENT);
    assert(parser->parse_tree->elements[0]->elements[1]->elements[0]->elements[0]->type == PTTC_JUMP_STATEMENT);
    assert(parser->parse_tree->elements[0]->elements[1]->elements[0]->elements[0]->token->type == T_RETURN);
    assert(parser->parse_tree->elements[0]->elements[1]->elements[0]->elements[0]->elements[0]->type == PTTC_PRIMARY_EXPRESSION);
    assert(parser->parse_tree->elements[0]->elements[1]->elements[0]->elements[0]->elements[0]->token->type == T_NUMBER);
    assert(strncmp(parser->parse_tree->elements[0]->elements[1]->elements[0]->elements[0]->elements[0]->token->value, "0", 1) == 0);

    parser_c_destroy(parser);

    token_type_c_regex_destroy();

    return 0;
}
