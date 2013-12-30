#include <check.h>
#include <stdlib.h>

#include "check_ace_index_filesize.h"

Suite *create_ace_testsuite (void)
{
  Suite *suite = suite_create ("Ace");
  suite_add_tcase (suite, create_ace_index_filesize_testcase ());
  return suite;
}

int main (void)
{
  int number_failed;
  Suite *suite = create_ace_testsuite ();
  SRunner *suiteRunner = srunner_create (suite);
  srunner_run_all (suiteRunner, CK_NORMAL);
  number_failed = srunner_ntests_failed (suiteRunner);
  srunner_free (suiteRunner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
