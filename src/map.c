#include "map.h"
#include "vector.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void *map_create()
{
    Map *map = (Map *)malloc(sizeof(Map));

    if (map == NULL) {
		return NULL;
	}

	map->elements = vector_create();
	map->elements_names = vector_create();

	if (map->elements == NULL || map->elements_names == NULL) {
		return NULL;
	}

    return map;
}

void map_destroy(Map *map)
{
    vector_destroy(map->elements);
    vector_destroy(map->elements_names);

    free(map);
}

int map_push_back(Map *map, void *element, char *name)
{
    vector_push_back(map->elements, element);
    vector_push_back(map->elements_names, name);

    return 0;
}

int map_cmp_at(Map *map, char *name, size_t index)
{
    const void *name_value = vector_at(map->elements_names, index);

    if (name_value == NULL) {
        return -1;
    }

    return strcmp(name_value, name);
}

void *map_get(Map *map, char *name)
{
    for (size_t i = 0; i < vector_size(map->elements); i++) {
        if (map_cmp_at(map, name, i) == 0) {
            return vector_at(map->elements, i);
        }
    }

    return NULL;
}

int map_add(Map *map, void *element, char *name)
{
    for (size_t i = 0; i < vector_size(map->elements); i++) {
        if (vector_at(map->elements, i) == NULL) {
            vector_set(map->elements, i, element);
            
            return 0;
        }
        
        if (map_cmp_at(map, name, i) == 0) {
            vector_set(map->elements, i, element);
            
            return 0;
        }
    }

    return map_push_back(map, element, name);
}

void token_list_named_c_remove(Map *map, char *name)
{
    for (size_t i = 0; i < vector_size(map->elements); i++) {
        if (map_cmp_at(map, name, i) == 0) {
            vector_set(map->elements, i, NULL);
            vector_set(map->elements_names, i, NULL);
            
            return;
        }
    }
}
