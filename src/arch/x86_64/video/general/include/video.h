




#ifndef VIDEO_H
#define VIDEO_H

#if GRAPHICS == 0
#define set_csr_x(x) vga_set_csr_x(x)
#define set_csr_y(y) vga_set_csr_y(y)
#define set_csr_xy(x,y) vga_set_csr_xy(x, y)
#define init_video() vga_init_video()

#define shell_putc(c) vga_putc(c)

#define cls() vga_cls()
#define move_csr() vga_move_csr()
#define move_csr_offset(x,y) vga_move_csr_offset(x, y)
#define settextcolor(forecolor,backcolor) vga_settextcolor(forecolor,backcolor)
#define settextcolorf(i) vga_settextcolorf(i)
#define gettextcolorf() vga_gettextcolorf()
#define waitscrol() vga_waitscroll()
#define autoscroll() vga_autoscroll()
#define linescroll() vga_linescroll()
#define slowscroll() vga_slowscroll()
#endif

#if GRAPHICS == 1

#endif

#endif