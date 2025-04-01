// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef  __unix__
#define _XOPEN_SOURCE 500
#include <unistd.h>
#endif//__unix__

#define NO_MEMCPY
#include <string.h>

//#include "ichigojam-stddef.h"
#include <stdint.h>

#define PSG_TICK_FREQ 60
#define PSG_TICK_PER_SEC 60

#define IJB_VER_STR 1.4

#define IJB_TITLE "IchigoJam BASIC " STRING2(IJB_VER_STR) "b MCX jig.jp\n"

#define VER_PLATFORM PLATFORM_CONSOLE

#define USE_EXTENSION

#endif // __CONFIG_H__
