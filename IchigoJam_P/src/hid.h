/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

 /** \ingroup group_class
  *  \defgroup ClassDriver_HID Human Interface Device (HID)
/*--------------------------------------------------------------------
* KEYCODE to Ascii Conversion
*  Expand to array of [128][2] (ascii without shift, ascii with shift)
*
* Usage: example to convert ascii from keycode (key) and shift modifier (shift).
* Here we assume key < 128 ( printable )
*
*  uint8_t const conv_table[128][2] =  { HID_KEYCODE_TO_ASCII };
*  char ch = shift ? conv_table[chr][1] : conv_table[chr][0];
*
*--------------------------------------------------------------------*/

//pico-sdk/lib/tinyusb/src/class/hid/hid.hから必要な部分を抜粋、修正

#ifndef __HID_H__
#define __HID_H__
#include "all_includes.h"

#define RETURN '\n'
#define SFTSP 14 //shift + space
#define KANA 15
#define SFTRET 16 //行の途中で改行
#define INSERT 17
#define HOME 18
#define END 23
#define PGUP 19 //ページアップ
#define PGDOWN 20 //ページダウン
#define CAPS 0
#define ESC 27
#define LEFT 28
#define RIGHT 29
#define UP 30
#define DOWN 31
#define DELETE 127
#define F1  0
#define F2  0
#define F3  0
#define F4  0
#define F5  0
#define F6  0
#define F7  0
#define F8  0
#define F9  0
#define F10 0
#define F11 0
#define F12 0

