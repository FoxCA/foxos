#include <kernel.h>

void enter_usermode() {
    switch_to_user();
}