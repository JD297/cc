#define list asset_test_token_list_source_usr_include_stdio

TokenList_C *list = token_list_c_create();

// LINE 1
token_list_c_push_back(list, token_c_create(T_MACRO_IFNDEF, "#ifndef"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "LIBC_SYSTEM_EXAMPLE_STDIO_H"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 2
token_list_c_push_back(list, token_c_create(T_MACRO_DEFINE, "#define"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "LIBC_SYSTEM_EXAMPLE_STDIO_H"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 3
token_list_c_push_back(list, token_c_create(T_EXTERN, "extern"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_INT, "int"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "printf"));
token_list_c_push_back(list, token_c_create(T_OPEN_PARENT, "("));
token_list_c_push_back(list, token_c_create(T_CONST, "const"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_CHAR, "char"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_ASTERISK, "*"));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "format"));
token_list_c_push_back(list, token_c_create(T_COMMA, ","));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_DOT, "."));
token_list_c_push_back(list, token_c_create(T_DOT, "."));
token_list_c_push_back(list, token_c_create(T_DOT, "."));
token_list_c_push_back(list, token_c_create(T_CLOSING_PARENT, ")"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 4
token_list_c_push_back(list, token_c_create(T_MACRO_ENDIF, "#endif"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));
token_list_c_push_back(list, token_c_create(T_EOF, "\0"));

#undef list

#define list asset_test_token_list_source_preprocessed_usr_include_stdio

TokenList_C *list = token_list_c_create();

// LINE 1
token_list_c_push_back(list, token_c_create(T_EXTERN, "extern"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_INT, "int"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "printf"));
token_list_c_push_back(list, token_c_create(T_OPEN_PARENT, "("));
token_list_c_push_back(list, token_c_create(T_CONST, "const"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_CHAR, "char"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_ASTERISK, "*"));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "format"));
token_list_c_push_back(list, token_c_create(T_COMMA, ","));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_DOT, "."));
token_list_c_push_back(list, token_c_create(T_DOT, "."));
token_list_c_push_back(list, token_c_create(T_DOT, "."));
token_list_c_push_back(list, token_c_create(T_CLOSING_PARENT, ")"));
token_list_c_push_back(list, token_c_create(T_SEMICOLON, ";"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

#undef list

