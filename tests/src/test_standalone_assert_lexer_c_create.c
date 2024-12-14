#include "lexer_c.h"

#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#define FREE(X) munmap(X, sizeof(X))

int main()
{
    Lexer_C *lexer;

    assert((lexer = lexer_c_create(__FILE__)) != LEXER_CREATION_FAILED);
    assert(strcmp(lexer->pathname, __FILE__) == 0);
    assert(strlen(lexer->buf) >= __LINE__);
    assert(lexer->buf == lexer->pbuf);
    assert(lexer->tokens != NULL);
    assert(lexer->error == NULL);

    FREE(lexer);

    return 0;
}
