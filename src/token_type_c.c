#include <stdlib.h>
#include <string.h>

#include <jd297/sv.h>

#include "token_type_c.h"

/*
static const TokenType_C_LookupEntry token_type_c_lookup_preprocessor_entries[] = {
	{.value = {.value = "ifndef", .len = 6}, .type = T_MACRO_IFNDEF},
	{.value = {.value = "if", .len = 2}, .type = T_MACRO_IF},
	{.value = {.value = "elif", .len = 4}, .type = T_MACRO_ELIF},
	{.value = {.value = "else", .len = 4}, .type = T_MACRO_ELSE},
	{.value = {.value = "endif", .len = 5}, .type = T_MACRO_ENDIF},
	{.value = {.value = "define", .len = 6}, .type = T_MACRO_DEFINE},
	{.value = {.value = "undef", .len = 5}, .type = T_MACRO_UNDEF},
	{.value = {.value = "include", .len = 7}, .type = T_MACRO_INCLUDE},
	{.value = {.value = "line", .len = 4}, .type = T_MACRO_LINE},
	{.value = {.value = "error", .len = 5}, .type = T_MACRO_ERROR},
	{.value = {.value = "pragma", .len = 6}, .type = T_MACRO_PRAGMA},
};
*/

TokenType_C token_type_c_lookup_keyword(sv_t *key)
{
	const char *character2 = key->value + 1;

	switch (*key->value)
	{
		case 'a': {
			if (key->len == 4 && memcmp(character2, "uto", 3) == 0) {
				return T_AUTO;
			}
		} break;
		case 'b': {
			if (key->len == 5 && memcmp(character2, "reak", 4) == 0) {
				return T_BREAK;
			}
		} break;
		case 'c': {
			switch (key->len) {
				case 4: {
					if (memcmp(character2, "ase", 3) == 0) {
						return T_CASE;
					}
					
					if (memcmp(character2, "har", 3) == 0) {
						return T_CHAR;
					}
				} break;
				case 5: {
					if (memcmp(character2, "onst", 4) == 0) {
						return T_CONST;
					}
				}  break;
				case 8: {
					if (memcmp(character2, "ontinue", 7) == 0) {
						return T_CONTINUE;
					}	
				} break;
				default: break;
			}
		} break;		
		case 'd': {
			switch (key->len) {
				case 2: {
					if (*character2 == 'o') {
						return T_DO;
					}
				} break;
				case 6: {
					if (memcmp(character2, "ouble", 5) == 0) {
						return T_DOUBLE;
					}
				} break;	
				case 7: {
					if (memcmp(character2, "efault", 6) == 0) {
						return T_DEFAULT;
					}
				} break;
				default: break;
			}
		} break;
		case 'e': {
			switch (key->len) {
				case 4: {
					if (memcmp(character2, "lse", 3) == 0) {
						return T_ELSE;
					}
					
					if (memcmp(character2, "num", 3) == 0) {
						return T_ENUM;
					}
				} break;
				case 6: {
					if (memcmp(character2, "xtern", 5) == 0) {
						return T_EXTERN;
					}
				} break;
				default: break;
			}
		} break;
		case 'f': {
			switch (key->len) {
				case 3: {
					if (memcmp(character2, "or", 2) == 0) {
						return T_FOR;
					}
				} break;
				case 5: {
					if (memcmp(character2, "loat", 4) == 0) {
						return T_FLOAT;
					}
				} break;
				default: break;
			}
		} break;		
		case 'g': {
			if (key->len == 4 && memcmp(character2, "oto", 3) == 0) {
				return T_GOTO;
			}
		} break;
		case 'i': {
			switch (key->len) {
				case 2: {
					if (*character2 == 'f') {
						return T_IF;
					}
				} break;
				case 3: {
					if (memcmp(character2, "nt", 2) == 0) {
						return T_INT;
					}
				} break;
				default: break;
			}
		} break;
		case 'l': {
			if (key->len == 4 && memcmp(character2, "ong", 3) == 0) {
				return T_LONG;
			}
		} break;
		case 'r': {
			switch (key->len) {
				case 6: {
					if (memcmp(character2, "eturn", 5) == 0) {
						return T_RETURN;
					}
				} break;
				case 8: {
					if (memcmp(character2, "egister", 7) == 0) {
						return T_REGISTER;
					}
				} break;
				default: break;
			}
		} break;
		case 's': {
			switch (key->len) {
				case 6: {
					if (memcmp(character2, "igned", 5) == 0) {
						return T_SIGNED;
					}
					
					if (memcmp(character2, "izeof", 5) == 0) {
						return T_SIZEOF;
					}
					
					if (memcmp(character2, "tatic", 5) == 0) {
						return T_STATIC;
					}
					
					if (memcmp(character2, "truct", 5) == 0) {
						return T_STRUCT;
					}
					
					if (memcmp(character2, "witch", 5) == 0) {
						return T_SWITCH;
					}
				} break;
				case 5: {
					if (memcmp(character2, "hort", 4) == 0) {
						return T_SHORT;
					}
				} break;
				default: break;
			}
		} break;
		case 't': {
			if (key->len == 7 && memcmp(character2, "ypedef", 6) == 0) {
				return T_TYPEDEF;
			}
		} break;
		case 'u': {
			switch (key->len) {
				case 5: {
					if (memcmp(character2, "nion", 4) == 0) {
						return T_UNION;
					}
				} break;
				case 8: {
					if (memcmp(character2, "nsigned", 7) == 0) {
						return T_UNSIGNED;
					}
				} break;
				default: break;
			}
		} break;
		case 'v': {
			switch (key->len) {
				case 4: {
					if (memcmp(character2, "oid", 3) == 0) {
						return T_VOID;
					}
				} break;
				case 8: {
					if (memcmp(character2, "olatile", 7) == 0) {
						return T_VOLATILE;
					}
				} break;
				default: break;
			}
		} break;
		case 'w': {
			if (key->len == 5 && memcmp(character2, "hile", 4) == 0) {
				return T_WHILE;
			}
		} break;
		default: break;
	}
	
	return T_IDENTIFIER;
}

/* TODO
TokenType_C_LookupEntry *token_type_c_lookup_preprocessor(sv_t *key)
{
	TokenType_C_LookupEntry key_entry;
	key_entry.value = *key;

	return bsearch(&key_entry, token_type_c_lookup_preprocessor_entries, TOKEN_TYPE_C_LOOKUP_PREPROCESSOR_ENTRIES_NMEMB, sizeof(TokenType_C_LookupEntry), token_type_c_lookup_entry_cmp);
}
*/
