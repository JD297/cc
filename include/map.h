#ifndef JD297_CC_MAP_H
#define JD297_CC_MAP_H

#include "vector.h"

#include <stddef.h>

typedef struct Map {
    Vector *elements;
    Vector *elements_names;
} Map;

extern void *map_create();

extern void map_destroy(Map *map);

extern int map_push_back(Map *map, void *element, char *name);

extern int map_cmp_at(Map *map, char *name, size_t index);

extern void *map_get(Map *map, char *name);

extern int map_add(Map *map, void *element, char *name);

extern void map_remove(Map *map, char *name);

#endif
