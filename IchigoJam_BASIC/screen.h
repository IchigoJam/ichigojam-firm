// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "ichigojam-stddef.h"
#include "vars.h"

// SCREEN SIZE 可変に
#define SCREEN_W _g.screenw
#define SCREEN_H _g.screenh

#if LANG == LANG_ALL
extern uint8 CHAR_PATTERN[];
#else
extern const uint8 CHAR_PATTERN[];
#endif

//extern uint8 screen_pcg[]; // CLP POKE PEEK
//extern uint8* screen_pcg;

/* for video */
//#define SCREEN_W 36 // 36が限界
//#define SCREEN_H 27 // 27行


//#define SCREEN_W 16 // for LCD?
//#define SCREEN_H 6

//#define SCREEN_W 12 // 36が限界
//#define SCREEN_H 17 // 27行


static void video_init(void);
static void video_on();
static void video_off(int clkdiv);
static int16 video_tick(int n);
S_INLINE void video_clt();
static void screen_clear(void);
static void video_waitSync(uint);
static void screen_putc(char c);
S_INLINE void screen_puts(char* s);
static uint8* screen_gets();
static uint8 screen_get(int x, int y);
static uint8 screen_getCurrent();
static void screen_locate(int x, int y);
static void screen_scroll(int dir);
static void screen_clp();
static void screen_line(int x1, int y1, int x2, int y2, int cmd); // 1.3b8 -> 1.3.2b19 add cmd
static uint screen_pset(int x, int y, int cmd); // 1.3.2b19

#define screen_showCursor(n) (_g.cursorflg = (n))
//#define screen_showCursor(n) (key_flg.cursorflg = n)

extern volatile uint16 frames;
#define screen_tick() (frames)

// screen.c

// created by Taisuke Fukuno, @taisukef, http://fukuno.jig.jp/
// license CC BY-NC-SA 4.0 http://creativecommons.org/licenses/by-nc-sa/4.0/deed.ja

// keyboard
#ifdef KEYBOARD_USB
void key_ping();
S_INLINE void key_tick();
void key_tick2();
#endif

// psg
static void psg_tick();
//


#include "ram.h"
//uint8 vram[SCREEN_W * SCREEN_H];
// constをつけると容量増
//uint8* const vram = (uint8*)(ram + OFFSET_RAM_VRAM);
//uint8* vram = (uint8*)(ram + OFFSET_RAM_VRAM);
uint8* vram; // -> video_init() へ vram = (uint8*)(ram + OFFSET_RAM_VRAM);

const uint8 *pvram;
//uint8 offsety;

volatile uint16 frames;
volatile int16 lines;
//volatile uint16 linecnt;
//volatile uint8 videoenable; // flg

#if LANG == LANG_ALL
#include "ichigojam-jp.fnt.h"
#include "ichigojam-mn.fnt.h"
#include "ichigojam-vi.fnt.h"
#include "ichigojam-zh.fnt.h"
uint8 CHAR_PATTERN[256 * 8];
#endif

#if LANG == LANG_JP || LANG == LANG_FR
//IMPORT_BIN("../src/ichigojam-jp.fnt", CHAR_PATTERN); // 4byte節約できる (openglではng)
#include "ichigojam-jp.fnt.h"
#define CHAR_PATTERN CHAR_PATTERN_JP
#endif

#if LANG == LANG_MN
#include "ichigojam-mn.fnt.h"
#define CHAR_PATTERN CHAR_PATTERN_MN
#endif

#if LANG == LANG_VI
#include "ichigojam-vi.fnt.h"
#define CHAR_PATTERN CHAR_PATTERN_VI
#endif

#if LANG == LANG_ZH || LANG == LANG_BP
#include "ichigojam-zh.fnt.h"
#define CHAR_PATTERN CHAR_PATTERN_ZH
#endif

//uint8 screen_pcg[SIZE_PCG * 8] __attribute__ ((aligned(4))); // SIZE_PCG * 8 byte
//uint8* screen_pcg = (uint8*)(ram + OFFSET_RAM_PCG);
//#define screen_pcg ((uint8*)(ram + OFFSET_RAM_PCG))
uint8* const screen_pcg = ((uint8*)(ram + OFFSET_RAM_PCG)); // 同じ

// screen

