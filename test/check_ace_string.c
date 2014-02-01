#include <check.h>
#include "check_ace_string.h"
#include "../src/ace_string.h"


// unit tests ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

START_TEST (join_two_strings)
{
  char *result = ace_str_join_2 ("hello", " world");
  ck_assert_str_eq (result, "hello world");
}
END_TEST

START_TEST (join_three_strings)
{
  char *result = ace_str_join_3 ("whiskey", " tango", " foxtrot");
  ck_assert_str_eq (result, "whiskey tango foxtrot");
}
END_TEST

START_TEST (string_ending)
{
  ck_assert (ace_str_ends_with ("", ""));
  ck_assert (ace_str_ends_with ("a", "a"));
  ck_assert (ace_str_ends_with ("aa", "a"));
  ck_assert (ace_str_ends_with ("hello world", "world"));
  
  ck_assert (!ace_str_ends_with ("a", "b"));
  ck_assert (!ace_str_ends_with ("a", "aa"));
  ck_assert (!ace_str_ends_with ("hello world", "hello"));
}
END_TEST


// test case ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TCase *create_ace_string_testcase (void)
{
  TCase *testcase = tcase_create ("String utils");
  tcase_add_test (testcase, join_two_strings);
  tcase_add_test (testcase, join_three_strings);
  tcase_add_test (testcase, string_ending);
  return testcase;
}
