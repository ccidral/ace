#ifndef ACE_TESTUTIL_FS_H
#define ACE_TESTUTIL_FS_H

char *ace_tu_fs_create_temp_dir (char *name);
char *ace_tu_fs_create_temp_file_at (char *dirpath, char *file_contents);
void ace_tu_fs_delete_recursively (char *dirpath);

#endif
