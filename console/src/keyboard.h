#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdio.h>

static inline uint key_getKeyboardID() {
  return 0;
}

struct keyflg_def key_flg;
uint8 displaymode;

static inline void key_init() {
}

static inline void key_send_reset() {
}

INLINE void key_enable(uint8 b) {
//  key_enable_flg = b;
}
//void uart_checker();

inline int key_btn(int n) {
  return 0;
}

char* keybuf = (char*)(ram + (OFFSET_RAM_KEYBUF + 1)); // kbhit[-1], len:[0], buf:[1-(KEY_BUF_LEN-1] // 24512+60 // 小さい！

int key_getKey() {
  return getchar();
}
void key_clearKey() {
  *keybuf = 0;
}


INLINE void uart_init() {
  _g.uartmode_txd = 1;
}

inline void IJB_uart(int16 txd, int16 rxd) {
}

void uart_putc(char c) { // 1.1b7 uart3でバッファモード, b12 バッファモードやめる？
  putchar(c);
}

void uart_bps(int n) { // 0:115200, -1:57600, -2:38400
}

void put_chr(char c) {
  if (_g.uartmode_txd > 0) { // 1.0.2b12 uartを先に
    uart_putc(c);
  }
  screen_putc(c); // segmentation fault
}

// basic interface
inline int stopExecute() {
  return _g.key_flg_esc;
}

#endif
