
#ifndef KBBUF_H
#define KBBUF_H

#include <stdbool.h>

#define KEYBUFSIZ 64


typedef struct
{
  unsigned char buf[KEYBUFSIZ];
  unsigned char *head;
  unsigned char *tail;
} keyboard_buffer_t;

void (*kb_handler)(unsigned char scancode);
static unsigned char scan_to_ascii_US_shift[128];
static unsigned char scan_to_ascii_US[128];


void kb_buf_standard(unsigned char scancode);
void kb_buf_init();
void keyboard_enqueue(unsigned char ascii);
unsigned char keyboard_dequeue(void);

void set_kb_handler(void (*handler)(unsigned char scancode));
void reset_kb_handler(void);
void kb_buf_scan(unsigned char scancode);

#endif
