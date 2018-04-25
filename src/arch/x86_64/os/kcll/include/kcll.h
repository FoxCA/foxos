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

char fox_ascii_art[1209];

unsigned char shell_foreground_colour;
unsigned char shell_background_colour;

state main_process;

unsigned char kb_leds;
bool kb_led_numlock;
bool kb_led_caps;
bool kb_led_scroll;

bool shell_kb_shifted;
bool shell_kb_caps;

void change_caps_led(bool status);
void change_numlock_led(bool status);
void change_scroll_led(bool status);

void kb_ack(void);
void kb_update_leds(void);

bool halt_read;

char irq_input_exec[32];

char shell_scan_US[128];

char shell_scan_shift_US[128];

void change_kb_led(unsigned char index, unsigned char value);
void shell_kb_handler(unsigned char scancode);

void getInput(char *buffer, int buf_length);

int starts_with(char *s1, char* s2);

int shell_start(void);

void processInput(char *input);

#endif
