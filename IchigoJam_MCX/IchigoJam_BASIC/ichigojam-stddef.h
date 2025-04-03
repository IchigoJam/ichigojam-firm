// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#define LANG_ALL 0 // all
#define LANG_JP 1 // Japan
#define LANG_MN 2 // Mongol
#define LANG_VI 3 // Vietnam
#define LANG_FR 4 // French
#define LANG_ZH 5 // Chinese
#define LANG_BP 6 // Bopomofo

#define TV_MODE_NTSC 1
#define TV_MODE_PAL 2

#ifndef TV_MODE
#define TV_MODE TV_MODE_NTSC
#endif

#define KEY_LAYOUT_JP 1
#define KEY_LAYOUT_US 2
#define KEY_LAYOUT_JP_DVORAK 3
#define KEY_LAYOUT_AZERTY 4
//#define KEYLAYOUT_JP_DVORAK

//#define KEYBOARD_USB

#define PLATFORM_LPC1114       1
#define PLATFORM_RPI           2
#define PLATFORM_WASM          3
#define PLATFORM_PC            4
#define PLATFORM_LPC1114_DAKE  5
#define PLATFORM_LPC1114_CAKE  6
#define PLATFORM_GD32VF103     7
#define PLATFORM_SPRESENSE     8
#define PLATFORM_CONSOLE      10

//#define AUTO_LRUN0

/*
// basic
reset EXT_IOT, FLASH_ONLY_ONE(config.h)
reset pal
set us
./cm.sh
mv ../dest/ichigojam.hex ../release/ichigojam-ntsc-uskbd.hex
mv ../dest/ichigojam.bin ../release/ichigojam-ntsc-uskbd.bin

reset us
./cm.sh
mv ../dest/ichigojam.hex ../release/ichigojam-ntsc-jpkbd.hex
mv ../dest/ichigojam.bin ../release/ichigojam-ntsc-jpkbd.bin

set lang bp
set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-bp-ntsc-uskbd.hex
mv ichigojam.bin ../bin/ichigojam-bp-ntsc-uskbd.bin

reset us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-bp-ntsc-jpkbd.hex
mv ichigojam.bin ../bin/ichigojam-bp-ntsc-jpkbd.bin

set lang zh, us
./cm.sh
mv ichigojam.hex ../bin/funtick-zh-ntsc-uskbd.hex
mv ichigojam.bin ../bin/funtick-zh-ntsc-uskbd.bin

// iot

set EXT_IOT, FLASH_ONLY_ONE(config.h)
reset pal
set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-iot-ntsc-uskbd.hex
mv ichigojam.bin ../bin/ichigojam-iot-ntsc-uskbd.bin

reset us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-iot-ntsc-jpkbd.hex
mv ichigojam.bin ../bin/ichigojam-iot-ntsc-jpkbd.bin



set lang fr
./cm.sh
mv ichigojam.hex ../bin/ichigojam-ntsc-frkbd.hex
mv ichigojam.bin ../bin/ichigojam-ntsc-frkbd.bin


// pal

set pal
./cm.sh
mv ichigojam.hex ../bin/ichigojam-pal-jpkbd.hex
mv ichigojam.bin ../bin/ichigojam-pal-jpkbd.bin

set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-pal-uskbd.hex
mv ichigojam.bin ../bin/ichigojam-pal-uskbd.bin



// lang = mn

reset pal
set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-mn-ntsc-uskbd.hex
mv ichigojam.bin ../bin/ichigojam-mn-ntsc-uskbd.bin

reset us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-ntsc-jpkbd-mn.hex
mv ichigojam.bin ../bin/ichigojam-ntsc-jpkbd-mn.bin

// pal

set pal
./cm.sh
mv ichigojam.hex ../bin/ichigojam-pal-jpkbd-mn.hex
mv ichigojam.bin ../bin/ichigojam-pal-jpkbd-mn.bin

set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-pal-uskbd-mn.hex
mv ichigojam.bin ../bin/ichigojam-pal-uskbd-mn.bin

// lang = vi

reset pal
set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-vi-ntsc-uskbd.hex
mv ichigojam.bin ../bin/ichigojam-vi-ntsc-uskbd.bin

reset us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-ntsc-jpkbd-vi.hex
mv ichigojam.bin ../bin/ichigojam-ntsc-jpkbd-vi.bin

leng = mn

./cm.sh
mv ichigojam.hex ../bin/ichigojam-mn-ntsc-uskbd.hex
mv ichigojam.bin ../bin/ichigojam-mn-ntsc-uskbd.bin

// pal

set pal
./cm.sh
mv ichigojam.hex ../bin/ichigojam-pal-jpkbd-vi.hex
mv ichigojam.bin ../bin/ichigojam-pal-jpkbd-vi.bin

set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-pal-uskbd-vi.hex
mv ichigojam.bin ../bin/ichigojam-pal-uskbd-vi.bin


*/

