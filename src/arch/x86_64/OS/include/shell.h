#ifndef FOX_SHELL_H
#define FOX_SHELL_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <structs.h>
#include <unistd.h>

int starts_with(char *s1, char* s2);

int shell_start(void);

void processInput(char *input);

#endif
