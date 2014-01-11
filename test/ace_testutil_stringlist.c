#include <string.h>
#include <stdio.h>
#include "ace_testutil_stringlist.h"

int max_int (int a, int b)
{
  return a > b ? a : b;
}

void ace_tu_strlist_change_capacity (int new_capacity, ace_tu_strlist_t *list)
{
  const char ** old_items = list->items;
  const char ** new_items = (const char **)malloc(new_capacity * sizeof(char *));
  
  if (old_items != NULL)
  {
    for (int index = 0; index < list->size; index++)
    {
      new_items[index] = old_items[index];
    }
    free (old_items);
  }
  
  list->capacity = new_capacity;
  list->items = new_items;
}

void ace_tu_strlist_increase_capacity (ace_tu_strlist_t *list)
{
  int new_capacity = max_int(1, list->capacity) * 2;
  ace_tu_strlist_change_capacity (new_capacity, list);
}

void ace_tu_strlist_init (ace_tu_strlist_t *list)
{
  list->size = 0;
  list->capacity = 0;
  list->items = NULL;
}

void ace_tu_strlist_append (const char *str, ace_tu_strlist_t *list)
{
  if(list->size >= list->capacity)
  {
    ace_tu_strlist_increase_capacity (list);
  }
  int index = list->size++;
  list->items[index] = str;
}

int ace_tu_strlist_size (ace_tu_strlist_t *list)
{
  return list->size;
}

bool ace_tu_strlist_contains (const char *str, ace_tu_strlist_t *list)
{
  for (int index = 0; index < list->size; index++)
  {
    if (strcmp (str, list->items[index]) == 0)
    {
      return true;
    }
  }
  return false;
}
