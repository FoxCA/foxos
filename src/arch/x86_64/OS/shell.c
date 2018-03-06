/*
 * Fox shell layer
 * This is the main shell called after kernel initialization.
 * It will handle all interaction between the user and the system (except programs, of course).
 */

#include <shell.h>

char fox_ascii_art[1209] = "                                                                    ,-,\n                                                              _.-=;~ /_\n                                                           _-~   '     ;.\n                                                       _.-~     '   .-~-~`-._\n                                                 _.--~~:.             --.____88\n                               ____.........--~~~. .' .  .        _..-------~~\n                      _..--~~~~               .' .'             ,'\n                  _.-~                        .       .     ` ,'\n                .'                                    :.    ./\n              .:     ,/          `                   ::.   ,'\n            .:'     ,(            ;.                ::. ,-'\n           .'     ./'.`.     . . /:::._______.... _/:.o/\n          /     ./'. . .)  . _.,'               `88;?88|\n        ,'  . .,/'._,-~ /_.o8P'                  88P ?8b\n     _,'' . .,/',-~    d888P'                    88'  88|\n _.'~  . .,:oP'        ?88b              _..--- 88.--'8b.--..__\n :     ...' 88o __,------.88o ...__..._.=~- .    `~~   `~~      ~-._ Seal _\n `.;;;:='    ~~            ~~~                ~-    -       -   -\n";

unsigned char shell_foreground_colour = white;
unsigned char shell_background_colour = black;

unsigned char kb_leds = 0;
char shell_kb_shifted = 0;
char shell_kb_caps = 0;

int offset_write = 0;
int offset_read = 0;

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


void shell_kb_handler(unsigned char scancode)
{
  #pragma fallthrough
  switch (scancode)
  {
    case 0x2a: // LSHIFT pressed
    case 0x36: // RSHIFT pressed
      shell_kb_shifted = 1;
      return;
    case 0xaa: // LSHIFT released
    case 0xb6: // RSHIFT released
      shell_kb_shifted = 0;
      return;
    case 0x01: // ESC pressed
      shutdown();
      return; // As if this is needed.
    case 0x3b: // F1 (Help)
      nullString(kb_buf, 128);
      processInput("help");
      return;
    case 0x3c: // F2 (Restart)
      reboot();
      return; // As if v2.0 xd
  }
  if (population_count == 128)
  {
    printf("Something went terribly wrong. The keyboard buffer is about to overflow!\n");
    printf("Ima panic now.\n");
    while(1);
  }
  if (offset_write == 127)
    offset_write = 0;
  kb_buf[127-offset_write] = scancode;
}

/*
 * This checks whether the input starts with a comparison string + ' '.
 * The function returns an int value where 0 represents it doesn't start with the comparison string,
 * and 1 if it does.
 */
int starts_with(char *s1, char* s2)
{
  int s1_len = strlen(s1); // Gets the length of the input strings
  int s2_len = strlen(s2);

  if (s1_len < s2_len) // If the comparison string is bigger than the input string, return 0
  {
    return 0;
  }

  for (int i = 0; s2[0] != '\0'; i++)
  {
    if (s1[i] != s2[i]) return 0; // If the current char is not the same as the input string, s1 isn't starting with s2.
  }

  if (s1[s2_len] != ' ') return 0; // If the input string doesn't end with a space, it technically starts with s2, but not for our usage.

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
  char *input_p = &main_process.input;
  nullString(input_p, 79);

  printf("Fox v0.0.1 successfully loaded. Enter \"help\" or \"?\" for help.\n");

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
 */
void getInput(char *buffer, int buf_size)
{
  unsigned char currchar;
  unsigned int counter = 0;

  while (1)
  {
    currchar = getchar();
    counter++;
    if (counter > buf_size || currchar == '\n')
    {
      putchar('\n');
      return;
    }
    if (currchar == '\b')
    {
      buffer--;
      continue;
    }
    *(buffer++) = currchar;
    putchar(currchar);
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
  else if (strcmp(input, " "))
  {
    printf("Unknown command.\n");
    return;
  }
}
