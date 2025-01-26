#ifndef JD297_CC_VECTOR_H
#define JD297_CC_VECTOR_H

#include <stddef.h>

typedef struct Vector {
    void **elements;
    size_t len;
    size_t num;
} Vector;

#define VECTOR_RESIZE_FACTOR 2

extern void *vector_create();

extern void vector_destroy(Vector *vector);

extern int vector_push_back(Vector *vector, void *element);

#define vector_at(vector_ptr, index) vector_ptr->elements[index]

#define vector_size(vector_ptr) vector_ptr->num

#define vector_capacity(vector_ptr) vector_ptr->len

#define vector_set(vector_ptr, index, value) vector_ptr->elements[index] = value

#endif
