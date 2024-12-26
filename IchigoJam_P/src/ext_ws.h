//LED(WS2812B)の仕様は https://akizukidenshi.com/goodsaffix/WS2812B_20200225.pdf 参照

#ifndef __EXT_WS_H__
#define __EXT_WS_H__
#include "all_includes.h"

#define NOP5 __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
#define SLEEP_100NS NOP5 NOP5 NOP5 NOP5 NOP5 //クロックは252MHzなので、1クロック約4ns

void __attribute__((always_inline)) inline sleep_300ns() {
    SLEEP_100NS SLEEP_100NS SLEEP_100NS
}

void __not_in_flash_func(wsled_send_one_signal) (bool bit) {
    gpio_put(LED, true);
    if (bit) {
        sleep_300ns();
    }
    sleep_300ns();
    gpio_put(LED, false);
    sleep_300ns();
    sleep_300ns();
}

void __not_in_flash_func(ws_out) (int port, int nled, int repeat) {
    gpio_put(LED, false);
    sleep_us(300);
    int save = save_and_disable_interrupts();
    for (int i = 0; i < repeat; i++) {
        for (int j = 0; j < nled * 3; j++) {//1つのLEDにつき、3つの変数が必要
            uint16 v = var[j];
            for (int k = 7; k >= 0; k--) {
                bool bit = v & (0x01 << k);
                wsled_send_one_signal(bit);
            }
        }
    }
    restore_interrupts(save);
}

/*// __not_in_flash_funcを使えば、アセンブリで書く必要はなさそうだが、一応残しておく
//WS.LEDとしてはまだ不完全なので、必要になったら完成させる
//--ここから
#define NOP5 "\tnop\n""\tnop\n""\tnop\n""\tnop\n""\tnop\n" //asm用
#define SLEEP_100NS NOP5 NOP5 NOP5 NOP5 NOP5
void __not_in_flash_func(my_ws_out) (int ledarr[], int nled, int repeat) {
    // int ledarr[] = { 0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1 };
    gpio_put(LED, false);
    sleep_us(300);
    int save = save_and_disable_interrupts();

    asm volatile(
        ""      "movs r2, #208 "     "\n"       //| void* SIO_BASE = 0xd0
        "\t"    "lsls r2, r2, #24"   "\n"       //\                       << 24;
        "\t"    "movs r3, #4"        "\n"       //  int port = 1 << 2;
        "\t"    "movs r4, #0"        "\n"       //  int i = 0;
        // "\t"    "bl   L.sleep"       "\n"       //      sleep();
        // "\t"    "bl   L.sleep"       "\n"       //      sleep();
        "L.loop:"                    "\n"       //
        "\t"    "cmp  r4, %1"       "\n"       //|         i == 24
        "\t"    "beq  L.end"         "\n"       //\ while(!         ){
        "\t"    "ldr  r1, [%0, r4]"  "\n"       //    int v = ledarr[i];
        "\t"    "str  r3, [r2, #20]" "\n"       //    *(SIO_BASE+20) = port;
        "\t"    "cmp  r1, #0"        "\n"       //|        v == 0
        "\t"    "beq  L.skip"        "\n"       //\   if(!        ){
        "\t"    "bl   L.sleep"       "\n"       //      sleep();
        "L.skip:"                    "\n"       //    }
        "\t"    "bl   L.sleep"       "\n"       //    sleep();
        "\t"    "str  r3, [r2, #24]" "\n"       //    *(SIO_BASE+24) = port;
        "\t"    "bl   L.sleep"       "\n"       //    sleep();
        "\t"    "bl   L.sleep"       "\n"       //    sleep();
        "\t"    "adds r4, #4"        "\n"       //    ++i;
        "\t"    "b    L.loop"        "\n"       //  }
        "L.sleep:"                   "\n"
        SLEEP_100NS                             //void sleep(){
        SLEEP_100NS                             //  sleep_300ns
        SLEEP_100NS
        "\t"    "mov  pc, lr"        "\n"       //}
        "L.end:"                     "\n"
        :
    : "r" (ledarr), "r"(nled)
        : "r1", "r2", "r3", "r4"
        );
    restore_interrupts(save);
}

void set_ledarr(int ledarr[], int nled) {
    for (int i = 0; i < nled; i++) {
        for (int j = 0; j < 8; j++) {
            ledarr[i * 8 + j] = var[i] & (0x01 << (7 - j)) ? 1 : 0;
        }
    }
}

static inline void ws_out(int port, int nled, int repeat) {
    static int ledarr[102 * 8];
    set_ledarr(ledarr, 3);
    for (int i = 0; i < 24; i++) {
        printf("%d, ", ledarr[i]);
    }
    my_ws_out(ledarr, 96, 1);
}
//--ここまで*/

#endif