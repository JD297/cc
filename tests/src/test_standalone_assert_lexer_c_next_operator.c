#include "lexer_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>

int main()
{
    Lexer_C *lexer;
    
    const char *src =   "obj.attr\n"
                        "obj->attr\n"
                        "obj ? attr : attr2\n"
                        "~value\n"
                        "a / b\n"
                        "a % b\n"
                        "a--\n"
                        "a < b\n"
                        "a > b\n"
                        "a != b\n"
                        "!a\n"
                        "a & b\n"
                        "a | b\n"
                        "a ^ b\n"
                        "a << b\n"
                        "a >> b\n"
                        "a += b\n"
                        "a -= b\n"
                        "a *= b\n"
                        "a /= b\n"
                        "a %= b\n"
                        "a &= b\n"
                        "a |= b\n"
                        "a ^= b\n"
                        "a >>= b\n"
                        "a <<= b\n";

    const Token_C src_tokens[] = {
        { .type = T_DOT                      , .value = "."   , .len = 1  },
        { .type = T_ARROW                    , .value = "->"  , .len = 2  },
        { .type = T_TERNARY                  , .value = "?"   , .len = 1  },
        { .type = T_COLON                    , .value = ":"   , .len = 1  },
        { .type = T_TILDE                    , .value = "~"   , .len = 1  },
        { .type = T_DIVIDE                   ,  .value = "/"  , .len = 1  },
        { .type = T_MODULUS                  , .value = "%"   , .len = 1  },
        { .type = T_DECREMENT                , .value = "--"  , .len = 2  },
        { .type = T_LESS_THAN                , .value = "<"   , .len = 1  },
        { .type = T_GREATER_THAN             , .value = ">"   , .len = 1  },
        { .type = T_NOT_EQUAL_TO             , .value = "!="  , .len = 2  },
        { .type = T_LOGICAL_NOT              , .value = "!"   , .len = 1  },
        { .type = T_BITWISE_AND              , .value = "&"   , .len = 1  },
        { .type = T_BITWISE_OR               , .value = "|"   , .len = 1  },
        { .type = T_BITWISE_XOR              , .value = "^"   , .len = 1  },
        { .type = T_BITWISE_LEFTSHIFT        , .value = "<<"  , .len = 2  },
        { .type = T_BITWISE_RIGHTSHIFT       , .value = ">>"  , .len = 2  },
        { .type = T_PLUS_ASSIGN              , .value = "+="  , .len = 2  },
        { .type = T_MINUS_ASSIGN             , .value = "-="  , .len = 2  },
        { .type = T_MULTIPLY_ASSIGN          , .value = "*="  , .len = 2  },
        { .type = T_DIVIDE_ASSIGN            , .value = "/="  , .len = 2  },
        { .type = T_MODULUS_ASSIGN           , .value = "%="  , .len = 2  },
        { .type = T_BITWISE_AND_ASSIGN       , .value = "&="  , .len = 2  },
        { .type = T_BITWISE_OR_ASSIGN        , .value = "|="  , .len = 2  },
        { .type = T_BITWISE_XOR_ASSIGN       , .value = "^="  , .len = 2  },
        { .type = T_BITWISE_RIGHTSHIFT_ASSIGN, .value = ">>=" , .len = 3  },
        { .type = T_BITWISE_LEFTSHIFT_ASSIGN , .value = "<<=" , .len = 3  },
        { .type = T_EOF                      , .value = "\0"  , .len = 0  },
    };
    
    assert(token_type_c_regex_create() == 0);
    
    lexer = lexer_c_create(src, "operator.c");

    assert(lexer != NULL);
    
    for (size_t i = 0; i < sizeof(src_tokens) / sizeof(Token_C); i++) {
        Token_C *token = lexer_c_next(lexer);

        assert(token != NULL);

        if (token->type == T_WHITESPACE || token->type == T_IDENTIFIER) {
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
