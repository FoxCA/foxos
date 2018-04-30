#include <kernel.h>

int attrib = 0x0F;
int csr_x = 0, csr_y = 0;
int doautoscoll = 0;

void vga_autoscroll(){
    doautoscoll = 0;
}

void vga_waitscroll(){
    doautoscoll = 1;
}

void vga_linescroll(){
    doautoscoll = 2;
}

void vga_slowscroll(){
    doautoscoll = 3;
}

void vga_set_csr_x(int x)
{
  csr_x = x;
}

void vga_set_csr_y(int y)
{
  csr_y = y;
}

void vga_set_csr_xy(int x, int y)
{
  csr_x = x;
  csr_y = y;
}

void vga_scroll(void)
{
    unsigned blank, temp;
    
    blank = 0x20 | (attrib << 8);

    if(csr_y >= vga_height)
    {
        if(keyboard_initialized() && doautoscoll == 1){
            while(keyboard_dequeue() == 0);
            vga_cls();
            return;
        }else if(keyboard_initialized() && doautoscoll == 2){
            while(keyboard_dequeue() == 0);
        }else if(doautoscoll == 3){
            int i;
            for (i = 0; i < 400000000; ++i);
        }
        temp = csr_y - vga_height + 1;
        memcpy (vga_SCREEN, vga_SCREEN + temp * vga_width, (vga_height - temp) * vga_width * 2);

        memsetw (vga_SCREEN + (vga_height - temp) * vga_width, blank, vga_width);
        csr_y = vga_height - 1;
    }
}

void vga_move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * vga_width) + x] */
    temp = csr_y * vga_width + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'.*/
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void vga_cls()
{   
    unsigned blank;
    int i;

    blank = 0x20 | (attrib << 8);

    memsetw(vga_SCREEN, blank, vga_width * vga_height);

    csr_x = 0;
    csr_y = 0;
    vga_move_csr();    
}


void vga_putc(char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    if(c == 0x08){
        if(csr_x != 0){
            csr_x--;
            where = vga_SCREEN + (csr_y * vga_width + csr_x);
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
        where = vga_SCREEN + (csr_y * vga_width + csr_x);
        *where = c | att;       
        csr_x++;        
    }  

    if(csr_x >= vga_width){
        csr_x = 0;
        csr_y++;
    }

    vga_scroll();
    vga_move_csr();
}

void vga_settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void vga_settextcolorf(unsigned int color)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = color;
}

unsigned int vga_gettextcolorf()
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    return attrib;
}



void vga_init_video(void)
{  
    outportb(0x03c8, 0x03);
    outportb(0x03c9, 0x72 >> 2);
    outportb(0x03c9, 0x9f >> 2);
    outportb(0x03c9, 0xcf >> 2);

    outportb(0x03c8, 0x01);
    outportb(0x03c9, 0x34 >> 2);
    outportb(0x03c9, 0x65 >> 2);
    outportb(0x03c9, 0xA4 >> 2);

    vga_settextcolor(white, black);

    vga_cls();

}


void vga_move_csr_offset(int x, int y){
  csr_x += x;
  csr_y += y;
  if(csr_x < 0)
  {
    if(csr_y > 0)
    {
      csr_y--;
      csr_x = 80 - 1;
    }
    else
    {
      csr_x = 0;
    }
  }
  if(csr_x >= 80)
  {
    if(csr_y < (25 - 1))
    {
      csr_x = 0;
      csr_y++;
    }
    else
    {
      csr_x = 80 - 1;
    }
  }
  if(csr_y < 0)
  {
    csr_y = 0;
  }
  if(csr_y >= 25)
  {
    csr_y = 25 - 1;
  }

  vga_move_csr();
}