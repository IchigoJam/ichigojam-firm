// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

void video_on() {
  SCREEN_W = 32;
  SCREEN_H = 24;
}
inline void video_off(int clkdiv) {
}
inline int video_active() {
  return 0;
}

inline void IJB_lcd(uint mode) {
}

inline void video_waitSync(uint n) {
}
