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

#ifndef JD297_LMAP_H
#define JD297_LMAP_H

#include <stddef.h>

#include <jd297/vector.h>

typedef struct {
    vector_t pairs;
} lmap_t;

extern void lmap_free(lmap_t *map);

extern int lmap_has(lmap_t *map, char *k);

extern void *lmap_get(lmap_t *map, char *k);

extern int lmap_add(lmap_t *map, char *k, void *v);

extern void lmap_remove(lmap_t *map, char *k);

#endif

#ifdef JD297_LMAP_IMPLEMENTATION

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <jd297/vector.h>

typedef struct {
    char *k;
    void *v;
} lmap_node_t;

lmap_node_t *lmap_node_t_create(char *k, void *v)
{
    lmap_node_t *node = (lmap_node_t *)malloc(sizeof(lmap_node_t));

    node->k = k;
    node->v = v;
    
    return node;
}

#define lmap_node_cmp(node_p, k) (node_p) != NULL && strcmp((node_p)->k, k)

#define lmap_begin(lmap) (lmap_node_t **)vec_begin(&(lmap)->pairs)
#define lmap_end(lmap) (lmap_node_t **)vec_end(&(lmap)->pairs)

void lmap_free(lmap_t *map) 
{
    for (lmap_node_t **it = lmap_begin(map); it < lmap_end(map); ++it) {
        free(*it);
    }

    vec_free(&map->pairs);
}

int lmap_has(lmap_t *map, char *k)
{
    for (lmap_node_t **it = lmap_begin(map); it < lmap_end(map); ++it) {
        if (lmap_node_cmp(*it, k) == 0) {
            return 1;
        }
    }

    return 0;
}

void *lmap_get(lmap_t *map, char *k)
{
    for (lmap_node_t **it = lmap_begin(map); it < lmap_end(map); ++it) {
        if (lmap_node_cmp(*it, k) == 0) {
            return (*it)->v;
        }
    }

    return NULL;
}

int lmap_add(lmap_t *map, char *k, void *v)
{
    for (lmap_node_t **it = lmap_begin(map); it < lmap_end(map); ++it) {
        if (*it == NULL) {
            *it = lmap_node_t_create(k, v);

            return 0;
        }

        if (lmap_node_cmp(*it, k) == 0) {
            (*it)->k = k;
            (*it)->v = v;
            
            return 0;
        }
    }

    return vec_push_back(&map->pairs, lmap_node_t_create(k, v));
}

void lmap_remove(lmap_t *map, char *k)
{
    for (lmap_node_t **it = lmap_begin(map); it < lmap_end(map); ++it) {
        if (lmap_node_cmp(*it, k) == 0) {
            free(*it);

            *it = NULL;
            
            return;
        }
    }
}

#undef lmap_node_cmp
#undef lmap_begin
#undef lmap_end

#endif
