#ifndef ACE_TESTUTIL_STRINGLIST_H
#define ACE_TESTUTIL_STRINGLIST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct ace_tu_strlist_t {
  int size;
  int capacity;
  const char ** items;
} ace_tu_strlist_t;

void ace_tu_strlist_init (ace_tu_strlist_t *list);
void ace_tu_strlist_append (const char *str, ace_tu_strlist_t *list);
int ace_tu_strlist_size (ace_tu_strlist_t *list);
bool ace_tu_strlist_contains (const char *str, ace_tu_strlist_t *list);

#endif
