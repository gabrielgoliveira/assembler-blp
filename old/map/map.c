#include <stdio.h>
#include "map.h"


Map* map_create(int capacity) {
  Map *map = (Map*) malloc(sizeof(Map));
  map->vector = (Pair**) malloc(sizeof(Pair*)*capacity);
  map->size = 0;
  map->capacity = capacity;
  return map;
}

void extendsMap(Map *map) {
  if (map->size >= map->capacity) {
    map->capacity *= 2;
    map->vector = (Pair**)realloc(map->vector, map->capacity * sizeof(Pair*));
  }
}

Pair * map_get(Map *m, char* key) {
  Pair* p = NULL;
  if(!m->capacity) return p;
  if(!m->size) return p;

  for(int i = 0; i < m->size; i++) {
    if(!m->vector[i]) continue;
    char *str = m->vector[i]->first;
    if(strcmp(key, str) == 0) {
      return m->vector[i];
    }
  }
  return p;
}

int map_indexof(Map *m, char* key) {
  int index = -1;

  if(!m->capacity) return index;
  if(!m->size) return index;

  for(int i = 0; i < m->size; i++) {
    if(!m->vector[i]) continue;
    char *str = m->vector[i]->first;
    if(strcmp(key, str) == 0) {
      return i;
    }
  }
  return index;
}



int map_push(Map *m, char* key, char *value) {
  Pair* p = NULL;
  if(!m->capacity) return p;
  if(!m->size) return p;

  if (map_get(m, key) != NULL) {
    printf("ERRO: Elemento duplicado !!");
    return -1;
  }

  Pair *p = pair_create(key, value);

  int i = m->size;
  m->vector[i] = p;
  m->size++;

  return p;
}

int map_pop(Map* m, char* key) {
  int index = map_indexof(m, key);

  if(index == -1) return -1;
    
  free(m->vector[index]);

  // Desloca os elementos seguintes para preencher a lacuna
  for (int i = index; i < m->size - 1; i++) {
    m->vector[i] = m->vector[i + 1];
  }

  m->size--;

}

void map_free(Map* map) {

  for (int i = 0; i < map->size; i++) {
    free(map->vector[i]);
  }
  free(map->vector);
  free(map);
}