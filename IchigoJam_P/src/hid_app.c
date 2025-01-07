/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021, Ha Thach (tinyusb.org)
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
 */

 //pico-examples/usb/host/host_cdc_msc_hid/hid_app.cを拡張

#ifndef __HID_APP_C__
#define __HID_APP_C__
#include "all_includes.h"

#include "bsp/board.h"
#include "tusb.h"
#include "IchigoJam_BASIC/screen.h"
#include "IchigoJam_BASIC/romajikana.h"


 //--------------------------------------------------------------------+
 // MACRO TYPEDEF CONSTANT ENUM DECLARATION
 //--------------------------------------------------------------------+

 // If your host terminal support ansi escape code such as TeraTerm
 // it can be use to simulate mouse cursor movement within terminal
#define USE_ANSI_ESCAPE   0

#define MAX_REPORT  4

extern bool is_deep_sleeping;
uint8_t keycode2ascii[128][4];
hid_keyboard_report_t prev_report = { 0, 0, {0} }; // previous report to check key released
struct long_press {
    uint8_t last_char;
    bool is_first_putc;
    uint64_t last_key_report_time;
};

struct long_press lp = { 0,true,0 };

// Each HID instance can has multiple reports
static struct
{
    uint8_t report_count;
    tuh_hid_report_info_t report_info[MAX_REPORT];
}hid_info[CFG_TUH_HID];

static void process_kbd_report(hid_keyboard_report_t const* report);
static void process_mouse_report(hid_mouse_report_t const* report);
static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len);
void delete_line_and_key_push(char*);
void put_function_key(uint8_t);

void hid_app_task(void)
{
    // nothing to do
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
    printf("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);

    //   Interface protocol (hid_interface_protocol_enum_t)
    const char* protocol_str[] = { "None", "Keyboard", "Mouse" };
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

    printf("HID Interface Protocol = %s\r\n", protocol_str[itf_protocol]);

    // By default host stack will use activate boot protocol on supported interface.
    // Therefore for this simple example, we only need to parse generic report descriptor (with built-in parser)
    if (itf_protocol == HID_ITF_PROTOCOL_NONE)
    {
        hid_info[instance].report_count = tuh_hid_parse_report_descriptor(hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);
        printf("HID has %u reports \r\n", hid_info[instance].report_count);
    }

    //NUMLOCKを無効化する
    //https://github.com/hathach/tinyusb/discussions/1191 参照
    uint8_t leds = 0;
    tuh_hid_set_report(dev_addr, instance, 0, HID_REPORT_TYPE_OUTPUT, &leds, sizeof(leds));

    // request to receive report
    // tuh_hid_report_received_cb() will be invoked when report is available
    if (!tuh_hid_receive_report(dev_addr, instance))
    {
        printf("Error: cannot request to receive report\r\n");
    }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
    printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

    switch (itf_protocol)
    {
    case HID_ITF_PROTOCOL_KEYBOARD:
        TU_LOG2("HID receive boot keyboard report\r\n");
        process_kbd_report((hid_keyboard_report_t const*)report);
        break;

    case HID_ITF_PROTOCOL_MOUSE:
        TU_LOG2("HID receive boot mouse report\r\n");
        process_mouse_report((hid_mouse_report_t const*)report);
        break;

    default:
        // Generic report requires matching ReportID and contents with previous parsed report info
        process_generic_report(dev_addr, instance, report, len);
        break;
    }

    // continue to request to receive report
    if (!tuh_hid_receive_report(dev_addr, instance))
    {
        printf("Error: cannot request to receive report\r\n");
    }
}

//--------------------------------------------------------------------+
// Keyboard
//--------------------------------------------------------------------+

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const* report, uint8_t keycode)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        if (report->keycode[i] == keycode)  return true;
    }

    return false;
}

void toggle_kana_mode() {
    key_flg.kana = !key_flg.kana;
    _g.key_kana_buf_0 = _g.key_kana_buf_1 = 0;
}

