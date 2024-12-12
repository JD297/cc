#define _GNU_SOURCE
#include "lexer_c.h"

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <sys/mman.h>

void *lexer_c_create_token(TokenType_C type, const char* value)
{
	Token_C *token = (Token_C *)mmap(NULL, sizeof(Token_C), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (token == MAP_FAILED) {
		return TOKEN_CREATION_FAILED;
	}

	token->type = type;
	token->value = value;

	return token;
}

char *lexer_c_token_type_representation(TokenType_C type)
{
	switch (type) {
		case T_IDENTIFIER:
		case T_STRING:
		case T_TYPE_CAST:
		case T_NUMBER:
		case T_MACRO_INCLUDE_LIBRARY_PATH:
		case T_COMMENT_LINE:
		case T_COMMENT_MULTILINE:
		    return NULL;
		case T_OPEN_BRACKET: return "[";
		case T_CLOSING_BRACKET: return "]";
		case T_OPEN_PARENT: return "(";
		case T_CLOSING_PARENT: return ")";
		case T_OPEN_BRACE: return "{";
		case T_CLOSING_BRACE: return "}";
		case T_DOT: return ".";
		case T_ARROW: return "->";
		case T_COMMA: return ",";
		case T_COLON: return ":";
		case T_SEMICOLON: return ";";
		case T_ASTERISK: return "*";
		case T_ASSIGNMENT: return "=";
		case T_TILDE: return "~";
		case T_PLUS: return "+";
		case T_MINUS: return "-";
		case T_MULTIPLY: return "*";
		case T_DIVIDE: return "/";
		case T_MODULUS: return "%";
		case T_UNARY_PLUS: return "+";
		case T_UNARY_MINUS: return "-";
		case T_INCREMENT: return "++";
		case T_DECREMENT: return "--";
		case T_LESS_THAN: return "<";
		case T_GREATER_THAN: return ">";
		case T_LESS_THAN_OR_EQUAL_TO: return "<=";
		case T_GREATER_THAN_OR_EQUAL_TO: return ">=";
		case T_EQUAL_TO: return "==";
		case T_NOT_EQUAL_TO: return "!=";
		case T_LOGICAL_AND: return "&&";
		case T_LOGICAL_OR: return "||";
		case T_LOGICAL_NOT: return "!";
		case T_BITWISE_AND: return "&";
		case T_BITWISE_OR: return "|";
		case T_BITWISE_XOR: return "^";
		case T_BITWISE_FIRST_COMPLEMENT: return "~";
		case T_BITWISE_LEFTSHIFT: return "<<";
		case T_BITWISE_RIGHTSHIFT: return ">>";
		case T_PLUS_ASSIGN: return "+=";
		case T_MINUS_ASSIGN: return "-=";
		case T_MULTIPLY_ASSIGN: return "*=";
		case T_DIVIDE_ASSIGN: return "/=";
		case T_MODULUS_ASSIGN: return "%=";
		case T_BITWISE_AND_ASSIGN: return "&=";
		case T_BITWISE_OR_ASSIGN: return "|=";
		case T_BITWISE_XOR_ASSIGN: return "^=";
		case T_BITWISE_RIGHTSHIFT_ASSIGN: return ">>=";
		case T_BITWISE_LEFTSHIFT_ASSIGN: return "<<=";
		case T_TERNARY: return "?";
		case T_ADDRESSOF: return "&";
		case T_DEREFERENCE: return "*";
		case T_ALIGNAS: return "alignas";
		case T_ALIGNOF: return "alignof";
		case T_AUTO: return "auto";
		case T_BOOL: return "bool";
		case T_BREAK: return "break";
		case T_CASE: return "case";
		case T_CHAR: return "char";
		case T_CONST: return "const";
		case T_CONSTEXPR: return "constexpr";
		case T_CONTINUE: return "continue";
		case T_DEFAULT: return "default";
		case T_DO: return "do";
		case T_DOUBLE: return "double";
		case T_ELSE: return "else";
		case T_ENUM: return "enum";
		case T_EXTERN: return "extern";
		case T_FALSE: return "false";
		case T_FLOAT: return "float";
		case T_FOR: return "for";
		case T_GOTO: return "goto";
		case T_IF: return "if";
		case T_INLINE: return "inline";
		case T_INT: return "int";
		case T_LONG: return "long";
		case T_NULLPTR: return "nullptr";
		case T_REGISTER: return "register";
		case T_RESTRICT: return "restrict";
		case T_RETURN: return "return";
		case T_SHORT: return "short";
		case T_SIGNED: return "signed";
		case T_SIZEOF: return "sizeof";
		case T_STATIC: return "static";
		case T_STATIC_ASSERT: return "static_assert";
		case T_STRUCT: return "struct";
		case T_SWITCH: return "switch";
		case T_THREAD_LOCAL: return "thread_local";
		case T_TRUE: return "true";
		case T_TYPEDEF: return "typedef";
		case T_TYPEOF: return "typeof";
		case T_TYPEOF_UNQUAL: return "typeof_unqual";
		case T_UNION: return "union";
		case T_UNSIGNED: return "unsigned";
		case T_VOID: return "void";
		case T_VOLATILE: return "volatile";
		case T_WHILE: return "while";
		case T_MACRO_IF: return "#if";
		case T_MACRO_ELIF: return "#elif";
		case T_MACRO_ELSE: return "#else";
		case T_MACRO_ENDIF: return "#endif";
		case T_MACRO_IFDEF: return "#ifdef";
		case T_MACRO_IFNDEF: return "#ifndef";
		case T_MACRO_ELIFDEF: return "#elifdef";
		case T_MACRO_ELIFNDEF: return "#elifndef";
		case T_MACRO_DEFINE: return "#define";
		case T_MACRO_UNDEF: return "#undef";
		case T_MACRO_INCLUDE: return "#include";
		case T_MACRO_EMBED: return "#embed";
		case T_MACRO_LINE: return "#line";
		case T_MACRO_ERROR: return "#error";
		case T_MACRO_WARNING: return "#warning";
		case T_MACRO_PRAGMA: return "#pragma";
		case T_MACRO_DEFINDED: return "#defined";
		case T_MACRO___HAS_INCLUDE: return "__has_include";
		case T_MACRO___HAS_EMBED: return "__has_embed";
		case T_MACRO___HAS_C_ATTRIBUTE: return "__has_c_attribute";
		case T_MACRO_ASM: return "__asm__";
		case T_MACRO_FORTRAN: return "__fortran__";
		case T_WHITESPACE_TAB: return "\t";
		case T_WHITESPACE_LINE_FEED: return "\n";
		case T_WHITESPACE_LINE_TABULATION: return "\v";
		case T_WHITESPACE_FORM_FEED: return "\f";
		case T_WHITESPACE_CARRIAGE_RETURN: return "\r";
		case T_WHITESPACE_SPACE: return " ";
		case T_WHITESPACE_NEXT_LINE: return "\\";
		case T_EOF: return "\0";
		default: assert(0 && "TokenType_C (type) not implemented!");
	}
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
	for (TokenType_C type = 0; type <= T_EOF; type++) {
		if (lexer_c_token_type_is_in_expected_token_types(type, TOKEN_TYPE_C_WITH_NO_REPRESENTATION_NUM, TOKEN_TYPE_C_WITH_NO_REPRESENTATION)) {
			switch (type) {
				/**
				 * If the previous TOKEN_TYPE_C (excluding all whitespaces)
				 * is T_MACRO_INCLUDE, this function searches for '<' in lexer->pbuf.
				 * If found, it then searches for '>'. The text between '<' and '>'
				 * is set as Token_C->value.
				 *
				 * ERRORS
				 *	If no '>' character will be encountered, LEXER_NEXT_FAILED is returned and Lexer_C->error is set to 'missing terminating > character'..
				 *	If Token_C_>value would exceed T_MACRO_INCLUDE_LIBRARY_PATH_MAX_LEN, LEXER_NEXT_FAILED is returned and Lexer_C->error is set to 'file name too long'.
				 */
				case T_MACRO_INCLUDE_LIBRARY_PATH: {
					for (int i = lexer->tokens_num - 1; i >= 0; i--) {
						if (lexer_c_token_type_is_in_expected_token_types(lexer->tokens[i]->type, TOKEN_TYPE_C_ALL_WHITESPACES)) {
							continue;
						}

						if (!lexer_c_token_type_is_in_expected_token_types(lexer->tokens[i]->type, 1, T_MACRO_INCLUDE)) {
							break;
						}

						if (*lexer->pbuf != '<') {
							break;
						}

						char *start = lexer->pbuf + 1;
						char *end = strstr(start, ">");

						if (end == NULL) {
							lexer->error = "missing terminating > character";

							return LEXER_NEXT_FAILED;
						}

						if (end == start) {
							break;
						}

						size_t length = end - start;

						if (length >= T_MACRO_INCLUDE_LIBRARY_PATH_MAX_LEN) {
							lexer->error = "filename too long";

							return LEXER_NEXT_FAILED;
						}

						char *value = (char *)mmap(NULL, T_MACRO_INCLUDE_LIBRARY_PATH_MAX_LEN, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

						strncpy(value, start, length);

						lexer->pbuf = end + 1;

						return lexer_c_create_token(type, value);
					}
				} break;
				/**
				 * If Lexer_C->pbuf (i.e., the first character) is an '_' or an alpha character,
				 * and as long as the character is either '_' or alphanumeric, all valid characters
				 * are stored as the value of the identifier, and pbuf is advanced past the identifier.
				 */
				case T_IDENTIFIER: {
					char *end;

					for (end = lexer->pbuf; *end != '\0'; end++) {
						if (end == lexer->pbuf) {
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

					if (*end == '\0') {
						break;
					}

					size_t length = end - lexer->pbuf;

                                        if (length >= T_IDENTIFIER_MAX_LEN) {
						lexer->error = "identifier too long";

                                                return LEXER_NEXT_FAILED;
                                        }

					char *value = (char *)mmap(NULL, T_IDENTIFIER_MAX_LEN, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

                                        strncpy(value, lexer->pbuf, length);

                                        lexer->pbuf = end;

                                        return lexer_c_create_token(type, value);
				} break;
				/**
				 * If Lexer_C->pbuf matches the string "//", all characters are read as a comment until a newline or EOF is encountered.
				 */
				case T_COMMENT_LINE: {
					if (strlen(lexer->pbuf) < 2) {
						break;
					}

					if (strncmp(lexer->pbuf, "//" , 2) != 0) {
						break;
					}

					char *end = NULL;

					for (end = lexer->pbuf; *end != '\0' && *end != '\n'; end++);

					if (end == NULL) {
						return lexer_c_create_token(type, "//");
					}

					size_t length = end - lexer->pbuf;

					if (length >= T_COMMENT_LINE_MAX_LEN) {
						lexer->error = "comment too long";

						return LEXER_NEXT_FAILED;
					}

					char *value = (char *)mmap(NULL, T_COMMENT_LINE_MAX_LEN, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

					strncpy(value, lexer->pbuf, length);

					lexer->pbuf = end;

					return lexer_c_create_token(type, value);
				} break;
				/**
				 * If Lexer_C->pbuf matches the character '/' followed by a '*', all characters are read as a multi line comment until a '*' and a '/' is encountered.
				 */
				case T_COMMENT_MULTILINE: {
					if (strlen(lexer->pbuf) < 2) {
						break;
					}

					if (strncmp(lexer->pbuf, "/*" , 2) != 0) {
						break;
					}

					const char *start = lexer->pbuf + 2;
					char *end = strstr(start, "*/");

					if (end == NULL) {
						lexer->error = "unterminated comment";

						return LEXER_NEXT_FAILED;
					}

					if (end == start) {

						return lexer_c_create_token(type, "/**/");
					}

					end++;

					size_t length = end + 1 - lexer->pbuf;

					if (length >= T_COMMENT_MULTILINE_MAX_LEN) {
						lexer->error = "multi line comment too long";

						return LEXER_NEXT_FAILED;
					}

					char *value = (char *)mmap(NULL, T_COMMENT_MULTILINE_MAX_LEN, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

					strncpy(value, lexer->pbuf, length);

					lexer->pbuf = end + 1;

					return lexer_c_create_token(type, value);
				} break;
				/**
				 * If Lexer_C->pbuf matches the character '"', all characters are read as a string until another '"' is encountered (unless it is escaped with '\').
				 */
				case T_STRING: {
					if (*lexer->pbuf != '"') {
						break;
					}

					char *start = lexer->pbuf + 1;
					char *end;

					for (end = start; *end != '\0'; end++) {
						if (start == end && *start == '"') {
							return lexer_c_create_token(type, "");
						}

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

					size_t length = end - start;

					if (length >= T_STRING_MAX_LEN) {
						lexer->error = "string too long";

						return LEXER_NEXT_FAILED;
					}

					char *value = (char *)mmap(NULL, T_STRING_MAX_LEN, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

					strncpy(value, start, length);

					lexer->pbuf = end + 1;

					return lexer_c_create_token(type, value);
				} break;
				case T_TYPE_CAST:

				break;
				/**
				 * If Lexer_C->pbuf is a digit, all subsequent characters are read as a number as long as they are digits or a '.'.
				 *
				 * ERRORS
				 *	If more than one '.' is encountered, LEXER_NEXT_FAILED is returned and Lexer_C->error is set to 'too many decimal points in number'.
				 */
				case T_NUMBER: {
					if (isdigit(*lexer->pbuf) == 0) {
						break;
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

					size_t length = end - start;

					if (length >= T_NUMBER_MAX_LEN) {
						lexer->error = "number too long";

						return LEXER_NEXT_FAILED;
					}

					char *value = (char *)mmap(NULL, T_NUMBER_MAX_LEN, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

					strncpy(value, start, length);

					lexer->pbuf = end;

					return lexer_c_create_token(type, value);
				} break;
				default: assert(0 && "TokenType_C (type) not implemented!");
			}

			continue;
		}

		const char* token_type_representation = lexer_c_token_type_representation(type);

		char *found = strstr(lexer->pbuf, token_type_representation);

		if (found == lexer->pbuf && (type != T_EOF) == (*lexer->pbuf != '\0')) {
			lexer->pbuf += strlen(token_type_representation);

			return lexer_c_create_token(type, token_type_representation);
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

		if (lexer_c_push_back_token(lexer, token) == -1) {
			return -1;
		}
	} while (token->type != T_EOF);

	return 0;
}
