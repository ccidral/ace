#include <dirent.h>
#include <ftw.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ace_errcode.h"
#include "ace_fs.h"
#include "ace_string.h"
#include "ace_index_filesize.h"

char *index_dirpath;

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

int index_file (const char *source_filepath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
  if (S_ISREG(sb->st_mode))
  {
    char *index_filepath;
    off_t file_size;
    char *file_size_str;
  
    file_size = ace_fs_get_file_size (source_filepath);
    if (file_size > 0)
    {
      file_size_str = file_size_to_str (file_size);
      index_filepath = choose_target_filepath (ace_str_join_3 (index_dirpath, "/", file_size_str));
    
      ace_fs_append_line_to_file (index_filepath, source_filepath);
    
      free (index_filepath);
      free (file_size_str);
    }
  }
  
  return 0;
}

int ace_index_filesize (char *source_dirpath, char *target_dirpath)
{
  if (!ace_fs_does_directory_exist (source_dirpath))
    return ACE_ERR_NO_SOURCE_DIR;

  if (!ace_fs_does_directory_exist (target_dirpath))
    return ACE_ERR_NO_TARGET_DIR;
  
  index_dirpath = target_dirpath;
  
  int error = nftw (source_dirpath, index_file, 64, FTW_DEPTH | FTW_PHYS);

  if(error) {
    perror(source_dirpath);
    return ACE_UNKNOWN_ERROR;
  }

  return ACE_SUCCESS;
}
