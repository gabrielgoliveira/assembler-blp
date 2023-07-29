#ifndef __PAIR_H__
#define __PAIR_H__

typedef struct {
  char *first;
  char *second;
} Pair;

Pair* pair_create(char *, char *);

void  pair_free(Pair *pair);

#endif //__PAIR_H__