static void process_kbd_report(hid_keyboard_report_t const* report) {
    static uint8_t last_keycode = 0;

    //------------- example code ignore control (non-printable) key affects -------------//
    if (is_deep_sleeping) {
        //ディープスリープ直前のエンターが誤って長押しされている判定にならないようにしてから早期リターン
        prev_report = *report;
        lp.last_char = 0;
        return;
    }
    lp.last_key_report_time = time_us_64();
    bool no_keycode = true;
    bool should_reset_last_char = true;
    bool const is_ctrl = report->modifier & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL);
    bool const is_shift = report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
    bool const is_alt = (report->modifier & (KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_RIGHTALT))
        || find_key_in_report(report, NOCHG);//ALTか無変換を押していたらtrue
    for (uint8_t i = 0; i < 6; i++) {
        uint8_t keycode = report->keycode[i];
        if (keycode) {
            no_keycode = false;
            if (find_key_in_report(&prev_report, keycode)) {
                // exist in previous report means the current key is holding
                if (keycode == last_keycode) {
                    should_reset_last_char = false;
                }
            } else {
                // not existed in previous report means the current key is pressed
                int mods = (is_alt << 1) | is_shift;
                uint8_t ch = 0;
                if (keycode >= 128) {
                    if (keycode == UNDBAR) {
                        ch = is_shift ? '_' : '\\';
                    } else if (keycode == JAKANA) {
                        toggle_kana_mode();
                    } else if (keycode == YENPIPE) {
                        ch = is_shift ? '|' : '\\';
                    }
                } else {
                    ch = keycode2ascii[keycode][mods];
                }

                if (is_ctrl && ch == ' ') {
                    toggle_kana_mode();
                } else if (ch == 0) {
                    if (keycode == 0x48) {//Pause/BreakはF12と同じ動作にする
                        keycode = 0x45;
                    }
                    if (0x3a <= keycode && keycode <= 0x45) {
                        put_function_key(keycode);
                    } else if (keycode == 0x39) {
                        key_flg.caps = !key_flg.caps;
                    }
                } else {
                    if (key_flg.caps) {
                        if ('a' <= ch && ch <= 'z') {
                            ch -= 32;
                        } else if ('A' <= ch && ch <= 'Z') {
                            ch += 32;
                        }
                    }
                    if (key_flg.kana) {
                        ch = romajikana_input(ch);
                    }
                    if (ch == ESC) {
                        _g.key_flg_esc = 1;
                    }
                    key_pushc(ch);
                }
                lp.last_char = ch;
                lp.is_first_putc = true;
                last_keycode = keycode;
                should_reset_last_char = false;
            }
        }
    }

    if (no_keycode) {
        if (is_ctrl && is_shift) {
            toggle_kana_mode();
        }
        if (is_ctrl && is_alt) {
            key_flg.insert = !key_flg.insert;
        }
    }
    if (should_reset_last_char) {
        lp.last_char = 0;
    }

    prev_report = *report;
}

//--------------------------------------------------------------------+
// Mouse
//--------------------------------------------------------------------+

void cursor_movement(int8_t x, int8_t y, int8_t wheel)
{
#if USE_ANSI_ESCAPE
    // Move X using ansi escape
    if (x < 0)
    {
        printf(ANSI_CURSOR_BACKWARD(% d), (-x)); // move left
    } else if (x > 0)
    {
        printf(ANSI_CURSOR_FORWARD(% d), x); // move right
    }

    // Move Y using ansi escape
    if (y < 0)
    {
        printf(ANSI_CURSOR_UP(% d), (-y)); // move up
    } else if (y > 0)
    {
        printf(ANSI_CURSOR_DOWN(% d), y); // move down
    }

    // Scroll using ansi escape
    if (wheel < 0)
    {
        printf(ANSI_SCROLL_UP(% d), (-wheel)); // scroll up
    } else if (wheel > 0)
    {
        printf(ANSI_SCROLL_DOWN(% d), wheel); // scroll down
    }

    printf("\r\n");
#else
    printf("(%d %d %d)\r\n", x, y, wheel);
#endif
}

