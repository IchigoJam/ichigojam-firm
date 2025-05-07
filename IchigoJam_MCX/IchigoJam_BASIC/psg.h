// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __PSG_H__
#define __PSG_H__

// playMML

// 定義必要
#ifndef PSG_TICK_FREQ
//#define PSG_TICK_FREQ (261 * 60) // ver1.2b23 - for LPC1114
#define PSG_TICK_FREQ 60 // for every 1 frame
#endif
#ifndef PSG_TICK_PER_SEC
#define PSG_TICK_PER_SEC 60
#endif
// #define PSG_MML_MAXLEN 128 // ver 1.2 削除
//#define PSG_DEFAULT_OCT 4 // ver 1.1
#define PSG_DEFAULT_OCT 3 // ver 1.2
#define PSG_DEFAULT_LEN 8 // == 4分音符
#define PSG_DEFAULT_TEMPO 120

//INLINE void psg_tick0();
S_INLINE void psg_tick();
S_INLINE void psg_playMML(char* mml);
S_INLINE void psg_beep(int tone, int len);
S_INLINE void psg_tempo(int tempo);

// sound.h
S_INLINE void sound_tick();
static void sound_switch(int on);
static void sound_init();

// playMML

#ifdef PSG_TRUE_TONE

#ifndef PSG_TRUE_TONE_UNIT
#define PSG_TRUE_TONE_UNIT 381
#endif

S_INLINE void sound_tone(int t);

/* // 何向け??
uint32 TONE_O0[] = { // 12*4 = 48byte
	91734,
	86586,
	81726,
	77139,
	72809,
	68723,
	64866,
	61225,
	57789,
	54545,
	51484,
	48594,
};
*/
uint16 TONE_O0[] = { // IchigoJam R 96MHz O0C-
	52420,
	49477,
	46700,
	44079,
	41605,
	39270,
	37066,
	34986,
	33022,
	31169,
	29419,
	27768,
};

#define TONE_LEN (12 * 9) // O1-O9

#else

//#define TONE11
#define TONE12 // 1.2 .. 入らない 1.2b54 復活 -> 1.3.2b22 こちらベースでO6対応
//#define TONE12_2 // 1.2b52 音痴版
//#define TONE12_3 // 1.3.2b16 16bit版 実験
//#define TONE_OFF

#ifdef TONE11
#define TONE_LEN (12 * 4 + 3) // O1C - O5D // ver 1.1
const uint8 TONE[] = {
	239, 226, 213, 201, 190, 179, 169, 160, 151, 142, 134, 127, 120, 113, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 48, 45, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 // ver 1.1
};
#endif

#ifdef TONE12
//#define TONE_LEN (12 * 5) // O1C - O5B // ver 1.2? ただし O5の+は効かない // 60byte
#define TONE_LEN (12 * 6) // O1C - O6B // ver 1.3.2b22 ただし O5の+は効かない O5/O6はVIDEO0時推奨

