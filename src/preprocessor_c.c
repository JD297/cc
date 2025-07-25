#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <jd297/lmap.h>
#include <jd297/vector.h>
#include <jd297/logger.h>

#include "lexer_c.h"
#include "parse_tree_type_c.h"
#include "parser_c.h"
#include "preprocessor_c.h"
#include "token_c.h"
#include "token_type_c.h"

#define LOG_AT(level, lexer, token, format, ...)\
    flog_at(stderr, (level), (lexer)->loc.pathname, (lexer)->loc.row, (lexer)->loc.col, (format), ##__VA_ARGS__);\
    \
    const char *lbegin = (lexer)->pbuf - (lexer)->loc.col + 1;\
    const char *lend = strstr((lexer)->pbuf, "\n");\
    \
    if (lend == NULL) {\
        flog_line(stderr, (lexer)->loc.row, "%s", lbegin);\
    } else {\
        flog_line(stderr, (lexer)->loc.row, "%.*s", (int)(lend - lbegin), lbegin);\
    }\
    \
    flog_ptr(stderr, lbegin, (token)->value, (token)->len);

int preprocessor_c_run(Preprocessor_C *preprocessor)
{
    for (char **it = (char **)vec_begin(preprocessor->source_files); it < (char **)vec_end(preprocessor->source_files); ++it) {
        if (preprocessor_c_parse_file(preprocessor, *it) == -1) {
            return -1;
        }
    }

    return 0;
}

int preprocessor_c_parse_file(Preprocessor_C *preprocessor, const char* pathname)
{
    FILE *file;
    char *src = NULL;

    if ((file = fopen(pathname, "r")) == NULL) {
        return -1;
    }

    const size_t block = 4096;
    size_t nread_total = 0;

    do {
        src = (char *)realloc(src, nread_total + block);

        nread_total += fread(src + nread_total, sizeof(char), block, file);
    } while (feof(file) == 0);

    fclose(file);

    Lexer_C lexer = {
        .buf = src,
        .pbuf = src,
        .loc = {
            .pathname = pathname,
            .row = 1,
            .col = 1
        }
    };

    fprintf(preprocessor->output, "#line 0 \"%s\"\n", pathname);

    int parse_next_result;

    parse_next_result = preprocessor_c_parse_lexer(preprocessor, &lexer, src + strlen(src));

    free(src);

    return parse_next_result;
}

int preprocessor_c_parse_lexer(Preprocessor_C *preprocessor, Lexer_C *lexer, const char* end)
{
    int parse_next_result;

    while(lexer->pbuf != end && (parse_next_result = preprocessor_c_parse_next(preprocessor, lexer)) == 0);

    return parse_next_result;
}

int preprocessor_c_parse_next(Preprocessor_C *preprocessor, Lexer_C *lexer)
{
    Lexer_C lexer_saved_begin = *lexer;

    Token_C token;

    if (lexer_c_next(lexer, &token) == -1) {
        fprintf(preprocessor->output, "%c", *lexer->pbuf);
        lexer->pbuf += 1;

        return 0;
    }

    switch (token.type) {
        case T_MACRO_INCLUDE: {
            return preprocessor_c_parse_include(preprocessor, lexer, &token);
        }
        case T_IDENTIFIER: {
            return preprocessor_c_parse_identifier(preprocessor, lexer, &token);
        }
        case T_MACRO_DEFINE: {
            return preprocessor_c_parse_define(preprocessor, lexer, &token);
        }
        case T_MACRO_UNDEF: {
            return preprocessor_c_parse_undef(preprocessor, lexer, &token);
        }
        case T_MACRO_IF:
        case T_MACRO_IFDEF:
        case T_MACRO_IFNDEF: {
            *lexer = lexer_saved_begin;

            return preprocessor_c_parse_conditional(preprocessor, lexer, &token);
        }
        case T_MACRO_ERROR: {
            return preprocessor_c_parse_error(preprocessor, lexer, &token);
        }
        case T_COMMENT_LINE:
        case T_COMMENT_MULTILINE: return 0;

        case T_MACRO_ELIFDEF:
        case T_MACRO_ELIFNDEF:
        case T_MACRO_ELIF:
        case T_MACRO_ELSE:
        case T_MACRO_ENDIF: {
            LOG_AT(L_ERROR, lexer, &token, "%.*s without #if", (int)(token.len), token.value);

            return -1;
        }
        case T_EOF: {
            return 1;
        }
        default: {
            return preprocessor_c_parse_default(preprocessor, lexer, &token);
        }
    }
    
    return -1;
}

int preprocessor_c_parse_default(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)lexer;

    fprintf(preprocessor->output, "%.*s", (int)token->len, token->value);

    return 0;
}

