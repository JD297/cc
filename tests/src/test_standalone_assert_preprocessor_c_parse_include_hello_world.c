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

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);

    #include "asset_test_token_list_source_hello_world.h"

    Token_C **ptoken = asset_test_token_list_hello_world->elements;
    
    assert((*ptoken)->type == T_MACRO_INCLUDE);

    assert(preprocessor_c_parse_next(preprocessor, asset_test_token_list_hello_world, &ptoken) == 0);

    #include "asset_test_token_list_source_usr_include_stdio.h"

    assert(asset_test_token_list_source_preprocessed_usr_include_stdio->num == preprocessor->output->num);

    for (size_t i = 0; i < asset_test_token_list_source_preprocessed_usr_include_stdio->num; i++) {
        assert(asset_test_token_list_source_preprocessed_usr_include_stdio->elements[i]->type == preprocessor->output->elements[i]->type);
        assert(strcmp(asset_test_token_list_source_preprocessed_usr_include_stdio->elements[i]->value, preprocessor->output->elements[i]->value) == 0);
    }

    FREE(preprocessor);

    return 0;
}
