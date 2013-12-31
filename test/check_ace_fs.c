#include <check.h>
#include <unistd.h>

#include "../src/ace_fs.h"
#include "ace_testutil_fs.h"


// unit tests ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

START_TEST (test_file_does_exist)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "~~ file contents ~~");
  ck_assert_msg (ace_fs_does_file_exist (filepath), "File should exist");
}
END_TEST

START_TEST (test_file_does_not_exist)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "~~ file contents ~~");
  unlink (filepath);
  ck_assert_msg (!ace_fs_does_file_exist (filepath), "File should not exist");
}
END_TEST

START_TEST (test_file_does_not_exist_when_path_refers_to_a_directory)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  ck_assert_msg (!ace_fs_does_file_exist (dirpath), "File should not exist because it's a directory");
}
END_TEST

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

START_TEST (test_is_current_or_parent_directory)
{
  ck_assert_msg (ace_fs_is_current_or_parent_directory ("."));
  ck_assert_msg (ace_fs_is_current_or_parent_directory (".."));
}
END_TEST

START_TEST (test_is_not_current_or_parent_directory)
{
  ck_assert_msg (!ace_fs_is_current_or_parent_directory ("..."));
  ck_assert_msg (!ace_fs_is_current_or_parent_directory ("foobar"));
}
END_TEST

START_TEST (test_file_size_is_zero_when_file_does_not_exist)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "~~ file contents ~~");
  unlink (filepath);
  off_t file_size = ace_fs_get_file_size (filepath);
  ck_assert_int_eq (file_size, 0);
}
END_TEST

START_TEST (test_file_size_is_zero_when_path_refers_to_a_directory)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  off_t file_size = ace_fs_get_file_size (dirpath);
  ck_assert_int_eq (file_size, 0);
}
END_TEST

START_TEST (test_file_size_is_zero_when_file_is_empty)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "");
  off_t file_size = ace_fs_get_file_size (filepath);
  ck_assert_int_eq (file_size, 0);
}
END_TEST

START_TEST (test_file_size)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "this file has 22 bytes");
  off_t file_size = ace_fs_get_file_size (filepath);
  ck_assert_int_eq (file_size, 22);
}
END_TEST

START_TEST (test_append_one_line_to_file)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "");
  
  ace_fs_append_line_to_file (filepath, "foo bar");
  
  ck_assert_int_eq (ace_tu_fs_get_number_of_lines_in_file (filepath), 1);
  ck_assert (ace_tu_fs_does_file_contain_line_at (1, filepath, "foo bar"));
}
END_TEST

START_TEST (test_append_two_lines_to_file)
{
  char *dirpath = ace_tu_fs_create_temp_dir ("fs_test");
  char *filepath = ace_tu_fs_create_temp_file_at (dirpath, "");

  ace_fs_append_line_to_file (filepath, "first line");
  ace_fs_append_line_to_file (filepath, "second line");

  ck_assert_int_eq (ace_tu_fs_get_number_of_lines_in_file (filepath), 2);
  ck_assert (ace_tu_fs_does_file_contain_line_at (1, filepath, "first line"));
  ck_assert (ace_tu_fs_does_file_contain_line_at (2, filepath, "second line"));
}
END_TEST


// test case ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TCase *create_ace_fs_testcase (void)
{
  TCase *testcase = tcase_create ("File system functions");
  tcase_add_test (testcase, test_file_does_exist);
  tcase_add_test (testcase, test_file_does_not_exist);
  tcase_add_test (testcase, test_file_does_not_exist_when_path_refers_to_a_directory);
  tcase_add_test (testcase, test_directory_does_exist);
  tcase_add_test (testcase, test_directory_does_not_exist);
  tcase_add_test (testcase, test_directory_does_not_exist_when_path_refers_to_a_file);
  tcase_add_test (testcase, test_is_current_or_parent_directory);
  tcase_add_test (testcase, test_is_not_current_or_parent_directory);
  tcase_add_test (testcase, test_file_size_is_zero_when_file_does_not_exist);
  tcase_add_test (testcase, test_file_size_is_zero_when_path_refers_to_a_directory);
  tcase_add_test (testcase, test_file_size_is_zero_when_file_is_empty);
  tcase_add_test (testcase, test_file_size);
  tcase_add_test (testcase, test_append_one_line_to_file);
  tcase_add_test (testcase, test_append_two_lines_to_file);
  return testcase;
}
