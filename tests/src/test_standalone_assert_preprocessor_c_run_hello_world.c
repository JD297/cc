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
    #include "asset_test_token_list_source_hello_world.h"

    #include "asset_test_token_list_source_usr_include_stdio.h"

    #define token_list_expected_preprocessed asset_test_token_list_source_preprocessed_usr_include_stdio

    for (size_t i = 0; i < asset_test_token_list_preprocessed_hello_world->num; i++) {
        token_list_c_push_back(token_list_expected_preprocessed, asset_test_token_list_preprocessed_hello_world->elements[i]);
    }
    
    token_list_c_push_back(token_list_expected_preprocessed, token_c_create(T_EOF, "\0"));

    #include "asset_test_preprocessor_c_create_arguments.h"

    Preprocessor_C *preprocessor;

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);

    assert(preprocessor_c_run(preprocessor) == 0);

    assert(token_list_expected_preprocessed->num == preprocessor->output->num);

    for (size_t i = 0; i < token_list_expected_preprocessed->num; i++) {
        assert(token_list_expected_preprocessed->elements[i]->type == preprocessor->output->elements[i]->type);
        assert(strcmp(token_list_expected_preprocessed->elements[i]->value, preprocessor->output->elements[i]->value) == 0);
    }

    FREE(preprocessor);

    return 0;
}
