#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

#include "ace_errcode.h"
#include "ace_fs.h"
#include "ace_string.h"
#include "ace_index_filesize.h"


int ace_index_filesize (char *source_dirpath, char *target_dirpath)
{
  if (!ace_fs_does_directory_exist (source_dirpath))
    return ACE_ERR_NO_SOURCE_DIR;

  if (!ace_fs_does_directory_exist (target_dirpath))
    return ACE_ERR_NO_TARGET_DIR;
  
  struct dirent *entity;
  char *source_filepath;
  char *target_filepath;
  DIR *source_dir;
  off_t file_size;
  char index_file_name_with_leading_slash[32];
  
  source_dir = opendir (source_dirpath);
  
  while ((entity = readdir (source_dir)) != NULL)
  {
    if (!ace_fs_is_current_or_parent_directory (entity->d_name))
    {
      source_filepath = ace_str_join_3 (source_dirpath, "/", entity->d_name);
      file_size = ace_fs_get_file_size (source_filepath);
      if (ace_fs_get_file_size (source_filepath) > 0)
      {
        snprintf (index_file_name_with_leading_slash, 32, "/%lld", file_size);
        target_filepath = ace_str_join_2 (target_dirpath, index_file_name_with_leading_slash);
        ace_fs_append_line_to_file (target_filepath, source_filepath);
        free (target_filepath);
      }
      free (source_filepath);
    }
  }
  closedir (source_dir);

  return ACE_SUCCESS;
}
