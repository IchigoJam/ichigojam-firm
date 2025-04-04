// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#include "audioplayer.h"

#define TOKEN_SPR_PLAY 1000

void audioplayer_play(const char* fn);

void extension_init() {
  audioplayer_init();
}

int extension_parse() { // if match ret code else ret 0
  const char* ext = "SPR.PLAY";
  for (int i = 0; i < 8; i++) {
    if (basic_toupper(*(pc + i)) != *(ext + i)) {
      return 0;
    }
  }
  pc += 8;
  return TOKEN_SPR_PLAY;
}

int extension_command(int token) { // if match ret 1
  if (token == TOKEN_SPR_PLAY) {
  	int16 n = token_expression();
		if (n >= OFFSET_RAMROM) {
			const char* p = (char*)(RAM_AREA + n - OFFSET_RAMROM);
      char fn[128];
      fn[127] = '\0';
      for (int i = 0; i < 128 - 5; i++) {
        if (p[i] == '"' || p[i] == '\0') {
          fn[i++] = '.';
          fn[i++] = 'm';
          fn[i++] = 'p';
          fn[i++] = '3';
          fn[i++] = '\0';
          break;
        }
        fn[i] = p[i];
      }
      audioplayer_play(fn);
		} else if (n >= 0 && n < 10) {
      char fn[128];
      int i = 0;
      fn[i++] = '0' + n;
      fn[i++] = '.';
      fn[i++] = 'm';
      fn[i++] = 'p';
      fn[i++] = '3';
      fn[i++] = '\0';
      audioplayer_play(fn);
    }
  	token_end();
    return 1;
  }
  return 0;
}
