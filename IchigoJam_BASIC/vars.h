// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

// 24804 
// 24736 // randam
// 24700 // main

// 24628	     56
// 24572	     40	 // rnd以降
// 24556	     40 // lastsfile, fileslot
// 24364	     32	// screen
// 24256	     28 // psg
// 24260	     28 // ps2 +4 増えた
// 24224	     28 // uartmode / uartmode_rxd

// 第二期
// 27000 
// 24576 // psg
// 24556 // key_flg_esc
// 24500 // basic
// 24480 // led_init まとめ
// 24372 // basic-main inline最適化
// 24600 // COPY

#ifndef __VARS_H__
#define __VARS_H__

struct GLOBAL {
	// flgs -> screen.h key_flgへ
//	uint8 errorignore:1; // = 1 // flg
//	uint8 noresmode:1; // 1:OK、エラー表示しない // flg

	// main

	volatile char key_flg_esc; // = 0 // key_def.esc へ移行すると増加する // flg
	uint8 lastfile; // = 0;
	uint8 fileslot;
	uint8 uartmode_txd; // = 0; // txd 0:no uart, 1:print only, 2:print lc cls scroll, 3:buffered \r\n // init内で設定する

	uint8 uartmode_rxd; // = DEFAULT_UARTMODE_RXD; // rxd 0:no uart, 1:enable, (2 auto comment mode?)
	uint8 sleepflg; // 自動起動フラグ

	// basic
	uint8 tokenmode; // トークンモード、0:コマンド 1:式 // flg
	uint8 ngosubstack;

	uint8 nforstack;
	uint8 err;// = 0;

	// romajikana
	uint8 key_kana_buf_0; // 増えた
	uint8 key_kana_buf_1; // 増えた

	// keyboard_ps2
	uint8 key_data_g;
	char key_nbit_g;
	char key_cnt_g;

	// iic.h
	volatile uint8 i2c_stat; // 1.4b10
//	uint16 key_bk_g;

	int16 listsize; // プログラムサイズ 最後の (int16)0 を除く

// random
// 8x4 = 32byte
/*
	// screen
	uint16 lines;
*/
	// volatile uint16 frames; // _gに入れると増える
	volatile uint16 linecnt; // 8byte空いた // 元々USBキーボード対応用 -> TICK(1)で使う

	int8 cursorx;
	int8 cursory;
	uint8 screen_locatemode; // flg
	volatile uint8 cursorflg; // flg = 1

	uint8 screen_insertmode; // flg = 1
	uint8 screen_invert; // flg
	uint8 screen_big; // 段階あり
	volatile uint8 vflag;

	// lcd
	uint8 display_mode; // flg ntsc:0 or lcd:1 // key_flgにいれると容量増加
	uint8 lcd_init; // flg notyet:0 or init:1 // key_flgにいれると容量増加

	// psg
	uint8 psgoct;
	uint8 psgdeflen;

	volatile uint8 psg_sounder; // bit
	volatile uint8 psgratio; // 0
	volatile uint16 psgwaitcnt; // 1.3.2b17 uint8 -> 16
	volatile uint16 psgtone; // = 0; // b17 uint8 -> 16
	uint16 psgtempo;

	// main
	//uint8 sleepflg; // 自動起動フラグ
	// /uint8 dummy; // あまり

	// psg

	volatile uint psglen;
	char* psgmml; // = NULL;
	char* psgrep;

/*
	//ps2
	int ps2_bkt;// = 0; // 32bitは使いすぎ？
	uint8 ps2_data;// = 0;
	char ps2_nbit;// = 0;
	char ps2_cnt;// = 0; // PS2信号の1の数を数える
	char ps2_enable_flg;// = 0; // flg
	char ps2_kana_nbuf;// = 0;
*/

	// display
	uint screenw; // uint16にすると容量増
	uint screenh;

	// io
	uint16 outbuf;
	// iic
	uint16 i2cbps;
	// eeprom
//	uint8 cmdaddress[2]; // 増えた

	// system
	//uint32_t SystemCoreClock; // 変わらない

};
struct GLOBAL _g;

/*
                0x0000000010000a44                _g

                0x0000000010000a38                pcbreak
                0x0000000010000b14                bklasttoken
                0x0000000010000b48                lasttoken
                0x0000000010000b4c                pc
                0x0000000010000b54                lasttokenpc

                0x0000000010000a7c                gosubstack
                0x0000000010000af4                forstack

                0x0000000010000a40                cmdaddress x

                0x0000000010000b0c                SystemCoreClock x

                0x0000000010000b10                key_kana_buf1 x
                0x0000000010000b44                key_kana_buf0 x

                0x0000000010000b11                key_flg // bit field

                0x0000000010000b18                I2cCtrl

                0x0000000010000b2c                rndn
                0x0000000010000b3c                pvram
                0x0000000010000b40                keybuf
                0x0000000010000b50                vram

                0x0000000010000a3e                linecnt v
                0x0000000010000b58                frames x
                0x0000000010000b5a                lines ?
*/

// __VARS_H__
#endif