int preprocessor_c_find_include_file(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *include_file_token, char *include_file_path/*[PATH_MAX]*/)
{
    const char *pathname = include_file_token->value;
    const size_t pathname_len = include_file_token->len - 2; // to get the len between "..." or <...>

    const int mode = (*pathname == '"');

    if (pathname_len == 0) {
        LOG_AT(L_ERROR, lexer, include_file_token, "empty filename in #include");

        return -1;
    }

    ++pathname; // this will be after the < or "

    for (int i = 0 - mode; i < (int)vec_size(preprocessor->include_dirs); ++i) {
        const char *include_dir;
        
        if (i >= 0) {
            include_dir = vec_at(preprocessor->include_dirs, i);
        } else {
            include_dir = ".";
        }
    
        if ((strlen(include_dir) + pathname_len + 1) >= PATH_MAX) { // 1: for the NULL terminator
            continue;
        }
        
        strcpy(include_file_path, include_dir);
        strcat(include_file_path, "/");
        strncat(include_file_path, pathname, pathname_len);

		if (access(include_file_path, R_OK) == -1) {
		    if (errno == ENOENT && ((int)vec_size(preprocessor->include_dirs) - 1) != i) {
    			continue;
		    }

	        break;
		}

        return 0;
    }

    LOG_AT(L_ERROR, lexer, include_file_token, "%s: %s", include_file_path + strlen(include_file_path) - pathname_len, strerror(ENOENT));

    return -1;
}

int preprocessor_c_parse_include(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)token;

    Token_C include_file;

    if (lexer_c_next_skip_whitespace(lexer, &include_file) == -1) {
        LOG_AT(L_ERROR, lexer, &include_file, "#include expects \"FILENAME\" or <FILENAME>");

        return -1;
    }

    if (include_file.type != T_MACRO_INCLUDE_FILE && include_file.type != T_STRING) {
        LOG_AT(L_ERROR, lexer, &include_file, "#include expects \"FILENAME\" or <FILENAME>");

        return -1;
    }

    char include_file_path[PATH_MAX];

    if (preprocessor_c_find_include_file(preprocessor, lexer, &include_file, include_file_path) != 0) {
        return -1;
    }

    return preprocessor_c_parse_file(preprocessor, (const char *)&include_file_path);
}

int preprocessor_c_parse_identifier(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)lexer;

    fprintf(preprocessor->output, "%.*s", (int)token->len, token->value);

    // TODO token->value in preprocessor->defines

    return 0;
}

int preprocessor_c_parse_define(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)token;

    Token_C identifier;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, &identifier, T_IDENTIFIER) == 0) {
        LOG_AT(L_ERROR, lexer, &identifier, "macro names must be identifiers");

        return -1;
    }

    Token_C next_token;

    // TODO ?? not needed
    // TODO i think now we should parse first the "^#\\s*" token and then a real directive like define, undef
    if (lexer_c_next(lexer, &next_token) == -1 || next_token.type != T_WHITESPACE) {
        LOG_AT(L_ERROR, lexer, &next_token, "missing whitespace after the macro name");

        return -1;
    }

    char *identifier_str = malloc(sizeof(char) * (identifier.len + 1));
    strncpy(identifier_str, identifier.value, identifier.len);

    if (strncmp(next_token.value, "\n", 1) == 0) { // TODO ?? use Lexer
        lmap_add(preprocessor->defines, identifier_str, NULL);
        
        return 0;
    }

    Lexer_C lexer_saved = *lexer;

    Token_C macro_sequenze;
    
    if (lexer_c_next(lexer, &macro_sequenze) == -1 || macro_sequenze.type != T_MACRO_TOKEN_SEQUENZE) {
        *lexer = lexer_saved;

        return 0;
    }

    char *macro_sequenze_str = malloc(sizeof(char) * (macro_sequenze.len + 1));
    strncpy(macro_sequenze_str, macro_sequenze.value, macro_sequenze.len);

    lmap_add(preprocessor->defines, identifier_str, macro_sequenze_str);

    return 0;
}