#if TV_MODE == TV_MODE_NTSC
const uint8 TONE[] = {
//	240, 227, 214, 202, 191, 180, 170, 160, 151, 143, 135, 127, 120, 113, 107, 101, 95, 90, 85, 80, 76, 71, 67, 64, 60, 57, 54, 51, 48, 45, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8 // ver 1.2 -> ver1.2b44
//	239, 226, 213, 201, 190, 179, 169, 160, 151, 142, 134, 127, 120, 113, 107, 101, 95, 90, 85, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 45, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8 // ver 1.2b23
//	240, 226, 213, 201, 190, 179, 169, 160, 151, 142, 134, 127, 120, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 44, 42, 40, 37, 35, 33, 31, 30, 28, 26, 25, 23, 22, 21, 20, 18, 17, 16, 15, 15, 14, 13, 12, 11, 11, 10, 10, 9, 8, 8, 7, // ver 1.3.2b16 -- o5でかぶる
//	240, 227, 214, 202, 191, 180, 170, 160, 151, 143, 135, 127, 120, 113, 107, 101, 95, 90, 85, 80, 76, 71, 67, 64, 60, 57, 53, 50, 48, 45, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8, // ver 1.3.2b16 ronud
#ifdef XTAL_358MHz
  241, 227, 214, 202, 191, 180, 170, 161, 152, 143, 135, 127, 120, 114, 107, 101, 95, 90, 85, 80, 76, 72, 68, 64, 60, 57, 54, 51, 48, 45, 43, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 23, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8 // 1.4b17 for 3.58MHz
#else
	240, 227, 214, 202, 191, 180, 170, 160, 151, 143, 135, 127, 120, 113, 107, 101, 95, 90, 85, 80, 76, 71, 67, 64, 60, 57, 53, 50, 48, 45, 43, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24, 23, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8, // ver 1.4.2 VSYNC 3056->3052
#endif
};
#elif TV_MODE == TV_MODE_PAL
const uint8 TONE[] = { // O1C - O5B
	239, 225, 213, 201, 189, 179, 169, 159, 150, 142, 134, 126, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 45, 42, 40, 38, 35, 33, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8
};
#endif

#endif

#ifdef TONE12_2
// -48byte でもオクターブ計算で増えて、結局-16byte、結構高音域でずれるのでNG
#define TONE_LEN (12 * 5) // O1C - O5B // ver 1.2? ただし O4の+は効かない

#if TV_MODE == TV_MODE_NTSC
const uint8 TONE[] = {
	//239, 226, 213, 201, 190, 179, 169, 160, 151, 142, 134, 127
	240, 227, 214, 202, 191, 180, 170, 160, 151, 143, 135, 127, // 1.3.2b16

//	120, 113, 107, 101,  95,  90,  85,  80,  75,  71,  67,  63,
//	 60,  56,  53,  50,  47,  45,  42,  40,  38,  36,  34,  32,
//   30,  28,  27,  25,  24,  22,  21,  20,  19,  18,  17,  16,
//   15,  14,  13,  13,  12,  11,  11,  10,   9,   9,   8,   8 // ver 1.2b23
};
#elif TV_MODE == TV_MODE_PAL
const uint8 TONE[] = { // O1C - O5B
	239, 225, 213, 201, 189, 179, 169, 159, 150, 142, 134, 126, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 45, 42, 40, 38, 35, 33, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8
};
#endif

#endif

#ifdef TONE12_3
#define TONE_LEN (12 * 6) // O1C - O6B ただし O5の+は効かない (O5以上はVIDEO0推奨）
const uint16 TONE[] = {
	61476, 58026, 54769, 51695, 48794, 46055, 43470, 41031, 38728, 36554, 34502, 32566, 
};
#endif

// PSG_TRUE_TONE
#endif 

#ifdef TONE_OFF
#define TONE_LEN 0

#if TV_MODE == TV_MODE_NTSC
const uint8 TONE[] = {
};
#elif TV_MODE == TV_MODE_PAL
const uint8 TONE[] = { // O1C - O5B
	239, 225, 213, 201, 189, 179, 169, 159, 150, 142, 134, 126, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60, 56, 53, 50, 47, 45, 42, 40, 38, 35, 33, 32, 30, 28, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 11, 10, 9, 9, 8, 8
};
#endif

#endif

static uint32 psg_parseInt(char** s) { // ver 1.1まで+-入ってた! nullチェック入ってなかった
	if (!**s) // ver 1.2 add
		return 0;
	while (**s == ' ')
		(*s)++;
	uint32 a = 0;
	while (**s >= '0' && **s <= '9') {
		a = a * 10 + (**s - '0');
		(*s)++;
	}
	return a;
}

//#define psg_toupper _toupper // 使いまわしても変わらない
S_INLINE uint8 psg_toupper(uint8 c) {
	if (c >= 'a' && c <= 'z') {
		return c & 0b1011111; // 速度一緒 なぜか小文字のほうが速い
//		return c + ('A' - 'a');
	}
	return c;
}

