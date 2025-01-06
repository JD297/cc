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
    #include "asset_test_preprocessor_c_create_arguments.h"

    Preprocessor_C *preprocessor;

    assert((preprocessor = preprocessor_c_create(include_dirs, source_files, defines)) != PREPROCESSOR_CREATION_FAILED);

    TokenList_C *list;
    
    assert((list = token_list_c_create()) != LIST_CREATION_FAILED);
    assert(token_list_c_push_back(list, test_token_c_create(T_MACRO_UNDEF, "#undef")) == 0);
    assert(token_list_c_push_back(list, test_token_c_create(T_WHITESPACE_SPACE, " ")) == 0);
    assert(token_list_c_push_back(list, test_token_c_create(T_IDENTIFIER, "TARGET")) == 0);
    assert(token_list_c_push_back(list, test_token_c_create(T_EOF, "\0")) == 0);

    Token_C **ptoken = list->elements;
    
    assert((*ptoken)->type == T_MACRO_UNDEF);

    assert(token_list_named_c_get(preprocessor->defines, "TARGET") != NULL);

    assert(preprocessor_c_parse_next(preprocessor, list, &ptoken) == 0);

    assert(token_list_named_c_get(preprocessor->defines, "TARGET") == NULL);
    
    assert((*ptoken)->type == T_EOF);

    FREE(preprocessor);

    return 0;
}
