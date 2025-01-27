#include "lexer_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
int main()
{
    Lexer_C *lexer;
    
    const char *src =   "a $ b\n"
                        "a $ b\n";

    const Token_C src_tokens[] = {
        { .type = T_EOF                      , .value = "\0"  , .len = 0  },
    };
    
    assert(token_type_c_regex_create() == 0);
    
    lexer = lexer_c_create(src, "unexpected.c");

    assert(lexer != NULL);
    
    int count = 2;
    
    for (size_t i = 0; i < sizeof(src_tokens) / sizeof(Token_C); i++) {
        assert(count >= 0);
    
        Token_C *token = lexer_c_next(lexer);

        if (token == NULL) {
            i--;
            count -= 1;

            
            continue;
        }

        if (token->type == T_WHITESPACE || token->type == T_IDENTIFIER) {
            i--;
            continue;        
        }

        assert(count == 0);

        assert(token->type == src_tokens[i].type);
        assert(token->len == src_tokens[i].len);
        assert(strncmp(token->value, src_tokens[i].value, src_tokens[i].len) == 0);
        
        token_c_destroy(token);
    }

    lexer_c_destroy(lexer);
    
    token_type_c_regex_destroy();

    return 0;
}
