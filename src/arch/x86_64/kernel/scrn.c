
#include <stdio.h>
#include <system.h>
#include <vga.h>
#include <draw.h>

int attrib = 0x0F;
int csr_x = 0, csr_y = 0;
int doautoscoll = 0;

void autoscroll(){
    doautoscoll = 0;
}

void waitscroll(){
    doautoscoll = 1;
}

void linescroll(){
    doautoscoll = 2;
}

void slowscroll(){
    doautoscoll = 3;
}

void set_csr_x(int x)
{
  csr_x = x;
}

void set_csr_y(int y)
{
  csr_y = y;
}

void set_csr_xy(int x, int y)
{
  csr_x = x;
  csr_y = y;
}

void scroll(void)
{
    unsigned blank, temp;

    blank = 0x20 | (attrib << 8);

    if(csr_y >= height)
    {
        if(keyboard_initialized() && doautoscoll == 1){
            while(keyboard_dequeue() == 0);
            cls();
            return;
        }else if(keyboard_initialized() && doautoscoll == 2){
            while(keyboard_dequeue() == 0);
        }else if(doautoscoll == 3){
            for (int i = 0; i < 400000000; ++i);
        }
        temp = csr_y - height + 1;
        memcpy (SCREEN, SCREEN + temp * width, (height - temp) * width * 2);

        memsetw (SCREEN + (height - temp) * width, blank, width);
        csr_y = height - 1;
    }
}

void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * width + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'.*/
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void cls()
{
    if(vesa_initialized){
        return vesa_cls();
    }
    
    unsigned blank;
    int i;

    blank = 0x20 | (attrib << 8);

    memsetw(SCREEN, blank, width * height);

    csr_x = 0;
    csr_y = 0;
    move_csr();    
}

int is_transmit_empty() {
   return inportb(0x3f8 + 5) & 0x20;
}

void putch_qemu(char c){
    while (is_transmit_empty() == 0);
    outportb(0x3f8,c);
}

int puts_qemu(char * text){

    int i;

    for (i = 0; i < strlen(text); i++){
        putch_qemu(text[i]);
    }

    return i;

}


void putch(char c)
{
    if(vesa_initialized){
        return vesa_putch(c);
    }

    unsigned short *where;
    unsigned att = attrib << 8;

    if(c == 0x08){
        if(csr_x != 0){
            csr_x--;
            where = SCREEN + (csr_y * width + csr_x);
            *where = ' ' | att;
        }else{
            csr_y--;
        }
    }

    else if(c == 0x09){       
        csr_x = (csr_x + 8) & ~(8 - 1);
    }

    else if(c == '\r'){
        csr_x = 0;
    }

    else if(c == '\n'){      
        csr_x = 0;
        csr_y++;
    }

    else if(c >= ' '){
        where = SCREEN + (csr_y * width + csr_x);
        *where = c | att;       
        csr_x++;        
    }  

    if(csr_x >= width){
        csr_x = 0;
        csr_y++;
    }

    scroll();
    move_csr();
}

int puts(char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++){
        putch(text[i]);
    }

    return i;
}

void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void settextcolorf(unsigned int color)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = color;
}

unsigned int gettextcolorf()
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    return attrib;
}



void init_video(void)
{  
    outportb(0x03c8, 0x03);
    outportb(0x03c9, 0x72 >> 2);
    outportb(0x03c9, 0x9f >> 2);
    outportb(0x03c9, 0xcf >> 2);

    outportb(0x03c8, 0x01);
    outportb(0x03c9, 0x34 >> 2);
    outportb(0x03c9, 0x65 >> 2);
    outportb(0x03c9, 0xA4 >> 2);

    settextcolor(white, black);

    cls();

}
