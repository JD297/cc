#include "lexer_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>

int main()
{
    Lexer_C *lexer;
    
    const char *src =   "#include <filename>\n"
                        "# define\n"
                        "#ifdef\n"
                        "#ifndef\n"
                        "#if __has_include\n"
                        "#elif defined\n"
                        "#else\n"
                        "#endif\n"
                        "#elifdef\n"
                        "#elifndef\n"
                        "#undef\n"
                        "__LINE__\n"
                        "__FILE__\n"
                        "__TIME__\n"
                        "#error\n"
                        "#pragma\n"
                        "asm\n";

    const Token_C src_tokens[] = {
        { .type = T_MACRO_INCLUDE    , .value = "#include <filename>"  , .len = 19 },
        { .type = T_MACRO_DEFINE     , .value = "# define"             , .len = 8  },
        { .type = T_MACRO_IFDEF      , .value = "#ifdef"               , .len = 6  },
        { .type = T_MACRO_IFNDEF     , .value = "#ifndef"              , .len = 7  },
        { .type = T_MACRO_IF         , .value = "#if"                  , .len = 3  },
        { .type = T_MACRO_HAS_INCLUDE, .value = "__has_include"        , .len = 13 },
        { .type = T_MACRO_ELIF       , .value = "#elif"                , .len = 5  },
        { .type = T_MACRO_DEFINDED   , .value = "defined"              , .len = 7  },
        { .type = T_MACRO_ELSE       , .value = "#else"                , .len = 5  },
        { .type = T_MACRO_ENDIF      , .value = "#endif"               , .len = 6  },
        { .type = T_MACRO_ELIFDEF    , .value = "#elifdef"             , .len = 8  },
        { .type = T_MACRO_ELIFNDEF   , .value = "#elifndef"            , .len = 9  },
        { .type = T_MACRO_UNDEF      , .value = "#undef"               , .len = 6  },
        { .type = T_MACRO_LINE       , .value = "__LINE__"             , .len = 8  },
        { .type = T_MACRO_FILE       , .value = "__FILE__"             , .len = 8  },
        { .type = T_MACRO_TIME       , .value = "__TIME__"             , .len = 8  },
        { .type = T_MACRO_ERROR      , .value = "#error"               , .len = 6  },
        { .type = T_MACRO_PRAGMA     , .value = "#pragma"              , .len = 7  },
        { .type = T_MACRO_ASM        , .value = "asm"                  , .len = 3  },
        { .type = T_EOF              , .value = "\0"                   , .len = 0  },
    };
    
    assert(token_type_c_regex_create() == 0);
    
    lexer = lexer_c_create(src, "macro.c");

    assert(lexer != NULL);
    
    for (size_t i = 0; i < sizeof(src_tokens) / sizeof(Token_C); i++) {
        Token_C *token = lexer_c_next(lexer);

        assert(token != NULL);
        
        if (token->type == T_WHITESPACE) {
             i--;
            continue;
        }

        assert(token->type == src_tokens[i].type);
        assert(token->len == src_tokens[i].len);
        assert(strncmp(token->value, src_tokens[i].value, src_tokens[i].len) == 0);
        
        token_c_destroy(token);
    }

    lexer_c_destroy(lexer);
    
    token_type_c_regex_destroy();

    return 0;
}
