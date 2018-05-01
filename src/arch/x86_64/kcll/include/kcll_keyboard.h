#ifndef KCLL_KEYBOARD_H
#define KCLL_KEYBOARD_H

#include <stdbool.h>

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

#endif
