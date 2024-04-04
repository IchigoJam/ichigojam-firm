// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#include <Arduino.h>
//#include <Watchdog.h>

void system_init();
void enterDeepSleep(int waitsec);
INLINE void deepPowerDown();
INLINE void IJB_sleep();
S_INLINE void IJB_reset();

int getSleepFlag() {
	return IJB_btn(0);
}
void system_init() {
	_g.screen_big = 0;
	_g.screen_invert = 0;
	_g.lastfile = 0;
	_g.screen_insertmode = 1;
	noresmode = 0;
	key_flg.insert = 0;
}
void enterDeepSleep(int waitsec) {
}
INLINE void deepPowerDown() {
}
INLINE void IJB_sleep() {
}

void ichigojam_main(void);


S_INLINE void IJB_reset() {
	//void (*reset)(void) = NULL; //declare reset function at address 0
	//reset();

	// todo: use Watchdog
	//Watchdog.start(1);
}
int IJB_wait(int n, int active) { // if stop ret 1
	video_waitSync(1);
	int offset = 2;
	//delay(n * 1000 / 60);
	for (int i = offset; i < n; i++) {
		delay(16);
		if (stopExecute()) {
			return 1;
		}
	}
	return 0;
}

int main() {
	ichigojam_main();
	return 0;
}

// debug

void putn(int n) {
	char buf[100];
	int m = 1000;
	for (int i = 0; i < 4; i++) {
		int a = n / m % 10;
		buf[i] = a + '0';
		n -= m * a;
		m /= 10;
	}
	buf[4] = '\n';
	buf[5] = 0;
	Serial.write(buf, 5);
}

// for Arduino
extern "C" void setup(void) {
	main();
}
extern "C" void loop(void) {
}

