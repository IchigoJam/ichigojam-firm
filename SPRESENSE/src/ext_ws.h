// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __EXT_WS_H__
#define __EXT_WS_H__

S_INLINE void ws_out(int port, int nled, int repeat) {
	if (nled <= 0 || nled > 34 || !repeat)
		return;
	/*
	FUNC_WS_OUT_ASM ws_out_asm = (FUNC_WS_OUT_ASM)WS_OUT_ASM;
	//ws_out_asm(-(nled * 3 + (--repeat << 16)), RAM_VAR, 14); // v3
	//ws_out_asm(-((nled * 3) | (--repeat << 16)), RAM_VAR, 14); // v3
	ws_out_asm(-nled * 3, RAM_VAR, repeat); // v3 ... うまくいかないのでしかたなく
	*/
}

#endif	//__EXT_WS_H__