S_INLINE int16 video_tick(int n) {
	//return (int16)(frames & 0x7fff); // 1.0.0b10 0x3fff -> 0x7fff
	return (int16)((n ? _g.linecnt : frames) & 0x7fff); // 1.0.0b10 0x3fff -> 0x7fff
}
S_INLINE void video_clt() {
	frames = 0;
	_g.linecnt = 0; // 1.4.0b03
}

/* bench mark
10 CLT:FOR I=1 TO 10000:A=A+1:NEXT:?TICK()
1650 1.2b61
916 1.2b61 video0
1841 1.2b21 ... SPI送るのを待つ場合
1379 1.2b21 ... SPI_OFF 割り込み
797 1.2b21 ... SPIO_OFF VIDEO0
1415 1.2b22
837 1.2b22 LCD
791 1.2b22 LCD版 noLCD
2155 1.1.1
1284 1.2b23 ... 261lines
1793 1.2b64
// 速度、b20:1738 b19:2105 1.1.1:2154 b19->b20:20%速度アップ 1.1->b20:24%アップ
// b21:1379 1.1->36%アップ
*/

struct keyflg_def { // 15bit: key 13bit + err 2bit
	char release:1;
	char shift_r:1;
	char shift_l:1;
	char alt_r:1;
	char alt_l:1;
	char ctrl_r:1;
	char ctrl_l:1;
	char e0:1;
	char caps:1; // default caps_on:0, off:1
	unsigned char insert:1; // 1.0.2b2 1 -> 0 insert:0: update:1
	char bkinsert:1; // 1.3b4
	char kana:1;
//	char esc:1; // 1.3b4 // 容量増加した
//	char lcd_init:1; // 1.3b4 // 容量増加した
//	char display_mode:1; // 1.3b4 容量増加した
	char enable:1; // 30byte減った！

	uint8 keyflg_errorignore:1; // = 1 // flg
	uint8 keyflg_noresmode:1; // 1:OK、エラー表示しない // flg

	//uint dummy:16;
};

#define errorignore key_flg.keyflg_errorignore
#define noresmode key_flg.keyflg_noresmode

extern struct keyflg_def key_flg;
//

#if TV_MODE == TV_MODE_NTSC

// http://elm-chan.org/docs/rs170a/spec_j.html
// fsc = 3579545 Hz +- 10Hz
// fH = fsc/227.5 = 15734.263736263736 Hz 水平同期周波数
//   48000000/15734.263736263736 -> 3050.6670540529594
//     -> 3056(15706.806282722513Hz) -> fsc' 3573298Hz(-6246Hz) だいぶずれてる 1.4.1までの値
//     -> 3054(15717.092337917486Hz) -> fsc' 3575638Hz(-3906Hz)
//     -> 3053(15722.240419259744Hz) -> fsc' 3576809Hz(-2735Hz)
//     -> 3052(15727.391874180865Hz) -> fsc' 3577981Hz (-1563Hz) - しょうがなくこちら
//     -> 3051(15732.546705998033Hz) -> fsc' 3579154Hz (-390Hz) - 一番近い +-10Hzに収まらず - でも揺れる
//     -> 3050(15737.704918032787Hz) -> fsc' 3580327Hz (782Hz)
// fH/262.5 = 59.94Hz
//   3056 15706.806282722513/262=59.94964230046761 - ぴったり59.94fps
//   3056 15706.806282722513/261=60.17933441656135 - いままでの1.4.1
//   3054 15717.092337917486/262=59.98890205312017 - 結構近い、ほぼ60fps
//   3054 15717.092337917486/261=60.218744589722164
//   3052 15737.704918032787/263=59.83918219784329 - VIDEO5でずれる
//   3052 15727.391874180865/262=60.028213260232306 - こっちの方がより良い！（大型テレビではNG）
//   3052 15727.391874180865/261=60.258206414486075 - これが一番映りがよい
//   3051 15732.546705998033/261=60.277956727961815
//   3051 15732.546705998033/262=60.04788819083219 (+0.10788819083219181) - これが一番近い (VSYNC 3051-1, 262 lines)
//   3051 15732.546705998033/263=59.81956922432712 (-0.12043077567287952)
//   3050 15737.704918032787/261=60.29771999246278
//   3050 15737.704918032787/262=60.0675760230259
// 今 3053 48000000/3053 = 15722.240419259744 Hz,  60.238469039309365 (0.2984690393093672) (VSYNC 2053-1, 261 lines)

