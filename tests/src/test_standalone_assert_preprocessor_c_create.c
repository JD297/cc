#include "preprocessor_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    #include "asset_test_preprocessor_c_create_arguments.h"

    Preprocessor_C *preprocessor;

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);

	assert(preprocessor->include_dirs->num == 3);
    assert(preprocessor->include_dirs->elements[0] == include_assets_prefix);
    assert(preprocessor->include_dirs->elements[1] == include_local_prefix);
    assert(preprocessor->include_dirs->elements[2] == include_system_prefix);

    assert(preprocessor->source_files->num == 1);
    assert(preprocessor->source_files->elements[0] == asset_test_hello_world);

    assert(preprocessor->lexers != LIST_CREATION_FAILED);
    assert(preprocessor->lexers->len == LEXER_LIST_INIT_LEN);

    assert(preprocessor->defines != LIST_CREATION_FAILED);
    assert(preprocessor->defines->len == TOKEN_LIST_NAMED_INIT_LEN);
    assert(preprocessor->defines->num == 2);

	assert(preprocessor->output != LIST_CREATION_FAILED);
    assert(preprocessor->output->len == LEXER_LIST_INIT_LEN);
    assert(preprocessor->output->num == 0);

    assert(preprocessor->error == NULL);

    FREE(preprocessor);
    FREE(defines);
    FREE(source_files);
    FREE(include_dirs);

    return 0;
}
