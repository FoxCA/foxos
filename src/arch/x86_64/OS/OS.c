#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void nullString(char *s, int l) // Nulls a string given a &pointer and a length.
{
	for (int i = 0; i != l; i++)
	{
		*s = 0x00;
		s++;
	}
}

void OS(){
	// printf(0);
	char buf[81] = "";
	int index = 0;
	char in;
	nullString(&buf, 81);
	while(1){
		in = getchar();
		if (index == 80 && in != '\n')
		{
			printf("%s\n", buf);
			nullString(&buf, 81);
			in = 0x00;
			index = 0;
			printf("Outputting because the screen has only 80 columns. Sorry!\n");
		}
		else
		{
			putchar(in);
			if (in == '\n'){
				printf(buf);
				putchar('\n');
				nullString(&buf, 81);
				in = 0x00;
				index = 0;
			}
			buf[index++] = in;
		}
	}
}
