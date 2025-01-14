#include "lexer_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>

#include <stdio.h>

int main()
{
    Lexer_C *lexer;
    
    const char *src =   "int atoi(char s[])\n"
                        "{\n"
                        "\tint i, n;\n"
                        "\n"
                        "\tn = 0;\n"
                        "\tfor (i = 0; s[i] >= \'0\' && s[i] <= \'9\'; ++i)\n"
                        "\t\tn = 10 * n + (s[i] - \'0\');\n"
                        "\treturn n;\n"
                        "}\n";

    const Token_C src_tokens[] = {
        { .type = T_INT              , .value = "int"                                                , .len = 3  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "atoi"                                               , .len = 4  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_CHAR             , .value = "char"                                               , .len = 4  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "s"                                                  , .len = 1  },
        { .type = T_OPEN_BRACKET     , .value = "["                                                  , .len = 1  },
        { .type = T_CLOSING_BRACKET  , .value = "]"                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_OPEN_BRACE       , .value = "{"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_INT              , .value = "int"                                                , .len = 3  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "i"                                                  , .len = 1  },
        { .type = T_COMMA            , .value = ","                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_ASSIGNMENT       , .value = "="                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "0"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_FOR              , .value = "for"                                                , .len = 3  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "i"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_ASSIGNMENT       , .value = "="                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "0"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "s"                                                  , .len = 1  },
        { .type = T_OPEN_BRACKET     , .value = "["                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "i"                                                  , .len = 1  },
        { .type = T_CLOSING_BRACKET  , .value = "]"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_GREATER_THAN_OR_EQUAL_TO, .value = ">="                                          , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_CHARACTER        , .value = "\'0\'"                                              , .len = 3  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_LOGICAL_AND      , .value = "&&"                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "s"                                                  , .len = 1  },
        { .type = T_OPEN_BRACKET     , .value = "["                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "i"                                                  , .len = 1  },
        { .type = T_CLOSING_BRACKET  , .value = "]"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_LESS_THAN_OR_EQUAL_TO, .value = "<="                                             , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_CHARACTER        , .value = "\'9\'"                                              , .len = 3  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_INCREMENT        , .value = "++"                                                 , .len = 2  },
        { .type = T_IDENTIFIER       , .value = "i"                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_ASSIGNMENT       , .value = "="                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "10"                                                 , .len = 2  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_MULTIPLY         , .value = "*"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_PLUS             , .value = "+"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "s"                                                  , .len = 1  },
        { .type = T_OPEN_BRACKET     , .value = "["                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "i"                                                  , .len = 1  },
        { .type = T_CLOSING_BRACKET  , .value = "]"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_MINUS            , .value = "-"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_CHARACTER        , .value = "\'0\'"                                              , .len = 3  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_RETURN           , .value = "return"                                             , .len = 6  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "n"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_CLOSING_BRACE    , .value = "}"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_EOF              , .value = "\0"                                                 , .len = 0  },
    };
    
    assert(token_type_c_regex_create() == 0);
    
    lexer = lexer_c_create(src);

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
