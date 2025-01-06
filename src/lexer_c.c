#define _GNU_SOURCE

#include "lexer_c.h"
#include "token_list_c.h"
#include "token_type_c.h"
#include "token_c.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>

void *lexer_c_create(const char* pathname)
{
    Lexer_C *lexer = (Lexer_C *)mmap(NULL, sizeof(Lexer_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (lexer == MAP_FAILED) {
        return LEXER_CREATION_FAILED;
    }

    lexer->pathname = pathname;

    if (stat(lexer->pathname, &lexer->sb) == -1) {
        return LEXER_CREATION_FAILED;
    }

    int fd;

    if ((fd = open(lexer->pathname, O_RDONLY)) == -1) {
        return LEXER_CREATION_FAILED;
    }

    lexer->buf = (char *)mmap(NULL, lexer->sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (lexer->buf == MAP_FAILED) {
        return LEXER_CREATION_FAILED;
    }

    if (close(fd) == -1) {
        return LEXER_CREATION_FAILED;
    }

    lexer->pbuf = lexer->buf;

    lexer->tokens = token_list_c_create();

    if (lexer->tokens == LIST_CREATION_FAILED) {
        return LEXER_CREATION_FAILED;
    }

    lexer->error = NULL;

    return lexer;
}

void *lexer_c_next_macro_include_library_path(Lexer_C *lexer)
{
    for (int i = lexer->tokens->num - 1; i >= 0; i--) {
        if (token_type_c_is_in_expected_token_types(lexer->tokens->elements[i]->type, TOKEN_TYPE_C_ALL_WHITESPACES)) {
            continue;
        }

        if (!token_type_c_is_in_expected_token_types(lexer->tokens->elements[i]->type, 1, T_MACRO_INCLUDE)) {
            return LEXER_NEXT_SKIPPED;
        }

        if (*lexer->pbuf != '<') {
            return LEXER_NEXT_SKIPPED;
        }

        char *start = lexer->pbuf;
        char *end = strstr(start, ">");

        if (end == NULL) {
            lexer->error = "missing terminating > character";

            return LEXER_NEXT_FAILED;
        }

        end++;

        const size_t length = end - start;

        lexer->pbuf = end;

        return token_c_create(T_MACRO_INCLUDE_LIBRARY_PATH, start, length);
	}

    return LEXER_NEXT_SKIPPED;
}

void *lexer_c_next_identifier(Lexer_C *lexer)
{
    char *start = lexer->pbuf;
    char *end;

    for (end = start; *end != '\0'; end++) {
        if (end == start) {
            if (*end == '_' || isalpha(*end) != 0) {
                continue;
            } else {
                end = NULL;

                break;
            }
        }

        if (*end != '_' && isalnum(*end) == 0) {
            break;
        }
    }

    if (end == NULL || *end == '\0') {
        return LEXER_NEXT_SKIPPED;
    }

    const size_t length = end - start;

    lexer->pbuf = end;

    return token_c_create(T_IDENTIFIER, start, length);
}

void *lexer_c_next_comment_line(Lexer_C *lexer)
{
    char *start = lexer->pbuf;
    char *end = NULL;

    if (strlen(start) < 2) {
        return LEXER_NEXT_SKIPPED;
    }

    if (strncmp(start, "//" , 2) != 0) {
        return LEXER_NEXT_SKIPPED;
    }

    for (end = start; *end != '\0' && *end != '\n'; end++);

    const size_t length = end - start;

    lexer->pbuf = end;

    return token_c_create(T_COMMENT_LINE, start, length);
}

void *lexer_c_next_comment_multiline(Lexer_C *lexer)
{
    char *start = lexer->pbuf;

    if (strlen(start) < 2) {
        return LEXER_NEXT_SKIPPED;
    }

    if (strncmp(start, "/*" , 2) != 0) {
        return LEXER_NEXT_SKIPPED;
    }

    char *end = strstr(start, "*/");

    if (end == NULL) {
        lexer->error = "unterminated comment";

        return LEXER_NEXT_FAILED;
    }

    end += 2;

    const size_t length = end - start;

    lexer->pbuf = end;

    return token_c_create(T_COMMENT_MULTILINE, start, length);
}

void *lexer_c_next_string(Lexer_C *lexer)
{
    if (*lexer->pbuf != '"') {
        return LEXER_NEXT_SKIPPED;
    }

    char *start = lexer->pbuf;
    char *end;

    for (end = start + 1; *end != '\0'; end++) {
        if (*end == '"' && *end - 1 == '\\') {
            continue;
        }

        if (*end == '"') {
            break;
        }

        if (*end == '\0') {
            lexer->error = "missing terminating \" character";

            return LEXER_NEXT_FAILED;
        }
    }

    end++;

    const size_t length = end - start;

    lexer->pbuf = end;

    return token_c_create(T_STRING, start, length);
}

void *lexer_c_next_type_cast(Lexer_C *lexer)
{
    return LEXER_NEXT_SKIPPED;
}

void *lexer_c_next_number(Lexer_C *lexer)
{
    if (isdigit(*lexer->pbuf) == 0) {
        return LEXER_NEXT_SKIPPED;
    }

    char *start = lexer->pbuf;
    char *end;

    size_t decimal_points = 0;

    for (end = start; *end != '\0'; end++) {
        assert(*end != 'x' && "TODO: hex numbers are not supported!");

        if (*end == '.') {
            decimal_points++;
        }

        if (decimal_points >= 2) {
            lexer->error = "too many decimal points in number";

            return LEXER_NEXT_FAILED;
        }

        if (isdigit(*end) == 0 && *end != '.') {
            break;
        }
    }

    const size_t length = end - start;

    lexer->pbuf = end;

    return token_c_create(T_NUMBER, start, length);
}

void *lexer_c_next_with_representation(Lexer_C *lexer, TokenType_C type)
{
    const char* token_type_representation = token_type_c_to_representation(type);

    char *found = strstr(lexer->pbuf, token_type_representation);

    if (found != NULL) {
        const char *found_after = found + strlen(token_type_representation);

        if (token_type_c_is_in_expected_token_types(type, TOKEN_TYPE_C_ALNUM_REPRESENTATION)) {
            if (isalnum(*found_after) != 0 || *found_after == '_') {
                return LEXER_NEXT_SKIPPED;
            }
        }
    }

    if (found == lexer->pbuf && (type != T_EOF) == (*lexer->pbuf != '\0')) {
        const size_t length = strlen(token_type_representation);
    
        lexer->pbuf += length;

        return token_c_create(type, found, length);
    }

    return LEXER_NEXT_SKIPPED;
}

void *lexer_c_next(Lexer_C *lexer)
{
	for (TokenType_C type = 0; type <= T_EOF; type++) {
		if (token_type_c_is_in_expected_token_types(type, TOKEN_TYPE_C_WITH_NO_REPRESENTATION)) {
			switch (type) {
				case T_MACRO_INCLUDE_LIBRARY_PATH: {
					Token_C *token = lexer_c_next_macro_include_library_path(lexer);

                    if (token != LEXER_NEXT_SKIPPED) {
                        return token;
                    }
				} break;
				case T_IDENTIFIER: {
                    Token_C *token = lexer_c_next_identifier(lexer);

                    if (token != LEXER_NEXT_SKIPPED) {
                        return token;
                    }
				} break;
				case T_COMMENT_LINE: {
					Token_C *token = lexer_c_next_comment_line(lexer);

                    if (token != LEXER_NEXT_SKIPPED) {
                        return token;
                    }
				} break;
				case T_COMMENT_MULTILINE: {
					Token_C *token = lexer_c_next_comment_multiline(lexer);

                    if (token != LEXER_NEXT_SKIPPED) {
                        return token;
                    }
				} break;
				case T_STRING: {
					Token_C *token = lexer_c_next_string(lexer);

                    if (token != LEXER_NEXT_SKIPPED) {
                        return token;
                    }
				} break;
				case T_TYPE_CAST:
                    Token_C *token = lexer_c_next_type_cast(lexer);

                    if (token != LEXER_NEXT_SKIPPED) {
                        return token;
                    }
				break;
				case T_NUMBER: {
					Token_C *token = lexer_c_next_number(lexer);

                    if (token != LEXER_NEXT_SKIPPED) {
                        return token;
                    }
				} break;
				default: assert(0 && "TokenType_C (type) not implemented!");
			}

			continue;
		}

        Token_C *token = lexer_c_next_with_representation(lexer, type);

        if (token != LEXER_NEXT_SKIPPED) {
            return token;
        }
	}

	return LEXER_NEXT_FAILED;
}

int lexer_c_run(Lexer_C *lexer)
{
    Token_C *token;

    do {
        if ((token = lexer_c_next(lexer)) == LEXER_NEXT_FAILED) {
            return -1;
        }

        if (token_list_c_push_back(lexer->tokens, token) == -1) {
            return -1;
        }
    } while (token->type != T_EOF);

    return 0;
}

