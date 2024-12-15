#include "preprocessor_c.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    #include "asset_test_preprocessor_c_create_arguments.h"

    Preprocessor_C *preprocessor;

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);

    assert(preprocessor_c_find_include_file(preprocessor, "does_not_exist.h", PREPROCESSOR_INCLUDE_MODE_LIBRARARY) == NULL);
    assert(preprocessor->error != NULL);
    preprocessor->error = NULL;

    assert(preprocessor_c_find_include_file(preprocessor, "unistd.h", PREPROCESSOR_INCLUDE_MODE_LIBRARARY) != NULL);
	assert(strcmp(preprocessor_c_find_include_file(preprocessor, "unistd.h", PREPROCESSOR_INCLUDE_MODE_LIBRARARY), include_system_prefix));

	assert(preprocessor_c_find_include_file(preprocessor, "string.h", PREPROCESSOR_INCLUDE_MODE_LIBRARARY) != NULL);
	assert(strcmp(preprocessor_c_find_include_file(preprocessor, "string.h", PREPROCESSOR_INCLUDE_MODE_LIBRARARY), include_local_prefix));

	assert(preprocessor_c_find_include_file(preprocessor, "asset_test_hello_world.h", PREPROCESSOR_INCLUDE_MODE_LIBRARARY) != NULL);
	assert(strcmp(preprocessor_c_find_include_file(preprocessor, "asset_test_hello_world.h", PREPROCESSOR_INCLUDE_MODE_LIBRARARY), include_assets_prefix));


    assert(preprocessor_c_find_include_file(preprocessor, "does_not_exist.h", PREPROCESSOR_INCLUDE_MODE_STRING) == NULL);
    assert(preprocessor->error != NULL);
    preprocessor->error = NULL;
    
    assert(preprocessor_c_find_include_file(preprocessor, "asset_test_hello_world.h", PREPROCESSOR_INCLUDE_MODE_STRING) != NULL);
	assert(strcmp(preprocessor_c_find_include_file(preprocessor, "asset_test_hello_world.h", PREPROCESSOR_INCLUDE_MODE_STRING), include_assets_prefix));

    assert(preprocessor_c_find_include_file(preprocessor, "tests/assets/asset_test_hello_world.h", PREPROCESSOR_INCLUDE_MODE_STRING) != NULL);
	assert(strcmp(preprocessor_c_find_include_file(preprocessor, "tests/assets/asset_test_hello_world.h", PREPROCESSOR_INCLUDE_MODE_STRING), include_assets_prefix));

    FREE(preprocessor);
    FREE(defines);
    FREE(source_files);
    FREE(include_dirs);

    return 0;
}
