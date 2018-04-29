#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <draw.h>
#include <vesa.h>
#include <font.h>

void * framebuffer;
int screenwidth;
int screenheight;
color fg, bg;
int vesa_initialized = false;

int csr_x;
int csr_y;

int fontscale;

void gui_init(){
	framebuffer = vesa_get_lfb();
	screenwidth = vesa_get_resolution_x();
	screenheight = vesa_get_resolution_y();
	vesa_set_color((color){255,255,255},(color){0,0,0});
	vesa_set_fontsize(2);
	vesa_cls();
	vesa_initialized = true;
}

void vesa_set_csr_x(int x)
{
  csr_x = x;
}

void vesa_set_csr_y(int y)
{
  csr_y = y;
}

void vesa_set_csr_xy(int x, int y)
{
  csr_x = x;
  csr_y = y;
}

void vesa_cls(){
	draw_rect(0,0,screenwidth,screenheight,bg);
}

void vesa_set_fontsize(int size){
	fontscale = size;
}

void vesa_scroll(){

}

void vesa_move_csr(){

}

void draw_font_character(int fontx, int fonty, char c){
	remove_font_character(fontx, fonty);
	uint8_t * fnt = get_font_array();
	
	for(int y = 0; y < FONTHEIGHT; y++){
		for(int x = 0; x < FONTWIDTH; x++){
			if(fnt[get_fontmap(c) * FONTHEIGHT + y] >> (8 - x) & 1){
				
				for (int i = 0; i < fontscale; ++i){
					for (int j = 0; j < fontscale; ++j){
						draw_pixel(fontx + (x*fontscale) + i, fonty + (y*fontscale) + j,fg);
					}
				}
			}
		}
	}
	printf_qemu("\n\n");
}

void remove_font_character(int fontx, int fonty){
	printf_qemu("%i,%i,%i,%i\n",fontx, fonty,FONTWIDTH*fontscale,FONTHEIGHT*fontscale);
	draw_rect(fontx, fonty,FONTWIDTH*fontscale,FONTHEIGHT*fontscale,bg);
}


void vesa_putch(char c){
    unsigned short *where;

    if(c == 0x08){
        if(csr_x != 0){
            csr_x--;
        }else{
            csr_y--;
        }    	
		remove_font_character(csr_x*FONTWIDTH*fontscale,csr_y*FONTHEIGHT*fontscale);
    }

    else if(c == 0x09){       
    	csr_x = (csr_x + 8) & ~(8 - 1);
    }

    else if(c == '\r'){
    	for (int i = csr_x; i >= 0; i--){
    		remove_font_character(i*FONTWIDTH*fontscale,csr_y*FONTHEIGHT*fontscale);
    	}
    }

    else if(c == '\n'){
    	csr_x=0; 
		csr_y++;
    }

    else if(c >= ' '){
		draw_font_character(csr_x*FONTWIDTH*fontscale,csr_y*FONTHEIGHT*fontscale, c);
		csr_x++;
    }  

    if(csr_x*FONTWIDTH*fontscale+(FONTWIDTH*fontscale) > screenwidth){
        csr_x = 0;
        csr_y++;
    }

    vesa_scroll();
    vesa_move_csr();
}



void vesa_set_color(color fgc, color bgc){
	bg = bgc;
	fg = fgc;
}


uint32_t get_pixel_index(int x, int y){
 	return ((y*screenwidth)+x)*3;
}

void draw_pixel(int x, int y, color c){
	vesa_memset_rgb(framebuffer+get_pixel_index(x,y),c,1);
}

void draw_line(int x0, int y0, int x1, int y1, color c){
    int dx, dy, p, x, y;
 
    dx=x1-x0;
    dy=y1-y0;
 
    x=x0;
    y=y0;
 
    p=2*dy-dx;
 
    while(x<x1)
    {
        if(p>=0)
        {
            draw_pixel(x,y,c);
            y=y+1;
            p=p+2*dy-2*dx;
        }
        else
        {
            draw_pixel(x,y,c);
            p=p+2*dy;
        }
        x=x+1;
    }
}

void draw_rect(int x, int y, int w, int h, color c){
  	for(int i = y; i < y + w; i++) {
		for(int j = x; j < x + h; j++) {
            draw_pixel(j, i, c);
        }
    }
}
