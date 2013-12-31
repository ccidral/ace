#ifndef ACE_FS_H
#define ACE_FS_H

#include <stdbool.h>
#include <unistd.h>

bool ace_fs_does_file_exist (char *filepath);
bool ace_fs_does_directory_exist (char *path);
bool ace_fs_is_current_or_parent_directory (char *dirname);
off_t ace_fs_get_file_size (char *filepath);
void ace_fs_append_line_to_file (char *filepath, char* str_without_linebreak);

#endif
