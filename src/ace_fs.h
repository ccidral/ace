#ifndef ACE_FS_H
#define ACE_FS_H

#include <stdbool.h>
#include <unistd.h>

bool ace_fs_does_file_exist (const char *filepath);
bool ace_fs_does_directory_exist (const char *path);
bool ace_fs_is_current_or_parent_directory (const char *dirname);
off_t ace_fs_get_file_size (const char *filepath);
void ace_fs_append_line_to_file (const char *filepath, const char* str_without_linebreak);

#endif
