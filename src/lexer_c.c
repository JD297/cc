#include "lexer_c.h"
#include "token_type_c.h"
#include "token_c.h"
#include <jd297/sv.h>

#include <assert.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int lexer_c_is_at_end(Lexer_C *lexer);

static char lexer_c_advance(Lexer_C *lexer);

static char lexer_c_peek(Lexer_C *lexer, size_t n);

static int lexer_c_match(Lexer_C *lexer, char expected);

static void lexer_c_set_token(Lexer_C *lexer, Token_C *token, const TokenType_C type);

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
			lexer_c_set_token(lexer, token, T_PREPROCESSOR);
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
			if (lexer_c_peek(lexer, 0) == '.' && lexer_c_peek(lexer, 1) == '.') {
				lexer->current += 2;
				/* TODO maybe we could do a variadic match(n, '.', '.') ?? */
				lexer_c_set_token(lexer, token, T_DOT_DOT_DOT);

				break;
			}
			
			// TODO check for a number:
				// true: advance and fallthorugh to NUMBER
				// false: set_token and break
		
			lexer_c_set_token(lexer, token, T_DOT);
		} break;
	
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
        case 's': case 't': case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z': 
        case '_': {
        	assert(0 && "IDENTIFIER / KEYWORDS");
        } break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9': {
			// TOOD +- can follow a number to
			// TODO . can be at first
			assert(0 && "NUMBER");
			break;
		}
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
			printf("%s:%zu:%zu: unexpected character: \""SV_FMT"\"\n", lexer->loc.pathname, lexer->loc.line, lexer->loc.col, 1, lexer->start);
			
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

	return *(lexer->current++); // TODO test this
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
