// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

// IchigoJam RAM
#ifndef __RAM_H__
#define __RAM_H__

#define SIZE_PCG 32
#define N_LINEBUF 200 // もうちょっと増やしてもいいかも? 32x7 = 224 - 行番号の長さ最小2、222? あと22byte? // 1.2b32 200->202 bugfixついでに

//#define SIZE_ROM_PATTERN ((256 - 32) * 8) // 0x700

// サイズ変更したら、lpc1114.ld を変更すること!

#define SIZE_RAM_PCG (SIZE_PCG * 8) // 0x100
#define SIZE_RAM_VAR ((102 + 26) * 2) // 0x100
#define SIZE_RAM_VRAM (32 * 24) // 0x300
#define SIZE_RAM_LIST (1024 + 2) // 0x402
//#define SIZE_RAM_KEYBUF 32 // 1.1b14 16->32
//#define SIZE_RAM_KEYBUF 128 // 1.2b9 32->128, 1.2b56 130にしたい
//#define SIZE_RAM_KEYBUF 66 // 1.2.3 128だった -> 1.2b57 64 (62文字) -> 1.3b2 66 (64文字)
#define SIZE_RAM_KEYBUF 128 // 1.2.3 128 -> 1.3b4 128に戻す (126文字)
#define SIZE_RAM_LINEBUF N_LINEBUF // 1.2
#define SIZE_RAM_I2CBUF 54 // 1.2b42, 1.3b2 1->32, 1.3b3 32->54 A-Zまでを保存できる52byte+2byte
#define SIZE_RAM (SIZE_RAM_PCG + SIZE_RAM_VRAM + SIZE_RAM_VAR + SIZE_RAM_LIST + SIZE_RAM_KEYBUF + SIZE_RAM_LINEBUF + SIZE_RAM_I2CBUF)

#define OFFSET_RAM_PCG 0 // basic:#700
#define OFFSET_RAM_VAR (OFFSET_RAM_PCG + SIZE_RAM_PCG) // basic:#800
#define OFFSET_RAM_VRAM (OFFSET_RAM_VAR + SIZE_RAM_VAR) // basic:#900
#define OFFSET_RAM_LIST (OFFSET_RAM_VRAM + SIZE_RAM_VRAM) // basic:#C00
#define OFFSET_RAM_KEYBUF (OFFSET_RAM_LIST + SIZE_RAM_LIST) // basic:#1002
#define OFFSET_RAM_LINEBUF (OFFSET_RAM_KEYBUF + SIZE_RAM_KEYBUF) // basic:#1044 -> #1082-#1149
#define OFFSET_RAM_I2CBUF (OFFSET_RAM_LINEBUF + SIZE_RAM_LINEBUF) // basic:#110C -> #114A-#117F

#define OFFSET_RAMROM ((0x100 - SIZE_PCG) * 8)

/*
SIZE_RAM = 4396 112C - (0x700) = 2604 #A2C -> 1.3b3 2636 #A4C
virtual address	description
#000-#6FF	character pattern (ROM)
#700-#7FF	PCG (RAM)
#800-#8FF	VAR (array + var A-Z)
#900-#BFF	VARM
#C00-#FFF	LIST
#1000-#1001	end of list (must be 0)
#1002-#1002	keystate (5bit space, down, up, right, left)
#1003-#1003	nkeybuf (0-126)
#1004-#1081	keybuf (126byte)
#1082-#1149	linebuf (200byte)
#114A-#117F	i2cbuf for IoT.OUT/IoT.IN() (54byte)

x 110C-112B i2cbuf IoTコマンド (32byte) 1.3b2
*/

#if RAM_FIXED == 1

// RAMの先頭に持っていく、#700 == 0x10000000
//#define ram ((uint8_t*)0x10000000)

// RAMの仕様領域末尾に持っていく * ichigojam.mapファイル参照
//                0x00000000100000e0                _end_noinit = .
//#define ram ((uint8_t*)0x100000e0)
#define ram ((uint8_t*)0x10000000 + STACK_SIZE) // after stack

/*
memmap

4Kbyte ram
1152 byte for stack (other)
2688	RAM (1152+2688 = 3840)
224	for system
32	for ISP
*/

#else

uint8 ram[SIZE_RAM] __attribute__ ((aligned(4)));

#endif

// セクション頭に持っていくと容量オーバーする
//uint8_t ram[SIZE_RAM] __attribute__((section(".ichigojam_ram")));
//uint8_t ram[SIZE_RAM] __attribute__((section(".bss")));
// token_expression5 が変わる

//extern uint8 _ichigojam_ram[SIZE_RAM]; // RAM先頭に設置する
//#define ram _ichigojam_ram

#endif
