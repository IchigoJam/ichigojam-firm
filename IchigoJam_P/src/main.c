#define IJB_PICO_NO_PRINT
#define IJB_BUILD 0
#define IJB_VER 161
#include "all_includes.h"

STATIC void exec(char* s) {
    key_flg.bkinsert = key_flg.insert;
    key_flg.insert = 1;

    _g.screen_insertmode = 1;
    _g.key_flg_esc = 0;
    screen_showCursor(0);
    int res = basic_execute(s);
    if (_g.cursory == -1)
        _g.cursory = 0;
    if (res == BASIC_RESULT_EXECUTE && !noresmode) {// stop, exec, edit, err
        put_str("OK\n");
    }
#ifndef IJB_USE_EXCEPTION // 例外使うときには使わないコード
    if (res == BASIC_RESULT_STOP_OR_ERR) { // 1.3b4 エラー停止の時だけ、キークリア
        key_clearKey();
    }
#endif

    key_flg.insert = key_flg.bkinsert;
}

int main() {
    pico_init();
    picodvi_init();
    ichigojam_init();

    char* linebuf = (char*)ram + OFFSET_RAM_LINEBUF;
    if (*linebuf) {
        if (_g.cursory == -1) { // 1.4.1 前にもってくる
            _g.cursory = 0;
        }
        if (!noresmode) {
            if (!_g.err) {
                _g.err = ERR_COMPLEX_EXPRESSION;
            }
            basic_printError();
        }
        // from exec
        key_clearKey(); // 1.3b4 エラー停止の時だけ、キークリア

        key_flg.insert = key_flg.bkinsert;
    }
    while (1) {
        if (_g.sleepflg) {
            _g.sleepflg = 0;
            //自動起動時は2秒待ってから、ESCが押されていなかったらプログラムを実行する
            //2秒待つのはロボットに差したときなどにいきなり動かないためと、USBキーボードの接続をするため
            //ESCで実行しないのはARUNとSLEEPを組み合わせると一生操作できない状態になるので、その対策
            sleep_ms(2000);
            int ch = key_getKey();
            if (ch == ESC) {
                psg_beep(10, 3);
                put_str("Break\n");
            } else {
                *linebuf = 1;
                exec("LRUN");
            }
        }
        screen_showCursor(1);
        IJB_random(1);
        int ch = key_getKey();
        if (ch <= 0) {
            continue;
        }
        if (_g.uartmode_txd & 4) { // 1.2b62 UART echo back
            uart_putc(ch); // 1.3b2
        }
        if (ch == ESC) {
            continue;
        }
        _g.screen_insertmode = key_flg.insert;
        screen_putc(ch);
        if (ch == RETURN) {
            uint8* s = screen_gets();
            if (*s == '\'') { // 1.1b14
            } else if (*s != 0) {
                uint8 i;
                for (i = 0; i < N_LINEBUF; i++) {
                    linebuf[i] = s[i];
                    if (!s[i])
                        break;
                }
                if (s[i]) {
                    //					put_str("Too long line\n");
                    put_str("Too long\n"); // 1.2b45
                } else {
                    linebuf[i] = 0; // いっぱいまで入れるとバグっていた 1.2b32
                    exec(linebuf);
                }
            }
        }
        // __wfe();
    }
    __builtin_unreachable();
}