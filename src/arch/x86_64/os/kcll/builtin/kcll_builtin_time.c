// cub-kcll | 26.04.2018
// By daelvn
// Get time since boot

void kcll_builtin_time() {
  printf("%i\n",timer_get_time_since_boot());
}
