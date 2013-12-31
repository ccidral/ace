#include <check.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/ace_errcode.h"
#include "../src/ace_fs.h"
#include "../src/ace_string.h"
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

START_TEST (test_target_directory_is_empty_when_source_directory_is_empty)
{
  int error_code = ace_index_filesize (source_dirpath, target_dirpath);
  ck_assert_int_eq (error_code, ACE_SUCCESS);
  ck_assert_msg (ace_tu_fs_is_directory_empty (target_dirpath), "The target directory should be empty");
}
END_TEST

START_TEST (test_ignore_empty_files)
{
  ace_tu_fs_create_temp_file_at (source_dirpath, "");
  ace_tu_fs_create_temp_file_at (source_dirpath, "");
  ace_tu_fs_create_temp_file_at (source_dirpath, "");
  
  int error_code = ace_index_filesize (source_dirpath, target_dirpath);
  
  ck_assert_int_eq (error_code, ACE_SUCCESS);
  ck_assert_msg (ace_tu_fs_is_directory_empty (target_dirpath), "The target directory should be empty");
}
END_TEST

START_TEST (test_files_of_same_size_have_their_paths_recorded_into_a_file_named_after_the_file_size)
{
  char *filepath_1 = ace_tu_fs_create_temp_file_at (source_dirpath, "this text has 22 bytes");
  char *filepath_2 = ace_tu_fs_create_temp_file_at (source_dirpath, "this text has 22 bytes");
  char *filepath_3 = ace_tu_fs_create_temp_file_at (source_dirpath, "this text has 22 bytes");
  
  char *index_filepath = ace_str_join_2 (target_dirpath, "/22");
  int error_code = ace_index_filesize (source_dirpath, target_dirpath);
  
  ck_assert_int_eq (error_code, ACE_SUCCESS);
  ck_assert_int_eq (ace_tu_fs_get_number_of_files_in_directory (target_dirpath), 1);
  ck_assert (ace_fs_does_file_exist (index_filepath));
  ck_assert_int_eq (ace_tu_fs_get_number_of_lines_in_file (index_filepath), 3);
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath, filepath_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath, filepath_2));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath, filepath_3));
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
  tcase_add_test (testcase, test_target_directory_is_empty_when_source_directory_is_empty);
  tcase_add_test (testcase, test_ignore_empty_files);
  tcase_add_test (testcase, test_files_of_same_size_have_their_paths_recorded_into_a_file_named_after_the_file_size);
  return testcase;
}
