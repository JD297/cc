#define _GNU_SOURCE

#include "lexer_c.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/mman.h>

void *lexer_c_create_token(TokenType_C type, const char* value, size_t value_len)
{
	Token_C *token = (Token_C *)mmap(NULL, sizeof(Token_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (token == MAP_FAILED) {
		return TOKEN_CREATION_FAILED;
	}

	token->type = type;
	token->value = value;
	token->value_len = value_len;

	return token;
}

char *lexer_c_token_type_representation(TokenType_C type)
{
	return "!";
}

int lexer_c_token_type_is_in_expected_token_types(TokenType_C type, size_t num_types, /* TokenType_C types */...)
{
	if (num_types == 0) {
		return 0;
	}

	va_list ap;
	va_start(ap, num_types);

	for (int i = 0; i < num_types; i++) {
		if (type == va_arg(ap, TokenType_C)) {
			va_end(ap);

			return 1;
		}
	}

	va_end(ap);

	return 0;
}

void *lexer_c_create_lexer(char* pathname)
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

	lexer->tokens_len = LEXER_CREATION_TOKENS_LEN;
	lexer->tokens_num = 0;
	lexer->tokens = (Token_C **)mmap(NULL, lexer->tokens_len * sizeof(Token_C *), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (lexer->tokens == MAP_FAILED) {
		return LEXER_CREATION_FAILED;
	}

	lexer->error = NULL;

	return lexer;
}

int lexer_c_push_back_token(Lexer_C *lexer, Token_C *token)
{
	if (lexer->tokens_num == lexer->tokens_len) {
		size_t tokens_new_len = lexer->tokens_len + LEXER_CREATION_TOKENS_LEN;

		lexer->tokens = (Token_C **)mremap(lexer->tokens, lexer->tokens_len * sizeof(Token_C *), tokens_new_len * sizeof(Token_C *), MREMAP_MAYMOVE);

		if (lexer->tokens == MAP_FAILED) {
			return -1;
		}

		lexer->tokens_len = tokens_new_len;
	}

	lexer->tokens[lexer->tokens_num] = token;

	lexer->tokens_num++;

	return 0;
}

void *lexer_c_next(Lexer_C *lexer)
{
	return LEXER_NEXT_FAILED;
}

int lexer_c_run(Lexer_C *lexer)
{
	return -1;
}
