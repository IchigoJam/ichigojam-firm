// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __RANDOM_H__
#define __RANDOM_H__

// random

// xorshift : http://www.jstatsoft.org/v08/i14/paper。Marsaglia (July 2003). “Xorshift RNGs”
// http://hexadrive.sblo.jp/article/63660775.html

// 8x4 = 32byte

#include "ichigojam-stddef.h"
#include "config.h"

/*
const unsigned int RNDN[] = {
	123456789,
	362436069,
	521288629,
	88675123
};
*/
uint32 rndn[4];

S_INLINE uint32 rnd() {
	uint32 t = rndn[0] ^ (rndn[0] << 11);
	rndn[0] = rndn[1];
	rndn[1] = rndn[2];
	rndn[2] = rndn[3];
	return rndn[3] = (rndn[3] ^ (rndn[3] >> 19)) ^ (t ^ (t >> 8));
}

S_INLINE void random_seed(int32 n) {
	rndn[0] = n;
	rndn[1] = 362436069;
	rndn[2] = 521288629;
	rndn[3] = 88675123;
}
S_INLINE void random_init() {
	random_seed(123456789);
}

#endif	// __RANDOM_H__
