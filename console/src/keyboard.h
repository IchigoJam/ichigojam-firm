// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdio.h>

S_INLINE uint key_getKeyboardID() {
  return 0;
}

struct keyflg_def key_flg;
uint8 displaymode;

S_INLINE void key_init() {
}

S_INLINE void key_send_reset() {
}

S_INLINE void key_enable(uint8 b) {
//  key_enable_flg = b;
}
//void uart_checker();

INLINE int key_btn(int n) {
  return 0;
}

//以下の記法はz88dk-sccz80においてerror: Expecting constant expressionとなる
//char* keybuf = (char*)(RAM_KEYBUF + 1); // kbhit[-1], len:[0], buf:[1-(KEY_BUF_LEN-1] // 24512+60 // 小さい！
char* keybuf = (char*)RAM_KEYBUF + 1; // kbhit[-1], len:[0], buf:[1-(KEY_BUF_LEN-1] // 24512+60 // 小さい！

int key_getKey() {
  return getchar();
}
void key_clearKey() {
  *keybuf = 0;
}


S_INLINE void uart_init() {
  _g.uartmode_txd = 1;
}

INLINE void IJB_uart(int16 txd, int16 rxd) {
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
INLINE int stopExecute() {
  return _g.key_flg_esc;
}

#endif
