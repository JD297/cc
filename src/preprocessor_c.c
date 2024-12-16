#include "preprocessor_c.h"
#include "lexer_c.h"
#include "lexer_list_c.h"
#include "str_list.h"
#include "token_list_c.h"
#include "token_list_named_c.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

void *preprocessor_c_create(StrList *include_dirs, StrList *source_files, TokenListNamed_C *defines)
{
    Preprocessor_C *preprocessor = (Preprocessor_C *)mmap(NULL, sizeof(Preprocessor_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (preprocessor == MAP_FAILED) {
        return PREPROCESSOR_CREATION_FAILED;
    }
    
    preprocessor->include_dirs = include_dirs;
    preprocessor->source_files = source_files;
    preprocessor->defines = defines;
    
    if ((preprocessor->lexers = lexer_list_c_create()) == LIST_CREATION_FAILED) {
        return PREPROCESSOR_CREATION_FAILED;
    }
    
    if((preprocessor->output = token_list_c_create()) == LIST_CREATION_FAILED) {
        return PREPROCESSOR_CREATION_FAILED;
    }
    
    preprocessor->error = NULL;

    return preprocessor;
}

char *preprocessor_c_find_include_file(Preprocessor_C *preprocessor, const char *pathname, int mode)
{
    char *include_file = (char *)mmap(NULL, sizeof(char) * PATH_MAX * 2, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    for (int i = 0 - mode; i < (int)preprocessor->include_dirs->num; i++) {
        const char *include_dir;
        
        if (i >= 0) {
            include_dir = preprocessor->include_dirs->elements[i];
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
		    if (errno == ENOENT && ((int)preprocessor->include_dirs->num - 1) != i) {
    			continue;
		    }

            strcat(include_file, ": ");
	        strcat(include_file, strerror(errno));

            preprocessor->error = include_file;

	        return NULL;
		}

        return include_file;
    }
    
    munmap(include_file, sizeof(char) * PATH_MAX);

    return NULL;
}

int preprocessor_c_parse_include(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken)
{
    (*ptoken)++;

    while (token_type_c_is_in_expected_token_types((**ptoken)->type, 2, T_WHITESPACE_SPACE, T_WHITESPACE_TAB)) {
        (*ptoken)++;
    }
    
    if (!token_type_c_is_in_expected_token_types((**ptoken)->type, 2, T_MACRO_INCLUDE_LIBRARY_PATH, T_STRING)) {
        preprocessor->error = "#include expects \"FILENAME\" or <FILENAME>";
    
        return -1;
    }

    int include_mode = (**ptoken)->type == T_STRING ? PREPROCESSOR_INCLUDE_MODE_STRING : PREPROCESSOR_INCLUDE_MODE_LIBRARARY;
    
    const char *include_file = preprocessor_c_find_include_file(preprocessor, (**ptoken)->value, include_mode);
    
    if (include_file == NULL) {
        return -1;
    }
    
    (*ptoken)++;

    return preprocessor_c_parse(preprocessor, include_file);
}

int preprocessor_c_parse_identifier(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken)
{
    const Token_C *identifier = (**ptoken);

    const char *identifier_name = identifier->value;

    TokenList_C *define_tokens = NULL;

    // TODO use get method of list named 
    for (size_t i = 0; i < preprocessor->defines->num; i++) {
        if (preprocessor->defines->elements_names[i] == NULL) {
            continue;
        }

        if (strcmp(preprocessor->defines->elements_names[i], identifier_name) == 0) {
            define_tokens = preprocessor->defines->elements[i];
            
            break;
        }
    }

    (*ptoken)++;

    if (define_tokens == NULL) {
        if (token_list_c_push_back(preprocessor->output, identifier) == -1) {
            preprocessor->error = strerror(errno);

            return -1;
        }

        return 0;
    }

    for (size_t i = 0; i < define_tokens->num; i++) {
        if (token_list_c_push_back(preprocessor->output, define_tokens->elements[i]) == -1) {
            preprocessor->error = strerror(errno);

            return -1;
        }
    }

    return 0;
}

int preprocessor_c_parse_define(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken)
{
    (*ptoken)++;

    while (token_type_c_is_in_expected_token_types((**ptoken)->type, 2, T_WHITESPACE_SPACE, T_WHITESPACE_TAB)) {
        (*ptoken)++;
    }
    
    if ((**ptoken)->type != T_IDENTIFIER) {
        preprocessor->error = "macro names must be identifiers";
    
        return -1;
    }

    const char *identifier = (**ptoken)->value;

    (*ptoken)++;

    TokenList_C *define_tokens;
    
    if ((define_tokens = token_list_c_create()) == LIST_CREATION_FAILED) {
        preprocessor->error = strerror(errno);
    
        return -1;
    }

    while (1) {
        size_t whitespaces_skipped = 0;
    
        while (token_type_c_is_in_expected_token_types((**ptoken)->type, 2, T_WHITESPACE_SPACE, T_WHITESPACE_TAB)) {
            whitespaces_skipped++;
            (*ptoken)++;
        }
    
        if ((**ptoken)->type == T_EOF) {
            break;
        }
        
        if ((**ptoken)->type == T_WHITESPACE_LINE_FEED) {
            (*ptoken)++;
        
            break;
        }
        
        if (define_tokens->num > 0 && whitespaces_skipped > 0) {
            if (token_list_c_push_back(define_tokens, token_c_create(T_WHITESPACE_SPACE, " ")) == -1) {
                preprocessor->error = strerror(errno);

                return -1;
            }
        }
        
        if (token_list_c_push_back(define_tokens, (**ptoken)) == -1) {
            preprocessor->error = strerror(errno);

            return -1;
        }
    }
    
    // TODO use this as add(const char *name) method in list_named
    // this tries to get the element with get method if its null that it pushes back else it overrides
    for (size_t i = 0; i < preprocessor->defines->num; i++) {
        if (preprocessor->defines->elements_names[i] == NULL) {
            continue;
        }

        if (strcmp(preprocessor->defines->elements_names[i], identifier) == 0) {
            preprocessor->defines->elements[i] = define_tokens;
            
            return 0;
        }
    }
    
    if (token_list_named_c_push_back(preprocessor->defines, define_tokens, identifier) == -1) {
        preprocessor->error = strerror(errno);

        return -1;
    }

    return 0;
}

int preprocessor_c_parse_ifndef(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken)
{
    (*ptoken)++;

    while (token_type_c_is_in_expected_token_types((**ptoken)->type, 2, T_WHITESPACE_SPACE, T_WHITESPACE_TAB)) {
        (*ptoken)++;
    }
    
    if ((**ptoken)->type != T_IDENTIFIER) {
        preprocessor->error = "macro names must be identifiers";
    
        return -1;
    }

    const char *identifier = (**ptoken)->value;

    (*ptoken)++;

    size_t depth = 0;

    Token_C **endif = NULL;
    
    for (Token_C **token = (*ptoken); (*token)->type != T_EOF; token++) {
        if (token_type_c_is_in_expected_token_types((*token)->type, 3, T_MACRO_IFDEF, T_MACRO_IFNDEF, T_MACRO_IF)) {
            depth++;

            continue;
        }

        if ((*token)->type == T_MACRO_ENDIF) {
            if (depth == 0) {
                endif = token;

                break;
            }

            depth--;

            continue;
        }
    }

    if (endif == NULL) {
        preprocessor->error = "unterminated #ifndef";
        
        return -1;
    }
    
    // TODO use this as get(const char *name) method in list_named
    for (size_t i = 0; i < preprocessor->defines->num; i++) {
        if (preprocessor->defines->elements_names[i] == NULL) {
            continue;
        }

        if (strcmp(preprocessor->defines->elements_names[i], identifier) == 0) {
            (*ptoken) = endif + 1;
        
            return 0;
        }
    }

    return 0;
}

int preprocessor_c_parse_whitespace(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken)
{
    while (token_type_c_is_in_expected_token_types((**ptoken)->type, TOKEN_TYPE_C_ALL_WHITESPACES)) {
        (*ptoken)++;
    }
    
    if (preprocessor->output->num == 0) {
        return 0;
    }
    
    Token_C **last = preprocessor->output->elements + preprocessor->output->num - 1;
    
    if ((*last)->type == T_WHITESPACE_LINE_FEED) {
        return 0;
    }
    
    for (Token_C **token = (*ptoken) - 1; token != tokens->elements; token--) {
        if ((*token)->type == T_WHITESPACE_LINE_FEED) {
            if (token_list_c_push_back(preprocessor->output, (*token)) == -1) {
                preprocessor->error = strerror(errno);

                return -1;
            }
            
            return 0;
        }
        
        if (!token_type_c_is_in_expected_token_types((*token)->type, TOKEN_TYPE_C_ALL_WHITESPACES)) {
            break;
        }
    }
    
    if (token_list_c_push_back(preprocessor->output, token_c_create(T_WHITESPACE_SPACE, " ")) == -1) {
        preprocessor->error = strerror(errno);

        return -1;
    }

    return 0;
}

int preprocessor_c_parse_next(Preprocessor_C *preprocessor, TokenList_C *tokens, Token_C ***ptoken)
{
    switch ((**ptoken)->type) {
        case T_MACRO_INCLUDE: {
            return preprocessor_c_parse_include(preprocessor, tokens, ptoken);
        }
        case T_IDENTIFIER: {
            return preprocessor_c_parse_identifier(preprocessor, tokens, ptoken);
        }
        case T_MACRO_DEFINE: {
            return preprocessor_c_parse_define(preprocessor, tokens, ptoken);
        }
        case T_MACRO_IFNDEF: {
            return preprocessor_c_parse_ifndef(preprocessor, tokens, ptoken);
        }
        /* NOT IMPLEMENTED */
        case T_MACRO_IF:
        case T_MACRO_IFDEF:
        case T_MACRO_UNDEF:
        case T_MACRO_EMBED:
        case T_MACRO_LINE:
        case T_MACRO_ERROR:
        case T_MACRO_WARNING:
        case T_MACRO_PRAGMA:
        case T_MACRO_DEFINDED:
        case T_MACRO___HAS_INCLUDE:
        case T_MACRO___HAS_EMBED:
        case T_MACRO___HAS_C_ATTRIBUTE:
        case T_MACRO_ASM:
        case T_MACRO_FORTRAN: {
            assert(0 && "TokenType_C (ptoken->type) not implemented!");
        }
        /* WHITESPACES */
        case T_WHITESPACE_TAB:
        case T_WHITESPACE_LINE_FEED:
        case T_WHITESPACE_LINE_TABULATION:
        case T_WHITESPACE_FORM_FEED:
        case T_WHITESPACE_CARRIAGE_RETURN:
        case T_WHITESPACE_SPACE:
        case T_WHITESPACE_NEXT_LINE: {
            return preprocessor_c_parse_whitespace(preprocessor, tokens, ptoken);
        }
        /* SKIP T_MACRO_ENDIF */
        case T_MACRO_ENDIF:
        /* SKIP COMMENTS */
        case T_COMMENT_LINE:
        case T_COMMENT_MULTILINE:
        /* SKIP OPTIMIZATIONS */
        case T_CONSTEXPR:
        case T_RESTRICT:
            (*ptoken)++;
        break;
        
        /* SKIP EOF */
        case T_EOF: {
            break;
        }
        
        default: {
            if (token_list_c_push_back(preprocessor->output, (**ptoken)) == -1) {
                preprocessor->error = strerror(errno);

                return -1;
            }
        
            (*ptoken)++;
        }
    }
    
    return 0;
}

int preprocessor_c_parse(Preprocessor_C *preprocessor, const char* filepath)
{
    Lexer_C *lexer = lexer_c_create(filepath);

    if (lexer == LEXER_CREATION_FAILED) {
        preprocessor->error = strerror(errno);

        return -1;
    }

    if (lexer_list_c_push_back(preprocessor->lexers, lexer) == -1) {
        preprocessor->error = strerror(errno);

        return -1;
    }

    if (lexer_c_run(lexer) == -1) {
        return -1;
    }

    if (lexer->tokens->num == 0) {
        return 0;
    }

    Token_C **ptoken = lexer->tokens->elements;

    while ((*ptoken)->type != T_EOF) {
        if (preprocessor_c_parse_next(preprocessor, lexer->tokens, &ptoken) == -1) {
            return -1;
        }
    }

    return 0;
}

int preprocessor_c_run(Preprocessor_C *preprocessor)
{
    for (size_t i = 0; i < preprocessor->source_files->num; i++) {
        if (preprocessor_c_parse(preprocessor, preprocessor->source_files->elements[i]) == -1) {
            return -1;
        }
    }
    
    if (token_list_c_push_back(preprocessor->output, token_c_create(T_EOF, "\0")) == -1) {
        preprocessor->error = strerror(errno);

        return -1;
    }

    return 0;
}

