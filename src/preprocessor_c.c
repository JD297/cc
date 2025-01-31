#include "lexer_c.h"
#include "map.h"
#include "preprocessor_c.h"
#include "token_c.h"
#include "token_type_c.h"
#include "vector.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

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
    
    preprocessor->include_dirs = include_dirs;
    preprocessor->source_files = source_files;
    preprocessor->defines = defines;

    preprocessor->output = tmpfile();

    return preprocessor;
}

void preprocessor_c_destroy(Preprocessor_C *preprocessor)
{
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

    Lexer_C *lexer = lexer_c_create(src, pathname);

    if (lexer == NULL) {
        return -1;
    }

    fprintf(preprocessor->output, "#line 0 \"%s\"\n", pathname);

    int parse_next_result;

    while((parse_next_result = preprocessor_c_parse_next(preprocessor, lexer)) == 0);

    lexer_c_destroy(lexer);

    return parse_next_result;
}

int preprocessor_c_parse_next(Preprocessor_C *preprocessor, Lexer_C *lexer)
{
    Token_C *token = lexer_c_next(lexer);
    
    if (token == NULL) {
        lexer_c_log(lexer, "unreconized token");
        return -1;
    }

    switch (token->type) {
        case T_MACRO_INCLUDE: {
            return preprocessor_c_parse_include(preprocessor, lexer, token) == 1 ? 0 : -1;
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
        case T_COMMENT_MULTILINE: return 0;

        case T_CONSTEXPR:
        case T_RESTRICT:
            lexer_c_log(lexer, "warning: ´constexpr´ unsupported optimization keyword");
            return 0;
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

    fprintf(preprocessor->output, "%.*s", (int)token->len, token->value);

    token_c_destroy(token);

    return 0;
}

char *preprocessor_c_find_include_file(Preprocessor_C *preprocessor, const char *pathname, int mode)
{
    char *include_file = (char *)malloc(sizeof(char) * PATH_MAX);

    for (int i = 0 - mode; i < (int)vector_size(preprocessor->include_dirs); i++) {
        const char *include_dir;
        
        if (i >= 0) {
            include_dir = vector_at(preprocessor->include_dirs, i);
        } else {
            include_dir = ".";
        }
    
        if ((strlen(include_dir) + strlen(pathname) + 1) >= PATH_MAX) {
            continue;
        }

        memset(include_file, 0, PATH_MAX);
        
        strcpy(include_file, include_dir);
        strcat(include_file, "/");
        strcat(include_file, pathname);

		if (access(include_file, R_OK) == -1) {
		    if (errno == ENOENT && ((int)vector_size(preprocessor->include_dirs) - 1) != i) {
    			continue;
		    }

	        return NULL;
		}

        return include_file;
    }
    
    free(include_file);

    return NULL;
}

int preprocessor_c_parse_include(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)lexer;

    int parse_result = -1;

    regex_t regex;
    regmatch_t matches[2];

    regcomp(&regex, "^#\\s*include\\s*<([^>]+)>", REG_EXTENDED);

    if (regexec(&regex, token->value, 2, matches, 0) == 0) {
        size_t start = matches[1].rm_so;
        size_t end = matches[1].rm_eo;

        size_t length = end - start;

        char* include_file = (char*)malloc(length + 1);

        if (include_file == NULL) {
            return -1;
        }

        strncpy(include_file, token->value + start, length);
        include_file[length] = '\0';
        
        char *include_file_path = preprocessor_c_find_include_file(preprocessor, include_file, PREPROCESSOR_INCLUDE_MODE_LIBRARARY);
        
        free(include_file);
        
        parse_result = preprocessor_c_parse(preprocessor, include_file_path);
        
        free(include_file_path);
        
        goto ret;
    }
    
    regcomp(&regex, "^#\\s*include\\s*\"([^\"]+)\"", REG_EXTENDED);

    if (regexec(&regex, token->value, 2, matches, 0) == 0) {
        size_t start = matches[1].rm_so;
        size_t end = matches[1].rm_eo;

        size_t length = end - start;

        char* include_file = (char*)malloc(length + 1);

        if (include_file == NULL) {
            return -1;
        }

        strncpy(include_file, token->value + start, length);
        include_file[length] = '\0';

        char *include_file_path = preprocessor_c_find_include_file(preprocessor, include_file, PREPROCESSOR_INCLUDE_MODE_STRING);
        
        free(include_file);
        
        parse_result = preprocessor_c_parse(preprocessor, include_file_path);
        
        free(include_file_path);
        
        goto ret;
    }

    ret: {
        token_c_destroy(token);

        return parse_result;
    }
}

int preprocessor_c_parse_identifier(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)lexer;

    fprintf(preprocessor->output, "%.*s", (int)token->len, token->value);

    // TODO token->value in preprocessor->defines

    token_c_destroy(token);

    return 0;
}

