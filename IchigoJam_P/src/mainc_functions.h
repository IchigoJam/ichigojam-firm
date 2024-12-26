//IchigoJamのmain.cの中にあった関数群
#include "all_includes.h"

INLINE int IJB_peek(int ad) {
    if (ad < 0) {
        return 0;
    } else if (ad < OFFSET_RAMROM) {
        return CHAR_PATTERN[ad];
    } else if (ad < OFFSET_RAMROM + SIZE_RAM) {
        return ram[ad - OFFSET_RAMROM];
    }
    return 0;
}

INLINE void IJB_poke(int ad, int n) {
    ad -= OFFSET_RAMROM;
    if (ad >= 0 && ad < SIZE_RAM) {
        ram[ad] = n;
        //		_printf("poke %x, %d\n", ad, n);
    }
}

int IJB_random(int n) {
    uint r = rnd();
    if (n <= 0) {
        return 0;
    }
    return (r >> 1) % n; // マイナスがでてしまう対処、udivが使われる div?? こっちの方が小さい
}

INLINE void IJB_random_seed(int n) {
    random_seed(n);
}

STATIC void IJB_input(char** line) {
    screen_showCursor(1);
    key_flg.insert = key_flg.bkinsert;
    int8 ox = _g.cursorx;
    int8 oy = _g.cursory;
    for (;;) {
        if (stopExecute()) {
            break;
        }
        int key = key_getKey();
        if (key == '\n') {
            *line = (char*)(vram + ox + SCREEN_W * oy);
            break;
        } else if (key >= 0 && key != 27 && key != 30 && key != 31 && !((key == '\b' || key == 28) && _g.cursorx == ox)) {
            _g.screen_insertmode = key_flg.insert;

            if (_g.uartmode_txd & 4) { // 1.2b62 
                put_chr(key);
            } else {
                screen_putc(key); // 1.2b61まで
            }
        }
    }
    key_flg.bkinsert = key_flg.insert;
    key_flg.insert = 1;
    _g.screen_insertmode = 1; // update
    screen_showCursor(0);
}

inline void IJB_bps(int uart, int i2c) {
    uart_bps(uart);
#ifdef ENABLE_I2C_BPS
    i2c0_bps(i2c);
#endif
}