/*
set big
set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-big-us.hex
mv ichigojam.bin ../bin/ichigojam-xtal-big-us.bin

reset us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-big.hex
mv ichigojam.bin ../bin/ichigojam-big.bin


set lang 3 // vi
set us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-vi-us.hex
mv ichigojam.bin ../bin/ichigojam-xtal-vi-us.bin

set lang 3 // vi
reset us
./cm.sh
mv ichigojam.hex ../bin/ichigojam-vi-xtal.hex
mv ichigojam.bin ../bin/ichigojam-vi-xtal.bin


set US
./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-us.hex
mv ichigojam.bin ../bin/ichigojam-xtal-us.bin

set LCD
./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-lcd-us.hex
mv ichigojam.bin ../bin/ichigojam-xtal-lcd-us.bin

reset LCD

set MN
./cm.sh
mv ichigojam.hex ../bin/ichigojam-mn-xtal-us.hex
mv ichigojam.bin ../bin/ichigojam-mn-xtal-us.bin

reset US
./cm.sh
mv ichigojam.hex ../bin/ichigojam-mn-xtal.hex
mv ichigojam.bin ../bin/ichigojam-mn-xtal.bin

reset MN

set LCD
./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-lcd.hex
mv ichigojam.bin ../bin/ichigojam-xtal-lcd.bin

reset LCD

./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal.hex
mv ichigojam.bin ../bin/ichigojam-xtal.bin


./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-biglcd.hex
mv ichigojam.bin ../bin/ichigojam-xtal-biglcd.bin

./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-biglcd-us.hex
mv ichigojam.bin ../bin/ichigojam-xtal-biglcd-us.bin


LED_MODE2 BIG_SCREEN US
./cm.sh 
mv ichigojam.hex ../bin/ichigojam-xtal-mobilebig-us.hex
mv ichigojam.bin ../bin/ichigojam-xtal-mobilebig-us.bin

LED_MODE2 BIG_SCREEN
./cm.sh
mv ichigojam.hex ../bin/ichigojam-xtal-mobilebig.hex
mv ichigojam.bin ../bin/ichigojam-xtal-mobilebig.bin

*** 仮アップ用
./cm.sh
mv ichigojam.hex ../bin/ichigojam.hex
mv ichigojam.bin ../bin/ichigojam.bin


*/

#ifndef __STDDEF_H__
#define __STDDEF_H__

// #define EXT_SENSEHAT // Sense HAT用拡張

#ifdef INLINE_AS_STATIC
#define INLINE static
#define S_INLINE static
#else
#define INLINE inline
#define S_INLINE static inline
#endif

#define ALL_STATIC

#ifdef ALL_STATIC
#define STATIC static
#else
#define STATIC
#endif

#define STRING2(s) STRING(s)
#define STRING(str) #str

#include <stdint.h>
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef unsigned int uint;
typedef uint64_t uint64;
typedef uintptr_t pint;

#ifndef NULL
#define NULL 0
#endif

#ifdef NONE_STATIC_MEMCPY // for wasm

//void memcpy(uint8* dst, const uint8* src, int len) {
void* memcpy(void* dst, const void* src, unsigned long len) {
	for (int i = len; i > 0; i--)
		*(uint8*)dst++ = *(const uint8*)src++;
	return dst;
}

#else
#ifndef NO_MEMCPY

static void memcpy(uint8* dst, const uint8* src, int len) {
	for (int i = len; i > 0; i--)
		*dst++ = *src++;
}

#else
#endif
#endif

static void memclear(volatile uint8* p, int len) {
	for (int i = len; i > 0; i--)
		*p++ = 0;
}
/*
#define memset(p, n, len) clearMemory(p, len)
void clearMemory(char* p, int len) {
	for (int i = 0; i < len; i++)
		*p++ = 0;
}
*/


#define memclear4(p, len) memclear((p), (len))
#define memcpy4(dst, src, len) memcopy((dst), (src), (len))
#define memcopy(dst, src, len) memcpy((dst), (src), (len))


//extern uint __aeabi_uidiv(uint numerator, uint denominator);
//uint (*calcDivU)(uint, uint);
#define calcDivU(n, m) ((uint)(n) / (uint)(m))
#define calcModU(n, m) ((uint)(n) % (uint)(m))
//#define calcDivU(n, m) __aeabi_uidiv((uint)(n), (uint)(m))

// for LPC1114
static int calcDiv(int n, int m) {
	int flg = 1;
	if (n < 0) {
		n = -n;
		flg = -flg;
	}
	if (m < 0) {
		m = -m;
		flg = -flg;
	}
	n = (uint)n / (uint)m;
//	n = __aeabi_uidiv(n, m); // 1.3b12
//	n = calcDivU(n, m); // 1.3b12
	return n * flg;
}
static int calcMod(int n, int m) {
	int d = calcDiv(n, m);
	return n - d * m;
}

// __STDDEF_H__
#endif
