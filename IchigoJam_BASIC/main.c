// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

//#define VERSION15

#ifdef VERSION15
// 1.5 for R
#define IJB_BUILD 11
#define IJB_VER 150
#else
// 1.4.3
#define IJB_BUILD 28 // for write lock
#define IJB_VER 143
#endif

// platform depends
#include "config.h"

#include "ichigojam-stddef.h"

// platform independs
#include "lang.h"
#include "ram.h"
#include "vars.h"
#include "screen.h"
#include "error.h"
#include "basic.h"
#include "psg.h"

// platform depends
#include "system.h"
#include "display.h"
#include "keyboard.h"
#include "ichigojam-io.h"
//#include "storage.h"
#include "flashstorage.h" // rename storage -> flashstorage
#include "random.h"
#include "sound.h"
#include "ext_iot.h"
#include "ext_ws.h"

#if defined(USR_ARMCORTEXM0)
#include "usr/armcortexm0/usr.h"
#elif defined(USR_MOS6502)
#include "usr/mos6502/usr.h"
#elif defined(USR_Z80)
#include "usr/z80/usr.h"
#else
#include "usr.h"
#endif

/*
__attribute__ ((section(".api")))
const uint16_t API[] = {
//void (*const API[])(void) = {
//    (void (*const)(void))__STACK_TOP,		    				// The initial stack pointer
//    (void*)(mstk + STACK_SIZE),	 			// The initial stack pointer
//	(uint16_t)(uint32_t)put_chr, // rnd,
	3,//(uint16_t)(void*)rnd,
	5,
};
*/

S_INLINE int IJB_peek(int ad) {
	if (ad < 0) {
		return 0;
	} else if (ad < OFFSET_RAMROM) {
		return CHAR_PATTERN[ad];
	} else if (ad < OFFSET_RAMROM + SIZE_RAM) {
		return ram[ad - OFFSET_RAMROM];
	}
	return 0;
}
S_INLINE void IJB_poke(int ad, int n) {
	ad -= OFFSET_RAMROM;
	if (ad >= 0 && ad < SIZE_RAM) {
		ram[ad] = n;
//		_printf("poke %x, %d\n", ad, n);
	}
}

/*S_INLINE*/ int IJB_random(int n) {
	uint r = rnd();
	if (n <= 0) {
		return 0;
//		return r;
	}
	//	return (int)rnd() % n;
	return (r >> 1) % n; // マイナスがでてしまう対処、udivが使われる div?? こっちの方が小さい
	//	return calcMod(rnd() >> 1, n); // % これの方が大きい
}
S_INLINE void IJB_random_seed(int n) {
	random_seed(n);
}

STATIC void IJB_input(char** line) {
	screen_showCursor(1);
	key_flg.insert = key_flg.bkinsert;
	int8 ox = _g.cursorx;
	int8 oy = _g.cursory;
	for (;;) {
		if (stopExecute()) {
//			screen_putc('\n');
			break;
		}
		int key = key_getKey();
//		IJB_random(1);
		if (key == '\n') {
			*line = (char*)(vram + ox + SCREEN_W * oy);
//			screen_putc('\n');
			break;
		} else if (key >= 0 && key != 27 && key != 30 && key != 31 && !((key == '\b' || key == 28) && _g.cursorx == ox)) {
			_g.screen_insertmode = key_flg.insert;
			
			if (_g.uartmode_txd & 4) { // 1.2b62 
				put_chr(key);
			} else {
				screen_putc(key); // 1.2b61まで
			}
		}
	}
	key_flg.bkinsert = key_flg.insert;
	key_flg.insert = 1;
	_g.screen_insertmode = 1; // update
	screen_showCursor(0);
}

#ifdef DEBUG_PRINT
#include <xprintf.h>
#endif

INLINE void IJB_bps(int uart, int i2c) {
	uart_bps(uart);
#ifdef ENABLE_I2C_BPS
	i2c0_bps(i2c);
#endif
}

S_INLINE void init() {
	flash_init();
#ifndef KEY_INIT_DELAY // for IchigoJam R
	key_init();
#endif
	IJB_bps(0, 0);
	
#ifdef DEBUG_PRINT
	xdev_out(put_chr); // xprintfの出力先を変更
#endif
	
	io_init();
	
	//	psg_init();
	sound_init();
	
	random_init();
	
	basic_init();
	
//	_g.uartmode = 0; // sleepflg ? 0 : 2;
	
	video_init();
	uart_init();
}

