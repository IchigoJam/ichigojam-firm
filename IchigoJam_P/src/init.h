#ifndef __INIT_H__
#define __INIT_H__
#include "all_includes.h"

//pico
//TODO 高解像度に対応する(TVでも使えるようにするため)  
// TMDS bit clock 252 MHz
// DVDD 1.2V (1.1V seems ok too)
//320*240ピクセルの画面として扱う
#define FRAME_MAX_WIDTH 320
#define FRAME_MAX_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define SCANLINE_INIT 2

//IchigoJam
#define FONT_SIZE 8
// #define MARGIN_WIDTH 32
// #define MARGIN_HEIGHT 24
#define MARGIN_WIDTH ((FRAME_MAX_WIDTH - CHAR_MAX_COLS * FONT_SIZE) / 2)
#define MARGIN_HEIGHT ((FRAME_MAX_HEIGHT - CHAR_MAX_ROWS * FONT_SIZE) / 2)

#define UART_TX_PIN 0
#define UART_RX_PIN 1

//pico
static uint16_t framebuf[FRAME_MAX_WIDTH * FRAME_MAX_HEIGHT];

void core1_main() {
    dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
    dvi_start(&dvi0);
    dvi_scanbuf_main_16bpp(&dvi0);
    __builtin_unreachable();
}

//この関数の実行時間が長くなるとUSBキーボードを使う時、不具合が起こる
void core1_scanline_callback() {
    // Discard any scanline pointers passed back
    uint16_t* bufptr;
    while (queue_try_remove_u32(&dvi0.q_colour_free, &bufptr))
        ;
    // // Note first two scanlines are pushed before DVI start
    static uint scanline = SCANLINE_INIT;
    bufptr = &framebuf[FRAME_MAX_WIDTH * scanline];
    queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
    scanline = (scanline + 1) % FRAME_MAX_HEIGHT;
    _g.linecnt++;
}

//1scanline分vramの内容をframebufに反映する
//framebufには320*240ピクセルの色情報が入っている(ここでは白か黒)
//vramには32*24の文字情報が入っている(mag=1の場合)
//_g.screen_bigは0~3の範囲
void vram_to_framebuf_scanline(uint scanline, bool visible_cursor) {
    int vram_y = ((scanline - MARGIN_HEIGHT) / FONT_SIZE) >> _g.screen_big;
    int mag = 1 << _g.screen_big;//文字の大きさの倍率　最大8倍
    if (0 <= vram_y && vram_y < SCREEN_H) {//scanlineが画面の表示範囲なら処理、そうでなければ黒のままでいいので何もしない
        int font_y = ((scanline - MARGIN_HEIGHT) % (FONT_SIZE << _g.screen_big)) >> _g.screen_big;
        uint16_t* framebuf_base = &framebuf[scanline * FRAME_MAX_WIDTH + MARGIN_WIDTH];
        uint8* c = &vram[vram_y * SCREEN_W];
        for (int vram_x = 0; vram_x < SCREEN_W; vram_x++) {
            unsigned char char_line = CHAR_PATTERN[*c * FONT_SIZE + font_y];
            if (*c >= 0xE0) {
                char_line = screen_pcg[(*c - 0xE0) * FONT_SIZE + font_y];//POKEで書き換えができる文字はscreen_pcgを参照する
            }
            c++;
            char_line ^= 0xff * _g.screen_invert;//VIDEOコマンドでの画面の反転を反映する
            if ((frames >> 4) & visible_cursor && _g.cursorx == vram_x && _g.cursory == vram_y) {//カーソルの位置の文字だけ反転させる
                char_line ^= key_flg.insert ? 0xff : 0xf0;//上書きモードなら文字全体を反転、挿入モードなら文字の左半分を反転
            }
            for (int x = 0; x < FONT_SIZE; x++) {
                int pixel = 0xffff * ((char_line >> (7 - x)) & 0x01);//char_lineのビットが1なら0xffff(白)、0なら0x0000(黒)に変換
                // for (int y = 0; y < mag; y++) {
                //     *framebuf_base = pixel;
                //     framebuf_base++;
                // }
                //展開するとめちゃくちゃ速くなる
                //duff's deviceは速くならなかった
                if (mag == 1) {
                    *framebuf_base = pixel;
                    framebuf_base++;
                } else if (mag == 2) {
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                } else if (mag == 4) {
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                } else if (mag == 8) {
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                    *framebuf_base = pixel;
                    framebuf_base++;
                }
            }
        }
    }
}

void vram_to_framebuf_all(bool visible_cursor) {
    for (int sl = 0; sl < FRAME_MAX_HEIGHT; sl++) {
        vram_to_framebuf_scanline(sl, visible_cursor);
    }
}

