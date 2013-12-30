#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ace_string.h"

char *ace_str_alloc (size_t length)
{
  char *str = malloc (length);
  str[0] = '\0';
  return str;
}

char *ace_str_join_2 (char *s1, char *s2)
{
  size_t length = strlen (s1) + strlen (s2) + 1;
  char *result = ace_str_alloc (length);
  strncat (result, s1, length);
  strncat (result, s2, length);
  return result;
}

char *ace_str_join_3 (char *s1, char *s2, char *s3)
{
  size_t length = strlen (s1) + strlen (s2) + strlen (s3) + 1;
  char *result = ace_str_alloc (length);
  strncat (result, s1, length);
  strncat (result, s2, length);
  strncat (result, s3, length);
  return result;
}

bool ace_str_ends_with (char *str, char *expected_ending)
{
  /* the code in this function was copied from:
     https://github.com/benbai123/C_Cplusplus_Practice/blob/master/C_StringProcessing/utils/startsWith_endsWith_indexOf_lastIndexOf.c
  */
  int blen = strlen(str);
  int slen = strlen(expected_ending);
  return (blen >= slen) && (0 == strcmp(str + blen - slen, expected_ending));
}
