#include "lexer_c.h"

#include <assert.h>

int main()
{
    assert(token_type_c_is_in_expected_token_types(T_PLUS   , 1, T_PLUS                                           ) == 1);
    assert(token_type_c_is_in_expected_token_types(T_PLUS   , 1, T_MODULUS                                        ) == 0);
    assert(token_type_c_is_in_expected_token_types(T_PLUS   , 5, T_PLUS, T_MINUS, T_MULTIPLY, T_DIVIDE, T_MODULUS ) == 1);
    assert(token_type_c_is_in_expected_token_types(T_MODULUS, 4, T_PLUS, T_MINUS, T_MULTIPLY, T_DIVIDE            ) == 0);

    return 0;
}
