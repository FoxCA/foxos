// cub-kcll | 25.04.2018
// By jonay2000
// Test VGA

#include <string.h>
#include <vga.h>

void kcll_builtin_vgatest() {
  char hex[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
  };
  
  for (int i = 0; i != 15; i++) {
    for (int j = 0; j != 15; j++) {
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
