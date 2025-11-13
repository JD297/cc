#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "lexer_c.h"
#include "token_c.h"
#include "token_type_c.h"

#include <jd297/sv.h>

int main(int argc, char **argv)
{
	if (argc == 1) {
		return 1;
	}
	
	int fd = open(argv[1], O_RDONLY);
	
	struct stat sb;
	stat(argv[1], &sb);
	
	char *input = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE | MAP_FILE, fd, 0);

	Lexer_C lexer;
	Token_C token;

	token_type_c_create_lookups();

	lexer_c_create(&lexer, sv_from_cstr(argv[1]), input, LEXER_MODE_NORMAL);

	do {
		lexer_c_next(&lexer, &token);
		
		printf(SV_FMT":%zu:%zu: token: <%d>: \""SV_FMT"\"", SV_PARAMS(&lexer.loc.pathname), lexer.loc.line, lexer.loc.col, token.type, SV_PARAMS(&token.view));
		
		if (token.type == T_INTEGER_CONSTANT) {
			printf(" literal: %lu", token.literal.lu);
		} else if (token.type == T_FLOATING_CONSTANT) {
			printf(" literal: %Lf", token.literal.Lf);
		} if (token.type == T_CHARACTER_CONSTANT) {
			printf(" literal: %d", token.literal.d);
		}
		
		printf("\n");
	} while (token.type != T_EOF);

	munmap(input, sb.st_size);

	return 0;
}