// もし108Mhzなら
//  108000000/15734.263736263736 = 6864.000871619159 -> 6864 誤差少ない！
//    -> 15734.265734265735*227.5 -> fsc1 3579545.4545454546 (0.4545454545877874Hz) 仕様内、ほぼぴったり！
//    6864 15734.265734265735/263=59.82610545348188 (-0.11389454651811803)
//    6864 15734.265734265735/262=60.05444936742647 (0.11444936742647371) - 48MHz版と近いのでこちらで

// 正式仕様
//  15734.263736263736/262.5 = 59.94005232862376

#ifdef XTAL_358MHz
#define LINES 262 // NTSC 525/2 = 262.5 ... 262と263を交互に行って、525とする
#else
#define LINES 261 // 1.4.1 NTSC 525/2 = 262.5 ... 262だと垂直同期がずれるものがある -> 261lines // 1.4.1
#endif
//#define LINES 262 // 垂直同期ずれ大丈夫かな？？
//#define VSYNC 3067	// 48MHz 48000000/(261*59.94) = 3068.2023 = 3067, 3067/48*261*59.94/1000000 = 0.99960814125 (1 - 0.0003918) ver 1.2b24?

//#define VSYNC (3056 - 1)	// 48MHz 48000000/(262*59.94) = 3056.49 = 3056, 3056/48*262*59.94/1000000 = 0.9995119875000001 (1 - 0.000488) ver 1.1 .. 1.4.1

// 48000000/(262*59.94) = 3056.491606109927 -> 3057


// IchigoJam BASIC の VSYNC (3056-1) だったのを (3052-1) にしたら改善した (IchigoDyhook)、IchigoLatte:3048
//#define VSYNC (3068 - 1)	// 48MHz 48000000/(261*59.94) = 3068.20 -> 3068 いい感じ -> こっちがよくない？
//#define VSYNC (3056 + 6 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3062 うっすら震える
//#define VSYNC (3056 + 5 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3061 震える
//#define VSYNC (3056 + 4 - 1)	//* 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3060 ほぼ震えない、VIDEO5から震える、こっちいいかも？
//#define VSYNC (3056 + 3 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3059 震える
//#define VSYNC (3056 + 2 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3058 画面白黒はげしく点滅
//#define VSYNC (3056 + 1 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3057 時々、画面白黒はげしく点滅 (262 VIDEO3で揺れるけどまぁまぁ普通に使える）
//#define VSYNC (3056 + 0 - 1)	//+ 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3056 1.4.1と一緒、USBシリアル外すと、画面白黒はげしく点滅、うっすら震える (262はいい感じだったけど、IchigoDake+IchigoDyhookでNG、点滅する）
//#define VSYNC (3056 - 1 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3055 画面白黒はげしく点滅 (262でも激しく点滅）
//#define VSYNC (3056 - 2 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3054 VIDEO3で震える (262 VIDEO5で揺れる、IchigoDyhook+IchigoDake VIDEO3で時々点滅）
//#define VSYNC (3056 - 3 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053  震える (262 白黒点滅）
#ifdef XTAL_358MHz
// 水平同期周波数
// 白黒60fps fh=15.750kHz=15750Hz, 50113630/15750=3181.8
// カラー 50113630/15734.263736263736 = 3185
// 23*3579545 = 82329535, 82329535/15734.263736263736 = 5232
//#define VSYNC (3185 - 1)
//#define VSYNC (5232 - 1) // for 82.3MHz
#define VSYNC (5005 - 1) // for 78MHz
#else
#define VSYNC (3056 - 4 - 1)	//* 1.4b16 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3052 いい感じ、微妙に震えてる？ VIDEO5から震える、ver 1.4.2?、IchigoDakeだと震えない ただ、大型テレビでいまいちVIDEO3でガタガタに、VIDEO5で震える (262の場合、VIDEO5でも震えない、ただ、大型テレビでいまいち）
#endif
//#define VSYNC (3056 - 5 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3051 震える (262の場合、VIDEO3で震える）
//#define VSYNC (3056 - 6 - 1)	// 48MHz 48000000/(262*60) = 3053.435 = 3053 -> 3050 VIDEO1はok、VIDEO3震える (262の場合、なかなか）


