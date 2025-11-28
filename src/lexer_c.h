#include "token_c.h"

#include <jd297/lmap_sv.h>

#include <stddef.h>

#ifndef JD297_CC_LEXER_C_H
#define JD297_CC_LEXER_C_H

typedef struct Lexer_Location_C {
    const char *pathname; // TODO change to sv_t ??
    size_t line;
    size_t col;
} Lexer_Location_C;

typedef struct Lexer_C {
    const char *start;
    const char *current;
    Lexer_Location_C loc; // TODO add loc_old / loc_start for logging
} Lexer_C;

extern lmap_sv_t lexer_c_lookup_keywords;

extern int lexer_c_init_lookup_keywords(void);

// extern int lexer_c_destroy_lookup_keywords(void); // TODO

extern void lexer_c_create(Lexer_C *lexer, const char *pathname, const char *source);

extern int lexer_c_next(Lexer_C *lexer, Token_C *token);

#endif
