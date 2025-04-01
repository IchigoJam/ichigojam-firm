// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

inline int IJB_usr(int ad, int n);

extern uint64 __aeabi_uidiv(uint numerator, uint denominator);
#define FUNC_DIVU __aeabi_uidiv

inline int IJB_usr(int ad, int n) {
	if (ad >= OFFSET_RAMROM && ad < SIZE_RAM + OFFSET_RAMROM) { // b10 limit:0x800 -> 0x1000
		int (*f)(int, void*, void*, void*) = (void*)(ram + ad - (OFFSET_RAMROM - 1));
		void* mem = (void*)((uint)ram - OFFSET_RAMROM); // R1
		void* rom = (void*)CHAR_PATTERN; // R2
		void* fdiv = (void*)FUNC_DIVU; // R3
		_g.err = ERR_SEGMENTATION_FAULT;
		n = f(n, mem, rom, fdiv); // 1.2.2b31
		_g.err = 0;
	} else {
		command_error(ERR_ILLEGAL_ARGUMENT);
	}
	return n;
}
