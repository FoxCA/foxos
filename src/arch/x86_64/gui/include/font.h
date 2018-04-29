
#ifndef FONT_H
#define FONT_H

#define FONT 2


#include <stdio.h>


#if FONT == 1

#define FONTWIDTH 8
#define FONTHEIGHT 8
#define get_fontmap(x) get_font_map_1(x)
#define get_font_array() get_font_array_1()

#endif

#if FONT == 2

#define FONTWIDTH 8
#define FONTHEIGHT 8
#define get_fontmap(x) get_font_map_default(x)
#define get_font_array() get_font_array_2()

#endif


#endif