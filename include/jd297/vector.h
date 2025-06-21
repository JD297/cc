/**
BSD 2-Clause License

Copyright (c) 2025, JD297
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef JD297_VECTOR_H
#define JD297_VECTOR_H

#include <stddef.h>

typedef struct {
    void **elements;
    size_t len;
    size_t num;
} vector_t;

extern void vec_free(vector_t *vec);

extern int vec_push_back(vector_t *vec, void *el);

#define vec_size(vec_ptr) (vec_ptr)->num

#define vec_capacity(vec_ptr) (vec_ptr)->len

#define vec_set(vec_ptr, index, value) (vec_ptr)->elements[(index)] = (value)

#define vec_at(vec_ptr, index) (vec_ptr)->elements[(index)]

#define vec_at_last(vec_ptr) (vec_ptr)->elements[(vec_ptr)->num > 0 ? (vec_ptr)->num- 1 : 0]

#define vec_begin(vec_ptr) (vec_ptr)->elements

#define vec_end(vec_ptr) (vec_ptr)->elements + (vec_ptr)->num

#endif

#ifdef JD297_VECTOR_IMPLEMENTATION

#include <stddef.h>
#include <stdlib.h>

#ifndef JD297_VECTOR_RESIZE_FACTOR
#define JD297_VECTOR_RESIZE_FACTOR 2
#elif JD297_VECTOR_RESIZE_FACTOR < 2
#error "JD297_VECTOR_RESIZE_FACTOR must be greater then 2!"
#endif

void vec_free(vector_t *vec)
{
    free(vec->elements);
}

int vec_push_back(vector_t *vec, void *el)
{
    if (vec->num == vec->len) {
        size_t new_len = (vec->len == 0) ? 1 : vec->len * JD297_VECTOR_RESIZE_FACTOR;

        vec->elements = (void **)realloc(vec->elements, new_len * sizeof(void *));

        if (vec->elements == NULL) {
            return -1;
        }

        vec->len = new_len;
    }

    vec->elements[vec->num++] = el;

    return 0;
}

#endif
