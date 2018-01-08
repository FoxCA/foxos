/*
 * Fox shell layer
 * This is the main shell called after kernel initialization.
 * It will handle all interaction between the user and the system (except programs, of course).
 */

#include <shell.h>

int starts_with(char *s1, char* s2)
{
  int s1_len = strlen(s1);
  int s2_len = strlen(s2);

  if (s1_len < s2_len)
  {
    return 0;
  }

  for (int i = 0; s2[0] != '\0'; i++)
  {
    if (s1[i] != s2[i]) return 0;
    if (i+1 == s2_len) break;
  }
  return 1;
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
    getline(input_p, 79);
    processInput(input_p);
    nullString(input_p, 79);
  }

  return main_process.state;
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
  else if (starts_with(input, "color"))
  {

  }
  else if (strcmp(input, " "))
  {
    printf("Unknown command.\n");
    return;
  }
}
