#ifndef FOX_SHELL_H
#define FOX_SHELL_H



typedef struct
{
  int state;
  int loop;
  char input[79]; // TODO: replace with malloc resizing later on
} state;

typedef struct
{
  unsigned char flags;
  char key[11];
} key_press;

typedef void (*callback_argv)(list_t * argv, int argc);
typedef void (*callback_noargv)();

typedef union callback{
  callback_noargv noargv;
  callback_argv argv;
}callback;

typedef struct command{
  char * prefix;
  int arguments;
  callback callback;
  int giveargs;
}command;

unsigned char shell_foreground_colour;
unsigned char shell_background_colour;

state main_process;


bool halt_read;

char irq_input_exec[32];

char shell_scan_US[128];

char shell_scan_shift_US[128];

void change_kb_led(unsigned char index, unsigned char value);
void shell_kb_handler(unsigned char scancode);

void getInput(char *buffer, int buf_length);

int starts_with(char *s1, char* s2);

int kcll_start();

void kcll_register_command(struct command c);
void processInput(char *input);

#endif
