// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#define TOKEN_LOG 1000

void extension_init() {
}

int extension_parse() { // if match ret code else ret 0
  const char* ext = "CONSOLE.LOG";
  for (int i = 0; i < 11; i++) {
    if (basic_toupper(*(pc + i)) != *(ext + i)) {
      return 0;
    }
  }
  pc += 11;
  return TOKEN_LOG;
}

int extension_command(int token) { // if match ret 1
  if (token == TOKEN_LOG) {
    int16 n = token_expression();
    put_str("console.log: ");
    put_num(n);
    put_str("\n");

    if (n >= OFFSET_RAMROM) {
      const char* p = (char*)(RAM_AREA + n - OFFSET_RAMROM);
      char fn[128];
      fn[127] = '\0';
      for (int i = 0; i < 127; i++) {
        if (p[i] == '"' || p[i] == '\0') {
          fn[i] = '\0';
          break;
        }
        fn[i] = p[i];
      }
      //audioplayer_play(fn);
      put_str(fn);
    }
    token_end();

    return 1;
  }
  return 0;
}
