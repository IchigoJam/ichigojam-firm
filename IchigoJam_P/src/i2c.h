#ifndef __I2C_H__
#define __I2C_H__
#include "all_includes.h"

#define I2C_DEFAULT_BPS 400000
#define TIMEOUT_US 1000000 //TODO タイムアウト入れるべきか、入れるなら何秒にすべきか検討する

static int i2c_baudrate = I2C_DEFAULT_BPS;

/* Initialize I2C module  0:ok 1:_g.err */
int i2c0_init() {
    i2c_init(i2c_default, I2C_DEFAULT_BPS);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    return 0;
}

// 0:success 1:io error 2:parm error
//readのときwritemode=1
INLINE int IJB_i2c(uint8 writemode, uint16* param) {
    int client_address = param[0];
    uint8_t* src1 = (uint8_t*)(param[1] + (uint)ram - OFFSET_RAMROM);
    int len1 = param[2];
    uint8_t* src2 = (uint8_t*)(param[3] + (uint)ram - OFFSET_RAMROM);
    int len2 = param[4];
    int res;
    res = i2c_write_timeout_us(i2c_default, client_address, src1, len1, false, TIMEOUT_US);//nostop=trueにすると、sakura.ioが正しく動かない
    if (!writemode) {
        res += i2c_write_timeout_us(i2c_default, client_address, src2, len2, false, TIMEOUT_US);
    } else {
        res += i2c_read_timeout_us(i2c_default, client_address, src2, len2, false, TIMEOUT_US);
    }
    return res == len1 + len2 ? 0 : 1;
}

void set_i2c_bps() {
    i2c_set_baudrate(i2c_default, i2c_baudrate);
}

void i2c0_bps(int n) {
    i2c_baudrate = n * 1000;
    set_i2c_bps();
}

#endif