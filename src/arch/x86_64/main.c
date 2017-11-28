
#define	VGA_COLOR_BLACK 0
#define	GA_COLOR_BLUE  1
#define	VGA_COLOR_GREEN  2
#define	VGA_COLOR_CYAN  3
#define	VGA_COLOR_RED  4
#define	VGA_COLOR_MAGENTA  5
#define	VGA_COLOR_BROWN  6
#define	VGA_COLOR_LIGHT_GREY  7
#define	VGA_COLOR_DARK_GREY  8
#define	VGA_COLOR_LIGHT_BLUE  9
#define	VGA_COLOR_LIGHT_GREEN  10
#define	VGA_COLOR_LIGHT_CYAN  11
#define	VGA_COLOR_LIGHT_RED  12
#define	VGA_COLOR_LIGHT_MAGENTA  13
#define	VGA_COLOR_LIGHT_BROWN  14
#define	VGA_COLOR_WHITE  15

#define TABDEPTH 8

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char * video;
int row,col;


void cursorset(int x, int y){
	row = y;
	col = x;
	video = (volatile char *) 0xb8000;
	for(int i = 0; i < 2*((y*VGA_WIDTH)+x); i++){
		*video++;
	}
}
void cursorsetrel(int x, int y){
	for(int i = 0; i < 2*((y*VGA_WIDTH)+x); i++){
		col++;
		if(col > VGA_WIDTH){
			col = 0;
			row ++;
		}	
		*video++;
	}
}
void cursorinc(){
	col++;
	if(col > VGA_WIDTH){
		col = 0;
		row ++;
	}	
	*video+=2;
}
void cursordec(){
	col--;
	if(col < 0){
		col = VGA_WIDTH;
		row--;
	}
	if(row < 0){
		row = 0;
	}		
	*video-=2;
}

void cls(int color){
	cursorset(0,0);
	for (int i = 0; i < VGA_WIDTH*VGA_HEIGHT; i++){
		*video = (char)0x20;
		*video++;
		*video = color;
		*video++; 
	}
	cursorset(0,0);
}
void clpos(int color,int x, int y){
	int a = row;
	int b = col;
	cursorset(x,y);
	*video = (char)0x20;
	*video++;
	*video = color;
	*video++; 
	cursorset(a,b);
}
void clline(int color,int x, int y){
	int a = row;
	cursorset(0,y);
	for (int i = 0; i < x; i++){
		*video = (char)0x20;;
		*video++;
		*video = color;
		*video++; 
	}
	cursorset(0,a);
}

void init_console(){
	video = (volatile char *) 0xb8000;
	cls(VGA_COLOR_BLACK);
	printf(VGA_COLOR_WHITE,"console init complete\n");
} 

void putchar(int color_FG, char str){
	if(str == '\n'){
		cursorset(0,row+1);
	}else if(str == '\t'){
		cursorsetrel(TABDEPTH,0);
	}else if(str == '\r'){
		clline(VGA_COLOR_WHITE,col,row);
	}else{
		*video = str;
		*video++;
		*video = color_FG;
		*video++; 
	}
	col++;
	if(col == VGA_WIDTH){
		col = 0;
		row++;
	}
}

void printf(int color_FG, const char * text){
	for (; *text != 0; text++){
		putchar(color_FG,*text);
	}
}

void main(){
	init_console();	
	printf(VGA_COLOR_WHITE,"hey");
	printf(VGA_COLOR_WHITE,"hey");
	while(1){}
}