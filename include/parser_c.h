#include "lexer_c.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

typedef struct Parser_C {
    Lexer_C *lexer;

    ParseTreeNode_C *parse_tree;

    size_t expected_token_types[TOKEN_TYPE_C_LENGTH];

    size_t num;
} Parser_C;


extern void *parser_c_create(const char *src);

extern void parser_c_destroy(Parser_C *parser);

extern int parser_c_parse(Parser_C *parser);
