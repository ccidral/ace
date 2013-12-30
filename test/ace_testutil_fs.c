#include <ftw.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../src/ace_fs.h"
#include "../src/ace_string.h"
#include "ace_testutil_fs.h"


// internal ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int ace_tu_fs_delete_file_or_directory (const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
  int error = remove(path);
  if(error)
    perror(path);
  return error;
}

void ace_tu_fs_append_str_to_file (char *path, char *str)
{
  FILE *file = fopen (path, "a");
  fputs (str, file);
  fclose (file);
}


// implementation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

char *ace_tu_fs_create_temp_dir (char *name)
{
  char template[256];
  snprintf (template, 256, "/tmp/ace_test_%s_XXXXXX", name);
  char *path = mkdtemp (template);
  if (path == NULL)
  {
    perror ("Failed to create temporary directory");
    return NULL;
  }
  return strdup (path);
}

char *ace_tu_fs_create_temp_file_at (char *dirpath, char *file_contents)
{
  char *filepath_template = ace_str_join_2 (dirpath, "/XXXXXX");
  char *filepath = mktemp (filepath_template);
  ace_tu_fs_append_str_to_file (filepath, file_contents);
  return filepath;
}

void ace_tu_fs_delete_recursively (char *dirpath)
{
  if(ace_fs_does_directory_exist (dirpath))
  {
    int error = nftw (dirpath, ace_tu_fs_delete_file_or_directory, 64, FTW_DEPTH | FTW_PHYS);
    if(error)
      perror(dirpath);
  }
}
