// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#include <unistd.h>

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
#if defined(__CPM__)
  /* unsupported */
#elif defined(__MSX__) /* NTSC only */
  #define JIFFY 0xfc9e
  unsigned short t;
  if (n > 0) {
    asm("di");
    t = (*(volatile unsigned short *)JIFFY) + n;
    asm("ei");
    while (*(volatile unsigned short *)JIFFY != t);
  }
#else
  usleep(n * 1000000 / 60);
#endif
  return 0;
}

void ichigojam_main(void);

int main() {
  ichigojam_main();
  return 0;
}
