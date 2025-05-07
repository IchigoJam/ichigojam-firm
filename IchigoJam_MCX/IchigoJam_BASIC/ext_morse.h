// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#define ASCIIMORSE_LEN 63
#define ASCIIMORSE_MIN 33
#define ASCIIMORSE_MAX 95
static const int ASCIIMORSE[] = { 0x313133, 0x131131, 0x313111, 0x1113113, 0x13313, 0x13111, 0x13331, 0x31331, 0x313313, 0x131113, 0x13131, 0x331131, 0x311113, 0x131313, 0x31131, 0x33333, 0x13333, 0x11333, 0x11133, 0x11113, 0x11111, 0x31111, 0x33111, 0x33311, 0x33331, 0x333111, 0x313131, 0x1311111, 0x31113, 0x3313131, 0x113311, 0x133131, 0x13, 0x3111, 0x3131, 0x311, 0x1, 0x1131, 0x331, 0x1111, 0x11, 0x1333, 0x313, 0x1311, 0x33, 0x31, 0x333, 0x1331, 0x3313, 0x131, 0x111, 0x3, 0x113, 0x1113, 0x133, 0x3113, 0x3133, 0x3311, 0x1113111, 0x3111111, 0x1113131, 0x13113, 0x113313 };

int morse_encode(char c) {
    if (c >= 'a' && c <= 'z') {
        c = c - ('a' - 'A');
    }
    if (c >= ASCIIMORSE_MIN && c <= ASCIIMORSE_MAX) {
        return ASCIIMORSE[c - ASCIIMORSE_MIN];
    }
    return 0;
}

void morse_beep(int n);
void morse_wait(int n);

void morse_out(char c) {
    if (c == ' ') {
        return;
    }
    if (c == '\n') {
        morse_wait(4);
    }
    // add for IchigoJam
    if (c >= 28 && c <= 31 && _g.cursory >= 0) {
        int now = _g.cursory * SCREEN_W + _g.cursorx;
        c = vram[now];
    }
    //
    int m = morse_encode(c);
    if (!m) {
        return;
    }
    for (int i = 7; i >= 0; i--) {
        int l = (m >> (i * 4)) & 0xf;
        if (l) {
            morse_beep(l);
            morse_wait(l + 1);
        }
    }
    morse_wait(2);
}

#define MORSE_DT 2
#define MORSE_TONE 20

void morse_beep(int n) { // async
    psg_beep(MORSE_TONE, n * MORSE_DT);
    //printf("morse_beep %d\n", n);
}
void morse_wait(int n) { // sync
    IJB_wait((n + 1) * MORSE_DT, 1);
    //printf("morse_wait %d\n", n);
}