// C+D+EF+G+A+B
S_INLINE void psg_tick() {
	// -> sound_tick() or here
	if (_g.psgtone) {
		_g.psgwaitcnt++;
		if (_g.psgwaitcnt == _g.psgtone) {
			_g.psg_sounder = ~_g.psg_sounder;
			_g.psgwaitcnt = 0;
		}
	}
	
	if (_g.psglen) {
//		if (_g.psglen--) {
		if (--_g.psglen) { // 1.2b53
			return;
		}
		if (!_g.psgmml) {
			sound_switch(0);
			_g.psgtone = 0;
		}
	}
	if (!_g.psgmml) {
//		sound_off(); // 1.2b53
//		_g.psgtone = 0;
		return;
	}
	// set next tone
	int flg = 0;
	for (;;) {
		int t = -2;
		int32 s = 0;
		int c = psg_toupper(*_g.psgmml++);
		if (c == '<') { // ver 1.2 から逆
			_g.psgoct++;
			continue;
		} else if (c == '>') { // ver 1.2 から逆
			_g.psgoct--;
			continue;
		} else if (c == 'O') {
			char c2 = *_g.psgmml++;
			if (!c2) {
				_g.psgmml--;
				continue;
			}
			_g.psgoct = c2 - '0';
			continue;
		} else if (c == ' ') {
			continue;
		} else if (c == 'L') {
			uint32 l = psg_parseInt(&_g.psgmml);
			if (l) {
				_g.psgdeflen = 32 / l;
				//_g.psgdeflen = calcDivU(32, l);
				int c3 = *_g.psgmml++;
				if (c3 == '.') {
					//_g.psgdeflen += _g.psgdeflen / 2;
					_g.psgdeflen += _g.psgdeflen >> 1;
				} else
					_g.psgmml--;
			}
			continue;
		} else if (c == 'T') {
			_g.psgtempo = psg_parseInt(&_g.psgmml);
			continue;
		} else if (c == 'N') {
			s = psg_parseInt(&_g.psgmml);
			if (s > 255)
				s = 255;
		} else if (c == 'C') {
			t = 0;
		} else if (c == 'D') {
			t = 2;
		} else if (c == 'E') {
			t = 4;
		} else if (c == 'F') {
			t = 5;
		} else if (c == 'G') {
			t = 7;
		} else if (c == 'A') {
			t = 9;
		} else if (c == 'B') {
			t = 11;
		} else if (c == 'R') {
			t = -2;
		} else if (c == ' ') {
			continue;
		} else if (c == '$') {
			if (*_g.psgmml)
				_g.psgrep = _g.psgmml;
			continue;
		} else { // その他の記号で停止、$があればリピート
			if (_g.psgrep && !flg) {
				_g.psgmml = _g.psgrep;
				flg = 1;
				continue;
			}
			_g.psgmml = NULL;
			_g.psgtone = _g.psglen = 0;
			sound_switch(0);
			return;
		}
		int c2 = *_g.psgmml++;
		if (c2 == '+' || c2 == '#') {
			if (t != 11)
				t++;
		} else if (c2 == '-') {
			if (t)
				t--;
		} else
			_g.psgmml--;
		uint32 len = psg_parseInt(&_g.psgmml);
		if (len) {
			len = 32 / len;
			//len = calcDivU(32, len);
		} else {
			len = _g.psgdeflen;
		}
		int c4 = *_g.psgmml++;
		if (c4 == '.') {
			len += len >> 1;
		} else
			_g.psgmml--;
		if (t > -2)
			t += ((_g.psgoct - 1) * 12);
		
#ifdef PSG_TRUE_TONE
		if (!s) {
			if (t >= 0 && t < TONE_LEN) {
				//t = TONE_O0[calcMod(t, 12)] >> calcDiv(t, 12);
				//t = TONE_O0[t % 12] >> (t / 12 + 1);
				t = TONE_O0[calcMod(t, 12)] >> (calcDivU(t, 12) + 1); // for LPC1114
				sound_tone(t);
			} else if (t == -2) {
				sound_tone(0);
			}
		} else {
			sound_tone(s * PSG_TRUE_TONE_UNIT);
		}
#else
		if (!s) {
			if (t >= 0 && t < TONE_LEN) {
	#ifdef TONE12
				if (_g.psgratio == 1) {
					if (t < TONE_LEN - 12)
						s = TONE[t];
					else
						s = TONE[t - 12] / 2;
				} else {
					t -= 24;
					if (t >= 0)
						s = TONE[t];
					else if (t >= -12)
						s = TONE[t + 12] * 2;
					else
						s = TONE[t + 24] * 4;
				}
	#endif
	#ifdef TONE12_2
//				うまくいってない
//				uint32 t2 = t;
//				s = TONE[t2 % 12] >> (t2 / 12); // 1.2b51 ... 結構大きい 16byte縮むけど、音程ずれる
//				s = TONE[t % 12] >> (t / 12); // intの割り算が入って容量増える
				s = TONE[calcMod(t, 12)] >> calcDiv(t, 12); // 1.2b52 ... だいぶ縮んだ！ ... けど音程ずれるからNG
	#endif
	#ifdef TONE12_3
				s = TONE[calcMod(t, 12)] >> ((7 - (_g.psgratio >> 1)) + calcDiv(t, 12)); // 1.2b52 ... だいぶ縮んだ！ ... けど音程ずれるからNG
	#endif
			}
		}
#endif
		
		_g.psgtone = s;
		_g.psgwaitcnt = s - 1;
		//_g.psglen = calcDivU(len * ((60 * PSG_TICK_FREQ) >> 3), _g.psgtempo); // 1.3.2b12
		_g.psglen = calcDivU(len * ((60 * PSG_TICK_FREQ) >> 3) * _g.psgratio, _g.psgtempo); // 1.3.2b16
		//_g.psglen = len * ((60 * PSG_TICK_FREQ) / 8) / _g.psgtempo; // 1.2b35
//		_g.psglen = len * ((60 * 60 * VIEO_LINES) / (8 * _g.psgtempo)); // PSG_STDLEN;
//		_g.psglen = len * 117333 / _g.psgtempo; // ver 1.2b23 // VIDEO_LINES*60*59.94=117332.549
//#define PSG_STDLEN 500 // len of L32 , def stdlen 4 = (8 * 2, 16 * 4, 32 * 8), 1min 60sec = 60 * 261tick = 8 * 120, 60 * PSG_TICK_FREQ / (8 * 120)
		break;
	}
}

