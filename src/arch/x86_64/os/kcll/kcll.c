/*
 * kcll | 25.04.2018
 * By jonay2000
 * Primitive REPL for the Fox operating system.
 * Called after kernel initialization (most probably on failure)
 */

#include <kcll.h>
#include <kcll_builtin.h>

char fox_ascii_art[1209] = "                                                                    ,-,\n                                                              _.-=;~ /_\n                                                           _-~   '     ;.\n                                                       _.-~     '   .-~-~`-._\n                                                 _.--~~:.             --.____88\n                               ____.........--~~~. .' .  .        _..-------~~\n                      _..--~~~~               .' .'             ,'\n                  _.-~                        .       .     ` ,'\n                .'                                    :.    ./\n              .:     ,/          `                   ::.   ,'\n            .:'     ,(            ;.                ::. ,-'\n           .'     ./'.`.     . . /:::._______.... _/:.o/\n          /     ./'. . .)  . _.,'               `88;?88|\n        ,'  . .,/'._,-~ /_.o8P'                  88P ?8b\n     _,'' . .,/',-~    d888P'                    88'  88|\n _.'~  . .,:oP'        ?88b              _..--- 88.--'8b.--..__\n :     ...' 88o __,------.88o ...__..._.=~- .    `~~   `~~      ~-._ Seal _\n `.;;;:='    ~~            ~~~                ~-    -       -   -\n";

unsigned char shell_foreground_colour = white;
unsigned char shell_background_colour = black;

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
      putchar('\b');
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

/*
 * This checks whether the input starts with a comparison string + ' '.
 * The function returns an int value where 0 represents it doesn't start with the comparison string,
 * and 1 if it does.
 */
int starts_with(char *s1, char* s2)
{
  if(strncmp(s1, s2, strlen(s2)) == 0){
    return 1;
  };
  return 0;
  //old startswith (before strncmp)
  // int s1_len = strlen(s1); // Gets the length of the input strings
  // int s2_len = strlen(s2);


  // if (s1_len < s2_len) // If the comparison string is bigger than the input string, return 0
  // {
  //   return 0;
  // }

  // for (int i = 0; s2[0] != '\0'; i++)
  // {
  //   printf(s1[i]);
  //   printf(s2[i]);
  //   printf(">");
  //   if (s1[i] != s2[i]) {
  //     return 0; // If the current char is not the same as the input string, s1 isn't starting with s2.
  //   }
  // }

  // if (s1[s2_len] != ' ') return 0; // If the input string doesn't end with a space, it technically starts with s2, but not for our usage.

  return 1; // Everything is fine!
}

/*
 * This is the main shell function called by the OS loader.
 * It returns an int value, which represents an error code.
 * 0 - If this is returned, something went very very wrong.
 * 1 - no exit problems
 * 2 - general error, no further specification.
 */
int shell_start(void)
{
  state main_process;
  main_process.state = 0;
  main_process.loop = true;
  char *input_p = main_process.input;
  nullString(input_p, 79);
  change_caps_led(true);
  
  set_kb_handler(&shell_kb_handler);
  
  printf("kcll v2:25.04.2018");
  printf("kcll successfully loaded. Enter \"help\" or \"?\" for help.\n");

  while (main_process.loop)
  {
    settextcolor(shell_foreground_colour, shell_background_colour);
    printf("> ");
    getInput(input_p, 79);
    processInput(input_p);
    nullString(input_p, 79);
  }

  return main_process.state;
}

/*
 * This function fetches user input given a buffer pointer and a buffer size.
 * I call the memory model, which the keyboard buffer works with, "runaway buffer". Imagine an array of data:
 * […][10][9][8][7][6][5][4][3][2][1][0]
 * Note the reverse indexing, this is not completely important, it would work the other way around, but this
 * order of indexing has a few advantages, for one you are less prone to forgetting to loop write. If you forget
 * to loop the write, you can overwrite potentially important data (especially when working in a kernel).
 * 
 * The writing will start at 0, go to 1, then 2, and so on. 
 */
void getInput(char *buffer, int buf_size)
{
  int counter = 0;
  char c;
  
  while (counter < buf_size)
  {
    if (halt_read)
    {
      halt_read = false;
      nullString(buffer, buf_size);
      putchar('\n');
      return;
    }
    
    c = keyboard_dequeue();
    
    if (c == 0x00)
      continue;
    
    if (c == '\n')
    {
      putchar(c);
      *buffer = 0x00;
      return;
    }
    
    *(buffer++) = c;
    putchar(c);
  }
}

/*
 * This processes all input. When malloc() is done, it will be dynamic, but for now, it's static interpretation.
 */
void processInput(char *input)
{
  if (starts_with(input, "help") || starts_with(input, "?"))
  {
    printf("Fox v0.0.1 shell.\n");
    printf("help - Display this help page.\n");
    printf("shutdown - Shut the computer down. You can also press [ESC]!\n");
    printf("? - Display this help page.\n");
    return;
  }
  else if (starts_with(input, "shutdown"))
  {
    shutdown();
  }
  else if (starts_with(input, "reboot"))
  {
    reboot();
  }
  else if (starts_with(input, "fox"))
  {
    printf("You have found the secret fox ASCII art! :D Enjoy!\n");
    settextcolor(brown, black);
    puts(fox_ascii_art); // puts, because malloc.. lel
    settextcolor(white, black);
    return;
  }
  else if (starts_with(input, "vgatest"))
  {
    char hex[16] = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    for (int i = 0; i != 15; i++)
    {
      for (int j = 0; j != 15; j++)
      {
        settextcolor((unsigned char)i, (unsigned char)j);
        putchar(hex[i]);
        putchar(hex[j]);
        settextcolor(white, black);
        putchar(' ');
      }
      putchar('\n');
    }
    return;
  }
  else if (starts_with(input, "clear"))
  {
    set_csr_xy(0, 0);
    for (int i = 0; i <= 80*25; i++)
    {
      printf(" ");
    }
    set_csr_xy(0, 0);
    return;
  }
  else if(starts_with(input,"test")){
    PANIC("test");
  }else if(starts_with(input,"time")){
    printf("%i\n",timer_get_time_since_boot());
  }
  else
  {
    printf("Unknown command.\n");
    return;
  }
}