// 22648
STATIC void exec(char* s) {
	key_flg.bkinsert = key_flg.insert;
	key_flg.insert = 1;

	_g.screen_insertmode = 1;
	_g.key_flg_esc = 0;
	screen_showCursor(0);
	int res = basic_execute(s);
	//screen_showCursor(1); // 1.4b10
	if (_g.cursory == -1)
		_g.cursory = 0;
	if (res == BASIC_RESULT_EXECUTE && !noresmode) {// stop, exec, edit, err
		put_str("OK\n");
	}
	#ifndef IJB_USE_EXCEPTION // 例外使うときには使わないコード
//	if (res != 2) { // 1.2b36 追加、edit時(==2)以外に限定
	//if (res == BASIC_RESULT_ERR) { // 1.3b4 エラー停止の時だけ、キークリア
	if (res == BASIC_RESULT_STOP_OR_ERR) { // 1.3b4 エラー停止の時だけ、キークリア
		key_clearKey();
	}
	#endif

	key_flg.insert = key_flg.bkinsert;
}

#ifndef STATIC_ICHIGOJAM_MAIN
#define STATIC_ICHIGOJAM_MAIN
#endif

#ifndef IJB_TITLE
#define IJB_TITLE "IchigoJam BASIC 1.5? by jig.jp\n"
#endif

#ifdef BOOT_CHECK
int bootflg = 0;
#endif


void main_loop();

STATIC_ICHIGOJAM_MAIN void ichigojam_main(void) { // main
	system_init();
	
#ifdef BOOT_CHECK
	{
		char chk = 0;
		for (int i = 0; i < 6; i++) {
			chk ^= IJB_TITLE[i];
		}
		bootflg = chk != BOOT_CHECK;
	}
#endif
	
	init();

	{
#ifdef AUTO_LRUN0
		int sleepflg = 1;
#else
		video_waitSync(1); // add ver 1.2.3

//		#define BOOT_WAIT 60	// 60より短いとだめなキーボードがある (AOの?)
//		#define BOOT_WAIT 30	// 30だとだめなキーボードがあった 他のキーボードでチェック！ 1.4b10
		//#define BOOT_WAIT 59	// 60より短いとだめなキーボードがある (AOの?) 1.2.3 -> 1.4b11
		#define BOOT_WAIT 70	// 60より短いとだめなキーボードがある (AOの?) 1.2.3 -> 1.4b11 -> 1.4b28
		//#define BOOT_WAIT0 5	// 60より短いとだめなキーボードがある (AOの?) 1.2.3 -> 1.4b11

		//video_waitSync(BOOT_WAIT0);// test
		//video_waitSync(0); // これがないと、waitSync 0 呼び出しで最大ループしてしまう・・・ // 不要? 1.3.2b19
		int sleepflg = getSleepFlag();
#ifdef KEY_INIT_DELAY
		key_init();
#endif

		// 先頭行が 'ARUN または @ARUN で始まる時は自動起動 // 1.3.2b19 +24byte
		// sleepflg |= ((*(uint*)(0x1000 * _g.fileslot + 4) << 8) >> 8) == 0x555241; // 手抜き、ARUNだけ見る -8byte
#if VER_PLATFORM == PLATFORM_LPC1114
		int ad = 0x1000 * _g.fileslot;
		sleepflg |= *(uint8*)(ad + 3) == 0x40 && ((*(uint*)(ad + 4) << 8) >> 8) == 0x555241; // @ARUNかどうか
#endif
#if VER_PLATFORM == PLATFORM_GD32VF103
		int ad = (int)flash_getAddress(1);
		sleepflg |= *(uint8*)(ad + 3) == 0x40 && ((*(uint*)(ad + 4) << 8) >> 8) == 0x555241; // @ARUNかどうか
		if (*(uint*)(ad) == 0xffffffff) {
			sleepflg = 0;
		}
#endif
#endif
		
//		if (!sleepflg2) { // 高速復帰モード
		if (!sleepflg) {
			psg_beep(10, 3);
		}
		video_waitSync(BOOT_WAIT);
//	IJB_led(1);	for (;;);
#ifdef IJB_TITLE
		for (int i = 0;; i++) { // 自動起動時は表示だけしない
			char c = IJB_TITLE[i];
			if (!sleepflg)
				put_chr(c);
			video_waitSync(1);
			if (c == '\n')
				break;
		}
#endif
		if (!sleepflg) {
			put_str("OK\n");
		}
		
		// resetはここにないと、キーボードが効かない
		key_enable(1); // 1.2.1b27
		key_send_reset(); // reset // 1.2.1b27 // SANWAサプライなどに必要 308byte増える

		_g.sleepflg = sleepflg;
		/*
		if (sleepflg) {
			//_g.screen_insertmode = 1;
//			screen_showCursor(0);
			char* linebuf = (char*)ram + OFFSET_RAM_LINEBUF;
			*linebuf = 1;
			exec("LRUN");
		}
		*/
	}
	
	/*
	// simple test for PC
	for (;;) {
		char buf[1024];
		for (int i = 0; i < 1023; i++) {
			int key = key_getKey();
			if (key == '\n') {
				buf[i] = 0;
				break;
			}
			buf[i] = key;
		}
		exec(buf);
	}
	*/
	#ifdef IJB_USE_EXCEPTION
	*(char*)0 = 0; // HardFault -> main_loop
	#else
	main_loop();
	#endif
}

