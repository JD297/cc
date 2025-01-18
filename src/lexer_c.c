#include "lexer_c.h"
#include "token_type_c.h"
#include "token_c.h"

#include <regex.h>
#include <stddef.h>
#include <stdlib.h>

void *lexer_c_create(const char* src)
{
    Lexer_C *lexer = (Lexer_C *)malloc(sizeof(Lexer_C));

    if (lexer == NULL) {
        return NULL;
    }

    lexer->buf = src;
    lexer->pbuf = lexer->buf;

    return lexer;
}

void lexer_c_destroy(Lexer_C *lexer)
{
    free(lexer);
}

Token_C *lexer_c_next(Lexer_C *lexer)
{
    for (TokenType_C type = 0; type < TOKEN_TYPE_C_LENGTH; type++) {
        if (type == T_EOF && *(lexer->pbuf) != '\0') {
            continue;
        }

        regmatch_t match;

        if (regexec(token_type_c_regex[type], lexer->pbuf, 1, &match, 0) != 0) {
            continue;
        }

        const char* start = lexer->pbuf;

        lexer->pbuf += match.rm_eo;

        return token_c_create(type, start, match.rm_eo);
    }

    lexer->pbuf += 1;

	return NULL;
}

Token_C *lexer_c_next_skip_whitespace(Lexer_C *lexer)
{
    Token_C *token;

    for (token = lexer_c_next(lexer); token != NULL && token->type == T_WHITESPACE; token = lexer_c_next(lexer)) {
        token_c_destroy(token);
    }

    return token;
}

int lexer_c_next_skip_whitespace_token_is_type(Lexer_C *lexer, TokenType_C type)
{
    Token_C *token = lexer_c_next_skip_whitespace(lexer);

    if (token == NULL) {
        return 0;
    }

    int result = token->type == type;

    token_c_destroy(token);

    return result;
}
