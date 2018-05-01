#include <kernel.h>

uint32_t jiffies = 0;
uint16_t hz = 0;
list_t * wakeup_list;

float timer_get_time_since_boot(){
    return (float)jiffies/(float)hz;
}

void timer_init() {
    set_frequency(100);
    irq_install_handler(0, timer_handler);
    wakeup_list = list_create();    
}


void sleep(int sec) {
    uint32_t end = jiffies + sec * hz;
    while(jiffies < end);
}

void set_frequency(uint16_t h) {
    hz = h;
    uint16_t divisor = INPUT_CLOCK_FREQUENCY / h;
    // Init, Square Wave Mode, non-BCD, first transfer LSB then MSB
    outportb(TIMER_COMMAND, TIMER_ICW);
    outportb(TIMER_DATA, divisor & 0xFF);
    outportb(TIMER_DATA, (divisor >> 8) & 0xFF);

}

/*
 * Call some function every x seconds
 * */
void register_wakeup_call(wakeup_callback func, double sec) {
    uint32_t jiffy = jiffies + sec * hz;
    // Save the function sec, and jiffy to a list, when timer hits that func's jiffy it will call the func, and update next jiffies
    wakeup_info_t * w = kmalloc(sizeof(wakeup_info_t));
    w->func = func;
    w->sec = sec;
    w->jiffies = jiffy;
    list_insert_front(wakeup_list, w);
}


void timer_handler(regs * reg) {

    jiffies++;
    memcpy(&saved_context, reg, sizeof(regs));
    foreach(t, wakeup_list) {
         wakeup_info_t * w = t->val;
         w->func();
    }
}