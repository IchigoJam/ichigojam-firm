#ifndef __SOUND_H__
#define __SOUND_H__
#include "all_includes.h"

#define SOUND_PIN 20

static uint slice_num;

void sound_init() {
    _g.psgratio = 1;
    gpio_set_function(SOUND_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(SOUND_PIN);
    pwm_set_clkdiv(slice_num, CLKDIV);
    pwm_set_wrap(slice_num, 0);//適当に小さい値を設定しておかないと起動音が鳴らない
    pwm_set_enabled(slice_num, true);
}

void set_tone() {
    static uint16 prev_tone = 0;
    if (prev_tone != _g.psgtone) {
        if (_g.psgtone) {//オン
            int freq = 60 * 261 / 2 / _g.psgtone;   //IchigoJamのソースの式 / 2
            int wrap = PICO_CLOCK_FREQ / (freq * CLKDIV) - 1;
            if (wrap < 0) {
                wrap = 0;
            } else if (wrap > 0xffff) {
                wrap = 0xffff;
            }
            pwm_set_wrap(slice_num, wrap);
            pwm_set_gpio_level(SOUND_PIN, wrap / 2);
        } else {//オフ
            pwm_set_counter(slice_num, 0);
            pwm_set_gpio_level(SOUND_PIN, 0);
        }
    }
    prev_tone = _g.psgtone;
}

static void sound_switch(int on) {
    //何もしない
}

#endif