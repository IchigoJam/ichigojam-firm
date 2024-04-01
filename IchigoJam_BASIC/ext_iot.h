// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __IOT_H__
#define __IOT_H__

#ifdef EXT_IOT

/*
#if VER_PLATFORM != PLATFORM_LPC1114

static void iot_out(int n) {
}
static int16 iot_in() {
	return 0;
}
static void iot_out2(int ad, int len, int flash) { // 104 -> -44  .. +148byte
}

#else
*/

/*
// 統合しても縮まらなかった
#define iot_in() iot_inout(1, 0)
#define iot_out(n) iot_inout(0, (n))

static int16 iot_inout(int typein, int n) {
	int16* v = (int16*)(ram + OFFSET_RAM_I2CBUF); // i2cbuf size 54byte
	uint16 param[5];
	if (typein) {
		v[0] = v[1] = 0x30;
		param[0] = 0x4F;
		param[1] = 0x700 + OFFSET_RAM_I2CBUF; // 0x114A;
		param[2] = 3;
		param[3] = 0x704 + OFFSET_RAM_I2CBUF; // 0x11A4+4 v4 ... 0x11A4+4+8
		param[4] = 21; // Rx dequeue // 1.3.1:6, 1.2.4:22
		if (!IJB_i2c(1, param)) {   // 0:success 1:io error 2:parm error
			if (v[2] != 5) { //if (v[2] != 1) // 1 == 正常
				if (v[3] >> 8 == 'b') { // 上位8bitでバイナリだったら文字列として返す、下位8bitはchannnel
					memcpy((uint8*)(v + 13), (const uint8*)(v + 4), 8);
					v[18] = 0;
					return 0x700 + OFFSET_RAM_I2CBUF + 13 * 2; // 先頭アドレスを返す // #115E
				}
				return v[4]; // #1152
			}
		}
	} else {
		v[0] = 0x0A21;
		//v[1] = 0x4C01;	// unsigned 64bit integer
		//v[1] = 0x6C01;	// signed 64bit integer
		v[1] = 0x6900;		// signed 32bit integer
		v[2] = n;
		v[3] = n < 0 ? -1 : 0;
		v[4] = v[5] = 0;
		//v[6] = ((n >> 8) ^ n ^ 102) & 0xff;	// unsigned 64bit integer
		//v[6] = ((n >> 8) ^ n ^ 70) & 0xff;	// signed 64bit integer
		v[6] = ((n >> 8) ^ n ^ 0x42) & 0xff;	// signed 32bit integer
		uint16 param[5];
		param[0] = 0x4F;
		param[1] = 0x700 + OFFSET_RAM_I2CBUF; // 0x114A;
		param[2] = 13;
		param[3] = 0x70D + OFFSET_RAM_I2CBUF;
		param[4] = 3;
		IJB_i2c(1, param);
	}
	return 0;
}
*/

static int16 iot_in() {
	// POKE#114A,48,0,48:U=I2CR(79,#114A,3,#114E,22)
	//:F=PEEK(#[16]!=5:N=[18]:?F,N
	int16* v = (int16*)(ram + OFFSET_RAM_I2CBUF); // i2cbuf size 54byte
	uint16 param[5];
	v[0] = v[1] = 0x30;
	param[0] = 0x4F;
	param[1] = 0x700 + OFFSET_RAM_I2CBUF; // 0x114A;
	param[2] = 3;
	param[3] = 0x704 + OFFSET_RAM_I2CBUF; // 0x11A4+4 v4 ... 0x11A4+4+8
	param[4] = 21; // Rx dequeue // 1.3.1:6, 1.2.4:22
	if (IJB_i2c(1, param))   // 0:success 1:io error 2:parm error
		return 0; // err
	if (v[2] == 5) //if (v[2] != 1) // 1 == 正常
		return 0;
	if (v[3] >> 8 == 'b') { // 上位8bitでバイナリだったら文字列として返す、下位8bitはchannnel
		memcpy((uint8*)(v + 13), (const uint8*)(v + 4), 8);
		v[18] = 0;
		return 0x700 + OFFSET_RAM_I2CBUF + 13 * 2; // 先頭アドレスを返す // #115E
	}
	return v[4]; // #1152
}

