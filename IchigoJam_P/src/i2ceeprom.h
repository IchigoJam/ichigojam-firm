//https://www.mouser.jp/datasheet/2/268/24AA1025_24LC1025_24FC1025_1024_Kbit_I2C_Serial_EE-2853738.pdf 参照
/*client addressは最下位ビットを0ビット目とすると、
0ビット目はread/writeの指定(readなら1,writeなら0)　pico-sdkのread/writeで指定する
1~2ビット目は他のEEPROMと識別するときに使う(ここでは00でよい)
3ビット目はEEPROMのアドレスの指定に使う
4~7ビット目は1010で固定*/
//24FC1025-I/Pの容量は1Mbit=128KB=2^17Bなので、client addressの3ビット目とその後に送る2バイトの情報でアドレスを決める
//client=slave EEPROMのデータシートの記述に合わせる

#ifndef __I2CEEPROM_H__
#define __I2CEEPROM_H__
#include "all_includes.h"

#define EEPROM_PAGE_SIZE 128

static uint8_t calc_client_address(int memory_address) {
    uint8_t res = 0b1010000;
    if (memory_address >> 16 & 1) {
        res |= 0b100;
    }
    return res;
}

void make_write_src(int memory_address, uint8_t* list, int len, uint8_t* src) {
    src[0] = (uint8_t)(memory_address >> 8);
    src[1] = (uint8_t)memory_address;
    for (int i = 0; i < len; i++) {
        src[2 + i] = list[i];
    }
}

// err:1 no_err:0
int write_eeprom(int memory_address, uint8_t* list, int len) {
    uint8_t client_address = calc_client_address(memory_address);
    uint8_t src[EEPROM_PAGE_SIZE + 2];
    while (len > 0) {
        int tlen = len > EEPROM_PAGE_SIZE ? EEPROM_PAGE_SIZE : len;
        len -= tlen;
        make_write_src(memory_address, list, tlen, src);
        memory_address += tlen;
        list += tlen;
        int res = i2c_write_timeout_us(i2c_default, client_address, src, tlen + 2, false, TIMEOUT_US);
        //I2Cのデータを送り切ったことを何かしらの方法で判断できれば、時間を直接指定する必要はない
        sleep_us(5000 * I2C_DEFAULT_BPS / (double)i2c_baudrate);//40kHzのとき、計算上は3msくらいあればOK
        if (res < 0) {
            return 1;
        }
    }
    return 0;
}

// ret:size if:-1 err
int read_eeprom(int memory_address, uint8_t* list, int len) {
    uint8_t client_address = calc_client_address(memory_address);
    uint8_t src[2];
    src[0] = (uint8_t)(memory_address >> 8);
    src[1] = (uint8_t)memory_address;
    int res = i2c_write_timeout_us(i2c_default, client_address, src, 2, true, TIMEOUT_US);
    if (res < 0) {
        return -1;
    }
    // res = i2c_read_timeout_us(i2c_default, client_address, list, len, false, TIMEOUT_US);//これでも動くけど、一応データシートに合わせる
    res = i2c_read_timeout_us(i2c_default, client_address, list, 1, false, TIMEOUT_US);
    list++;
    res += i2c_read_timeout_us(i2c_default, client_address, list, len - 1, false, TIMEOUT_US);
    return res == len ? res : -1;
}

#endif