static void process_mouse_report(hid_mouse_report_t const* report)
{
    static hid_mouse_report_t prev_report = { 0 };

    //------------- button state  -------------//
    uint8_t button_changed_mask = report->buttons ^ prev_report.buttons;
    if (button_changed_mask & report->buttons)
    {
        printf(" %c%c%c ",
            report->buttons & MOUSE_BUTTON_LEFT ? 'L' : '-',
            report->buttons & MOUSE_BUTTON_MIDDLE ? 'M' : '-',
            report->buttons & MOUSE_BUTTON_RIGHT ? 'R' : '-');
    }

    //------------- cursor movement -------------//
    cursor_movement(report->x, report->y, report->wheel);
}

//--------------------------------------------------------------------+
// Generic Report
//--------------------------------------------------------------------+
static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void)dev_addr;

    uint8_t const rpt_count = hid_info[instance].report_count;
    tuh_hid_report_info_t* rpt_info_arr = hid_info[instance].report_info;
    tuh_hid_report_info_t* rpt_info = NULL;

    if (rpt_count == 1 && rpt_info_arr[0].report_id == 0)
    {
        // Simple report without report ID as 1st byte
        rpt_info = &rpt_info_arr[0];
    } else
    {
        // Composite report, 1st byte is report ID, data starts from 2nd byte
        uint8_t const rpt_id = report[0];

        // Find report id in the array
        for (uint8_t i = 0; i < rpt_count; i++)
        {
            if (rpt_id == rpt_info_arr[i].report_id)
            {
                rpt_info = &rpt_info_arr[i];
                break;
            }
        }

        report++;
        len--;
    }

    if (!rpt_info)
    {
        printf("Couldn't find the report info for this report !\r\n");
        return;
    }

    // For complete list of Usage Page & Usage checkout src/class/hid/hid.h. For examples:
    // - Keyboard                     : Desktop, Keyboard
    // - Mouse                        : Desktop, Mouse
    // - Gamepad                      : Desktop, Gamepad
    // - Consumer Control (Media Key) : Consumer, Consumer Control
    // - System Control (Power key)   : Desktop, System Control
    // - Generic (vendor)             : 0xFFxx, xx
    if (rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP)
    {
        switch (rpt_info->usage)
        {
        case HID_USAGE_DESKTOP_KEYBOARD:
            TU_LOG1("HID receive keyboard report\r\n");
            // Assume keyboard follow boot report layout
            process_kbd_report((hid_keyboard_report_t const*)report);
            break;

        case HID_USAGE_DESKTOP_MOUSE:
            TU_LOG1("HID receive mouse report\r\n");
            // Assume mouse follow boot report layout
            process_mouse_report((hid_mouse_report_t const*)report);
            break;

        default: break;
        }
    }
}

//IchigoJam
void delete_line_and_key_push(char* s) {
    key_pushc(24);
    key_push(s);
}

void put_function_key(uint8_t key) {
    switch (key) {
    case 0x3a: //F1
        key_push("\x13\x0c\0");
        break;
    case 0x3b: //F2　以下同様
        delete_line_and_key_push("LOAD");
        break;
    case 0x3c:
        delete_line_and_key_push("SAVE");
        break;
    case 0x3d:
        delete_line_and_key_push("LIST\n");
        break;
    case 0x3e:
        delete_line_and_key_push("RUN\n");
        break;
    case 0x3f:
        delete_line_and_key_push("?FREE()\n");
        break;
    case 0x40:
        delete_line_and_key_push("OUT0\n");
        break;
    case 0x41:
        delete_line_and_key_push("VIDEO1\n");
        break;
    case 0x42:
        delete_line_and_key_push("FILES");
        break;
    case 0x43:
        delete_line_and_key_push("SWITCH\n");
        break;
    case 0x44:
        key_pushc(0x0c);
        break;
    case 0x45: //F12
        key_pushc(24);
        break;
    default:
        break;
    }
}

#endif