//hid_app.cが複雑になってるので、もっと簡潔に処理できるなら直したい
void putc_long_press_key() {
    int save = save_and_disable_interrupts();//割り込みを止めないとなぜかtime-us-64() - lp.last_key_report_timeがオーバーフローする時がある
    int interval = lp.is_first_putc ? 500000 : 100000;//キーを長押しした時、最初の1回だけ入力の間隔を長くする
    if (time_us_64() - lp.last_key_report_time > interval) {
        lp.last_key_report_time = time_us_64();
        if (lp.last_char != 0) {
            key_pushc(lp.last_char);
            lp.is_first_putc = false;
        }
    }
    restore_interrupts(save);
}

bool timer(repeating_timer_t* rt) {
    frames++;
    psg_tick();
    set_tone();
    if (video_active()) {
        vram_to_framebuf_all(_g.cursorflg);
    }
    //中でsleep_ms()が呼ばれたときに止まるので、本来タイマーの中でtuh_task()を呼び出してはいけないが、なぜかPicoDVIを動かしていると止まらない
    //pico-sdk/lib/tinyusb/src/osal/osal_pico.h　のosal_task_delay()のsleepをwhileループに置き換えるとPicoDVIなしで一応解決する
    tuh_task();
    putc_long_press_key();
    return true;
}

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        if (ch == ESC) {
            _g.key_flg_esc = (_g.uartmode_rxd & 2) == 0; // 1.2b41
        }
        if (_g.uartmode_rxd & 1) {
            key_pushc(ch);
        }
    }
}

//0番のプログラムの先頭行の先頭が @ARUN かどうか
bool is_arun() {
    //0番のプログラムデータ
    //行番号0、行番号1、行の文字数、行の内容...とデータが入っているので、index3からチェックする(行番号1*256+行番号0=行番号)
    const uint8_t* flash = get_flash(calc_flash_offset(0));
    uint8_t arun[] = "@ARUN";
    for (int i = 0; arun[i] != '\0'; i++) {
        if (arun[i] != flash[i + 3]) {
            return false;
        }
    }
    return true;
}

void pico_init() {
    static repeating_timer_t out;
    board_init();

    vreg_set_voltage(VREG_VSEL);
#ifdef RUN_FROM_CRYSTAL
    set_sys_clock_khz(12000, true);
#else
    // Run system at TMDS bit clock
    set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);
#endif

    stdio_uart_init();//クロックを変えてから初期化する
    // uart_set_hw_flow(UART_ID, false, false);//いる？
    // uart_set_fifo_enabled(UART_ID, false);
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
    gpio_pull_up(UART_TX_PIN);
    gpio_pull_up(UART_RX_PIN);

    //ラズパイが動いていることを確認するために、省電力モード以外のときはLEDを常時点灯させる
    gpio_init(PICO_LED_PIN);
    gpio_set_dir(PICO_LED_PIN, GPIO_OUT);
    gpio_put(PICO_LED_PIN, 1);

    // init host stack on configured roothub port
    tuh_init(BOARD_TUH_RHPORT);
    tuh_task();//消すと起動時にキーボード接続していた時に、認識しない時がある？
    add_repeating_timer_us(-16666, timer, NULL, &out);//60FPS
}

void picodvi_init() {
    dvi0.timing = &DVI_TIMING;
    dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
    dvi0.scanline_callback = core1_scanline_callback;
    dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

    // Once we've given core 1 the framebuffer, it will just keep on displaying
    // it without any intervention from core 0
    sprite_fill16(framebuf, 0x0000, FRAME_MAX_WIDTH * FRAME_MAX_HEIGHT);
    uint16_t* bufptr = framebuf;
    for (int i = 0; i < SCANLINE_INIT; i++) {
        queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
        bufptr += FRAME_MAX_WIDTH;
    }

    multicore_launch_core1(core1_main);
}

void ichigojam_init() {
    //SCREEN_W,SCREEN_Hはグローバル変数に置換されるので、基本的にSCREEN_W等を介してアクセスする
    SCREEN_W = CHAR_MAX_COLS;
    SCREEN_H = CHAR_MAX_ROWS;
    screen_clear();

    key_clearKey();
    key_flg.caps = true;
    set_keymap(key_getKeyboardID());
    random_init();
    io_init();
    screen_clp();
    sound_init();
    uart_init_IJ();
    i2c0_init();

    int sleepflg = getSleepFlag();//起動時ボタンを押していたらtrue
    sleepflg |= is_arun();//プログラムの先頭が@ARUNならtrue
    sleepflg |= watchdog_enable_caused_reboot();//SLEEPコマンドで再起動していたらtrue
    if (!sleepflg) {
        psg_beep(10, 3);
    }
    // video_waitSync(BOOT_WAIT);
#ifdef IJB_TITLE
    for (int i = 0;; i++) { // 自動起動時は表示だけしない
        char c = IJB_TITLE[i];
        if (!sleepflg)
            put_chr(c);
        video_waitSync(1);
        if (c == '\n')
            break;
    }
#endif
    if (!sleepflg) {
        put_str("OK\n");
    }
    _g.sleepflg = sleepflg;

}

#endif