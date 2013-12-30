#include "ace_errcode.h"
#include "ace_fs.h"
#include "ace_index_filesize.h"

int ace_index_filesize (char *source_dirpath, char *target_dirpath)
{
  if (!ace_fs_does_directory_exist (source_dirpath))
    return ACE_ERR_NO_SOURCE_DIR;

  if (!ace_fs_does_directory_exist (target_dirpath))
    return ACE_ERR_NO_TARGET_DIR;

  return ACE_SUCCESS;
}
