#ifndef ACE_TESTUTIL_FS_H
#define ACE_TESTUTIL_FS_H

#include <stdbool.h>

char *ace_tu_fs_mksubdir (char *parent_dirpath, char *child_dirname);
char *ace_tu_fs_create_temp_dir (char *name);
char *ace_tu_fs_create_temp_file_at (char *dirpath, char *file_contents);
void ace_tu_fs_delete_recursively (char *dirpath);
bool ace_tu_fs_is_directory_empty (char *dirpath);
int ace_tu_fs_get_number_of_files_in_directory (char *dirpath);
int ace_tu_fs_get_number_of_lines_in_file (char *filepath);
bool ace_tu_fs_does_file_contain_line (char *filepath, char *str_without_linebreak);
bool ace_tu_fs_does_file_contain_line_at (int expected_line_number, char *filepath, char *str_without_linebreak);

#endif
