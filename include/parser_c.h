#include "lexer_c.h"
#include "parse_tree_node_c.h"
#include "token_c.h"

typedef struct Parser_C {
    Lexer_C *lexer;
    Token_C *current_token;
    ParseTreeNode_C *parse_tree;
} Parser_C;

extern int parser_c_parse(Parser_C *parser);
