// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

//#include <unistd.h>

void system_init();
void enterDeepSleep(int waitsec);
INLINE void deepPowerDown();
INLINE void IJB_sleep();
S_INLINE void IJB_reset();

int getSleepFlag() {
  return 0;
}
void system_init() {
}
void enterDeepSleep(int waitsec) {
}
INLINE void deepPowerDown() {
}
INLINE void IJB_sleep() {
}
S_INLINE void IJB_reset() {
}
int IJB_wait(int n, int active) { // if stop ret 1
  //usleep(n * 1000000 / 60);
    volatile uint32_t i = 0;
    for (i = 0; i < 48000 * n; i++) {
        __asm("NOP"); /* delay */
    }
    return 0;
}

S_INLINE void IJB_kbd(uint mode) { // 1.5b1
}

/*
void ichigojam_main(void);
int main() {
  ichigojam_main();
  return 0;
}
*/
