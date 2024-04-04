// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

void video_on() {
  SCREEN_W = 32;
  SCREEN_H = 24;
}
INLINE void video_off(int clkdiv) {
}
INLINE int video_active() {
  return 0;
}

INLINE void IJB_lcd(uint mode) {
}

INLINE void video_waitSync(uint n) {
}