S_INLINE void psg_playMML(char* mml) {
	sound_switch(mml != NULL);
	
	_g.psgmml = mml;
	_g.psgoct = PSG_DEFAULT_OCT;
	_g.psgdeflen = PSG_DEFAULT_LEN;
	_g.psgtempo = PSG_DEFAULT_TEMPO;
	_g.psgtone = 0;
	_g.psglen = 0;
	_g.psgrep = NULL;
	/*
	if (!mml)
		return;
	char* chkrep = mml;
	for (;;) {
		char c = *chkrep++;
		if (c == 0) {
			break;
		} else if (c == '$') {
			if (*chkrep)
				_g.psgrep = chkrep;
			break;
		}
	}
	*/
}

S_INLINE void psg_beep(int tone, int len) {
	_g.psglen = len * (PSG_TICK_FREQ / PSG_TICK_PER_SEC) * _g.psgratio; // v1.2b44
	_g.psgtone = tone * _g.psgratio;
	_g.psgwaitcnt = _g.psgtone - 1;
	_g.psgmml = NULL;
#ifdef PSG_TRUE_TONE
	sound_tone(tone * PSG_TRUE_TONE_UNIT);
#endif
	sound_switch(tone);
}

S_INLINE void psg_tempo(int tempo) {
	_g.psgtempo = tempo;
}
S_INLINE int psg_sound() {
	return _g.psgtone || _g.psgmml;
}

// __PSG_H__
#endif
