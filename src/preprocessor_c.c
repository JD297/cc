#include "lexer_c.h"
#include "map.h"
#include "preprocessor_c.h"
#include "token_c.h"
#include "token_type_c.h"
#include "vector.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

char *strncatrealloc(char *dst, const char *src, size_t size)
{
    const size_t size_dst = strlen(dst);
    const size_t size_src = size + 1;
    
    char *dst_new = (char *)realloc(dst, sizeof(char) * (size_dst + size_src));
    
    if (dst_new == NULL) {
        return NULL;
    }
    
    return strncat(dst_new, src, size);
}

const char *mmapfile(const char *pathname)
{
    struct stat sb;
    int fd;
    
    if (stat(pathname, &sb) == -1) {
		return NULL;
	}

	if ((sb.st_mode & S_IFMT) == S_IFDIR) {
		return NULL;
	}
	
	if ((fd = open(pathname, O_RDONLY)) == -1) {
        return NULL;
	}
	
	return (const char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
}

void *preprocessor_c_create(Vector *include_dirs, Vector *source_files, Map *defines)
{
    Preprocessor_C *preprocessor = (Preprocessor_C *)malloc(sizeof(Preprocessor_C));

    if (preprocessor == NULL) {
        return NULL;
    }

    preprocessor->buf = (char *)calloc(0, sizeof(char));
    
    preprocessor->include_dirs = include_dirs;
    preprocessor->source_files = source_files;
    preprocessor->defines = defines;

    if (preprocessor->buf == NULL) {
        free(preprocessor);

        return NULL;
    }
    
    return preprocessor;
}

void preprocessor_c_destroy(Preprocessor_C *preprocessor)
{
    if (preprocessor != NULL) {
        free(preprocessor->buf);
    }

    free(preprocessor);
}

int preprocessor_c_run(Preprocessor_C *preprocessor)
{
    for (size_t i = 0; i < vector_size(preprocessor->source_files); i++) {
        if (preprocessor_c_parse(preprocessor, vector_at(preprocessor->source_files, i)) == -1) {
            return -1;
        }
    }

    return 0;
}

int preprocessor_c_parse(Preprocessor_C *preprocessor, const char* pathname)
{
    const char *src = mmapfile(pathname);

    if (src == NULL) {
        return -1;
    }

    Lexer_C *lexer = lexer_c_create(src);

    if (lexer == NULL) {
        return -1;
    }

    preprocessor->buf = strncatrealloc(preprocessor->buf, "# 0 ", 4);
    preprocessor->buf = strncatrealloc(preprocessor->buf, pathname, strlen(pathname));
    preprocessor->buf = strncatrealloc(preprocessor->buf, "\n", 1);

    int parse_next_result;

    while((parse_next_result = preprocessor_c_parse_next(preprocessor, lexer)) == 0);

    return parse_next_result;
}

int preprocessor_c_parse_next(Preprocessor_C *preprocessor, Lexer_C *lexer)
{
    Token_C *token = lexer_c_next(lexer);
    
    if (token == NULL) {
        fprintf(stderr, "pathname:line:col: error: unreconized token\n");
        return -1;
    }

    switch (token->type) {
        case T_MACRO_INCLUDE: {
            return preprocessor_c_parse_include(preprocessor, lexer, token);
        }
        case T_IDENTIFIER: {
            return preprocessor_c_parse_identifier(preprocessor, lexer, token);
        }
        case T_MACRO_DEFINE: {
            return preprocessor_c_parse_define(preprocessor, lexer, token);
        }
        case T_MACRO_UNDEF: {
            return preprocessor_c_parse_undef(preprocessor, lexer, token);
        }
        case T_MACRO_IFNDEF: {
            return preprocessor_c_parse_ifndef(preprocessor, lexer, token);
        }
        case T_MACRO_IFDEF: {
            return preprocessor_c_parse_ifdef(preprocessor, lexer, token);
        }
        case T_MACRO_FILE: {
            return preprocessor_c_parse_file(preprocessor, lexer, token);
        }
        case T_MACRO_LINE: {
            return preprocessor_c_parse_line(preprocessor, lexer, token);
        }
        case T_MACRO_ERROR: {
            return preprocessor_c_parse_error(preprocessor, lexer, token);
        }
        case T_MACRO_IF: {
            return preprocessor_c_parse_if(preprocessor, lexer, token);
        }
        case T_MACRO_PRAGMA: {
            return preprocessor_c_parse_pragma(preprocessor, lexer, token);
        }
        case T_COMMENT_LINE:
        case T_COMMENT_MULTILINE: break;

        case T_CONSTEXPR:
        case T_RESTRICT:
            fprintf(stderr, "pathname:line:col: warning: ´constexpr´ unsupported optimization keyword\n");
        break;
        case T_MACRO_ELIF: break;
        case T_EOF: {
            token_c_destroy(token);

            return 1;
        }
        default: {
            return preprocessor_c_parse_default(preprocessor, lexer, token);
        }
    }
    
    token_c_destroy(token);
    
    return -1;
}

int preprocessor_c_parse_default(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)lexer;

    preprocessor->buf = strncatrealloc(preprocessor->buf, token->value, token->len);

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_include(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_identifier(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)lexer;

    preprocessor->buf = strncatrealloc(preprocessor->buf, token->value, token->len);

    // TODO token->value in preprocessor->defines

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_define(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_undef(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_ifndef(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_ifdef(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_file(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_line(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_error(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_if(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_pragma(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;
    (void)lexer;

    token_c_destroy(token);

    return 0;
}