static void iot_out(int n) {
	// LET[0],2593,19457,N,0,0,0,N>>8^N^102:U=I2CR(79,#800,13,#820,3)
	// 1 79 2048 13 2080 3
	int16* v = (int16*)(ram + OFFSET_RAM_I2CBUF);
	v[0] = 0x0A21;
	//v[1] = 0x4C01;	// unsigned 64bit integer
	//v[1] = 0x6C01;	// signed 64bit integer
	v[1] = 0x6900;		// signed 32bit integer // ch0
	v[2] = n;
	v[3] = n < 0 ? -1 : 0;
	v[4] = v[5] = 0;
	//v[6] = ((n >> 8) ^ n ^ 102) & 0xff;	// unsigned 64bit integer
	//v[6] = ((n >> 8) ^ n ^ 70) & 0xff;	// signed 64bit integer
	v[6] = ((n >> 8) ^ n ^ 0x42) & 0xff;	// signed 32bit integer
	uint16 param[5];
	param[0] = 0x4F;
	param[1] = 0x700 + OFFSET_RAM_I2CBUF; // 0x114A;
	param[2] = 13;
	param[3] = 0x70D + OFFSET_RAM_I2CBUF;
	param[4] = 3;
	IJB_i2c(1, param);
}

/*
https://fukuno.jig.jp/2240
810 @ADD
LET[95],2592,25089
C=0:FORI=0TO11:C=C^PEEK(#8BE+I):NEXT  '#8BE = &[95]
[101]=C:
?I2CR(79,#8BE,13,#8BE,3):
RTN


LET[95],2592,25089
C=0:FORI=0TO3:C=C^PEEK(#8BE+I):NEXT:?"C=";C

C=73
FORI=4TO11:C=C^PEEK(#8BE+I):NEXT  '#8BE = &[95]
*/
static void iot_out2(int ad, int len, int flash) { // 104 -> -44  .. +148byte
	//if (len > 256 || len < 0 || (len & 7)) { // adチェックしていない
	if (len > 256 || len <= 0) { // adチェックしていない
		command_error(ERR_ILLEGAL_ARGUMENT);
		return; // err
	}
	if (len & 7) { // 8の倍数でなかったら丸める
		len = ((len >> 3) + 1) << 3;
	}
	uint8_t* data = (uint8_t*)(ram + ad - OFFSET_RAMROM);
	uint16 param[5];
	param[0] = 0x4F; // =79
	param[1] = 0x700 + OFFSET_RAM_I2CBUF; // 0x114A;
	param[2] = 13;
	param[3] = 0x700 + 13 + OFFSET_RAM_I2CBUF;
	param[4] = 3;
	uint8_t* v = (uint8_t*)(ram + OFFSET_RAM_I2CBUF);
	for (int j = 0; j < len; j += 8) {
		*(uint16_t*)v = 0x0A20; // write queue = 2592
		//v[1] = 0x4C01;	// unsigned 64bit integer
		//v[1] = 0x6C01;	// signed 64bit integer
	//	v[1] = 0x6900;		// signed 32bit integer
		//*(uint16_t*)(v + 2) = 0x6201;		// 8 byte binary // ch1 1.4.2
		//int c = 73; // for ch1
		*(uint16_t*)(v + 2) = 0x6200;		// 8 byte binary // ch0 1.4.3 1.4b22
		int c = 72; // for ch0
		for (int i = 0; i < 8; i++) {
			int x = *data++;
			v[4 + i] = x;
			c ^= x;
		}
		v[12] = c;
		IJB_i2c(1, param);
	}
	if (flash) {
		// flash
		// @FL:LET[95],36,36:?I2CR(79,#8BE,3,#8BE,3):RTN
		*(uint16_t*)v = 36;
		*(uint16_t*)(v + 2) = 36;
	//	param[0] = 0x4F; // =79
	//	param[1] = 0x700 + OFFSET_RAM_I2CBUF; // 0x114A;
		param[2] = 3;
	//	param[3] = 0x700 + 13 + OFFSET_RAM_I2CBUF;
	//	param[4] = 3;
		IJB_i2c(1, param);
	}
}

//#endif // #if VER_PLATFORM != PLATFORM_LPC1114

#endif	// EXT_IOT

#endif	// __IOT_H__