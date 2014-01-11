#ifndef ACE_STRING_H
#define ACE_STRING_H

#include <stdbool.h>

char *ace_str_join_2 (const char *s1, const char *s2);
char *ace_str_join_3 (const char *s1, const char *s2, const char *s3);
bool ace_str_ends_with (const char *str, const char *expected_ending);

#endif