//void put_hex(uint32_t n);

void main_loop() {
	#if VER_PLATFORM == PLATFORM_LPC1114
	__enable_irq(); // 1.4b10
	#endif

	#ifdef IJB_USE_EXCEPTION
//	uint32* sp = (uint32*)__get_MSP();
//	put_hex(sp); // 100003C0 - 3E8
	/*
	__asm volatile (
		"mov %0, sp\n"
		"add %0, %0, #24\n"
		"mov sp, %0\n"
		: "=r" (sp)
	);
	*/
	__asm volatile("add sp, sp, #24\n"); // 不要なスタックを解放
//	__asm volatile("mov %0, sp" : "=r" (sp)); // __get_SP()
//	uint32* sp = (uint32*)__get_MSP(); // 正常!
//	put_hex(sp); // 100003C0 - 3E8

//	put_hex(sp); // 100003C0
//	sp = (uint32*)__get_MSP();
//	put_hex(sp); // 100003C0
//put_num(SIZE_RAM); // 2688
	#endif

	char* linebuf = (char*)ram + OFFSET_RAM_LINEBUF;
	if (*linebuf) {
//		put_str(ERR_MESSAGES[ERR_STACK_OVERFLOW - 1]);
//		put_chr('\n');
		if (_g.cursory == -1) { // 1.4.1 前にもってくる
			_g.cursory = 0;
		}
		if (!noresmode) {
			if (!_g.err) {
				_g.err = ERR_COMPLEX_EXPRESSION;
			}
			basic_printError();
//			put_str(ERR_MESSAGES[_g.err - 1]);
//			put_chr('\n');
		}
		// from exec
//		screen_showCursor(1);
	//	if (res != 2) { // 1.2b36 追加、edit時(==2)以外に限定
		//if (res == BASIC_RESULT_ERR) { // 1.3b4 エラー停止の時だけ、キークリア
		key_clearKey(); // 1.3b4 エラー停止の時だけ、キークリア

		key_flg.insert = key_flg.bkinsert;
	}
	for (;;) {
    	// put_num(hid_desc.header.bLength); // bCountryCode);
		//put_num(usb_host.dev_prop.dev_desc.idVendor);
		if (_g.sleepflg) {
			_g.sleepflg = 0;
			*linebuf = 1;

			#define BOOT_WAIT2 25	// 1.4b10 -> b11 ここに移動
			video_waitSync(BOOT_WAIT2);

			exec("LRUN");
		}
		screen_showCursor(1); // 1.4b10
		IJB_random(1);
		video_waitSync(1); // 消すとUART受信漏れ発生?
		
		int key = key_getKey();
		if (key < 0)
			continue;
		if (_g.uartmode_txd & 4) { // 1.2b62 UART echo back
			uart_putc(key); // 1.3b2
//			put_chr(key);
		}
		if (key == 27)
			continue;
		
		_g.screen_insertmode = key_flg.insert;

		screen_putc(key);
		
		if (key == '\n') {
			uint8* s = screen_gets();
	//		put_str(s);
			if (*s == '\'') { // 1.1b14
			} else if (*s != 0) {
				uint8 i;
				for (i = 0; i < N_LINEBUF; i++) {
					linebuf[i] = s[i];
					if (!s[i])
						break;
				}
//				_g.screen_insertmode = 1;
				if (s[i]) {
//					put_str("Too long line\n");
					put_str("Too long\n"); // 1.2b45
				} else {
					linebuf[i] = 0; // いっぱいまで入れるとバグっていた 1.2b32
					exec(linebuf);
				}
			}
		}
	}
}

/*
void put_hex(uint32_t n) {
	for (int i = 7; i >= 0; i--) {
		uint m = (n >> (i * 4)) & 0xf;
		if (m < 10)
			put_chr('0' + m);
		else
			put_chr('A' + m - 10);
	}
	put_chr('\n');
}
*/

void HardFault_Handler() {
	#ifdef IJB_USE_EXCEPTION
	uint32* sp = (uint32*)(0x10000000 + 1152 - 32);
	/*
	__asm(
		"mov %0, #1\n"
		"lsl %0, %0, #28\n"
		"add %0, %0, #32\n"
		"mov sp, %0\n"
		: "=r" (sp)
	);
	*/
	__asm(
		"mov sp, %0\n"
		:
		: "r" (sp)
	);
	*(sp + 6) = (uint32)(void*)main_loop; // set PC
	*(sp + 7) = (1 << 24); // need to set PSR, Thumb mode
	#endif
}

