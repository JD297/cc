#include "token_c.h"

#include <jd297/sv.h>
#include <jd297/lmap_sv.h>

#include <stddef.h>

#ifndef JD297_CC_LEXER_C_H
#define JD297_CC_LEXER_C_H

typedef enum {
	LEXER_MODE_NORMAL,
	LEXER_MODE_PREPROCESSOR
} Lexer_Mode_C;

typedef struct Lexer_Location_C {
    sv_t pathname;
    size_t line;
    size_t col;
} Lexer_Location_C;

typedef struct Lexer_C {
    const char *start;
    const char *current;

    Lexer_Location_C loc; // TODO add loc_old / loc_start for logging

    Lexer_Mode_C mode;
} Lexer_C;

extern lmap_sv_t lexer_c_lookup_keywords;

extern int lexer_c_init_lookup_keywords(void);

// extern int lexer_c_destroy_lookup_keywords(void); // TODO

extern void lexer_c_create(Lexer_C *lexer, sv_t pathname, const char *source, Lexer_Mode_C mode);

extern TokenType_C lexer_c_next(Lexer_C *lexer, Token_C *token);

#endif
