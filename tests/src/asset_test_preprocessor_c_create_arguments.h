StrList *include_dirs;

char* include_assets_prefix = "tests/assets";
char* include_local_prefix = "tests/assets/usr/local/include";
char* include_system_prefix = "tests/assets/usr/include";

assert((include_dirs = str_list_create()) != LIST_CREATION_FAILED);

assert(str_list_push_back(include_dirs, include_assets_prefix) != -1);
assert(str_list_push_back(include_dirs, include_local_prefix) != -1);
assert(str_list_push_back(include_dirs, include_system_prefix) != -1);

StrList *source_files;

char* asset_test_hello_world = "tests/assets/asset_test_hello_world.c";
assert((source_files = str_list_create()) != LIST_CREATION_FAILED);
assert(str_list_push_back(source_files, asset_test_hello_world) != -1);

TokenListNamed_C *defines;

assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);

TokenList_C *target;
assert((target = token_list_c_create()) != LIST_CREATION_FAILED);
assert(token_list_c_push_back(target, token_c_create(T_STRING, "hello")) != -1);
assert(token_list_named_c_push_back(defines, target, "TARGET") != -1);

TokenList_C *expr;
assert((expr = token_list_c_create()) != LIST_CREATION_FAILED);
assert(token_list_c_push_back(expr, token_c_create(T_OPEN_PARENT, "(")) != -1);
assert(token_list_c_push_back(expr, token_c_create(T_INT, "29")) != -1);
assert(token_list_c_push_back(expr, token_c_create(T_PLUS, "+")) != -1);
assert(token_list_c_push_back(expr, token_c_create(T_INT, "7")) != -1);
assert(token_list_c_push_back(expr, token_c_create(T_CLOSING_PARENT, ")")) != -1);
assert(token_list_named_c_push_back(defines, expr, "EXPR") != -1);

