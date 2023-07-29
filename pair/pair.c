#include "pair.h"

Pair* pair_create(char *first, char *second) {
  Pair *pair = (Pair *) malloc(sizeof(Pair));
  pair->first  = first;
  pair->second = second;

  return pair;
}

void pair_print(Pair *p) {
  printf("[%s: %s]\n", p->first, p->second);
}