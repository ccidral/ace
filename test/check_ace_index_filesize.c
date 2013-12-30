#include <check.h>
#include <stdlib.h>


// fixture ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup_test (void)
{
}

void teardown_test (void)
{
}


// unit tests ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

START_TEST (test_return_error_when_source_directory_does_not_exist)
{
}
END_TEST


// test case ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TCase *create_ace_index_filesize_testcase (void)
{
  TCase *testcase = tcase_create ("Index files by size");
  tcase_add_checked_fixture (testcase, setup_test, teardown_test);
  tcase_add_test (testcase, test_return_error_when_source_directory_does_not_exist);
  return testcase;
}
