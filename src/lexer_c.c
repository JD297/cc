#include "lexer_c.h"
#include "token_type_c.h"
#include "token_c.h"

#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *lexer_c_create(const char* src, const char* pathname)
{
    Lexer_C *lexer = (Lexer_C *)malloc(sizeof(Lexer_C));

    if (lexer == NULL) {
        return NULL;
    }

    lexer->buf = src;
    lexer->pbuf = lexer->buf;
    
    lexer->loc.pathname = pathname;
    lexer->loc.row = 1;
    lexer->loc.col = 1;

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

        Token_C* token = token_c_create(type, start, match.rm_eo);

        if (type == T_WHITESPACE && *start == '\n') {
            lexer->loc.row++;
            lexer->loc.col = 1;
        } else if (type == T_MACRO_LINE) {
            if (lexer_c_parse_line(lexer) == -1) {
                return NULL;
            }
        } else {
            lexer->loc.col += match.rm_eo;
        }

        return token;
    }

    lexer->pbuf += 1;

	return NULL;
}

Token_C *lexer_c_next_skip_whitespace(Lexer_C *lexer)
{
    // TODO GENERATE CONST LOCKUPTABLE
    int token_type_skipable[TOKEN_TYPE_C_LENGTH] = { 0 };
    token_type_skipable[T_WHITESPACE] = 1;
    token_type_skipable[T_COMMENT_LINE] = 1;
    token_type_skipable[T_COMMENT_MULTILINE] = 1;
    token_type_skipable[T_MACRO_LINE] = 1;

    Token_C *token;

    for (token = lexer_c_next(lexer); token != NULL && token_type_skipable[token->type]; token = lexer_c_next(lexer)) {
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

int lexer_c_parse_line(Lexer_C *lexer)
{
    Token_C *number = lexer_c_next_skip_whitespace(lexer);
 
    if (number == NULL || number->type != T_NUMBER) {
        lexer_c_log(lexer, "after #line is not a positive integer");
        
        return -1;
    }

    lexer->loc.col = 1;

    char *row_str = malloc(sizeof(char) * (number->len + 1));
    strncpy(row_str, number->value, number->len);

    lexer->loc.row = (size_t)atoi(row_str);

    Lexer_C lexer_saved = *lexer;

    Token_C *filename = lexer_c_next_skip_whitespace(lexer);

    if (filename == NULL || filename->type != T_STRING) {
        *lexer = lexer_saved;
        
        return 0;
    }

    char *filename_str = malloc(sizeof(char) * (filename->len + 1));
    strncpy(filename_str, filename->value + 1, filename->len - 2);

    lexer->loc.pathname = filename_str;

    return 0;
}

void lexer_c_log(Lexer_C *lexer, const char *msg)
{
    fprintf(stderr, "%s:%zu:%zu: \033[31merror\033[0m: %s\n", lexer->loc.pathname, lexer->loc.row, lexer->loc.col, msg);
    fprintf(stderr, "    %zu | ", lexer->loc.row);
    
    const char *begin = lexer->pbuf - (lexer->loc.col - 1);
    const char *end = strstr(lexer->pbuf, "\n");

    fprintf(stderr, "%.*s\n", (int)(end - begin), begin);
    
    fprintf(stderr, "      | ");

    for (size_t i = 0; i < lexer->loc.col; i++) {
        fprintf(stderr, " ");
    }

    fprintf(stderr, "\033[31m^\033[0m\n");
}
