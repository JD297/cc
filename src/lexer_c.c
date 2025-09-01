#include "lexer_c.h"
#include "token_type_c.h"
#include "token_c.h"
#include "jd297/logger.h"

#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lexer_c_next(Lexer_C *lexer, Token_C *token)
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

        if (token != NULL) {
            token->type = type;
            token->value = start;
            token->len = match.rm_eo;
        }

        if (type == T_WHITESPACE && *start == '\n') {
            lexer->loc.row++;
            lexer->loc.col = 1;
        } else if (type == T_MACRO_LINE) {
            if (lexer_c_parse_line(lexer) == -1) {
                return -1;
            }

            return lexer_c_next(lexer, token);
        } else {
            lexer->loc.col += match.rm_eo;
        }

        return 0;
    }

    // lexer->pbuf += 1; // ?? this should prevent to progress on failures

	return -1;
}

int lexer_c_next_skip_whitespace(Lexer_C *lexer, Token_C *token)
{
    do {
        if (lexer_c_next(lexer, token) == -1) {
            return -1;
        }
    } while (token_type_skipable_lookup[token->type] == 1);

    return 0;
}

int lexer_c_next_skip_whitespace_token_is_type(Lexer_C *lexer, Token_C *token, TokenType_C type)
{
    Token_C token_stack;
    
    if (token == NULL) {
        token = &token_stack;
    }

    if (lexer_c_next_skip_whitespace(lexer, token) == -1) {
        return 0;
    }

    return token->type == type;
}

int lexer_c_parse_line(Lexer_C *lexer)
{
    Token_C token_number;
 
    if (lexer_c_next_skip_whitespace_token_is_type(lexer, &token_number, T_NUMBER) == 0) {
        lexer_c_log_at(L_ERROR, lexer, &token_number, "after #line is not a positive integer");

        return -1;
    }

    lexer->loc.col = 1;

    char *row_str = malloc(sizeof(char) * (token_number.len + 1));
    strncpy(row_str, token_number.value, token_number.len);

    lexer->loc.row = (size_t)atoi(row_str);

    Lexer_C lexer_saved = *lexer;

    Token_C token_filename;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, &token_filename, T_STRING) == 0) {
        *lexer = lexer_saved;
        
        return 0;
    }

    char *token_filename_str = malloc(sizeof(char) * (token_filename.len + 1));
    strncpy(token_filename_str, token_filename.value + 1, token_filename.len - 2);
    token_filename_str[token_filename.len - 2] = '\0';

    lexer->loc.pathname = token_filename_str;

    return 0;
}

void lexer_c_log_at(int level, Lexer_C *lexer, Token_C *token, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	flog_at(stderr, level, lexer->loc.pathname, lexer->loc.row, lexer->loc.col, format, ap);

	va_end(ap);

	const char *lbegin = lexer->pbuf - lexer->loc.col + 1;
	const char *lend = strstr(lexer->pbuf, "\n");

	if (lend == NULL) {
		flog_line(stderr, lexer->loc.row, "%s", lbegin);
	} else {
		flog_line(stderr, lexer->loc.row, "%.*s", (int)(lend - lbegin), lbegin);
	}

	flog_ptr(stderr, lbegin, token->value, token->len);
}
