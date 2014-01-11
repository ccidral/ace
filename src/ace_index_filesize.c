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

char *choose_index_filepath (char *index_filepath)
{
  char *singleton_filepath;
  
  if (ace_fs_does_file_exist (index_filepath))
  {
    return index_filepath;
  }
  
  singleton_filepath = ace_str_join_2 (index_filepath, ".singleton");
  
  if (ace_fs_does_file_exist (singleton_filepath))
  {
    rename (singleton_filepath, index_filepath);
    free (singleton_filepath);
    return index_filepath;
  }
  else
  {
    free (index_filepath);
    return singleton_filepath;
  }
}

int index_file (const char *filepath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
  if (S_ISREG(sb->st_mode))
  {
    char *index_filepath;
    off_t file_size;
    char *file_size_str;
  
    file_size = ace_fs_get_file_size (filepath);
    
    if (file_size > 0)
    {
      file_size_str = file_size_to_str (file_size);
      index_filepath = choose_index_filepath (ace_str_join_3 (index_dirpath, "/", file_size_str));
    
      ace_fs_append_line_to_file (index_filepath, filepath);
    
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
