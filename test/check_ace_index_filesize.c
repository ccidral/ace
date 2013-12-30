#include <check.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/ace_errcode.h"
#include "../src/ace_index_filesize.h"
#include "ace_testutil_fs.h"


char *source_dirpath;
char *target_dirpath;


// fixture ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup_test (void)
{
  source_dirpath = ace_tu_fs_create_temp_dir ("source");
  target_dirpath = ace_tu_fs_create_temp_dir ("index_filesize");
}

void teardown_test (void)
{
  ace_tu_fs_delete_recursively (source_dirpath);
  ace_tu_fs_delete_recursively (target_dirpath);
}


// unit tests ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

START_TEST (test_return_error_when_source_directory_does_not_exist)
{
  rmdir (source_dirpath);
  int error_code = ace_index_filesize (source_dirpath, target_dirpath);
  ck_assert_int_eq (error_code, ACE_ERR_NO_SOURCE_DIR);
}
END_TEST

START_TEST (test_return_error_when_target_directory_does_not_exist)
{
  rmdir (target_dirpath);
  int error_code = ace_index_filesize (source_dirpath, target_dirpath);
  ck_assert_int_eq (error_code, ACE_ERR_NO_TARGET_DIR);
}
END_TEST

START_TEST (test_return_success_when_both_source_and_target_directories_do_exist)
{
  int error_code = ace_index_filesize (source_dirpath, target_dirpath);
  ck_assert_int_eq (error_code, ACE_SUCCESS);
}
END_TEST


// test case ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TCase *create_ace_index_filesize_testcase (void)
{
  TCase *testcase = tcase_create ("Index files by size");
  tcase_add_checked_fixture (testcase, setup_test, teardown_test);
  tcase_add_test (testcase, test_return_error_when_source_directory_does_not_exist);
  tcase_add_test (testcase, test_return_error_when_target_directory_does_not_exist);
  tcase_add_test (testcase, test_return_success_when_both_source_and_target_directories_do_exist);
  return testcase;
}
