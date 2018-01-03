
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void OS(){
	// printf(0);
	char buf[20] = "";
	size_t len = strlen(buf);	 
	while(true){
		char inp = getchar();
		putchar(inp);
		if (inp == '\n'){
			printf(buf);
			printf("\n");
			char buf[20] = "";
		}
		buf[len++] = inp;
		buf[len] = '\0';
	}
}

