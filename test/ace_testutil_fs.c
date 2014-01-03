#include <dirent.h>
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

char *ace_tu_fs_mksubdir (char *parent_dirpath, char *child_dirname)
{
  char *full_path = ace_str_join_3 (parent_dirpath, "/", child_dirname);
  mkdir (full_path, S_IRWXU);
  return full_path;
}

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

bool ace_tu_fs_is_directory_empty (char *dirpath)
{
  if(!ace_fs_does_directory_exist (dirpath))
    return true;
  
  bool is_empty = true;
  struct dirent *entity;
  DIR *directory = opendir (dirpath);
  while ((entity = readdir (directory)) != NULL)
  {
    if(!ace_fs_is_current_or_parent_directory (entity->d_name))
    {
      is_empty = false;
      break;
    }
  }
  closedir (directory);
  return is_empty;
}

int ace_tu_fs_get_number_of_files_in_directory (char *dirpath)
{
  DIR *directory;
  struct dirent *entity;
  int number_of_files = 0;
  
  directory = opendir (dirpath);
  while ((entity = readdir (directory)) != NULL)
  {
    if (!ace_fs_is_current_or_parent_directory (entity->d_name))
    {
      number_of_files++;
    }
  }
  closedir (directory);
  
  return number_of_files;
}

int ace_tu_fs_get_number_of_lines_in_file (char *filepath)
{
  FILE *file;
  char *line = NULL;
  ssize_t line_length;
  size_t buffer_length = 0;
  int number_of_lines = 0;
  
  file = fopen(filepath, "r");
  while ((line_length = getline(&line, &buffer_length, file)) != -1)
  {
    number_of_lines++;
  }
  fclose (file);
  
  if (line)
    free (line);
  
  return number_of_lines;
}

bool ace_tu_fs_does_file_contain_line (char *filepath, char *str_without_linebreak)
{
  return ace_tu_fs_does_file_contain_line_at(0, filepath, str_without_linebreak);
}

bool ace_tu_fs_does_file_contain_line_at (int expected_line_number, char *filepath, char *str_without_linebreak)
{
  FILE *file;
  char *line = NULL;
  int line_number = 0;
  ssize_t line_length;
  size_t buffer_length = 0;
  bool does_line_number_match;
  bool result = false;

  file = fopen(filepath, "r");
  while ((line_length = getline(&line, &buffer_length, file)) != -1)
  {
    line_number++;
    line[strlen (line) - 1] = '\0';
    does_line_number_match = (line_number == expected_line_number) || (expected_line_number <= 0);
    if (does_line_number_match && (strcmp (line, str_without_linebreak) == 0))
    {
      result = true;
      break;
    }
  }
  fclose (file);
  
  if (line)
    free(line);
  
  return result;
}
