#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

#include "ace_errcode.h"
#include "ace_fs.h"
#include "ace_string.h"
#include "ace_index_filesize.h"

char *file_size_to_str (off_t file_size)
{
  char *str = malloc (32);
  snprintf (str, 32, "%lld", file_size);
  return str;
}

char *choose_target_filepath (char *target_filepath)
{
  char *singleton_filepath;
  
  if (ace_fs_does_file_exist (target_filepath))
  {
    return target_filepath;
  }
  
  singleton_filepath = ace_str_join_2 (target_filepath, ".singleton");
  
  if (ace_fs_does_file_exist (singleton_filepath))
  {
    rename (singleton_filepath, target_filepath);
    free (singleton_filepath);
    return target_filepath;
  }
  else
  {
    free (target_filepath);
    return singleton_filepath;
  }
}

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
  char *file_size_str;
  
  source_dir = opendir (source_dirpath);
  
  while ((entity = readdir (source_dir)) != NULL)
  {
    if (!ace_fs_is_current_or_parent_directory (entity->d_name))
    {
      source_filepath = ace_str_join_3 (source_dirpath, "/", entity->d_name);
      file_size = ace_fs_get_file_size (source_filepath);
      if (ace_fs_get_file_size (source_filepath) > 0)
      {
        file_size_str = file_size_to_str (file_size);
        target_filepath = choose_target_filepath (ace_str_join_3 (target_dirpath, "/", file_size_str));
        
        ace_fs_append_line_to_file (target_filepath, source_filepath);
        
        free (target_filepath);
        free (file_size_str);
      }
      free (source_filepath);
    }
  }
  closedir (source_dir);

  return ACE_SUCCESS;
}
