#include <check.h>
#include <stdlib.h>

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

START_TEST (test_index_two_sets_of_files_with_same_size)
{
  char *path_to_file_of_size_21_1 = ace_tu_fs_create_temp_file_at (source_dirpath, "I wish I had 21 bytes");
  char *path_to_file_of_size_21_2 = ace_tu_fs_create_temp_file_at (source_dirpath, "I wish I had 21 bytes");
  
  char *path_to_file_of_size_40_1 = ace_tu_fs_create_temp_file_at (source_dirpath, "this is going to have a size of 40 bytes");
  char *path_to_file_of_size_40_2 = ace_tu_fs_create_temp_file_at (source_dirpath, "this is going to have a size of 40 bytes");
  
  char *index_filepath_21 = ace_str_join_2 (target_dirpath, "/21");
  char *index_filepath_40 = ace_str_join_2 (target_dirpath, "/40");
  
  int error_code = ace_index_filesize (source_dirpath, target_dirpath);
  
  ck_assert_int_eq (error_code, ACE_SUCCESS);
  ck_assert_int_eq (ace_tu_fs_get_number_of_files_in_directory (target_dirpath), 2);
  
  ck_assert (ace_fs_does_file_exist (index_filepath_21));
  ck_assert (ace_fs_does_file_exist (index_filepath_40));
  
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_2));
  
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_40, path_to_file_of_size_40_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_40, path_to_file_of_size_40_2));
}
END_TEST

START_TEST (test_singleton_files_are_indicated_with_the_word_singleton_appended_to_the_index_file)
{
  char *path_to_file_of_size_21_1 = ace_tu_fs_create_temp_file_at (source_dirpath, "3 files with 21 bytes");
  char *path_to_file_of_size_21_2 = ace_tu_fs_create_temp_file_at (source_dirpath, "3 files with 21 bytes");
  char *path_to_file_of_size_21_3 = ace_tu_fs_create_temp_file_at (source_dirpath, "3 files with 21 bytes");
  char *path_to_file_of_size_33_1 = ace_tu_fs_create_temp_file_at (source_dirpath, "my twin brother also has 33 bytes");
  char *path_to_file_of_size_33_2 = ace_tu_fs_create_temp_file_at (source_dirpath, "my twin brother also has 33 bytes");
  char *path_to_file_of_size_42 = ace_tu_fs_create_temp_file_at (source_dirpath, "I am the only one file containing 42 bytes");
  char *path_to_file_of_size_13 = ace_tu_fs_create_temp_file_at (source_dirpath, "13 bytes here");

  char *index_filepath_21 = ace_str_join_2 (target_dirpath, "/21");
  char *index_filepath_33 = ace_str_join_2 (target_dirpath, "/33");
  char *index_filepath_42 = ace_str_join_2 (target_dirpath, "/42.singleton");
  char *index_filepath_13 = ace_str_join_2 (target_dirpath, "/13.singleton");

  int error_code = ace_index_filesize (source_dirpath, target_dirpath);

  ck_assert_int_eq (error_code, ACE_SUCCESS);
  ck_assert_int_eq (ace_tu_fs_get_number_of_files_in_directory (target_dirpath), 4);

  ck_assert (ace_fs_does_file_exist (index_filepath_21));
  ck_assert (ace_fs_does_file_exist (index_filepath_33));
  ck_assert (ace_fs_does_file_exist (index_filepath_42));
  ck_assert (ace_fs_does_file_exist (index_filepath_13));

  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_2));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_3));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_33, path_to_file_of_size_33_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_33, path_to_file_of_size_33_2));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_42, path_to_file_of_size_42));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_13, path_to_file_of_size_13));
}
END_TEST

