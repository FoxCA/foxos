#ifndef TIMER_H
#define TIMER_H

#include <system.h>

#define INPUT_CLOCK_FREQUENCY 1193180
#define TIMER_COMMAND 0x43
#define TIMER_DATA 0x40
#define TIMER_ICW 0x36

extern uint32_t jiffies;
extern uint16_t hz;
typedef void (*wakeup_callback) ();

typedef struct wakeup_info {
    wakeup_callback func;
    double sec;
    uint32_t jiffies;
} wakeup_info_t;

void timer_handler(regs * reg);
void sleep(int seconds);
void timer_init();
float timer_get_time_since_boot();
void register_wakeup_call(wakeup_callback func, double sec);
void set_frequency(uint16_t h);

#endif