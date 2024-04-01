# Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

TOOLCHAIN=gcc
include Makefile.$(TOOLCHAIN)

all: IchigoJam_BASIC

IchigoJam_BASIC:
	$(CC) -I../IchigoJam_BASIC -I./src/ ../IchigoJam_BASIC/main.c -o IchigoJam_BASIC

run: all
	./IchigoJam_BASIC

clean:
	$(RM) IchigoJam_BASIC
