#include "lexer_c.h"
#include "token_type_c.h"
#include "token_c.h"
#include <jd297/sv.h>
#include <jd297/lmap_sv.h>

#include <assert.h>

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int lexer_c_is_at_end(Lexer_C *lexer);

static char lexer_c_advance(Lexer_C *lexer);

static char lexer_c_peek(Lexer_C *lexer, size_t n);

static int lexer_c_match(Lexer_C *lexer, char expected);

static void lexer_c_set_token(Lexer_C *lexer, Token_C *token, const TokenType_C type);

static int lexer_c_isalnum(Lexer_C *lexer);

void lexer_c_create(Lexer_C *lexer, const char *pathname, const char *source)
{
	*lexer = (Lexer_C) {
		.start = source,
		.current = source,
		.loc = (Lexer_Location_C) {
			.pathname = pathname,
			.line = 1,
			.col = 0
		}
	};
}

int lexer_c_next(Lexer_C *lexer, Token_C *token) // return type TokenType ?? easy equalty checks
{
	switch (lexer_c_advance(lexer))
	{
        case '(':
			lexer_c_set_token(lexer, token, T_OPEN_PARENT);
			break;
		case ')':
			lexer_c_set_token(lexer, token, T_CLOSING_PARENT);
			break;
		case '[':
			lexer_c_set_token(lexer, token, T_OPEN_BRACKET);
			break;
		case ']':
			lexer_c_set_token(lexer, token, T_CLOSING_BRACKET);
			break;
		case '{':
			lexer_c_set_token(lexer, token, T_OPEN_BRACE);
			break;
		case '}':
			lexer_c_set_token(lexer, token, T_CLOSING_BRACE);
			break;
		case '~':
			lexer_c_set_token(lexer, token, T_TILDE);
			break;
		case ',':
			lexer_c_set_token(lexer, token, T_COMMA);
			break;
		case ':':
			lexer_c_set_token(lexer, token, T_COLON);
			break;
		case ';':
			lexer_c_set_token(lexer, token, T_SEMICOLON);
			break;
		case '?':
			lexer_c_set_token(lexer, token, T_TERNARY);
			break;
		case '#':
			// TODO parse the line statement to manipulate the lexer->loc
			// TODO set preprocessor mode else normal mode ??
			// TODO if in preprocessor mode return whitespace as tokens (exspecielly newlines, remember there are also escaped new lines in cpp mode) ??
			assert(0 && "TODO implement preprocessor");
			break;
		case '=':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_EQUAL_TO : T_ASSIGNMENT);
			break;
		case '*':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_MULTIPLY_ASSIGN : T_MULTIPLY);
			break;
		case '%':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_MODULUS_ASSIGN : T_MODULUS);
			break;
		case '^':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_BITWISE_XOR_ASSIGN : T_BITWISE_XOR);
			break;
		case '!':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_NOT_EQUAL_TO : T_LOGICAL_NOT);
			break;	
		case '|':
        	lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_BITWISE_OR_ASSIGN : lexer_c_match(lexer, '|') == 0 ? T_LOGICAL_OR : T_BITWISE_OR);
        	break;
    	case '&':
        	lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_BITWISE_AND_ASSIGN : lexer_c_match(lexer, '&') == 0 ? T_LOGICAL_AND : T_BITWISE_AND);
        	break;
    	case '+':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_PLUS_ASSIGN : lexer_c_match(lexer, '+') == 0 ? T_INCREMENT : T_PLUS);
			break;
		case '-':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_MINUS_ASSIGN : lexer_c_match(lexer, '-') == 0 ? T_DECREMENT : lexer_c_match(lexer, '>') == 0 ? T_ARROW : T_MINUS);
			break;
		case '<':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_LESS_THAN_OR_EQUAL_TO : lexer_c_match(lexer, '<') == 0 ? lexer_c_match(lexer, '<') == 0 ? T_BITWISE_LEFTSHIFT_ASSIGN : T_BITWISE_LEFTSHIFT : T_LESS_THAN);
			break;
		case '>':
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_GREATER_THAN_OR_EQUAL_TO : lexer_c_match(lexer, '>') == 0 ? lexer_c_match(lexer, '>') == 0 ? T_BITWISE_RIGHTSHIFT_ASSIGN : T_BITWISE_RIGHTSHIFT : T_GREATER_THAN);
			break;
		case '/': {

			if (lexer_c_match(lexer, '/') == 0) {
				while (lexer_c_peek(lexer, 0) != '\n' && !lexer_c_is_at_end(lexer)) {
					lexer_c_advance(lexer);
				}

				return lexer_c_next(lexer, token);
			}

			if (lexer_c_match(lexer, '*') == 0) {
				
				while (1) {
					if (lexer_c_peek(lexer, 0) == '*' && lexer_c_peek(lexer, 1) == '/') {
						lexer_c_advance(lexer);
						lexer_c_advance(lexer);

						lexer->start = lexer->current;
						
						return lexer_c_next(lexer, token);
					}

					if (lexer_c_is_at_end(lexer)) {
						assert(0 && "NOT MULTILNE COMMENT NOT TERMINATED TODO:");	
					}
					
					if (lexer_c_peek(lexer, 0) == '\n') {
						++lexer->loc.line;
						lexer->loc.col = 0;

						lexer->current++;

						continue;
					}
					
					lexer_c_advance(lexer);
				}
			}
			
			lexer_c_set_token(lexer, token, lexer_c_match(lexer, '=') == 0 ? T_DIVIDE_ASSIGN : T_DIVIDE);
		} break;
		
		case '.': {
			switch (lexer_c_peek(lexer, 0)) {
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					goto L_FLOATING_CONSTANT;
				case '.': {
					if (lexer_c_peek(lexer, 1) == '.') {
						lexer->current += 2;

						lexer_c_set_token(lexer, token, T_DOT_DOT_DOT);

						break;
					}
				}
				default:
					lexer_c_set_token(lexer, token, T_DOT);
			}
		} break;
		case 'L':
			if (lexer_c_peek(lexer, 0) == '\'') {
				goto L_CHARACTER_CONSTANT;
			}
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I': case 'J': case 'K': 
        case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
        case 's': case 't': case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z': 
        case '_': {
        	while (lexer_c_isalnum(lexer) != 0) {
        		lexer_c_advance(lexer);
        	}
        	
        	lexer_c_set_token(lexer, token, T_IDENTIFIER);
        	
        	TokenType_C_LookupEntry *entry = lmap_sv_get(&token_type_c_lookup_keywords, &token->view);

        	if (entry != NULL) {
        		token->type = entry->type;
        	}    	
        } break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9': {
			char *endptr;
			unsigned long int lu;
			
			errno = 0;
			lu = strtoul(lexer->start, &endptr, 0);
			
			// TODO errno ERANGE
			// TODO error: integer literal is too large to be represented in any integer type
			
			// TODO u U + l L => advance
			// TODO error: invalid suffix 'ZZ' on integer constant

			switch (*endptr) {
				case 'f': case 'F':
				case 'e': case 'E':
				case '.':
					goto L_FLOATING_CONSTANT;
			}
			
			lexer->current = endptr;
			token->literal.lu = lu;
			lexer_c_set_token(lexer, token, T_INTEGER_CONSTANT);
		} break;
		L_FLOATING_CONSTANT: {
			char *endptr;
			long double Lf;
			
			errno = 0;
			Lf = strtold(lexer->start, &endptr);
			
			// TODO errno ERANGE
			//  warning: magnitude of floating-point constant too large for type 'double'
			
			// TODO F f | l L => advance
			// TODO error: invalid suffix 'ZZ' on floating constant

			lexer->current = endptr; // TODO advance location | check everywhere because it was probably forgoten sometimes !!
			token->literal.Lf = Lf;
			lexer_c_set_token(lexer, token, T_FLOATING_CONSTANT);
		} break;
		L_CHARACTER_CONSTANT:
			lexer_c_advance(lexer);
		case '\'': {
			const char *end;

			for (end = lexer->current; *end != '\''; ++end) {
				if (*end == '\n' || *end == '\0') {
					printf("%s:%zu:%zu: error: missing terminating ' character\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);
					
					lexer->start = lexer->current = end;
		
					return lexer_c_next(lexer, token);
				}
			}

			switch (lexer_c_advance(lexer)) {
				case '\'': {
					printf("%s:%zu:%zu: error: empty character constant\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);

					lexer->start = lexer->current;
			
					return lexer_c_next(lexer, token);
				}
				case '\\': {
					switch (lexer_c_advance(lexer)) {
						case 'n': case 't': case 'v': case 'b':
						case 'r': case 'f': case 'a': case '\\':
						case '?': case '\'': case '\"': {
							if (lexer_c_advance(lexer) != '\'') {
								printf("%s:%zu:%zu: error: multi-character character constant\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);
						
								lexer->start = lexer->current = end + 1;
						
								return lexer_c_next(lexer, token);
							}
						} break;
						case 'x': {
							for (size_t i = 0; ; i++) {
								switch (lexer_c_advance(lexer)) {
									case '0': case '1': case '2': case '3': case '4':
									case '5': case '6': case '7': case '8': case '9':
									case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
									case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
										if (i >= 2) {
											printf("%s:%zu:%zu: error: hex escape sequence out of range\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);

											lexer->start = lexer->current = end + 1;
											
											return lexer_c_next(lexer, token);
										}

										continue;
									default: {
										printf("%s:%zu:%zu: error: \\x used with no following hex digits\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);
							
										lexer->start = lexer->current = end + 1;
								
										return lexer_c_next(lexer, token);
									}
									case '\'':
										if (i == 0) {
											printf("%s:%zu:%zu: error: \\x used with no following hex digits\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);

											lexer->start = lexer->current = end + 1;
											
											return lexer_c_next(lexer, token);
										}
										
										break;
								}
								
								break;
							}
						} break;
						case '0': case '1': case '2': case '3':
						case '4': case '5': case '6': case '7': {
							for (size_t i = 0; ; i++) {
								switch (lexer_c_advance(lexer)) {
									case '0': case '1': case '2': case '3':
									case '4': case '5': case '6': case '7':
										if (i >= 2) {
											printf("%s:%zu:%zu: error: octal escape sequence out of range\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);

											lexer->start = lexer->current = end + 1;
											
											return lexer_c_next(lexer, token);
										}

										continue;
									default: {
										printf("%s:%zu:%zu: error: octal escape sequence used with no following octal digits\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);
							
										lexer->start = lexer->current = end + 1;
								
										return lexer_c_next(lexer, token);
									}
									case '\'':
										break;
								}
								
								break;
							}
						} break;
						default: {
							printf("%s:%zu:%zu: error: unknown escape sequence\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);
						
							lexer->start = lexer->current = end + 1;
					
							return lexer_c_next(lexer, token);
						}
					}
				} break;
				default: {
					if (lexer_c_advance(lexer) != '\'') {
						printf("%s:%zu:%zu: error: multi-character character constant\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col);
						
						lexer->start = lexer->current = end + 1;
				
						return lexer_c_next(lexer, token);
					}
				} break;
			}
			
			lexer_c_set_token(lexer, token, T_CHARACTER_CONSTANT);
		} break;
		case '\n': {
			++lexer->loc.line;
			lexer->loc.col = 0;

			lexer->start = lexer->current;

			return lexer_c_next(lexer, token);
		} break;
		case ' ': case '\r': case '\t': case '\f': case '\v': {
			lexer->start = lexer->current;

			return lexer_c_next(lexer, token);
		} break;
		case '\0': {
			lexer_c_set_token(lexer, token, T_EOF);
		} break;
		default: {
			printf("%s:%zu:%zu: error: unexpected character: \""SV_FMT"\"\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col, 1, lexer->start);
			
			lexer->start = lexer->current;
		
			return lexer_c_next(lexer, token);
		}
	}

	lexer->start = lexer->current;

    return 0;
}

static int lexer_c_is_at_end(Lexer_C *lexer)
{
	return *lexer->current == '\0';
}

static char lexer_c_advance(Lexer_C *lexer)
{
	++lexer->loc.col;

	return *(lexer->current++);
}

static char lexer_c_peek(Lexer_C *lexer, size_t n)
{
	return *(lexer->current + n);
}

static int lexer_c_match(Lexer_C *lexer, char expected)
{
	if (*lexer->current != expected) {
		return -1;
	}

	lexer->current++;

	return 0;
}

static void lexer_c_set_token(Lexer_C *lexer, Token_C *token, const TokenType_C type)
{
	token->type = type;
	token->view.value = lexer->start;
	token->view.len = lexer->current - lexer->start;
}

static int lexer_c_isalnum(Lexer_C *lexer)
{
	if (*lexer->current >= 'A' && *lexer->current <= 'Z') {
		return 1;
	}
	
	if (*lexer->current >= 'a' && *lexer->current <= 'z') {
		return 1;
	}

	if (*lexer->current >= '0' && *lexer->current <= '9') {
		return 1;
	}

	if (*lexer->current == '_') {
		return 1;
	}

	return 0;
}
