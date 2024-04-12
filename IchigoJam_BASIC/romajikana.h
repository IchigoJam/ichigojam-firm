// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef __ROMAJIKANA_H__
#define __ROMAJIKANA_H__

// romaji-kana

//uint8 key_kana_buf0;
//uint8 key_kana_buf1; // xtsu/ltsu など4文字に対応する場合増やす必要あり
#ifndef key_kana_buf0
#define key_kana_buf0 _g.key_kana_buf_0 // 容量増
#endif
#ifndef key_kana_buf1
#define key_kana_buf1 _g.key_kana_buf_1
#endif

S_INLINE int romajikana_input(int k) {
	if (k >= 'a' && k <= 'z') {
		k += 'A' - 'a';
	}
	int m = -1;
	if (k == 'A') { // switch文にすると容量増
		m = 0;
	} else if (k == 'I') {
		m = 1;
	} else if (k == 'U') {
		m = 2;
	} else if (k == 'E') {
		m = 3;
	} else if (k == 'O') {
		m = 4;
	} else if (k == '.') {
		k = 0xa1;
	} else if (k == ',') {
		k = 0xa4;
	} else if (k == '-') {
		k = 0xb0;
	} else if (k == '[') {
		k = 0xa2;
	} else if (k == ']') {
		k = 0xa3;
	} else if (k == '/') {
		k = 0xa5;
	} else if (k == '\\') {
		k = 0xa0; // yen mark
	}

	int a2z = k >= 'A' && k <= 'Z';

	if (!key_kana_buf0) {
		if (m >= 0) {
			k = 0xb1 + m; // アイウエオ
		} else if (a2z) {
			key_kana_buf0 = k;
			key_kana_buf1 = 0;
		}
	} else if (!key_kana_buf1) {
		if (m >= 0) {
			key_pushc(8); // back space
			switch (key_kana_buf0) { // if文にすると容量増
			  case 'K':
			  case 'C': // 1.3b2
					k = 0xb6 + m;
					break;
			  case 'S':
					k = 0xbb + m;
					break;
			  case 'T':
					k = 0xc0 + m;
					break;
			  case 'N':
					k = 0xc5 + m;
					break;
			  case 'H':
					k = 0xca + m;
					break;
			  case 'M':
					k = 0xcf + m;
					break;
			  case 'Y':
					if (!(m & 1)) {
						//k = 0xd4 + m / 2;
						k = 0xd4 + (m >> 1);
					} else {
						k = 0xb1 + m;	// _イ_エ_
					}
					break;
			  case 'R':
					k = 0xd7 + m;
					break;
			  case 'W':
					if (m == 0) {
						k = 0xdc;
					} else if (m == 2) {
						k = 0xb3;
					} else if (m == 4) {
						k = 0xa6;
					} else {	// _ウィ_ウェ_
						key_pushc(0xb3);
						k = 0xa7 + m;
					}
					break;
			  case 'L':
			  case 'X':
					k = 0xa7 + m;
					break;
			  case 'G':
					key_pushc(0xb6 + m);
					k = 0xde;
					break;
			  case 'Z':
					key_pushc(0xbb + m);
					k = 0xde;
					break;
			  case 'J': // じゃじじゅじぇじょ
					key_pushc(0xbc);
					key_pushc(0xde);
					if (m == 1) {
						k = 0;
					} else if (m == 3) {
						k = 0xaa;
					} else {
						//k = 0xac + m / 2;
						k = 0xac + (m >> 1);
					}
					break;
			  case 'F': // ふぁふぃふふぇふぉ
					key_pushc(0xcc);
					if (m == 2) {
						k = 0;
					} else {
						k = 0xa7 + m;
					}
					break;
			  case 'V': // ヴぁヴぃヴヴぇヴぉ
					key_pushc(0xb3);
					key_pushc(0xde); // 濁点 1.2.1
					if (m == 2) {
						k = 0;
					} else {
						k = 0xa7 + m;
					}
					break;
			  case 'D':
					key_pushc(0xc0 + m);
					k = 0xde;
					break;
			  case 'B':
					key_pushc(0xca + m);
					k = 0xde;
					break;
			  case 'P':
					key_pushc(0xca + m);
					k = 0xdf;
					break;
			  case 'Q':
					key_pushc(0xb8);
					k = 0xa7 + m;
					break;
			}
			key_kana_buf0 = 0;
		} else if (k == key_kana_buf0) {
			key_pushc(8);
			if (k == 'N') {
				k = 0xdd; // ン
				key_kana_buf0 = 0;
			} else {
				key_pushc(0xaf); // ッ
			}
		} else {
			if (key_kana_buf0 == 'N' && k != 'Y') {
				key_pushc(8);
				key_pushc(0xdd); // ン
				if (a2z) {
					key_kana_buf0 = k;
				} else {
					key_kana_buf0 = 0;
					if (k == 0x27) { // '
						k = 0;
					}
				}
			} else if (a2z) {
				key_kana_buf1 = k;
			} else {
				key_kana_buf0 = 0;
			}
		}
	} else {
		if (m >= 0) {
			key_pushc(8);
			key_pushc(8);
			if ((key_kana_buf0 == 'C' || key_kana_buf0 == 'S') && key_kana_buf1 == 'H') { // ちゃちちゅちぇちょ、しゃししゅしぇしょ(1.2.1)
				key_pushc(key_kana_buf0 == 'C' ? 0xc1 : 0xbc);
				if (m == 1) {
					k = 0;
				} else if (m == 3) {
					k = 0xaa;
				} else {
					//k = 0xac + m / 2;
					k = 0xac + (m >> 1);
				}
			} else if (key_kana_buf0 == 'T' && key_kana_buf1 == 'S' && m == 2) {
				k = 0xc2;
			} else if ((key_kana_buf0 == 'L' || key_kana_buf0 == 'X') && key_kana_buf1 == 'T' && m == 2) { // ltu xtu っ
				k = 0xaf;
			} else if ((key_kana_buf0 == 'T' || key_kana_buf0 == 'D') && key_kana_buf1 == 'H') {
				key_pushc(0xc3);
				if (key_kana_buf0 == 'D')
					key_pushc(0xde); // 濁点
				if (!(m & 1)) {
					//k = 0xac + m / 2;
					k = 0xac + (m >> 1);
				} else {
					k = 0xa7 + m;
				}
			} else if (key_kana_buf1 == 'Y') {
				switch (key_kana_buf0) { // if文にすると容量増
				  case 'K': // きゃきゅきょ
				  case 'C': // きゃきゅきょ 1.3b2
					key_pushc(0xb6 + 1);
					break;
				  case 'S':
					key_pushc(0xbb + 1);
					break;
				  case 'T':
					key_pushc(0xc0 + 1);
					break;
				  case 'N':
					key_pushc(0xc5 + 1);
					break;
				  case 'H':
					key_pushc(0xca + 1);
					break;
				  case 'F': // 1.2.1 ふゃふゅふょ
					key_pushc(0xca + 2);
					break;
				  case 'J': // 1.2.1 じゃじゅじょ
					key_pushc(0xbc);
					key_pushc(0xde);
					break;
				  case 'M':
					key_pushc(0xcf + 1);
					break;
				  case 'R':
					key_pushc(0xd7 + 1);
					break;
				  case 'G':
					key_pushc(0xb6 + 1);
					key_pushc(0xde);
					break;
				  case 'Z':
					key_pushc(0xbb + 1);
					key_pushc(0xde);
					break;
				  case 'D':
					key_pushc(0xc0 + 1);
					key_pushc(0xde);
					break;
				  case 'B':
					key_pushc(0xca + 1);
					key_pushc(0xde);
					break;
				  case 'P':
					key_pushc(0xca + 1);
					key_pushc(0xdf);
					break;
				}
				if (!(m & 1)) {
					//k = 0xac + m / 2;
					k = 0xac + (m >> 1);
				} else {
					k = 0xa7 + m;
				}
			} else {
				k = 0;
			}
			key_kana_buf0 = key_kana_buf1 = 0;
			/*
		} else if (k == key_kana_buf1) {
			key_pushc(8);
			if (k == 'N') {
				k = 0xdd; // ン
				key_kana_buf0 = 0;
			} else {
				key_pushc(0xaf); // ッ
				key_kana_buf0 = key_kana_buf1;
				key_kana_buf1 = 0;
			}
			*/
		} else if (a2z) {
			key_kana_buf0 = key_kana_buf1;
			key_kana_buf1 = k;
		} else if (k == 8) {
			key_kana_buf1 = 0;
		} else {
			key_kana_buf0 = 0;
		}
	}
	return k;
}

#endif	// __ROMAJIKANA_H__