#define ASSET_TEST_TOKEN_LIST_SOURCE_HELLO_WORLD_FILEPATH "tests/assets/asset_test_hello_world.c"

#define list asset_test_token_list_hello_world

TokenList_C *list = token_list_c_create();

// LINE 1
token_list_c_push_back(list, token_c_create(T_MACRO_INCLUDE, "#include"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_MACRO_INCLUDE_LIBRARY_PATH, "stdio.h"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 2
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 3
token_list_c_push_back(list, token_c_create(T_INT, "int"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "main"));
token_list_c_push_back(list, token_c_create(T_OPEN_PARENT, "("));
token_list_c_push_back(list, token_c_create(T_CLOSING_PARENT, ")"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 4
token_list_c_push_back(list, token_c_create(T_OPEN_BRACE, "{"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 5
token_list_c_push_back(list, token_c_create(T_WHITESPACE_TAB, "\t"));
token_list_c_push_back(list, token_c_create(T_COMMENT_LINE, "// This is a comment!"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 6 - 9
token_list_c_push_back(list, token_c_create(T_WHITESPACE_TAB, "\t"));
token_list_c_push_back(list, token_c_create(T_COMMENT_MULTILINE, "/*\n\t * This is a multi line\n\t * comment!\n\t */"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 10
token_list_c_push_back(list, token_c_create(T_WHITESPACE_TAB, "\t"));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "printf"));
token_list_c_push_back(list, token_c_create(T_OPEN_PARENT, "("));
token_list_c_push_back(list, token_c_create(T_STRING, "Hello world!\\n"));
token_list_c_push_back(list, token_c_create(T_CLOSING_PARENT, ")"));
token_list_c_push_back(list, token_c_create(T_SEMICOLON, ";"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 11
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 12
token_list_c_push_back(list, token_c_create(T_WHITESPACE_TAB, "\t"));
token_list_c_push_back(list, token_c_create(T_RETURN, "return"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_NUMBER, "0"));
token_list_c_push_back(list, token_c_create(T_SEMICOLON, ";"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 13
token_list_c_push_back(list, token_c_create(T_CLOSING_BRACE, "}"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));
token_list_c_push_back(list, token_c_create(T_EOF, "\0"));

#undef list

#define list asset_test_token_list_preprocessed_hello_world

TokenList_C *list = token_list_c_create();

// LINE 1
token_list_c_push_back(list, token_c_create(T_INT, "int"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "main"));
token_list_c_push_back(list, token_c_create(T_OPEN_PARENT, "("));
token_list_c_push_back(list, token_c_create(T_CLOSING_PARENT, ")"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 2
token_list_c_push_back(list, token_c_create(T_OPEN_BRACE, "{"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 3
token_list_c_push_back(list, token_c_create(T_IDENTIFIER, "printf"));
token_list_c_push_back(list, token_c_create(T_OPEN_PARENT, "("));
token_list_c_push_back(list, token_c_create(T_STRING, "Hello world!\\n"));
token_list_c_push_back(list, token_c_create(T_CLOSING_PARENT, ")"));
token_list_c_push_back(list, token_c_create(T_SEMICOLON, ";"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 4
token_list_c_push_back(list, token_c_create(T_RETURN, "return"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_SPACE, " "));
token_list_c_push_back(list, token_c_create(T_NUMBER, "0"));
token_list_c_push_back(list, token_c_create(T_SEMICOLON, ";"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

// LINE 5
token_list_c_push_back(list, token_c_create(T_CLOSING_BRACE, "}"));
token_list_c_push_back(list, token_c_create(T_WHITESPACE_LINE_FEED, "\n"));

#undef list

