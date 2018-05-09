
#ifndef FS_H
#define FS_H

#include <vfs.h>
#include <list.h>

#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STRING "/"
#define PATH_UP  ".."
#define PATH_DOT "."

void cd(char * directory);
char * getcd();
list_t * ls(char * directory);
char * cat(char * filename);
void pushd(char * directory);
void popd();
void mkdir(char * directory);
void fs_init();
char *canonicalize_path(char *cwd, char *input);
int path_exists(char * name);
void exec_elf(char * file);

#endif