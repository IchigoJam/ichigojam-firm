// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#include <stdio.h>

static char FlashFileName[] = "0.bin";

void flash_init() {
}
INLINE int IJB_file() {
	return _g.lastfile;
}
int IJB_save(int n, uint8* list, int size) { // success:0, err: 1
	FILE *fp;
	int sz;

	if (n < 0 || n > 9)
		return -1;

	FlashFileName[0] = '0' + n;
	if ((fp = fopen(FlashFileName, "wb")) == NULL) // z88dk needs "wb"
		return -1;

	sz = fwrite(list, 1, size, fp);
	fclose(fp);

	if (sz != size)
		return -1;

	_g.lastfile = n;
	return 0;
}
int IJB_load(int n, uint8* list, int sizelimit, int init) { // ret:size if:-1 err
	FILE *fp;
	int i, sz;

	if (n < 0 || n > 9)
		return -1;
	if (init)
		_g.lastfile = n;

	FlashFileName[0] = '0' + n;
	if ((fp = fopen(FlashFileName, "rb")) == NULL) // z88dk needs "rb"
		return -1;
		
	sz = fread(list, 1, sizelimit, fp);
	fclose(fp);

	if (sz < 0)
		return -1;

	for (i = sz; i < sizelimit; i++)
		list[i] = 0;

	// remove trailing garbage (CP/M pads ^Z)
	for (i = sz - 1; i >= 0 && list[i]; i--, sz--)
		list[i] = 0;

	return sz;
}
