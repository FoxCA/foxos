#include <kernel.h>

unsigned char kb_leds = 0;
bool kb_led_numlock = false;
bool kb_led_caps = false;
bool kb_led_scroll = false;

bool shell_kb_shifted = false;
bool shell_kb_caps = false;

int offset_write = 0;
int offset_read = 0;

char shell_scan_US[128] =
{
  0, // 0x00
  0, // 0x01
  '1', // 0x02
  '2', // ...
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  '0',
  '-',
  '=',
  '\b', // BackSpace
  '\t', // Tabulator
  'q',
  'w',
  'e',
  'r',
  't',
  'y',
  'u',
  'i',
  'o',
  'p',
  '[',
  ']',
  '\n', // Enter
  0, // LeftControl
  'a',
  's',
  'd',
  'f',
  'g',
  'h',
  'j',
  'k',
  'l',
  ';',
  '\'',
  '`',
  0, // LeftShift
  '\\',
  'z',
  'x',
  'c',
  'v',
  'b',
  'n',
  'm',
  ',',
  '.',
  '/',
  0, // RightShift
  0x7c, // keypad *
  0, // LeftAlt
  ' ', // Space
  0, // CapsLock
  0x61, // F1
  0x62, // F2
  0x63, // F3
  0x64, // F4
  0x65, // F5
  0x66, // F6
  0x67, // F7
  0x68, // F8
  0x69, // F9
  0x6a, // F10
  0, // NumLock
  0, // ScrollLock
  0x77, // keypad 7
  0x78, // keypad 8
  0x79, // keypad 9
  0x7b, // keypad -
  0x74, // keypad 4
  0x75, // keypad 5
  0x76, // keypad 6
  0x7a, // keypad +
  0x71, // keypad 1
  0x72, // keypad 2
  0x73, // keypad 3
  0x70, // keypad 0
  0x7e, // keypad .
  0, 0, 0,
  0x6b, // F11
  0x6c // F12
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

/*
 * The status byte sent to the keyboard:
 * 00000xxx
 *      ^^^
 *      ||+-> scroll lock
 *      |+--> numpad lock
 *      +---> caps   lock
 */

void change_caps_led(bool status)
{
  if ((kb_leds & 0b00000100) == 0)
  {
    kb_leds = kb_leds + (status ? 4 : 0);
  }
  else
  {
    kb_leds = kb_leds - (status ? 0 : 4);
  }
  kb_update_leds();
}

void change_numlock_led(bool status)
{
  if ((kb_leds & 0b00000010) == 0)
  {
    kb_leds = kb_leds + (status ? 2 : 0);
  }
  else
  {
    kb_leds = kb_leds - (status ? 0 : 2);
  }
  kb_update_leds();
}

void change_scroll_led(bool status)
{
  if ((kb_leds & 0b00000001) == 0)
  {
    kb_leds = kb_leds + (status ? 1 : 0);
  }
  else
  {
    kb_leds = kb_leds - (status ? 0 : 1);
  }
  kb_update_leds();
}

void kb_ack(void)
{
  while (inportb(0x60) != 0xfa);
}

void kb_update_leds(void)
{
  outportb(0x60, 0xed);
  kb_ack();
  outportb(0x60, kb_leds);
}

void shell_kb_handler(unsigned char scancode)
{
  switch (scancode)
  {
    case 0x00:
      return;
    case 0xff: // A keyboard fuckup
      putc('\b');
      return;
    case 0x2a: // LSHIFT pressed
    case 0x36: // RSHIFT pressed
      shell_kb_shifted = true;
      return;
    case 0xaa: // LSHIFT released
    case 0xb6: // RSHIFT released
      shell_kb_shifted = false;
      return;
    case 0x01: // ESC pressed
      shutdown();
      return; // As if this is needed.
    case 0x3b: // F1 (Help)
      halt_read = true;
      processInput("help");
      return;
    case 0x3c: // F2 (Restart)
      halt_read = true;
      reboot();
      return; // As if v2.0 xd
    case 0x3a: // Caps pressed
      shell_kb_caps = !shell_kb_caps;
      change_caps_led(shell_kb_caps);
      return;
    case 0xba: // Caps released
      return;
    case 0x45: // Numlock pressed
      kb_led_numlock = !kb_led_numlock;
      change_numlock_led(kb_led_numlock);
      return;
    case 0x46:
      kb_led_scroll = !kb_led_scroll;
      change_scroll_led(kb_led_scroll);
      return;
    default:
      break;
  }
  
  unsigned char scan_keymap_index = scancode & 0b01111111; // The keymap scan index (with the potential released flag bit removed).
  
  if ((scancode & 0b10000000) > 0) // Is the released flag bit set? 0bXyyyyyyy X = flag, y = scancode
  {
    return;
  }
  
  if (shell_kb_caps || shell_kb_shifted)
  {
    keyboard_enqueue(shell_scan_shift_US[scan_keymap_index]);
  }
  else
  {
    keyboard_enqueue(shell_scan_US[scan_keymap_index]);
  }
}