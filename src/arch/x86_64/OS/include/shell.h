#ifndef FOX_SHELL_H
#define FOX_SHELL_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <structs.h>
#include <unistd.h>
#include <system.h>
#include <vga.h>

char fox_ascii_art[1209] = "                                                                    ,-,\n                                                              _.-=;~ /_\n                                                           _-~   '     ;.\n                                                       _.-~     '   .-~-~`-._\n                                                 _.--~~:.             --.____88\n                               ____.........--~~~. .' .  .        _..-------~~\n                      _..--~~~~               .' .'             ,'\n                  _.-~                        .       .     ` ,'\n                .'                                    :.    ./\n              .:     ,/          `                   ::.   ,'\n            .:'     ,(            ;.                ::. ,-'\n           .'     ./'.`.     . . /:::._______.... _/:.o/\n          /     ./'. . .)  . _.,'               `88;?88|\n        ,'  . .,/'._,-~ /_.o8P'                  88P ?8b\n     _,'' . .,/',-~    d888P'                    88'  88|\n _.'~  . .,:oP'        ?88b              _..--- 88.--'8b.--..__\n :     ...' 88o __,------.88o ...__..._.=~- .    `~~   `~~      ~-._ Seal _\n `.;;;:='    ~~            ~~~                ~-    -       -   -\n";

unsigned char shell_foreground_colour = white;
unsigned char shell_background_colour = black;

int starts_with(char *s1, char* s2);

int shell_start(void);

void processInput(char *input);

#endif