int preprocessor_c_parse_undef(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    Lexer_C lexer_saved_begin = *lexer;

    (void)preprocessor;
    (void)token;

    Token_C identifier;

    if (lexer_c_next_skip_whitespace_token_is_type(lexer, &identifier, T_IDENTIFIER) == 0) {
        LOG_AT(L_ERROR, lexer, &identifier, "macro names must be identifiers");

        *lexer = lexer_saved_begin;
        return -1;
    }

    char *define_name = (char *)malloc(sizeof(char) * (identifier.len + 1));
    
    strncpy(define_name, identifier.value, identifier.len);
    
    lmap_remove(preprocessor->defines, define_name);
    
    free(define_name);

    return 0;
}

int preprocessor_c_parse_conditional(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)token;

    ParseTreeNode_C *conditional = parser_c_parse_preprocessor_conditional(lexer);

    // TODO ?? how to error
    if (conditional == NULL) {
        LOG_AT(L_ERROR, lexer, token, "conditional directive is not valid");

        return -1;
    }

    TokenType_C if_line_type = conditional->elements[0]->token.type;

    int parse_result = 0;

    // parse if_line
    switch(if_line_type) {
        case T_MACRO_IF: {
            assert(0 && "Not implemented: preprocessor_c_parse_conditional with TokenType: T_MACRO_IF");
        } break;
        case T_MACRO_IFDEF:
        case T_MACRO_IFNDEF: {
            int negate = if_line_type == T_MACRO_IFNDEF;

            Token_C identifier = conditional->elements[0]->elements[0]->token;
            
            char *identifier_name = (char *)malloc(sizeof(char) * (identifier.len + 1));
    
            strncpy(identifier_name, identifier.value, identifier.len);

            if ((lmap_has(preprocessor->defines, identifier_name) == 1) ^ (negate == 1)) {
                Token_C text = conditional->elements[1]->token;

                Lexer_C lexer_text = {
                    .buf = text.value,
                    .pbuf = text.value,
                    .loc = lexer->loc
                };

                parse_result = preprocessor_c_parse_lexer(preprocessor, &lexer_text, text.value + text.len);
                
                goto ret;
            }
        } break;
        default: assert(0 && "Not implemented: preprocessor_c_parse_conditional: if_line_type: only the following are supported: T_MACRO_IF, T_MACRO_IFDEF and T_MACRO_IFNDEF");
    }

    // parse elif_parts
    for (size_t i = 0; i < conditional->elements[1]->num; i++) {
        assert(0 && "Not implemented: preprocessor_c_parse_conditional elif parts");
    }

    // parse else_part
    if (conditional->num != 4) {
        goto ret;
    }

    Token_C text = conditional->elements[3]->elements[1]->token;

    Lexer_C lexer_text = {
        .buf = text.value,
        .pbuf = text.value,
        .loc = lexer->loc
    };

    parse_result = preprocessor_c_parse_lexer(preprocessor, &lexer_text, text.value + text.len);

    ret: {
        parse_tree_node_c_destroy(conditional);

        return parse_result;
    }
}

int preprocessor_c_parse_error(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;

    LOG_AT(L_ERROR, lexer, token, "%.*s", (int)(token->len), token->value);

    return -1;
}