START_TEST (test_search_for_files_deep_in_the_source_directory_tree)
{
  char *subdir_A = ace_tu_fs_mksubdir (source_dirpath, "a");
  char *subdir_B = ace_tu_fs_mksubdir (subdir_A, "b");
  char *subdir_C = ace_tu_fs_mksubdir (subdir_B, "c");
  char *subdir_D = ace_tu_fs_mksubdir (source_dirpath, "d");
  char *subdir_E = ace_tu_fs_mksubdir (subdir_D, "e");
  char *subdir_F = ace_tu_fs_mksubdir (subdir_D, "f");
  char *subdir_G = ace_tu_fs_mksubdir (subdir_F, "g");
  
  char *path_to_file_of_size_21_1 = ace_tu_fs_create_temp_file_at (subdir_A, "3 files with 21 bytes");
  char *path_to_file_of_size_21_2 = ace_tu_fs_create_temp_file_at (subdir_B, "3 files with 21 bytes");
  char *path_to_file_of_size_21_3 = ace_tu_fs_create_temp_file_at (subdir_C, "3 files with 21 bytes");
  char *path_to_file_of_size_33_1 = ace_tu_fs_create_temp_file_at (subdir_D, "my twin brother also has 33 bytes");
  char *path_to_file_of_size_33_2 = ace_tu_fs_create_temp_file_at (subdir_E, "my twin brother also has 33 bytes");
  char *path_to_file_of_size_42 = ace_tu_fs_create_temp_file_at (subdir_F, "I am the only one file containing 42 bytes");
  char *path_to_file_of_size_13 = ace_tu_fs_create_temp_file_at (subdir_G, "13 bytes here");
  char *path_to_file_of_size_24_1 = ace_tu_fs_create_temp_file_at (subdir_A, "some files have 24 bytes");
  char *path_to_file_of_size_24_2 = ace_tu_fs_create_temp_file_at (subdir_B, "some files have 24 bytes");
  char *path_to_file_of_size_24_3 = ace_tu_fs_create_temp_file_at (subdir_C, "some files have 24 bytes");
  char *path_to_file_of_size_24_4 = ace_tu_fs_create_temp_file_at (subdir_D, "some files have 24 bytes");
  char *path_to_file_of_size_24_5 = ace_tu_fs_create_temp_file_at (subdir_E, "some files have 24 bytes");
  char *path_to_file_of_size_24_6 = ace_tu_fs_create_temp_file_at (subdir_F, "some files have 24 bytes");
  char *path_to_file_of_size_24_7 = ace_tu_fs_create_temp_file_at (subdir_G, "some files have 24 bytes");

  char *index_filepath_21 = ace_str_join_2 (target_dirpath, "/21");
  char *index_filepath_24 = ace_str_join_2 (target_dirpath, "/24");
  char *index_filepath_33 = ace_str_join_2 (target_dirpath, "/33");
  char *index_filepath_42 = ace_str_join_2 (target_dirpath, "/42.singleton");
  char *index_filepath_13 = ace_str_join_2 (target_dirpath, "/13.singleton");

  int error_code = ace_index_filesize (source_dirpath, target_dirpath);

  ck_assert_int_eq (error_code, ACE_SUCCESS);
  ck_assert_int_eq (ace_tu_fs_get_number_of_files_in_directory (target_dirpath), 5);

  ck_assert (ace_fs_does_file_exist (index_filepath_13));
  ck_assert (ace_fs_does_file_exist (index_filepath_21));
  ck_assert (ace_fs_does_file_exist (index_filepath_24));
  ck_assert (ace_fs_does_file_exist (index_filepath_33));
  ck_assert (ace_fs_does_file_exist (index_filepath_42));
  
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_2));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_21, path_to_file_of_size_21_3));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_33, path_to_file_of_size_33_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_33, path_to_file_of_size_33_2));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_42, path_to_file_of_size_42));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_13, path_to_file_of_size_13));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_24, path_to_file_of_size_24_1));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_24, path_to_file_of_size_24_2));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_24, path_to_file_of_size_24_3));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_24, path_to_file_of_size_24_4));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_24, path_to_file_of_size_24_5));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_24, path_to_file_of_size_24_6));
  ck_assert (ace_tu_fs_does_file_contain_line (index_filepath_24, path_to_file_of_size_24_7));
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
  tcase_add_test (testcase, test_index_two_sets_of_files_with_same_size);
  tcase_add_test (testcase, test_singleton_files_are_indicated_with_the_word_singleton_appended_to_the_index_file);
  tcase_add_test (testcase, test_search_for_files_deep_in_the_source_directory_tree);
  return testcase;
}
