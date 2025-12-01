#include <stdio.h>

#include "lexer_c.h"
#include "token_c.h"
#include "token_type_c.h"

#include <jd297/sv.h>

int main(void)
{
	const char *input = "\'A\' \'\\x10\' \'\\777\' \'\\n\' 900 0xF0 0777";

	Lexer_C lexer;
	Token_C token;

	printf("input: \"%s\"\n", input);

	token_type_c_create_lookups();

	lexer_c_create(&lexer, "<inline>", input);

	do {
		lexer_c_next(&lexer, &token);
		
		printf("%s:%zu:%zu: token: <%d>: \""SV_FMT"\"", lexer.loc.pathname, lexer.loc.line, lexer.loc.col, token.type, SV_PARAMS(&token.view));
		
		if (token.type == T_INTEGER_CONSTANT) {
			printf(" literal: %lu", token.literal.lu);
		} else if (token.type == T_FLOATING_CONSTANT) {
			printf(" literal: %Lf", token.literal.Lf);
		} if (token.type == T_CHARACTER_CONSTANT) {
			printf(" literal: %d", token.literal.d);
		}
		
		printf("\n");
	} while (token.type != T_EOF);

	return 0;
}
