
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void OS(){
	char * buffer = ""; 
	while(true){
		char inp = getchar();
		putchar(inp);
		if (inp == '\n'){

		}
		buffer += inp;
	}
}

