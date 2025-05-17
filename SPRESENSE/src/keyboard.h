// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdio.h>

struct keyflg_def key_flg;
uint8 displaymode;

S_INLINE void key_init() {
  Serial.begin(115200);
  Serial2.begin(115200);
}

S_INLINE void key_send_reset() {
}

S_INLINE void key_enable(uint8 b) {
//	key_enable_flg = b;
}
//void uart_checker();

INLINE int key_btn(int n) {
	return 0;
}

char* keybuf = (char*)(RAM_KEYBUF + 1); // kbhit[-1], len:[0], buf:[1-(KEY_BUF_LEN-1] // 24512+60 // 小さい！

int keybuf1 = -1;

int key_readSerial() {
	if (Serial.available() > 0) {
		return Serial.read();
	}
	if (Serial2.available() > 0) {
		return Serial2.read();
	}
	return -1;
}

int key_getKey() {
	if (keybuf1 >= 0) {
		int res = keybuf1;
		keybuf1 = -1;
		return res;
	}
	int k = key_readSerial();
	if (k == 27 || k == 9) {
		_g.key_flg_esc = true;
	}
	return k;
}
void key_clearKey() {
	*keybuf = 0;
}

S_INLINE uint key_getKeyboardID() {
	return 0;
}

#define DEFAULT_UARTMODE_TXD 3 // 2
#define DEFAULT_UARTMODE_RXD 1

S_INLINE void uart_init() {
#if DEFAULT_UARTMODE_TXD != 0
	_g.uartmode_txd = DEFAULT_UARTMODE_TXD; // 最初のクリア信号を送らない
#endif
	_g.uartmode_rxd = DEFAULT_UARTMODE_RXD;
}

INLINE void IJB_uart(short txd, short rxd) {
}

void uart_putc(char c) { // 1.1b7 uart3でバッファモード, b12 バッファモードやめる？
	//putchar(c);
	Serial.write(c);
}

void uart_bps(int n) { // 0:115200, -1:57600, -2:38400
}

void put_chr(char c) {
	if ((_g.uartmode_txd & 3) == 3) { // 1.3b2
		uart_putc(c);
	}
	screen_putc(c); // segmentation fault
}

// basic interface
INLINE int stopExecute() {
	// todo: use keybuf
	int k = key_readSerial();
	if (k == 27 || k == 9) {
		_g.key_flg_esc = true;
	} else if (k >= 0) {
		keybuf1 = k;
	}
	return _g.key_flg_esc;
}

#endif
