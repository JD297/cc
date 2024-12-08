#include "lexer_c.h"

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
}

char *lexer_c_token_type_representation(TokenType_C type)
{
	return "!";
}

int lexer_c_token_type_is_in_expected_token_types(TokenType_C type, size_t num_types, /* TokenType_C types */...)
{
	return 0;
}

void *lexer_c_create_lexer(char* pathname)
{
	return LEXER_CREATION_FAILED;
}

int lexer_c_push_back_token(Lexer_C *lexer, Token_C *token)
{
	return -1;
}

void *lexer_c_next(Lexer_C *lexer)
{
	return LEXER_NEXT_FAILED;
}

int lexer_c_run(Lexer_C *lexer)
{
	return -1;
}
