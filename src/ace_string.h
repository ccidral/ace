#ifndef ACE_STRING_H
#define ACE_STRING_H

#include <stdbool.h>

char *ace_str_join_2 (char *s1, char *s2);
char *ace_str_join_3 (char *s1, char *s2, char *s3);
bool ace_str_ends_with (char *str, char *expected_ending);

#endif