#elif TV_MODE == TV_MODE_PAL

#define LINES 312 // PAL 625/2 = 312.5
//#define VSYNC 3087	// 48MHz 48000000/(311*50) = 3086.81 = 3087, 3087/48*311*50/1000000 = 1.000059375
//#define VSYNC 3086	// 48MHz 48000000/(311*50) = 3086.81 = 3087, 3087/48*311*50/1000000 = 1.000059375 // 波打つ
//#define VSYNC 3076	// 48MHz 48000000/(312*50) = 3076.923 = 3076, 3076/48*312*50/1000000 = 0.9997 // 同期乱れ
#define VSYNC 3075	// 48MHz 48000000/(312*50) = 3076.923 = 3076, 3075/48*312*50/1000000 = 0.999375

#endif

#define PSG_RATIO 4
#define VSYNC_0 ((VSYNC + 1) / PSG_RATIO - 1) // == (3056/8-1)

S_INLINE void video_initTV(void) {
	screen_clear();
	video_on();
}

void screen_clp() {
	memcpy4(screen_pcg, CHAR_PATTERN + (0x100 - SIZE_PCG) * 8, SIZE_PCG * 8);
}
INLINE void video_init(void) {
	
//	LPC_IOCON->PIO0_8 = 0xd0; // pullup // 1.2b10 なぜあった?
//	LPC_GPIO0->DIR &= ~(1 << 8); // 1.2b10 なぜあった?
//	_g.cursorflg = _g.screen_insertmode = 1;
//	_g.cursorflg = 1; // 1.4b10
	
	screen_clp();
	video_initTV();
}

// uart
S_INLINE void uart_putc(char c);

//  screen

INLINE void screen_clear() {
	vram = (uint8*)(ram + OFFSET_RAM_VRAM);
	_g.cursorx = _g.cursory = 0;
	memclear4(vram, SCREEN_W * SCREEN_H);
//	if (_g.uartmode == 2) {
//	if (_g.uartmode & 2) { // 1.2b62
	if ((_g.uartmode_txd & 3) == 2) { // 1.3b2
		uart_putc(0x13); // page up
		uart_putc(0x0c); // clear after cursor
//		uart_putc(21); // ??
//		uart_putc(0); // ??
	}
}

void screen_scroll(int n) {
	switch (n) {
		case 0:
			n = 30;
		case 30: // UP
			/*
			for (int i = 1; i < SCREEN_H; i++) {
				for (int j = 0; j < SCREEN_W; j++)
					vram[(i - 1) * SCREEN_W + j] = vram[i * SCREEN_W + j];
			}
			*/
			memcpy4(vram, vram + SCREEN_W, SCREEN_W * (SCREEN_H - 1)); // ok
			memclear4(vram + (SCREEN_H - 1) * SCREEN_W, SCREEN_W);
			break;
		case 1:
			n = 29;
		case 29: // RIGHT
			for (int i = SCREEN_W - 2; i >= 0; i--) {
				for (int j = 0; j < SCREEN_H; j++)
					vram[j * SCREEN_W + (i + 1)] = vram[j * SCREEN_W + i];
			}
			for (int j = 0; j < SCREEN_H; j++)
				vram[j * SCREEN_W] = 0;
			break;
		case 2:
			n = 31;
		case 31: // DOWN
			for (int i = SCREEN_H - 2; i >= 0; i--) {
				memcpy4(vram + (i + 1) * SCREEN_W, vram + i * SCREEN_W, SCREEN_W);
//				for (int j = 0; j < SCREEN_W; j++)
//					vram[(i + 1) * SCREEN_W + j] = vram[i * SCREEN_W + j];
			}
		//	memcpy(vram + SCREEN_W, vram, SCREEN_W * (SCREEN_H - 1)); // NG
			memclear4(vram, SCREEN_W);
			break;
		case 3:
			n = 28;
		case 28: // LEFT
			for (int i = 1; i < SCREEN_W; i++) {
				for (int j = 0; j < SCREEN_H; j++)
					vram[j * SCREEN_W + (i - 1)] = vram[j * SCREEN_W + i];
			}
			for (int j = 0; j < SCREEN_H; j++)
				vram[j * SCREEN_W + (SCREEN_W - 1)] = 0;
			break;
		default:
			return;
	}
//	if (_g.uartmode == 2) {
//	if (_g.uartmode & 2) { // 1.2b62
	if ((_g.uartmode_txd & 3) == 2) { // 1.3b2
		uart_putc(21);
		uart_putc(n);
	}
}

