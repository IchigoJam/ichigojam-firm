#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include "all_includes.h"

#define KEY_BUF_LEN (SIZE_RAM_KEYBUF - 2)
#define MODE_US 0
#define MODE_JA 1

#define DEFAULT_UARTMODE_TXD 2	// txd 0:disable, 1:only text, 2:with ctrl, +4:echo back mode, +8:画面表示オフ(PRINTやSyntax errorなどのメッセージがオフになり、キーボード入力は表示されたまま)
#define DEFAULT_UARTMODE_RXD 1	// rxd 0:disable, 1:enable, 2:ignore esc mode, 4:CR mode, 6:ignore esc & CR mode (auto comment mode??) // -> keyboard_ps2.h
#define UART_ID uart0
#define UART_IRQ UART0_IRQ
#define UART_DEFAULT_BPS 115200

volatile static char* keybuf = (char*)(ram + (OFFSET_RAM_KEYBUF + 1));
struct keyflg_def key_flg;
extern uint8_t keycode2ascii[128][4];
static int uart_baudrate = UART_DEFAULT_BPS;
static uint8_t tflash[FLASH_SECTOR_SIZE];//フラッシュの中身の一部を書き換えてから書き戻すための一時変数、トップレベルで宣言しておかないと画面の描画が止まる？

static inline uint key_getKeyboardID() {
    uint mode = get_flash(get_config_offset())[0];
    return mode == MODE_JA;
}

void set_keymap(uint mode) {
    if (mode == MODE_JA) {
        memcpy(keycode2ascii, keycode_to_ascii_ja, sizeof(keycode2ascii));
    } else {//MODE_US or KBDコマンド未使用時
        memcpy(keycode2ascii, keycode_to_ascii_us, sizeof(keycode2ascii));
    }
}

INLINE void IJB_kbd(uint mode) {
    mode = !!mode;//現状JAかUSなので、0以外は全てJAとみなす
    set_keymap(mode);
    uint32_t offset = get_config_offset();
    memcpy(tflash, get_flash(offset), FLASH_SECTOR_SIZE);
    tflash[0] = mode;
    video_off(0);
    int save = save_and_disable_interrupts();
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
    flash_range_program(offset, tflash, FLASH_SECTOR_SIZE);
    video_on();
    restore_interrupts(save);
}

INLINE void uart_init_IJ() {
    // #if DEFAULT_UARTMODE_TXD != 0
    _g.uartmode_txd = DEFAULT_UARTMODE_TXD; // 最初のクリア信号を送らない
    // #endif
    _g.uartmode_rxd = DEFAULT_UARTMODE_RXD;
}

INLINE void IJB_uart(int16 txd, int16 rxd) {
    _g.uartmode_txd = txd;
    _g.uartmode_rxd = rxd;
}

static inline void uart_putc(char c) {
    if ((_g.uartmode_txd & 3) == 3) { // 1.3b2
        if (c == '\n') {
            uart_putc('\r');
        }
    }
    uart_putc_raw(UART_ID, c);
}

STATIC void put_chr(char c) {
    if (_g.uartmode_txd & 3) { // 1.3b2
        uart_putc(c);
    }
    if (!(_g.uartmode_txd & 8)) { // 1.3b11
        screen_putc(c);
    }
}

void key_pushc(char c) {
    if (*keybuf < KEY_BUF_LEN) {
        (*keybuf)++;
        keybuf[(uint8)*keybuf] = c;
    }
}

void key_push(char* s) { // for function keys
    for (;;) {
        char c = *s++;
        if (!c)
            break;
        key_pushc(c);
    }
}

int key_getKey() {
    if (!*keybuf) {
        return -1; // 1.2b19
    }
    int res = keybuf[1];
    (*keybuf)--;
    for (uint8 i = 0; i < *keybuf; i++)
        keybuf[i + 1] = keybuf[i + 2];
    return res;
}

void key_clearKey() {
    *keybuf = 0;
}

void set_uart_bps() {
    uart_set_baudrate(UART_ID, uart_baudrate);
}

void uart_bps(int n) { // 0:115200, -1:57600, -2:38400
    if (n == 0) {
        n = 115200;
    } else if (n == -1) {
        n = 57600;
    } else if (n == -2) {
        n = 38400;
    } else if (n <= -100) {
        n *= -100;
    }
    uart_baudrate = n;
    set_uart_bps();
}

INLINE int stopExecute() {
    return _g.key_flg_esc;
}

#endif