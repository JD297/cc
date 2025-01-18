#include "token_c.h"

#ifndef JD297_CC_LEXER_C_H
#define JD297_CC_LEXER_C_H

typedef struct Lexer_C {
    const char *buf;
    const char *pbuf;
} Lexer_C;

extern void *lexer_c_create(const char *src);

extern void lexer_c_destroy(Lexer_C *lexer);

extern Token_C *lexer_c_next(Lexer_C *lexer);

extern Token_C *lexer_c_next_skip_whitespace(Lexer_C *lexer);

extern int lexer_c_next_skip_whitespace_token_is_type(Lexer_C *lexer, TokenType_C type);

#endif
