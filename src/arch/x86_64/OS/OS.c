
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void OS(){
	char * buf = ""; 
	while(true){
		char inp = getchar();
		putchar(inp);
		if (inp == '\n'){
			puts(buf);
			buf = "";
		}
		buf += inp;
	}
}