static void screen_enter() {
	if (_g.cursory == SCREEN_H - 1) {
		uint8 bk = _g.uartmode_txd; // 1.1b4
		_g.uartmode_txd = 0; // 1.1b4
		screen_scroll(0);
		_g.uartmode_txd = bk; // 1.1b4
	} else {
		_g.cursory++;
	}
	_g.cursorx = 0;
}
static uint8* screen_gets() {
	if (_g.cursory == -1) {
		_g.cursory = 0;
		_g.cursorx = 0;
	}
	uint8* p = vram + (_g.cursory - 1) * SCREEN_W;
	if ((!*p) && *(p - 1))
		p -= SCREEN_W;
	while (*p && p >= vram)
		p--;
	return (uint8*)(p + 1);
//	return (char*)(vram + (_g.cursory - 1) * SCREEN_W);
}
//extern uint8 screen_get(int x, int y); // 効かない？

static /*INLINE*/ uint8 screen_get(int x, int y) {
	if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H)
		return '\0';
	return vram[y * SCREEN_W + x];
}
S_INLINE uint8 screen_getCurrent() {
	return screen_get(_g.cursorx, _g.cursory);
}

//#include <stdio.h>

S_INLINE void screen_locate(int x, int y) {
	if (x < 0)
		x = 0;
	else if (x >= SCREEN_W)
		x = SCREEN_W - 1;
	if (y < 0)
		y = -1;
	else if (y >= SCREEN_H)
		y = SCREEN_H - 1;
	_g.cursorx = x;
	_g.cursory = y;
//	if (_g.uartmode == 2) {
//	if (_g.uartmode & 2) { // 1.2b62
	if ((_g.uartmode_txd & 3) == 2) { // 1.3b2
		uart_putc(21);
		uart_putc(_g.cursorx + 32);
		uart_putc(_g.cursory + 32);
	}
}
static void insertLine(int cxlast) {
	int h = cxlast / SCREEN_W + 1;
	uint8* v = vram;
	for (int i = SCREEN_H - 1; i > h; i--) {
//		for (int j = 0; j < SCREEN_W; j++)
//			vram[i * SCREEN_W + j] = vram[(i - 1) * SCREEN_W + j];
		memcpy4(v + i * SCREEN_W, v + (i - 1) * SCREEN_W, SCREEN_W);
	}
//	for (int j = 0; j < SCREEN_W; j++)
//		vram[h * SCREEN_W + j] = 0;
	if (h < SCREEN_H) // 1.2b33
		memclear4(v + h * SCREEN_W, SCREEN_W);
}
/*
static void memclear2(uint8* mem, int len) {
	for (int i = 0; i < len; i++) {
		mem[i] = '.';
	}
}
*/
static void screen_putc(char c) {
//	c = '1'; // test
//	printf("%d %d\n", _g.cursorx, _g.cursory);
	
	if (_g.screen_locatemode) {
		uint8 bk = _g.uartmode_txd;
		_g.uartmode_txd = 0;
		if (_g.screen_locatemode == 2) {
			if (c < 32) {
				screen_scroll(c);
				_g.screen_locatemode--;
			} else {
				screen_locate(c - 32, _g.cursory);
			}
		} else {
			screen_locate(_g.cursorx, c - 32);
		}
		_g.screen_locatemode--;
		_g.uartmode_txd = bk;
		return;
	}
	if (_g.cursory == -1)
		return;
	if (c == '\r') {
	} else if (c == '\n') {
		for (;;) { // 複数行に渡るコマンドのうえで改行した際の処理
			if (!vram[(_g.cursory + 1) * SCREEN_W - 1]) {
				break;
			} else if (_g.cursory == SCREEN_H - 1) {
				screen_enter(); // 1.2b58 add
				break;
			}
			_g.cursory++;
		}
		screen_enter();
	} else if (c == 0x10) { // 行分割
		// カーソル位置から行末までの数 シフト、\0でうめて、screen_enter()
		int now = _g.cursorx + _g.cursory * SCREEN_W;
		int last = now;
		for (;;) {
			if (vram[last] == 0 || last == SCREEN_W * SCREEN_H - 1) {
				break;
			}
			last++;
		}
		int n = SCREEN_W - _g.cursorx;
		if (_g.cursory == SCREEN_H - 1 || last % SCREEN_W >= _g.cursorx) { // 行追加必要? 1.2b35 bug fixed
			if ((_g.cursory > 0 && vram[SCREEN_W * (SCREEN_H - 1)]) || (_g.cursory == SCREEN_H - 1 && _g.cursorx == 0)) {
				for (int i = SCREEN_W; i <= now; i++) {
					vram[i - SCREEN_W] = vram[i];
				}
				memclear(vram + now - SCREEN_W, n);
				while (last >= now) {
					vram[now - _g.cursorx] = vram[now];
					now++;
				}
				last = (now - _g.cursorx) % SCREEN_W;
				if (last) {
					memclear(vram + now - _g.cursorx, SCREEN_W - last); // b14 bug fixed, 1.2b58 bug fixed
				}
				_g.cursorx = 0;
				return;
			}
			insertLine(last);
		}
		while (last >= now) {
			last--; // 1.2b58 bug fixed
			vram[last + n] = vram[last];
		}
		memclear(vram + now, n);
		_g.cursorx = 0;
		if (_g.cursory < SCREEN_H - 1) // 1.2b64 bug fixed
			_g.cursory++;
	} else if (c == '\b' || c == 0x7f) { // backspace and delete
		if (c == 0x7f) {
		} else if (_g.cursorx > 0) {
			_g.cursorx--;
		} else if (_g.cursory > 0 && vram[_g.cursory * SCREEN_W - 1]) {
			_g.cursory--;
			_g.cursorx = SCREEN_W - 1;
		} else {
			return;
		}
		for (int i = _g.cursory * SCREEN_W + _g.cursorx; vram[i]; i++) {
			if (i == SCREEN_W * SCREEN_H - 1) { // 1.2b58 画面末尾のbackspaceやdeleteでプログラムが消えるバグ修正
				vram[i] = 0;
				break;
			}
			vram[i] = vram[i + 1];
		}
	} else if (c == 28) { // left // b13で上書きモード時自由移動
		if (_g.cursorx > 0) {
			_g.cursorx--;
		} else if (_g.cursory > 0 && (vram[_g.cursory * SCREEN_W - 1] || _g.screen_insertmode)) {
			_g.cursory--;
			_g.cursorx = SCREEN_W - 1;
		}
	} else if (c == 29) { // right
		if (_g.cursorx != SCREEN_W - 1 || _g.cursory != SCREEN_H - 1) {
			if (vram[_g.cursory * SCREEN_W + _g.cursorx] || _g.screen_insertmode) {
				_g.cursorx++;
				if (_g.cursorx == SCREEN_W) {
					_g.cursorx = 0;
					_g.cursory++;
				}
			}
		}
	} else if (c == 30) { // up
		if (_g.cursory > 0) {
			_g.cursory--;
			if ((!_g.screen_insertmode) && !vram[_g.cursory * SCREEN_W + _g.cursorx]) {
				while (_g.cursorx > 0 && !vram[_g.cursory * SCREEN_W + _g.cursorx - 1])
					_g.cursorx--;
			}
		}
	} else if (c == 31) { // down
		if (_g.cursory < SCREEN_H - 1) {
			_g.cursory++;
			if ((!_g.screen_insertmode) && !vram[_g.cursory * SCREEN_W + _g.cursorx]) {
				while (_g.cursorx > 0 && !vram[_g.cursory * SCREEN_W + _g.cursorx - 1])
					_g.cursorx--;
			}
		}
	} else if (c == '\t') {
		// 1.4b06 は TABは1タブ
		screen_putc(' ');
		screen_putc(' '); // 1.4b07 は2tabに戻した
		/*
	} else if (c == 11) { // 0x0b v 垂直タブ // 上書きモードでカーソル移動追加したらかいらなくなったかも？？ 1.2b16で削除 16byte空いた
		if (_g.cursory < SCREEN_H - 1) {
			_g.cursory++;
		}
		*/
	} else if (c == 24) { // 0x18 行削除
		int now = _g.cursory * SCREEN_W + _g.cursorx;
		int cxlast = now;
		for (;;) {
			char c = vram[cxlast];
			if (!c)
				break;
			cxlast++;
			if (cxlast == SCREEN_W * SCREEN_H) // add v0.9.8
				break;
		}
		for (now = cxlast - 1; now >= 0; now--) {
			if (!vram[now])
				break;
			vram[now] = 0;
		}
		now++;
		_g.cursorx = now % SCREEN_W;
		_g.cursory = now / SCREEN_W;
	} else if (c == 12) { // 0x0c 書式送り カーソル位置以降削除
		int now = _g.cursory * SCREEN_W + _g.cursorx;
		memclear(vram + now, SCREEN_W * SCREEN_H - now);
	} else if (c == 0x12) { // home
		int now = _g.cursory * SCREEN_W + _g.cursorx;
		for (; now > 0;) {
			if (!vram[--now]) {
				now++;
				break;
			}
		}
		_g.cursorx = now % SCREEN_W;
		_g.cursory = now / SCREEN_W;
	} else if (c == 0x17) { // end
		int now = _g.cursory * SCREEN_W + _g.cursorx;
		for (; now < SCREEN_W * SCREEN_H - 1; now++) { // b58 add -1
			if (!vram[now]) {
				break;
			}
		}
		_g.cursorx = now % SCREEN_W;
		_g.cursory = now / SCREEN_W;
	} else if (c == 0x13) { // page up
		_g.cursorx = _g.cursory = 0;
	} else if (c == 0x14) { // page down
		_g.cursorx = 0;
		_g.cursory = SCREEN_H - 1;
	} else if (c == 0x0e) { // shift + space // いらない?
		uint8 bk = _g.screen_insertmode;
		_g.screen_insertmode = 0; // wasmだけ利かない?
		screen_putc(' ');
		screen_putc(28);
		_g.screen_insertmode = bk;
	} else if (c == 15) {
		key_flg.kana = !key_flg.kana;
	} else if (c == 17) { // 上書き <-> 挿入モード
		key_flg.insert = !key_flg.insert;
	} else if (c == 21) { // == 0x15 // LOCATE or SCROLL
		_g.screen_locatemode = 2;
	} else {
		if (!_g.screen_insertmode) { // insert mode
			int now = _g.cursory * SCREEN_W + _g.cursorx;
			int cxlast = now;
			for (;;) {
				if (cxlast == SCREEN_W * SCREEN_H) { // add v0.9.8 // 1.2b58 change if position
					break;
				}
				char c = vram[cxlast];
				if (!c)
					break;
				cxlast++;
			}
			if (cxlast == SCREEN_W * SCREEN_H) { // 1.2b58
				if (_g.cursory > 0) {
					int bk = _g.uartmode_txd; // 1.1b4
					_g.uartmode_txd = 0; // 1.1b4
					screen_scroll(0);
					_g.uartmode_txd = bk; // 1.1b4
					_g.cursory--;
					now -= SCREEN_W;
					cxlast -= SCREEN_W;
				} else {
					cxlast--;
				}
			}
			for (int i = cxlast - 1; i >= now; i--) {
				vram[i + 1] = vram[i];
			}
			vram[now] = c;
			_g.cursorx++;
			if (now == cxlast) {
				if (_g.cursorx == SCREEN_W) {
					screen_enter();
					
					// 下の行につながらないための処理
					// new 0.9.0
					//if (_g.cursory < SCREEN_H - 1) {
//					if (_g.cursory < SCREEN_H - 1 && vram[now + 1]) {	// 1.2.2
					if (vram[now + 1]) { // 1.2b31 -> 1.2b58
//					if (_g.cursory < SCREEN_H && vram[now + 1]) {	// 1.2b33 .. _g.cursoryはSCREEN_H-1を超えない?
						insertLine(cxlast);
					}
				}
			} else {
				if (_g.cursorx == SCREEN_W) {
					_g.cursorx = 0;
					if (_g.cursory < SCREEN_H - 1) { // add 1.0.0b7
						_g.cursory++;
					} else {
						screen_enter(); // add 1.0.0b7
					}
				}
				if (cxlast % SCREEN_W == SCREEN_W - 1 && vram[cxlast + 1]) { // 1.2b32
					insertLine(cxlast);
				}
			}
		} else { // overwrite mode
			vram[_g.cursory * SCREEN_W + _g.cursorx] = c;
			_g.cursorx++;
			if (_g.cursorx == SCREEN_W)
				screen_enter();
		}
	}
}
S_INLINE void screen_puts(char* s) {
	while (*s)
		screen_putc(*s++);
}

