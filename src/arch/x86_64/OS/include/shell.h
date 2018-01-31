#ifndef FOX_SHELL_H
#define FOX_SHELL_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <structs.h>
#include <unistd.h>
#include <system.h>
#include <vga.h>
#include <kbbuf.h>

char fox_ascii_art[1209] = "                                                                    ,-,\n                                                              _.-=;~ /_\n                                                           _-~   '     ;.\n                                                       _.-~     '   .-~-~`-._\n                                                 _.--~~:.             --.____88\n                               ____.........--~~~. .' .  .        _..-------~~\n                      _..--~~~~               .' .'             ,'\n                  _.-~                        .       .     ` ,'\n                .'                                    :.    ./\n              .:     ,/          `                   ::.   ,'\n            .:'     ,(            ;.                ::. ,-'\n           .'     ./'.`.     . . /:::._______.... _/:.o/\n          /     ./'. . .)  . _.,'               `88;?88|\n        ,'  . .,/'._,-~ /_.o8P'                  88P ?8b\n     _,'' . .,/',-~    d888P'                    88'  88|\n _.'~  . .,:oP'        ?88b              _..--- 88.--'8b.--..__\n :     ...' 88o __,------.88o ...__..._.=~- .    `~~   `~~      ~-._ Seal _\n `.;;;:='    ~~            ~~~                ~-    -       -   -\n";

unsigned char shell_foreground_colour = white;
unsigned char shell_background_colour = black;

state main_process;

unsigned char kb_leds = 0;
char shell_kb_shifted = 0;
char shell_kb_caps = 0;

char irq_input_exec[32];

char shell_scan_US[128][11] =
{
  "",
  "escape", // 0x01
  "1", // 0x02
  "2", // ...
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "0",
  "-",
  "=",
  "backspace",
  "tab",
  "q",
  "w",
  "e",
  "r",
  "t",
  "y",
  "u",
  "i",
  "o",
  "p",
  "[",
  "]",
  "enter",
  "lctrl",
  "a",
  "s",
  "d",
  "f",
  "g",
  "h",
  "j",
  "k",
  "l",
  ";",
  "\'",
  "`",
  "lshift",
  "\\",
  "z",
  "x",
  "c",
  "v",
  "b",
  "n",
  "m",
  ",",
  ".",
  "/",
  "rshift",
  "keypad*",
  "lalt",
  "space",
  "capslock",
  "f1",
  "f2",
  "f3",
  "f4",
  "f5",
  "f6",
  "f7",
  "f8",
  "f9",
  "f10",
  "numlock",
  "scrolllock",
  "keypad7",
  "keypad8",
  "keypad9",
  "keypad-",
  "keypad4",
  "keypad5",
  "keypad6",
  "keypad+",
  "keypad1",
  "keypad2",
  "keypad3",
  "keypad0",
  "keypad.",
  "", "", "",
  "f11",
  "f12"
};

char shell_scan_shift_US[128] =
{
  0, 0,
  '!',
  '@',
  '#',
  '$',
  '%',
  '^',
  '&',
  '*',
  '(',
  ')',
  '_',
  '+',
  0, 0,
  'Q',
  'W',
  'E',
  'R',
  'T',
  'Y',
  'U',
  'I',
  'O',
  'P',
  '{',
  '}',
  0, 0,
  'A',
  'S',
  'D',
  'F',
  'G',
  'H',
  'J',
  'K',
  'L',
  ':',
  '"',
  '~',
  0,
  '|',
  'Z',
  'X',
  'C',
  'V',
  'B',
  'N',
  'M',
  '<',
  '>',
  '?'
};

void change_kb_led(unsigned char index, unsigned char value);
void shell_kb_handler(unsigned char scancode);

int getinput(char *buffer, int buf_length);

int starts_with(char *s1, char* s2);

int shell_start(void);

void processInput(char *input);

#endif
