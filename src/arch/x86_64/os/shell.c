/*
 * Fox shell layer
 * This is the main shell called after kernel initialization.
 * It will handle all interaction between the user and the system (except programs, of course).
 */

#include <shell.h>
#include "commands"
#include <list.h>

struct command commands[1024] = {0};
int commandcurrent = 0;


unsigned char shell_foreground_colour = brightwhite;
unsigned char shell_background_colour = black;

unsigned char kb_leds = 0;
bool kb_led_numlock = false;
bool kb_led_caps = false;
bool kb_led_scroll = false;

bool shell_kb_shifted = false;
bool shell_kb_caps = false;

int offset_write = 0;
int offset_read = 0;

#define command_history_size 20
char * command_history[command_history_size];
int command_history_current = 0;
int command_history_pnt = 0;

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
    
  settextcolor(lightblue, shell_background_colour);

  printf("Fox ");
  printf(FOX_VERSION);
  printf(" successfully loaded. Enter \"help\" or \"?\" for help.\n");

  setup_commands();

  while (main_process.loop)
  {
    settextcolor(shell_foreground_colour, shell_background_colour);
    printf("> ");
    getInput(input_p, 79);
  
    command_history[command_history_pnt%command_history_size] = input_p;
    command_history_pnt++;

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
  int shouldprint;
  
  while (counter < buf_size)
  {
    shouldprint = false;

    if (halt_read)
    {
      halt_read = false;
      nullString(buffer, buf_size);
      putchar('\n');
      return;
    }
    
    c = keyboard_dequeue();  
    switch(c){
      // case 0x48://up arrow
      //   while(--counter>=0){
      //     buffer--;
      //     putchar('\b');
      //   }
      //   break;
      // case 0x4b://left arrow
      //   break;
      // case 0x6d://right arrow
      //   break;
      // case 0x50://down arrow
      //   break;
      case 0x00:
        continue;
      case '\n':
        putchar(c);
        *buffer = 0x00;
        return;   
        break;
      case '\b':
        if(counter > 0){
          putchar('\b');
          buffer--;
          counter--;
        }
        break;
      default:
        shouldprint = true;
    }
    
    if(c != 8){
      counter++;
      *(buffer++) = c;
    }

    if(counter >= 0 && shouldprint){
      putchar(c);
    }
  }
}

/*
 * This processes all input. When malloc() is done, it will be dynamic, but for now, it's static interpretation.
 */

void kcll_register_command(struct command c){
  commands[commandcurrent] = c;
  commandcurrent++;
}


void processInput(char *input)
{
  for (int i = 0; i < commandcurrent; ++i)
  {
    list_t * argv = str_split(input," ",0);


    if(starts_with((char *)list_get_node_by_index(argv,0)->val, commands[i].prefix)){
      if(list_size(argv) < commands[i].arguments){
        printf("not enough arguments given\n");    
      }else{
        list_remove_by_index(argv, 0);
        if(commands[i].arguments == 0){
          commands[i].callback.noargv();
        }else{ 
          commands[i].callback.argv(argv,list_size(argv));
        }
      }
      return;
    }
  }
  printf("Unknown command.\n");
}
