#define ASSET_TEST_C_TOKEN_NUM 36

Token_C asset_test_tokens[ASSET_TEST_C_TOKEN_NUM] = {
    {T_MACRO_INCLUDE, "#include"}, {T_WHITESPACE_SPACE, " "}, {T_MACRO_INCLUDE_LIBRARY_PATH, "stdio.h"}, {T_WHITESPACE_LINE_FEED, "\n"},              // LINE 1
    {T_WHITESPACE_LINE_FEED, "\n"},                                                                                                                   // LINE 2
    {T_INT, "int"}, {T_WHITESPACE_SPACE, " "}, {T_IDENTIFIER, "main"}, {T_OPEN_PARENT, "("}, {T_CLOSING_PARENT, ")"}, {T_WHITESPACE_LINE_FEED, "\n"}, // LINE 3
    {T_OPEN_BRACE, "{"}, {T_WHITESPACE_LINE_FEED, "\n"},                                                                                              // LINE 4
    {T_WHITESPACE_TAB, "\t"}, {T_COMMENT_LINE, "// This is a comment!"}, {T_WHITESPACE_LINE_FEED, "\n"},                                              // LINE 5
    {T_WHITESPACE_TAB, "\t"}, {T_COMMENT_MULTILINE, "/*\n\t * This is a multi line\n\t * comment!\n\t */"}, {T_WHITESPACE_LINE_FEED, "\n"},           // LINE 6 - 9
    {T_WHITESPACE_TAB, "\t"}, {T_IDENTIFIER, "printf"}, {T_OPEN_PARENT, "("}, {T_STRING, "Hello world!\\n"}, {T_CLOSING_PARENT, ")"},                 // LINE 10
    {T_SEMICOLON, ";"}, {T_WHITESPACE_LINE_FEED, "\n"},                                                                                               // LINE 10
    {T_WHITESPACE_LINE_FEED, "\n"},                                                                                                                   // LINE 11
    {T_WHITESPACE_TAB, "\t"}, {T_RETURN, "return"}, {T_WHITESPACE_SPACE, " "}, {T_NUMBER, "0"}, {T_SEMICOLON, ";"}, {T_WHITESPACE_LINE_FEED, "\n"},   // LINE 12
    {T_CLOSING_BRACE, "}"}, {T_WHITESPACE_LINE_FEED, "\n"}, {T_EOF, "\0"},                                                                            // LINE 13
};

