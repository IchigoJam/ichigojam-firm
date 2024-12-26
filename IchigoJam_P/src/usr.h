#include "all_includes.h"

extern uint64 __aeabi_uidivmod(unsigned numerator, unsigned denominator);

INLINE int IJB_usr(int ad, int n) {
	if (ad >= OFFSET_RAMROM && ad < SIZE_RAM + OFFSET_RAMROM) {
		int (*f)(int, void*, void*, void*) = (void*)(ram + ad - (OFFSET_RAMROM - 1)); // Thumbモードは 実アドレス+1
		void* mem = (void*)((uint)ram - OFFSET_RAMROM); // R1
		void* rom = (void*)CHAR_PATTERN;                // R2
		void* fdiv = (void*)(__aeabi_uidivmod);         // R3
		return f(n, mem, rom, fdiv);
	}
	return 0;
}