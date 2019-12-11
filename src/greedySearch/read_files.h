#ifndef READ_FILES_HEAD
#define READ_FILES_HEAD

#include <stdio.h>

int    nof_lines(const char* filename);
char*  create_fn(const char* dirname, int i, const char* ext);
FILE*  open_file(const char* dirname, int i, const char* ext, const char* mode);
FILE** open_files(int nof_vars, const char* dirname, const char* ext, const char* mode);
void   free_files(int nof_vars, FILE** files);

#define FREAD(A,B,C,D) {size_t _nmemb = fread(A,B,C,D); (void) _nmemb;}

#endif