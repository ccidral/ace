#include <check.h>
#include <unistd.h>

#include "../src/ace_fs.h"
#include "ace_testutil_fs.h"


// unit tests ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

START_TEST (test_directory_does_exist)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  ck_assert_msg (ace_fs_does_directory_exist (dirpath), "Directory should exist");
}
END_TEST

START_TEST (test_directory_does_not_exist)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  rmdir (dirpath);
  ck_assert_msg (!ace_fs_does_directory_exist (dirpath), "Directory should not exist");
}
END_TEST

START_TEST (test_directory_does_not_exist_when_path_refers_to_a_file)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "~~ file contents ~~");
  ck_assert_msg (!ace_fs_does_directory_exist (filepath), "Directory should not exist because it's not a directory");
}
END_TEST


// test case ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TCase *create_ace_fs_testcase (void)
{
  TCase *testcase = tcase_create ("File system functions");
  tcase_add_test (testcase, test_directory_does_exist);
  tcase_add_test (testcase, test_directory_does_not_exist);
  tcase_add_test (testcase, test_directory_does_not_exist_when_path_refers_to_a_file);
  return testcase;
}
