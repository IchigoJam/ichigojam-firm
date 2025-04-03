// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __LANG_H__
#define __LANG_H__

#ifndef LANG
#define LANG LANG_JP
//#define LANG LANG_MN
//#define LANG LANG_VI
//#define LANG LANG_FR  // use AZERTY automatically kana
//#define LANG LANG_BP // Bopomofo (for Taiwan)
//#define LANG LANG_ZH // FunTick
#endif

#if LANG == LANG_ZH
#define BOOT_CHECK 3
#elif LANG != LANG_JP
#define BOOT_CHECK 35
#endif

#if LANG == LANG_FR
#define KEY_LAYOUT KEY_LAYOUT_AZERTY
#else
//#define KEY_LAYOUT KEY_LAYOUT_US // if us
#endif

#ifndef KEY_LAYOUT
#define KEY_LAYOUT KEY_LAYOUT_JP
#endif

// __LANG_H__
#endif
