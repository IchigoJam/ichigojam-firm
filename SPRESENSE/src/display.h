#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
//#define TFT_CS -1
#define TFT_CS 10
#define TFT_RST 8

Adafruit_ILI9341 tft = Adafruit_ILI9341(&SPI, TFT_DC, TFT_CS, TFT_RST);

uint8 initflg = 0;

void tft_init() {
	if (initflg) {
		return;
	}
  tft.begin(40000000); // 40MHz
  //tft.begin(80000000); // 80MHz 化ける
 
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  //Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);
  tft.setRotation(3);
	tft.fillScreen(ILI9341_BLACK);
	initflg = 1;
}

uint16_t linebuf[256 * 8];

void show() {
	/*
	// test
	tft.setAddrWindow(100, 100, 8, 8);
	uint16_t colors[8 * 8];
	for (int i = 0; i < 8 * 8; i++) {
		colors[i] = (i * 8) << 8;
	}
	tft.writePixels(colors, 8 * 8, true, false);
	*/
	//tft.drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h);
	//uint16_t tft.color565(uint8_t r, uint8_t g, uint8_t b);
  //tft.setRotation(uint8_t r);
  
	/*
	// first version
	tft.fillScreen(ILI9341_BLACK);
	int x = (320 - 256) / 2;
	int y = (240 - 8 * 24) / 2;
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
	for (int i = 0; i < SCREEN_H; i++) {
		uint8* c = vram + i * SCREEN_W;
	  tft.setCursor(x, y);
		tft.println((const char*)c);
		y += 8;
	}
	*/
	int x = (320 - 256) / 2;
	int y = (240 - 8 * 24) / 2;

	const uint8* crom = CHAR_PATTERN;
	const uint8* crom2 = screen_pcg - ((0x100 - SIZE_PCG) * 8);

	uint8 invert = _g.screen_invert ? 0xff : 0; // invert or not

	tft.setAddrWindow(x, y, 256, 192);
	uint16_t white = ILI9341_WHITE;
	uint16_t black = ILI9341_BLACK;
	uint8* c = vram;
	for (int i = 0; i < SCREEN_H; i++) {
		for (int j = 0; j < SCREEN_W; j++) {
			int cflg = (frames & 0x10) && _g.cursorflg && j == _g.cursorx;
			uint8 cptn = invert;
			if (cflg && i == _g.cursory) {
				if (!key_flg.insert) {
					cptn ^= 0xf0;
				} else {
					cptn ^= 0xff;
				}
			}

			const uint8* cr = crom;
			if (*c >= 0x100 - SIZE_PCG) {
				cr = crom2;
			}
			cr += *c << 3;
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					linebuf[j * 8 + 256 * k + l] = (((*cr ^ cptn) >> (7 - l)) & 1) ? white : black;
				}
				cr++;
			}
			c++;
		}
		tft.writePixels(linebuf, 256 * 8);
	}
	frames++;
}

void video_on() {
	SCREEN_W = 32;
	SCREEN_H = 24;
	screen_clear();
	tft_init();
}
inline void video_off(int clkdiv) {
}
inline int video_active() {
	return 0;
}

inline void IJB_lcd(uint mode) {
}

inline void video_waitSync(uint n) {
	show();
	//delay(n * 1000 / 60); // 16.6
	//delay(n * 2); // 16 -> 2 // 調整
}
