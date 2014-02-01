#include <stdio.h>
#include "ace.h"
#include "ace_errcode.h"
#include "ace_fs.h"
#include "ace_index_filesize.h"

const int NUMBER_OF_EXPECTED_ARGUMENTS = 3;

int files_found = 0;

void print_usage ();
void report_file_found (const char *path);

int main (int argc, char **argv)
{
  if (argc != NUMBER_OF_EXPECTED_ARGUMENTS)
  {
    print_usage ();
    return ACE_ERR_WRONG_NUMBER_OF_ARGUMENTS;
  }
  
  char *source_directory = argv[1];
  char *index_directory = argv[2];
  
  if (!ace_fs_does_directory_exist (source_directory))
  {
    printf ("Error: source directory does not exit or is not a directory\n");
    return ACE_ERR_NO_SOURCE_DIR;
  }
  
  if (!ace_fs_does_directory_exist (index_directory))
  {
    printf ("Error: index directory does not exist or is not a directory\n");
    return ACE_ERR_NO_TARGET_DIR;
  }
  
  printf ("Indexing files by size:\n");
  int error_code = ace_index_filesize (source_directory, index_directory, report_file_found);
  
  if(error_code)
  {
    printf ("\nError: %d\n", error_code);
    return error_code;
  }
  
  return 0;
}

void report_file_found (const char *path)
{
  printf ("\r%d", ++files_found);
}

void print_usage ()
{
  printf ("Usage: ace <source_directory> <index_directory>\n\n");
  
  printf ("  source_directory: absolute path of the directory\n");
  printf ("                    to be scanned for duplicate files.\n\n");
    
  printf ("  index_directory: absolute path of the directory\n");
  printf ("                   where index files will be created.\n\n");
}
