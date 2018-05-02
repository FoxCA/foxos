/*
 * Kernel Command Line Layer
 * This is the main shell called after kernel initialization.
 * It will handle all interaction between the user and the system (except programs, of course).
 */

#include <kernel.h>
#include "commands.foxconfig"

struct command commands[1024] = {0};
int commandcurrent = 0;


unsigned char shell_foreground_colour = brightwhite;
unsigned char shell_background_colour = black;


#define command_history_size 20
char * command_history[command_history_size];
int command_history_current = 0;
int command_history_pnt = 0;



/*
 * This is the main shell function called by the OS loader.
 * It returns an int value, which represents an error code.
 * 0 - If this is returned, something went very very wrong.
 * 1 - no exit problems
 * 2 - general error, no further specification.
 */


int kcll_start(void)
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
    printf(getcd());
    printf("$");
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
      putc('\n');
      return;
    }
    
    c = keyboard_dequeue();  
    switch(c)
    {
      // case 0x48://up arrow
      //   while(--counter>=0){
      //     buffer--;
      //     putc('\b');
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
        puts("\n");
        *buffer = 0x00;
        return;   
        break;
      case '\b':
        if(counter > 0){
          putc('\b');
          buffer--;
          counter--;
        }
        break;
      default:
        shouldprint = true;
    }
    
    if(c != 8)
    {
      counter++;
      *(buffer++) = c;
    }

    if(counter >= 0 && shouldprint)
    {
      putc(c);
    }
  }
}

void kcll_register_command(struct command c)
{
  commands[commandcurrent] = c;
  commandcurrent++;
}

/*
 * This processes all input. It takes in a null-terminated string and calls the appropriate command.
 */
void processInput(char *input)
{
  int i;
  for (i = 0; i < commandcurrent; ++i)
  {
    list_t * argv = str_split(input," ",0);

    if(strcmp((char *)list_get_node_by_index(argv,0)->val, commands[i].prefix)==0)
    {
      if(list_size(argv) < commands[i].arguments)
      {
        printf("Not enough arguments given. %i expected, got %i\n",commands[i].arguments, list_size(argv) - 1);    
      }
      else
      {
        list_remove_by_index(argv, 0);
        
        if(commands[i].arguments == 0 && !commands[i].giveargs)
        {
          commands[i].callback.noargv();
        }
        else
        { 
          commands[i].callback.argv(argv,list_size(argv));
        }
      }
      return;
    }
  }
  printf("Unknown command.\n");
}
