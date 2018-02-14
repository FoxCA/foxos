/*
 * Fox shell layer
 * This is the main shell called after kernel initialization.
 * It will handle all interaction between the user and the system (except programs, of course).
 */

#include <shell.h>

void set_kb_led(unsigned char index, unsigned char value)
{
  // index 0 = all leds, index 11 = scroll lock, index 2 = num lock, index 3 = caps lock
  switch (index)
  {
    case 0:
      kb_leds = value;
      break;
    case 1:
      kb_leds = ((kb_leds << 7) >> 7) == 0 ? kb_leds + value : (value == 0 ? kb_leds - 1 : kb_leds);
      break;
    case 2:
      kb_leds = (((kb_leds >> 1) << 7) >> 7) == 0 ? kb_leds + value : (value == 0 ? kb_leds - 2 : kb_leds);
      break;
    case 3:
      kb_leds = (((kb_leds >> 2) << 7) >> 7) == 0 ? kb_leds + value : (value == 0 ? kb_leds - 4 : kb_leds);
      break;
  }

  outportb(0x60, 0xed);
  outportb(0x60, kb_leds);
}

#pragma fallthrough
void shell_kb_handler(unsigned char scancode)
{
  if (scancode == 0xaa || scancode == 0xb6)
  {
    shell_kb_shifted = 0;
    return;
  }
  if (scancode & 0x80) return;

  char input[11];
  strcpy(input, shell_scan_US[scancode]);

  if (starts_with(input, "escape"))
  {
    main_process.loop = false;
    shutdown();
    return;
  }
  else if (starts_with(input, "backspace"))
  {
    keyboard_enqueue('\b');
    return;
  }
  else if (starts_with(input, "tab"))
  {
    return;
  }
  else if (starts_with(input, "enter"))
  {
    keyboard_enqueue('\n');
    return;
  }
  else if (starts_with(input, "lctrl"))
  {
    return;
  }
  else if (starts_with(input, "lshift"))
  {
    shell_kb_shifted = 1;
    return;
  }
  else if (starts_with(input, "rshift"))
  {
    shell_kb_shifted = 1;
    return;
  }
  else if (starts_with(input, "lalt"))
  {
    return;
  }
  else if (starts_with(input, "space"))
  {
    keyboard_enqueue(' ');
    return;
  }
  else if (starts_with(input, "capslock"))
  {
    set_kb_led(3, (((~kb_leds) >> 2) << 7) >> 7);
    shell_kb_caps = shell_kb_caps ? 0 : 1;
    return;
  }
  else if (starts_with(input, "numlock"))
  {
    set_kb_led(2, (((~kb_leds) >> 1) << 7) >> 7);
    return;
  }
  else if (starts_with(input, "scrolllock"))
  {
    set_kb_led(1, ((~kb_leds) << 7) >> 7);
    return;
  }
  else if ((input[0] == 'f') && (input[1] != '\0'))
  {
    switch (input[1])
    {
      case '1':
        strcpy(irq_input_exec, "help");
        keyboard_enqueue(1);
        break;
      case '2':
        main_process.loop = false;
        reboot();
        break;
      default:
        break;
    }
    return;
  }
  else
  {
    if (shell_kb_caps || shell_kb_shifted)
    {
      keyboard_enqueue(shell_scan_shift_US[scancode]);
    }
    else
    {
      keyboard_enqueue(input[0]);
    }
  }
}

int getinput(char *buffer, int buf_length)
{
  int in_char;
  int i = 0;
  while (i < buf_length)
  {
    in_char = getchar();
    if (in_char == 1)
    {
      while (i > 0)
      {
        *(--buffer) = '\0';
        i--;
        putchar('\b');
      }
      return 0;
    }
    else if (in_char == '\n')
    {
      *buffer == '\0';
      putchar(in_char);
      break;
    }
    else if (in_char == '\b')
    {
      if (i > 0)
      {
        *(--buffer) = '\0';
        i--;
        putchar('\b');
      }
    }
    else
    {
      *buffer++ = (char)in_char;
      putchar(in_char);
      i++;
    }
  }
  return 1;
}

int starts_with(char *s1, char* s2)
{
  int s1_len = stringlength(s1);
  int s2_len = stringlength(s2);

  for (int i = 0; s2[i] != '\0'; i++)
  {
    if (s1[i] != s2[i]) return 0;
    if (i+1 == s1_len && i+1 < s2_len) return 0;
    if (i+1 == s1_len && i+1 == s2_len) break;
  }

  if (s1[s2_len] != '\0' && s1[s2_len] != ' ') return 0;

  return 1;
}

void malloctest(){
  int i = 5;
  int n;
  char * buffer;

  buffer = (char*) malloc (i+1);

  for (n=0; n<i; n++)
    buffer[n]=rand()%26+'a';
  buffer[i]='\0';

  printf ("Random string: %s\n",buffer);
  free (buffer);
}

/*
 * This is the main shell function called by the OS loader.
 * It returns an int value, which represents an error code.
 * 0 - general error, usually everything should be handled by the OS, including shutdowns. return = fuckup
 */
int shell_start(void)
{
  main_process.state = 0;
  main_process.loop = true;

  char *input_p = &main_process.input;
  nullString(input_p, 79);

  kb_handler = &shell_kb_handler;

  set_kb_led(0, 7);

  nullString(irq_input_exec, 32);

  printf("Fox v0.0.1 successfully loaded. Enter \"help\" or \"?\" for help.\n");

  while (main_process.loop)
  {
    settextcolor(shell_foreground_colour, shell_background_colour);
    printf("> ");
    int idk = getinput(input_p, 79);
    processInput(idk == 0 ? irq_input_exec : input_p);
    nullString(idk == 0 ? irq_input_exec : input_p, 79);
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
    printf("help | ? - Display this help page.   Keyboard alias: [F1]\n");
    printf("reboot   - Reboot the computer.      Keyboard alias: [F2]\n");
    printf("shutdown - Shut the computer down.   Keyboard alias: [ESC]\n");
    printf("vgatest  - Show the hex color codes.\n");
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
  }else if(starts_with(input, "memtest")){
    malloctest();
  }
  else if (strcmp(input, " "))
  {
    printf("Unknown command.\n");
    return;
  }
}
