#include "token_c.h"

#include <stddef.h>

#ifndef JD297_CC_LEXER_C_H
#define JD297_CC_LEXER_C_H

typedef struct Lexer_Location_C {
    const char *pathname;
    size_t row;
    size_t col;
} Lexer_Location_C;

typedef struct Lexer_C {
    const char *buf;
    const char *pbuf;
    
    Lexer_Location_C loc;
} Lexer_C;

extern void *lexer_c_create(const char *src, const char* pathname);

extern void lexer_c_destroy(Lexer_C *lexer);

extern Token_C *lexer_c_next(Lexer_C *lexer);

extern Token_C *lexer_c_next_skip_whitespace(Lexer_C *lexer);

extern int lexer_c_next_skip_whitespace_token_is_type(Lexer_C *lexer, TokenType_C type);

extern void lexer_c_log(Lexer_C *lexer, const char *msg);

#endif
