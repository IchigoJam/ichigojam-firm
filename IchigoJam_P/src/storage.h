//picoに焼いたプログラムはフラッシュメモリの先頭に配置される
//プログラム領域の終わりは、build/IchigoJam_P.elf.mapのflash_binary_endの下7桁を見ればわかるが、かなり余裕がある(2ブロックに収まっている)
//IchigoJamのプログラムはフラッシュメモリの後ろの方に保存する
//フラッシュメモリの容量=2M=0x200000, 1ブロックの容量=0x10000, FLASH_SECTOR_SIZE=0x1000(単位はバイト)
#ifndef __STORAGE_H__
#define __STORAGE_H__
#include "all_includes.h"

#define FLASH_IJ_OFFSET (0x200000 - FLASH_SECTOR_SIZE * 2)//SAVE0,LOAD0で操作するセクタの先頭(一番最後のセクタはKBDの設定値などを保存するのに使うので、その1つ前のセクタ)
#define EEPROM_OFFSET 100
#define EEPROM_SIZE 128
#define SIZELIMIT (IJB_SIZEOF_LIST - 2)//=1プログラムのサイズ=1024

INLINE int IJB_file() {
    return _g.lastfile;
}

uint32_t calc_flash_offset(int n) {
    return FLASH_IJ_OFFSET - n * FLASH_SECTOR_SIZE;
}

uint32_t get_config_offset() {
    return calc_flash_offset(-1);//一番最後のセクタ
}

uint8_t* get_flash(uint32_t offset) {
    return (uint8_t*)(XIP_BASE + offset);
}

// err:1 no_err:0
static int IJB_save(int n, uint8* list, int size) {
    _g.lastfile = n;
    int res;
    if (0 <= n && n < N_FLASH_STORAGE) {
        uint32_t offset = calc_flash_offset(n);
        //フラッシュメモリに書き込む時は排他制御する
        video_off(0);
        int save = save_and_disable_interrupts();
        flash_range_erase(offset, FLASH_SECTOR_SIZE);
        flash_range_program(offset, list, FLASH_SECTOR_SIZE);
        video_on();
        restore_interrupts(save);
        res = 0;
    } else if (EEPROM_OFFSET <= n && n < EEPROM_OFFSET + EEPROM_SIZE) {
        res = write_eeprom((n - EEPROM_OFFSET) * SIZELIMIT, list, SIZELIMIT);
    } else {
        res = 1;
    }
    return res;
}

// ret:size if:-1 err
static int IJB_load(int n, uint8* list, int sizelimit, int init) {
    if (init) {
        _g.lastfile = n;
    }
    if (0 <= n && n < N_FLASH_STORAGE) {
        const uint8_t* flash = get_flash(calc_flash_offset(n));
        //TODO IchigoJam以外で使ったことのあるpicoでは正しく判定できないので、別の方法を考える
        if (*(int16*)flash == -1) {//未使用領域の初期値
            *(uint16*)list = 0;
            return -1;
        }
        memcpy(list, flash, sizelimit);
        return sizelimit;
    } else if (EEPROM_OFFSET <= n && n < EEPROM_OFFSET + EEPROM_SIZE) {
        return read_eeprom((n - EEPROM_OFFSET) * SIZELIMIT, list, sizelimit);
    } else {
        return -1;
    }
}

#endif