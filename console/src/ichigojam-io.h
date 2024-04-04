// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

/*S_INLINE*/ void IJB_pwm(int port, int plen, int len);
S_INLINE void pwm_off(int port);
void io_init();
S_INLINE void io_set(int n);
S_INLINE int analog_get(int ch);
S_INLINE int io_get();
S_INLINE void IJB_led(int st);
/*S_INLINE*/ int IJB_in();
S_INLINE void IJB_clo();
S_INLINE int IJB_ana(int n);
/*S_INLINE*/ int IJB_btn(int n);
/*S_INLINE*/ void IJB_out(int port, int st);
S_INLINE int IJB_i2c(uint8 writemode, uint16* param); // 0:success 1:io error 2:parm error

int i2c0_init() {
  return 0;
}


// PWM
// PWM 2,100  100kHz PWM  OUT2,3,4
// PWM 2,100,2000 で周期設定、2-4はまとめて設定、5は独立
//   PWM n で、OUT出力からは除外される（設定してないと出力されない）
//  OUT n,0 でPWMは停止する
/*S_INLINE*/ void IJB_pwm(int port, int plen, int len) {
}
S_INLINE void pwm_off(int port) {
}
// I/O

/*
IN
  1 PIO0_10
  2 PIO0_11
  3 PIO0_5 (OD)
  4 PIO0_6
  5 PIO1_0 OUT1と共有
  6 PIO1_1 OUT2と共有
  7 PIO1_2 OUT3と共有
  8 PIO1_3 OUT4と共有
  OUT
  1 PIO1_0 IN5と共有
  2 PIO1_1 IN6と共有
  3 PIO1_2 IN7と共有
  4 PIO1_3 IN8と共有
  5 PIO1_9
  6 PIO1_8
  */ 
void io_init() {
}
// ADC
S_INLINE int analog_get(int ch) {
  return 0;
}

#define ANA_THRESHOLD (1024 / 4)
S_INLINE int io_get() {
  return 0;
}
S_INLINE void io_set(int n) {
}

S_INLINE void IJB_led(int st) {
//  IJB_out(7, st != 0);
}
/*S_INLINE*/ int IJB_in() {
  return io_get();
}
// out buffer
/*S_INLINE*/ void IJB_out(int port, int st) {
}
/*S_INLINE*/ int IJB_btn(int n) {
  return 0;
}
S_INLINE int IJB_ana(int n) {
  return 0;
}
S_INLINE void IJB_clo() {
  io_init();
}

S_INLINE int IJB_i2c(uint8 writemode, uint16* param) { // 0:success 1:io error 2:parm error
  return 1;
}
