#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ace_fs.h"

bool ace_fs_does_file_exist (const char *filepath)
{
  struct stat sb;
  return stat(filepath, &sb) == 0 && S_ISREG(sb.st_mode);
}

bool ace_fs_does_directory_exist (const char *path)
{
  struct stat sb;
  return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}

bool ace_fs_is_current_or_parent_directory (const char *dirname)
{
  return
    strcmp(dirname, ".") == 0 ||
    strcmp(dirname, "..") == 0;
}

off_t ace_fs_get_file_size (const char *filepath)
{
  struct stat sb;
  bool does_file_exist = stat(filepath, &sb) == 0;
  bool is_regular_file = S_ISREG(sb.st_mode);
  if (does_file_exist && is_regular_file)
  {
    return sb.st_size;
  }
  else
    return 0;
}

void ace_fs_append_line_to_file (const char *filepath, const char* str_without_linebreak)
{
  FILE *file = fopen (filepath, "a");
  fprintf (file, "%s\n", str_without_linebreak);
  fclose (file);
}
