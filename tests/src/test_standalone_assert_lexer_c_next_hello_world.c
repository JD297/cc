#include "lexer_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>

int main()
{
    Lexer_C *lexer;
    
    const char *src =   "#include <stdio.h>\n"
                        "\n"
                        "int main()\n"
                        "{\n"
                        "\t// This is a comment!\n"
                        "\t/*\n"
                        "\t * This is a multi line\n"
                        "\t * comment!\n"
                        "\t */\n"
                        "\tprintf(\"Hello world!\\n\");\n"
                        "\n"
                        "\treturn 0;\n"
                        "}\n";

    const Token_C src_tokens[] = {
        { .type = T_MACRO_INCLUDE    , .value = "#include <stdio.h>"                                 , .len = 18 },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_INT              , .value = "int"                                                , .len = 3  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "main"                                               , .len = 4  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_OPEN_BRACE       , .value = "{"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_COMMENT_LINE     , .value = "// This is a comment!"                              , .len = 21 },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_COMMENT_MULTILINE, .value = "/*\n\t * This is a multi line\n\t * comment!\n\t */", .len = 45 },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_IDENTIFIER       , .value = "printf"                                             , .len = 6  },
        { .type = T_OPEN_PARENT      , .value = "("                                                  , .len = 1  },
        { .type = T_STRING           , .value = "\"Hello world!\\n\""                                , .len = 16 },
        { .type = T_CLOSING_PARENT   , .value = ")"                                                  , .len = 1  },
        { .type = T_SEMICOLON        , .value = ";"                                                  , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\n"                                                 , .len = 1  },
        { .type = T_WHITESPACE       , .value = "\t"                                                 , .len = 1  },
        { .type = T_RETURN           , .value = "return"                                             , .len = 6  },
        { .type = T_WHITESPACE       , .value = " "                                                  , .len = 1  },
        { .type = T_NUMBER           , .value = "0"                                                  , .len = 1  },
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
