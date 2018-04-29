

#ifndef DRAW_H
#define DRAW_H

#include <vesa.h>

extern void * framebuffer;
extern int screenwidth;
extern int screenheight;
extern int vesa_initialized;

typedef uint24_t color;

void gui_init();
void vesa_putch(char c);
void vesa_cls();
void vesa_set_color(color fg, color bg);
void vesa_set_fontsize(int size);

void draw_pixel(int x, int y, color c);
void draw_line(int x0, int y0, int x1, int y1, color c);
void draw_rect(int x, int y, int w, int h, color c);

#endif