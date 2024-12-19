#include "preprocessor_c.h"
#include "lexer_c.h"
#include "token_list_c.h"
#include "token_type_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    #include "asset_test_preprocessor_c_create_arguments.h"

    Preprocessor_C *preprocessor;

    assert((source_files = str_list_create()) != LIST_CREATION_FAILED);
    assert(str_list_push_back(source_files, "tests/assets/asset_test_conditional_macros.h") != -1);

    // IFNDEF
    assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);
    assert(preprocessor_c_run(preprocessor) == 0);

    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFNDEF") != NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF1") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF2") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELSE") == NULL);
    
    // ELIFNDEF1
    assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "IFNDEF") != -1);

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);
    assert(preprocessor_c_run(preprocessor) == 0);

    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFNDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF1") != NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF2") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELSE") == NULL);
    
    // ELIFNDEF2
    assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "IFNDEF") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFNDEF1") != -1);

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);
    assert(preprocessor_c_run(preprocessor) == 0);

    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFNDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF1") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF2") != NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELSE") == NULL);

    // ELIFDEF
    assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "IFNDEF") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFNDEF1") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFNDEF2") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFDEF") != -1);

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);
    assert(preprocessor_c_run(preprocessor) == 0);

    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFNDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF1") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF2") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFDEF") != NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELSE") == NULL);

    // ELIFDEF AND IFDEF
    assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "IFNDEF") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFNDEF1") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFNDEF2") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFDEF") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "IFDEF") != -1);

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);
    assert(preprocessor_c_run(preprocessor) == 0);

    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFNDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF1") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF2") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFDEF") != NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFDEF") != NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELSE") == NULL);
    
    // RESULT_ELSE
    assert((defines = token_list_named_c_create()) != LIST_CREATION_FAILED);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "IFNDEF") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFNDEF1") != -1);
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFNDEF2") != -1);
    /* assert(token_list_named_c_push_back(defines, token_list_c_create(), "ELIFDEF") != -1); <- this is correct */
    assert(token_list_named_c_push_back(defines, token_list_c_create(), "IFDEF") != -1); /* <- should not be parsed so we can define it anyway */

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);
    assert(preprocessor_c_run(preprocessor) == 0);

    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFNDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF1") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFNDEF2") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELIFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_IFDEF") == NULL);
    assert(token_list_named_c_get(preprocessor->defines, "RESULT_ELSE") != NULL);

    FREE(preprocessor);

    return 0;
}
