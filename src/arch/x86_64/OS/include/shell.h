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
#include <heap.h>

char fox_ascii_art[1209];
unsigned char shell_foreground_colour;
unsigned char shell_background_colour;
state main_process;
unsigned char kb_leds;
char shell_kb_shifted;
char shell_kb_caps;

char irq_input_exec[32];

char shell_scan_US[128][11];

char shell_scan_shift_US[128];

void change_kb_led(unsigned char index, unsigned char value);
void shell_kb_handler(unsigned char scancode);

int getinput(char *buffer, int buf_length);

int starts_with(char *s1, char* s2);

int shell_start(void);

void processInput(char *input);

#endif
