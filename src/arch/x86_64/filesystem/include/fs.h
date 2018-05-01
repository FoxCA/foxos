
#ifndef FS_H
#define FS_H

#include <vfs.h>
#include <list.h>



char * cd(char * directory);
char * getcd();
list_t * ls(char * directory);
char * cat(char * filename);
void pushd(char * directory);
void popd(char * directory);
void mkdir(char * directory);
void fs_init();
char *canonicalize_path(char *cwd, char *input);

#endif