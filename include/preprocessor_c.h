#ifndef JD297_CC_PREPROCESSOR_C_H
#define JD297_CC_PREPROCESSOR_C_H

#include "lexer_c.h"
#include "map.h"
#include "vector.h"

#include <stddef.h>
#include <stdio.h>

typedef struct Preprocessor_C {
    FILE *output;

    Vector *include_dirs;
    Vector *source_files;
    Map *defines;
} Preprocessor_C;

#define PREPROCESSOR_INCLUDE_MODE_LIBRARARY 0x0
#define PREPROCESSOR_INCLUDE_MODE_STRING    0x1

extern void *preprocessor_c_create(Vector *include_dirs, Vector *source_files, Map *defines);

extern void preprocessor_c_destroy(Preprocessor_C *preprocessor);

extern int preprocessor_c_run(Preprocessor_C *preprocessor);

extern int preprocessor_c_parse_file(Preprocessor_C *preprocessor, const char* pathname);
extern int preprocessor_c_parse_lexer(Preprocessor_C *preprocessor, Lexer_C *lexer, const char* end);

extern int preprocessor_c_parse_next(Preprocessor_C *preprocessor, Lexer_C *lexer);

extern int preprocessor_c_parse_default(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_include(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_identifier(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_define(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_undef(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_conditional(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_line(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_error(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);
extern int preprocessor_c_parse_pragma(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token);

#endif
