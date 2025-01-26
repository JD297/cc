#include "vector.h"

#include <stddef.h>
#include <stdlib.h>

void *vector_create()
{
    Vector *vector = (Vector *)malloc(sizeof(Vector));

    if (vector == NULL) {
		return NULL;
	}

    vector->len = 1;
	vector->num = 0;
	vector->elements = (void **)calloc(vector->len, sizeof(void *));

	if (vector->elements == NULL) {
        free(vector);

		return NULL;
	}

    return vector;
}

void vector_destroy(Vector *vector)
{
    free(vector->elements);
    free(vector);
}

int vector_push_back(Vector *vector, void *element)
{
    if (vector->num == vector->len) {
        size_t new_len = vector->len * VECTOR_RESIZE_FACTOR;

        vector->elements = (void **)realloc(vector->elements, new_len * sizeof(void *));

        if (vector->elements == NULL) {
            return -1;
        }

        vector->len = new_len;
    }

    vector->elements[vector->num] = element;

    vector->num++;

    return 0;
}