int preprocessor_c_parse_define(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;

    token_c_destroy(token);

    Token_C *identifier = lexer_c_next_skip_whitespace(lexer); // TODO lexer_c_macro_next_skip_whitespace ??

    if (identifier == NULL || identifier->type != T_IDENTIFIER) {
        lexer_c_log(lexer, "macro names must be identifiers");

        return -1;
    }
    
    Token_C *next_token = lexer_c_next(lexer);

    if (next_token == NULL || next_token->type != T_WHITESPACE) {
        token_c_destroy(next_token);

        lexer_c_log(lexer, "missing whitespace after the macro name");
        
        return -1;
    }

    char *identifier_str = malloc(sizeof(char) * (identifier->len + 1));
    strncpy(identifier_str, identifier->value, identifier->len);

    if (strncmp(next_token->value, "\n", 1) == 0) {
        map_add(preprocessor->defines, NULL, identifier_str);
        
        return 0;
    }

    token_c_destroy(next_token);

    lexer_c_backup(lexer);

    Token_C *macro_sequenze = lexer_c_next(lexer);
    
    if (macro_sequenze == NULL || macro_sequenze->type != T_MACRO_TOKEN_SEQUENZE) {
        lexer_c_restore(lexer);

        return 0;
    }

    char *macro_sequenze_str = malloc(sizeof(char) * (macro_sequenze->len + 1));
    strncpy(macro_sequenze_str, macro_sequenze->value, macro_sequenze->len);

    map_add(preprocessor->defines, macro_sequenze_str, identifier_str);

    return 0;
}

int preprocessor_c_parse_undef(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    lexer_c_backup(lexer);

    token_c_destroy(token);

    (void)preprocessor;

    Token_C *identifier = lexer_c_next_skip_whitespace(lexer); // TODO lexer_c_macro_next_skip_whitespace ??

    if (identifier == NULL) {
        goto error;
    }
    
    if (identifier->type != T_IDENTIFIER) {
        goto error;
    }

    char *define_name = (char *)malloc(sizeof(char) * (identifier->len + 1));
    
    strncpy(define_name, identifier->value, identifier->len);
    
    map_remove(preprocessor->defines, define_name);
    
    free(define_name);

    return 0;
    
    error: {
        token_c_destroy(identifier);

        lexer_c_restore(lexer);

        lexer_c_log(lexer, "no macro name given in #undef directive");
        
        return -1;
    }
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

int preprocessor_c_parse_line(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;

    token_c_destroy(token);

    Token_C *number = lexer_c_next_skip_whitespace(lexer);
 
    if (number == NULL || number->type != T_NUMBER) {
        lexer_c_log(lexer, "after #line is not a positive integer");
        
        return -1;
    }

    lexer->loc.col = 1;

    char *row_str = malloc(sizeof(char) * (number->len + 1));
    strncpy(row_str, number->value, number->len);

    lexer->loc.row = (size_t)atoi(row_str);

    lexer_c_backup(lexer);

    Token_C *filename = lexer_c_next_skip_whitespace(lexer);

    if (filename == NULL || filename->type != T_STRING) {
        lexer_c_restore(lexer);
        
        return 0;
    }

    char *filename_str = malloc(sizeof(char) * (filename->len + 1));
    strncpy(filename_str, filename->value + 1, filename->len - 2);

    lexer->loc.pathname = filename_str;

    return 0;
}

int preprocessor_c_parse_error(Preprocessor_C *preprocessor, Lexer_C *lexer, Token_C *token)
{
    (void)preprocessor;

    lexer_c_log(lexer, "#error directive encountered");

    token_c_destroy(token);

    return -1;
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
