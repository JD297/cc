#include "lexer_c.h"

void *lexer_c_create_token(TokenType_C type, const char* value)
{
	return TOKEN_CREATION_FAILED;
}

char *lexar_c_token_type_representation(TokenType_C type)
{
	return "!";
}

int lexar_c_token_type_is_in_expected_token_types(TokenType_C type, size_t num_types, /* TokenType_C types */...)
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
