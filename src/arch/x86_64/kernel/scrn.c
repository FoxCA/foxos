
#include <system.h>
#include <vga.h>

unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

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
        temp = csr_y - height + 1;
        memcpy (textmemptr, textmemptr + temp * width, (height - temp) * width * 2);

        memsetw (textmemptr + (height - temp) * width, blank, width);
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
    unsigned blank;
    int i;

    blank = 0x20 | (attrib << 8);

    for(i = 0; i < height; i++){
        memsetw (textmemptr + i * width, blank, width);
    }

    csr_x = 0;
    csr_y = 0;
    move_csr();
}

void putch(char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    if(c == 0x08){
        if(csr_x != 0){
            csr_x--;
            where = textmemptr + (csr_y * width + csr_x);
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
        where = textmemptr + (csr_y * width + csr_x);
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
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
