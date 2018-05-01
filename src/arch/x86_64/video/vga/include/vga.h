#ifndef VGA_H
#define VGA_H

#define vga_width			80
#define vga_height			25

#define vga_SCREEN ((uint16_t*)(LOAD_MEMORY_ADDRESS + 0xB8000))

void vga_set_csr_x(int x);
void vga_set_csr_y(int y);
void vga_set_csr_xy(int x, int y);
void vga_init_video(void);

void vga_putc(char c);

void vga_cls();
void vga_move_csr(void);
void vga_move_csr_offset(int x, int y);
void vga_settextcolor(unsigned char forecolor, unsigned char backcolor);
void vga_settextcolorf(unsigned int);
unsigned int vga_gettextcolorf();
void vga_waitscroll();
void vga_autoscroll();
void vga_linescroll();
void vga_slowscroll();

#endif