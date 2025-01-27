#include "lexer_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>

int main()
{
    Lexer_C *lexer;
    
    const char *src =   "unsigned long fib(unsigned short n)\n"
                        "{\n"
                        "\tif (n == 0) {\n"
                        "\t\treturn 0;\n"
                        "\t}\n"
                        "\n"
                        "\tif (n == 1 || n == 2) {\n"
                        "\t\treturn 1;\n"
                        "\t}\n"
                        "\n"
                        "\treturn fib(n-1) + fib(n-2);\n"
                        "}\n";

    const Token_C src_tokens[] = {
        { .type = T_UNSIGNED         , .value = "unsigned"                                           , .len = 8  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_LONG             , .value = "long"                                               , .len = 4  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "fib"                                                , .len = 3  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_UNSIGNED         , .value = "unsigned"                                           , .len = 8  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_SHORT            , .value = "short"                                              , .len = 5  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_OPEN_BRACE       , .value = "{"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_IF               , .value = "if"                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_EQUAL_TO         , .value = "=="                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "0"                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_OPEN_BRACE       , .value = "{"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_RETURN           , .value = "return"                                             , .len = 6  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "0"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_CLOSING_BRACE    , .value = "}"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_IF               , .value = "if"                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_EQUAL_TO         , .value = "=="                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "1"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_LOGICAL_OR       , .value = "||"                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_EQUAL_TO         , .value = "=="                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "2"                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_OPEN_BRACE       , .value = "{"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_RETURN           , .value = "return"                                             , .len = 6  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "1"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_CLOSING_BRACE    , .value = "}"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_RETURN           , .value = "return"                                             , .len = 6  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "fib"                                                , .len = 3  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_MINUS            , .value = "-"                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "1"                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_PLUS             , .value = "+"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "fib"                                                , .len = 3  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_MINUS            , .value = "-"                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "2"                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_CLOSING_BRACE    , .value = "}"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_EOF              , .value = "\0"                                                 , .len = 0  },
    };
    
    assert(token_type_c_regex_create() == 0);
    
    lexer = lexer_c_create(src, "fibonacci.c");

    assert(lexer != NULL);
    
    for (size_t i = 0; i < sizeof(src_tokens) / sizeof(Token_C); i++) {
        Token_C *token = lexer_c_next(lexer);

        assert(token != NULL);

        assert(token->type == src_tokens[i].type);
        assert(token->len == src_tokens[i].len);
        assert(strncmp(token->value, src_tokens[i].value, src_tokens[i].len) == 0);
        
        token_c_destroy(token);
    }

    lexer_c_destroy(lexer);
    
    token_type_c_regex_destroy();

    return 0;
}
