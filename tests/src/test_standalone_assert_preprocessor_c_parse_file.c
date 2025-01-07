#include "preprocessor_c.h"
#include "lexer_c.h"
#include "token_list_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

#define test_token_c_create(T, S) token_c_create((T), (S), strlen((S)))

int main()
{
    StrList *include_dirs;
    
    assert((include_dirs = str_list_create()) != LIST_CREATION_FAILED);
    assert(str_list_push_back(include_dirs, "tests/assets") != -1);

    StrList *source_files;

    assert((source_files = str_list_create()) != LIST_CREATION_FAILED);
    assert(str_list_push_back(source_files, "tests/assets/asset_test_file_macro.c") != -1);

    TokenListNamed_C *defines;

    assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);

    Preprocessor_C *preprocessor;

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);

    assert(preprocessor_c_run(preprocessor) != -1);

    TokenList_C *list;
    
    assert((list = token_list_c_create()) != LIST_CREATION_FAILED);
    assert(token_list_c_push_back(list, test_token_c_create(T_STRING, "\"tests/assets/asset_test_file_macro.c\"")) == 0);
    assert(token_list_c_push_back(list, test_token_c_create(T_EOF, "\0")) == 0);

    Token_C **etoken, **ctoken;

    for (etoken = list->elements, ctoken = preprocessor->output->elements; (*etoken)->type != T_EOF; etoken++, ctoken++) {
        assert((*etoken)->type == (*ctoken)->type);
        assert((*etoken)->len == (*ctoken)->len);
        assert(strncmp((*etoken)->value, (*ctoken)->value, (*etoken)->len) == 0);
    }

    FREE(preprocessor);

    return 0;
}