//直接配列として宣言すると、自動整形で崩れる
#define HID_KEYCODE_TO_ASCII_US \
    {0     , 0     , 0     , 0     }, /* 0x00 */ \
    {0     , 0     , 0     , 0     }, /* 0x01 */ \
    {0     , 0     , 0     , 0     }, /* 0x02 */ \
    {0     , 0     , 0     , 0     }, /* 0x03 */ \
    {'a'   , 'A'   , 234   , 138   }, /* 0x04 */ \
    {'b'   , 'B'   , 235   , 139   }, /* 0x05 */ \
    {'c'   , 'C'   , 236   , 140   }, /* 0x06 */ \
    {'d'   , 'D'   , 237   , 141   }, /* 0x07 */ \
    {'e'   , 'E'   , 238   , 142   }, /* 0x08 */ \
    {'f'   , 'F'   , 239   , 143   }, /* 0x09 */ \
    {'g'   , 'G'   , 240   , 144   }, /* 0x0a */ \
    {'h'   , 'H'   , 241   , 145   }, /* 0x0b */ \
    {'i'   , 'I'   , 242   , 146   }, /* 0x0c */ \
    {'j'   , 'J'   , 243   , 147   }, /* 0x0d */ \
    {'k'   , 'K'   , 244   , 148   }, /* 0x0e */ \
    {'l'   , 'L'   , 245   , 149   }, /* 0x0f */ \
    {'m'   , 'M'   , 246   , 150   }, /* 0x10 */ \
    {'n'   , 'N'   , 247   , 151   }, /* 0x11 */ \
    {'o'   , 'O'   , 248   , 152   }, /* 0x12 */ \
    {'p'   , 'P'   , 249   , 153   }, /* 0x13 */ \
    {'q'   , 'Q'   , 250   , 154   }, /* 0x14 */ \
    {'r'   , 'R'   , 251   , 155   }, /* 0x15 */ \
    {'s'   , 'S'   , 252   , 156   }, /* 0x16 */ \
    {'t'   , 'T'   , 253   , 157   }, /* 0x17 */ \
    {'u'   , 'U'   , 254   , 158   }, /* 0x18 */ \
    {'v'   , 'V'   , 255   , 159   }, /* 0x19 */ \
    {'w'   , 'W'   , 224   , 128   }, /* 0x1a */ \
    {'x'   , 'X'   , 225   , 129   }, /* 0x1b */ \
    {'y'   , 'Y'   , 226   , 130   }, /* 0x1c */ \
    {'z'   , 'Z'   , 227   , 131   }, /* 0x1d */ \
    {'1'   , '!'   , 225   , 129   }, /* 0x1e */ \
    {'2'   , '@'   , 226   , 130   }, /* 0x1f */ \
    {'3'   , '#'   , 227   , 131   }, /* 0x20 */ \
    {'4'   , '$'   , 228   , 132   }, /* 0x21 */ \
    {'5'   , '%'   , 229   , 133   }, /* 0x22 */ \
    {'6'   , '^'   , 230   , 134   }, /* 0x23 */ \
    {'7'   , '&'   , 231   , 135   }, /* 0x24 */ \
    {'8'   , '*'   , 232   , 136   }, /* 0x25 */ \
    {'9'   , '('   , 233   , 137   }, /* 0x26 */ \
    {'0'   , ')'   , 224   , 128   }, /* 0x27 */ \
    {RETURN, SFTRET, RETURN, SFTRET}, /* 0x28 */ \
    {ESC   , ESC   , ESC   , ESC   }, /* 0x29 */ \
    {'\b'  , '\b'  , '\b'  , '\b'  }, /* 0x2a */ \
    {'\t'  , '\t'  , '\t'  , '\t'  }, /* 0x2b */ \
    {' '   , SFTSP , ' '   , SFTSP }, /* 0x2c */ \
    {'-'   , '_'   , '-'   , '_'   }, /* 0x2d */ \
    {'='   , '+'   , '='   , '+'   }, /* 0x2e */ \
    {'['   , '{'   , '_'   , '_'   }, /* 0x2f */ \
    {']'   , '}'   , '\\'  , '\\'  }, /* 0x30 */ \
    {'\\'  , '|'   , '\\'  , '|'   }, /* 0x31 */ \
    {'#'   , '~'   , '#'   , '~'   }, /* 0x32 */ \
    {';'   , ':'   , ';'   , ':'   }, /* 0x33 */ \
    {'\''  , '\"'  , '\''  , '\"'  }, /* 0x34 */ \
    {'`'   , '~'   , '`'   , '~'   }, /* 0x35 */ \
    {','   , '<'   , ','   , '<'   }, /* 0x36 */ \
    {'.'   , '>'   , '.'   , '>'   }, /* 0x37 */ \
    {'/'   , '?'   , '/'   , '?'   }, /* 0x38 */ \
                                  \
    {CAPS  , CAPS  , CAPS  , CAPS  }, /* 0x39 */ \
    {F1    , F1    , F1    , F1    }, /* 0x3a */ \
    {F2    , F2    , F2    , F2    }, /* 0x3b */ \
    {F3    , F3    , F3    , F3    }, /* 0x3c */ \
    {F4    , F4    , F4    , F4    }, /* 0x3d */ \
    {F5    , F5    , F5    , F5    }, /* 0x3e */ \
    {F6    , F6    , F6    , F6    }, /* 0x3f */ \
    {F7    , F7    , F7    , F7    }, /* 0x40 */ \
    {F8    , F8    , F8    , F8    }, /* 0x41 */ \
    {F9    , F9    , F9    , F9    }, /* 0x42 */ \
    {F10   , F10   , F10   , F10   }, /* 0x43 */ \
    {F11   , F11   , F11   , F11   }, /* 0x44 */ \
    {F12   , F12   , F12   , F12   }, /* 0x45 */ \
    {0     , 0     , 0     , 0     }, /* 0x46 */ \
    {0     , 0     , 0     , 0     }, /* 0x47 */ \
    {0     , 0     , 0     , 0     }, /* 0x48 */ \
    {INSERT, INSERT, INSERT, INSERT}, /* 0x49 */ \
    {HOME  , HOME  , HOME  , HOME  }, /* 0x4a */ \
    {PGUP  , PGUP  , PGUP  , PGUP  }, /* 0x4b */ \
    {DELETE, DELETE, DELETE, DELETE}, /* 0x4c */ \
    {END   , END   , END   , END   }, /* 0x4d */ \
    {PGDOWN, PGDOWN, PGDOWN, PGDOWN}, /* 0x4e */ \
    {RIGHT , RIGHT , RIGHT , RIGHT }, /* 0x4f */ \
    {LEFT  , LEFT  , LEFT  , LEFT  }, /* 0x50 */ \
    {DOWN  , DOWN  , DOWN  , DOWN  }, /* 0x51 */ \
    {UP    , UP    , UP    , UP    }, /* 0x52 */ \
    {0     , 0     , 0     , 0     }, /* 0x53 */ \
                                  \
    {'/'   , '/'   , '/'   , '/'   }, /* 0x54 */ \
    {'*'   , '*'   , '*'   , '*'   }, /* 0x55 */ \
    {'-'   , '-'   , '-'   , '-'   }, /* 0x56 */ \
    {'+'   , '+'   , '+'   , '+'   }, /* 0x57 */ \
    {RETURN, SFTRET, RETURN, SFTRET}, /* 0x58 */ \
    {'1'   , '1'   , 225   , 129   }, /* 0x59 */ \
    {'2'   , '2'   , 226   , 130   }, /* 0x5a */ \
    {'3'   , '3'   , 227   , 131   }, /* 0x5b */ \
    {'4'   , '4'   , 228   , 132   }, /* 0x5c */ \
    {'5'   , '5'   , 229   , 133   }, /* 0x5d */ \
    {'6'   , '6'   , 230   , 134   }, /* 0x5e */ \
    {'7'   , '7'   , 231   , 135   }, /* 0x5f */ \
    {'8'   , '8'   , 232   , 136   }, /* 0x60 */ \
    {'9'   , '9'   , 233   , 137   }, /* 0x61 */ \
    {'0'   , '0'   , 224   , 128   }, /* 0x62 */ \
    {'.'   , '.'   , '.'   , '.'   }, /* 0x63 */ \
    {0     , 0     , 0     , 0     }, /* 0x64 */ \
    {0     , 0     , 0     , 0     }, /* 0x65 */ \
    {0     , 0     , 0     , 0     }, /* 0x66 */ \
    {'='   , '='   , '='   , '='   }, /* 0x67 */ \

