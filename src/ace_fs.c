#include <sys/stat.h>

#include "ace_fs.h"

bool ace_fs_does_directory_exist (char *path)
{
  struct stat sb;
  return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}