S_INLINE uint screen_pset(int x, int y, int cmd) { // point機能追加 1.3.2b19 cmd:0 reset, 1: pset, 2: xor, other: none
	/*
	if (x >= 0 && x < _g.screenw && y >= 0 && y < _g.screenh) {
		vram[x + y * _g.screenw] = 1;
	}
	*/
	if (x >= 0 && x < _g.screenw * 2 && y >= 0 && y < _g.screenh * 2) {
		uint8* pp = &vram[(x >> 1) + (y >> 1) * _g.screenw];
		uint p = *pp;
		int n = (x & 1) + (y & 1) * 2;
		if (cmd == 3) {
			if ((p < 128 || p >= 128 + 16) && p)
				p = 128 + 15;
			return (p & (1 << n)) > 0;
		}
		if (p < 128 || p >= 128 + 16)
			p = 128;
		if (cmd == 0) {
			p &= ~(1 << n);
		} else if (cmd == 1) {
			p |= 1 << n;
		} else if (cmd == 2) {
			p ^= 1 << n;
		}
		*pp = p;
	}
	return 0;
}

static void screen_line(int x1, int y1, int x2, int y2, int cmd) { // screen_line2 +16byte
	int dx = x2 - x1;
	int dx2 = dx;
	if (dx < 0)
		dx2 = -dx;
	int dy = y2 - y1;
	int dy2 = dy;
	if (dy2 < 0)
		dy2 = -dy;
#ifndef DIVIDE0EQUALS0  // x/0 -> 0 と計算させる環境用（LPC1114版）、これを含めると28byte増
 	if (dx == 0 && dy == 0) {
		screen_pset(x1, y1, cmd);
	} else
#endif
	if (dx2 < dy2) {
		if (y1 < y2) {
			for (int i = 0; y1 + i <= y2; i++) {
				screen_pset(x1 + calcDiv(dx * i, dy), y1 + i, cmd);
			}
		} else {
			for (int i = 0; y2 + i <= y1; i++) {
				screen_pset(x2 + calcDiv(dx * i, dy), y2 + i, cmd);
			}
		}
	} else {
		if (x1 < x2) {
			for (int i = 0; x1 + i <= x2; i++) {
				screen_pset(x1 + i, y1 + calcDiv(dy * i, dx), cmd);
			}
		} else {
			for (int i = 0; x2 + i <= x1; i++) {
				screen_pset(x2 + i, y2 + calcDiv(dy * i, dx), cmd);
			}
		}
	}
}
/*
// https://ja.wikipedia.org/wiki/%E3%83%96%E3%83%AC%E3%82%BC%E3%83%B3%E3%83%8F%E3%83%A0%E3%81%AE%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0
static void screen_linex(int x0, int y0, int x1, int y1) {
	int dx = x1 - x0;
	if (dx < 0)
		dx = -dx;
	int dy = y1 - y0;
	if (dy < 0)
		dy = -dy;
	int d = 2 * dy - dx;
	screen_pset(x0, y0);
	int y = y0;
	for (int x = x0 + 1; x <= x1; x++) {
		if (d > 0) {
			y++;
			screen_pset(x, y);
			d += 2 * dy - 2 * dx;
		} else {
			screen_pset(x, y);
			d += 2 * dy;
		}
	}
}
*/

/*
static void screen_line2(int x0, int y0, int x1, int y1) {
	int dx = x1 - x0;
	if (dx < 0)
		dx = -dx;
	int dy = y1 - y0;
	if (dy < 0)
		dy = -dy;
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx - dy;
	for (;;) {
		screen_pset(x0, y0);
		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		} else if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}
*/

// __SCREEN_H__
#endif
