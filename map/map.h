#ifndef __MAP_H__
#define __MAP_H__

#include "../pair/pair.h";

typedef struct {
  int size;
  int capacity;
  Pair** vector;
} Map;

// create map
Map* map_create(int capacity);

void extendsMap(Map *map);

Pair * map_get(Map *m, char* key);

int map_indexof(Map *m, char* key);

// insert element
int map_push(Map *m, char* key, char *value);

int map_pop(Map* m, char* key);

void map_free(Map* map);

#endif //__MAP_H__