uint8_t keycode_to_ascii_us[128][4] = { HID_KEYCODE_TO_ASCII_US };

#define HID_KEYCODE_TO_ASCII_JA \
    {0     , 0     , 0     , 0     }, /* 0x00 */ \
    {0     , 0     , 0     , 0     }, /* 0x01 */ \
    {0     , 0     , 0     , 0     }, /* 0x02 */ \
    {0     , 0     , 0     , 0     }, /* 0x03 */ \
    {'a'   , 'A'   , 234   , 138   }, /* 0x04 */ \
    {'b'   , 'B'   , 235   , 139   }, /* 0x05 */ \
    {'c'   , 'C'   , 236   , 140   }, /* 0x06 */ \
    {'d'   , 'D'   , 237   , 141   }, /* 0x07 */ \
    {'e'   , 'E'   , 238   , 142   }, /* 0x08 */ \
    {'f'   , 'F'   , 239   , 143   }, /* 0x09 */ \
    {'g'   , 'G'   , 240   , 144   }, /* 0x0a */ \
    {'h'   , 'H'   , 241   , 145   }, /* 0x0b */ \
    {'i'   , 'I'   , 242   , 146   }, /* 0x0c */ \
    {'j'   , 'J'   , 243   , 147   }, /* 0x0d */ \
    {'k'   , 'K'   , 244   , 148   }, /* 0x0e */ \
    {'l'   , 'L'   , 245   , 149   }, /* 0x0f */ \
    {'m'   , 'M'   , 246   , 150   }, /* 0x10 */ \
    {'n'   , 'N'   , 247   , 151   }, /* 0x11 */ \
    {'o'   , 'O'   , 248   , 152   }, /* 0x12 */ \
    {'p'   , 'P'   , 249   , 153   }, /* 0x13 */ \
    {'q'   , 'Q'   , 250   , 154   }, /* 0x14 */ \
    {'r'   , 'R'   , 251   , 155   }, /* 0x15 */ \
    {'s'   , 'S'   , 252   , 156   }, /* 0x16 */ \
    {'t'   , 'T'   , 253   , 157   }, /* 0x17 */ \
    {'u'   , 'U'   , 254   , 158   }, /* 0x18 */ \
    {'v'   , 'V'   , 255   , 159   }, /* 0x19 */ \
    {'w'   , 'W'   , 224   , 128   }, /* 0x1a */ \
    {'x'   , 'X'   , 225   , 129   }, /* 0x1b */ \
    {'y'   , 'Y'   , 226   , 130   }, /* 0x1c */ \
    {'z'   , 'Z'   , 227   , 131   }, /* 0x1d */ \
    {'1'   , '!'   , 225   , 129   }, /* 0x1e */ \
    {'2'   , '\"'  , 226   , 130   }, /* 0x1f */ \
    {'3'   , '#'   , 227   , 131   }, /* 0x20 */ \
    {'4'   , '$'   , 228   , 132   }, /* 0x21 */ \
    {'5'   , '%'   , 229   , 133   }, /* 0x22 */ \
    {'6'   , '&'   , 230   , 134   }, /* 0x23 */ \
    {'7'   , '\''  , 231   , 135   }, /* 0x24 */ \
    {'8'   , '('   , 232   , 136   }, /* 0x25 */ \
    {'9'   , ')'   , 233   , 137   }, /* 0x26 */ \
    {'0'   , '0'   , 224   , 128   }, /* 0x27 */ \
    {RETURN, SFTRET, RETURN, SFTRET}, /* 0x28 */ \
    {ESC   , ESC   , ESC   , ESC   }, /* 0x29 */ \
    {'\b'  , '\b'  , '\b'  , '\b'  }, /* 0x2a */ \
    {'\t'  , '\t'  , '\t'  , '\t'  }, /* 0x2b */ \
    {' '   , SFTSP , ' '   , SFTSP }, /* 0x2c */ \
    {'-'   , '='   , '-'   , '='   }, /* 0x2d */ \
    {'^'   , '~'   , '^'   , '~'   }, /* 0x2e */ \
    {'@'   , '`'   , '@'   , '`'   }, /* 0x2f */ \
    {'['   , '{'   , '_'   , '_'   }, /* 0x30 */ \
    {'\\'  , '|'   , '\\'  , '|'   }, /* 0x31   日本語キーボードには存在しない？*/\
    {']'   , '}'   , '\\'  , '\\'  }, /* 0x32 */ \
    {';'   , '+'   , ';'   , '+'   }, /* 0x33 */ \
    {':'   , '*'   , ':'   , '*'   }, /* 0x34 */ \
    {' '   , ' '   , ' '   , ' '   }, /* 0x35   全角/半角キー　本家に合わせてスペースを出力する*/ \
    {','   , '<'   , ','   , '<'   }, /* 0x36 */ \
    {'.'   , '>'   , '.'   , '>'   }, /* 0x37 */ \
    {'/'   , '?'   , '/'   , '?'   }, /* 0x38 */ \
                                  \
    {CAPS  , CAPS  , CAPS  , CAPS  }, /* 0x39 */ \
    {F1    , F1    , F1    , F1    }, /* 0x3a */ \
    {F2    , F2    , F2    , F2    }, /* 0x3b */ \
    {F3    , F3    , F3    , F3    }, /* 0x3c */ \
    {F4    , F4    , F4    , F4    }, /* 0x3d */ \
    {F5    , F5    , F5    , F5    }, /* 0x3e */ \
    {F6    , F6    , F6    , F6    }, /* 0x3f */ \
    {F7    , F7    , F7    , F7    }, /* 0x40 */ \
    {F8    , F8    , F8    , F8    }, /* 0x41 */ \
    {F9    , F9    , F9    , F9    }, /* 0x42 */ \
    {F10   , F10   , F10   , F10   }, /* 0x43 */ \
    {F11   , F11   , F11   , F11   }, /* 0x44 */ \
    {F12   , F12   , F12   , F12   }, /* 0x45 */ \
    {0     , 0     , 0     , 0     }, /* 0x46 */ \
    {0     , 0     , 0     , 0     }, /* 0x47 */ \
    {0     , 0     , 0     , 0     }, /* 0x48 */ \
    {INSERT, INSERT, INSERT, INSERT}, /* 0x49 */ \
    {HOME  , HOME  , HOME  , HOME  }, /* 0x4a */ \
    {PGUP  , PGUP  , PGUP  , PGUP  }, /* 0x4b */ \
    {DELETE, DELETE, DELETE, DELETE}, /* 0x4c */ \
    {END   , END   , END   , END   }, /* 0x4d */ \
    {PGDOWN, PGDOWN, PGDOWN, PGDOWN}, /* 0x4e */ \
    {RIGHT , RIGHT , RIGHT , RIGHT }, /* 0x4f */ \
    {LEFT  , LEFT  , LEFT  , LEFT  }, /* 0x50 */ \
    {DOWN  , DOWN  , DOWN  , DOWN  }, /* 0x51 */ \
    {UP    , UP    , UP    , UP    }, /* 0x52 */ \
    {0     , 0     , 0     , 0     }, /* 0x53 */ \
                                  \
    {'/'   , '/'   , '/'   , '/'   }, /* 0x54 */ \
    {'*'   , '*'   , '*'   , '*'   }, /* 0x55 */ \
    {'-'   , '-'   , '-'   , '-'   }, /* 0x56 */ \
    {'+'   , '+'   , '+'   , '+'   }, /* 0x57 */ \
    {RETURN, SFTRET, RETURN, SFTRET}, /* 0x58 */ \
    {'1'   , '1'   , 225   , 129   }, /* 0x59 */ \
    {'2'   , '2'   , 226   , 130   }, /* 0x5a */ \
    {'3'   , '3'   , 227   , 131   }, /* 0x5b */ \
    {'4'   , '4'   , 228   , 132   }, /* 0x5c */ \
    {'5'   , '5'   , 229   , 133   }, /* 0x5d */ \
    {'6'   , '6'   , 230   , 134   }, /* 0x5e */ \
    {'7'   , '7'   , 231   , 135   }, /* 0x5f */ \
    {'8'   , '8'   , 232   , 136   }, /* 0x60 */ \
    {'9'   , '9'   , 233   , 137   }, /* 0x61 */ \
    {'0'   , '0'   , 224   , 128   }, /* 0x62 */ \
    {'.'   , '.'   , '.'   , '.'   }, /* 0x63 */ \
    {0     , 0     , 0     , 0     }, /* 0x64 */ \
    {0     , 0     , 0     , 0     }, /* 0x65 */ \
    {0     , 0     , 0     , 0     }, /* 0x66 */ \
    {'='   , '='   , '='   , '='   }, /* 0x67 */ \

uint8_t keycode_to_ascii_ja[128][4] = { HID_KEYCODE_TO_ASCII_JA };

//keycodeが128以上の場合
#define UNDBAR 135 //日本語キーボードのアンダーバー
#define JAKANA 136 //日本語キーボードのかなキー
#define YENPIPE 137 //日本語キーボードの¥と|
#define NOCHG 139 //日本語キーボードの無変換

#ifdef __cplusplus

#endif

#endif