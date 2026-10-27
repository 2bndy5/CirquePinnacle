/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *               2023 Brendan Doherty (2bndy5) to work with the CirquePinnacle library
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"

// MODIFIED for using the CirquePinnacle library
#include <CirquePinnacle.h>
#include "../defaultPins.h" // board presumptive default pin numbers for SS_PIN and DR_PIN

#ifndef USE_I2C
PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
#else // If using I2C, then use the following line (not the line above)
PinnacleTouchI2C trackpad(DR_PIN);
#endif

// interrupt related handling
volatile bool isDataReady = false; // track the interrupts with our own IRQ flag
/// A callback function that allows `loop()` to know when the trackpad's DR pin is active
void interruptHandler(uint gpio, uint32_t events)
{
    if (gpio != DR_PIN && !(events & GPIO_IRQ_EDGE_RISE)) {
        // the gpio pin and event does not match the configuration we specified
        return;
    }
    isDataReady = true; // forward event handling back to main loop()
}

// an object to hold data reported by the Cirque trackpad
RelativeReport trackpadData;

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTOTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum
{
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

/*------------- MAIN -------------*/
int main(void)
{
    board_init();
    tusb_init();

    if (!trackpad.begin()) {
        return 0;
    }

    trackpad.setDataMode(PINNACLE_RELATIVE); // ensure mouse mode is enabled
    // tell the Pinnacle ASIC to rotate the orientation of the axis data by +90 degrees
    trackpad.relativeModeConfig(true, true); // (enable taps, rotate90)
    trackpad.allowSleep(true);               // let power consumption drop if inactive for 5 seconds

    // setup interrupt handler
    gpio_set_irq_enabled_with_callback(DR_PIN, GPIO_IRQ_EDGE_RISE, true, &interruptHandler);

    while (1)
    {
        tud_task(); // tinyusb device task
        led_blinking_task();

        hid_task();
    }

    return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, bool freshData) // MODIFIED for using the CirquePinnacle library
{

    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;

    // MODIFIED for using the CirquePinnacle library
    if (freshData) {
        // reset because interrupt event is now handled
        isDataReady = false;
        // get fresh data now, so it may also be used for gamepad (or other) devices
        trackpad.read(&trackpadData);
    }

    switch (report_id)
    {
        case REPORT_ID_KEYBOARD: {
            // use to avoid send multiple consecutive zero report for keyboard
            static bool has_keyboard_key = false;

            if (freshData) // MODIFIED for using the CirquePinnacle library
            {
                uint8_t keycode[6] = {0};
                keycode[0] = HID_KEY_A;

                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
                has_keyboard_key = true;
            }
            else
            {
                // send empty key report if previously has key pressed
                if (has_keyboard_key)
                    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
                has_keyboard_key = false;
            }
        } break;

        case REPORT_ID_MOUSE: {

            // MODIFIED for using the CirquePinnacle library
            if (freshData)
            {
                // use trackpad data as mouse input (no panning data)
                tud_hid_mouse_report(REPORT_ID_MOUSE,
                                     trackpadData.buttons,
                                     trackpadData.x * -1, // invert the X axis
                                     trackpadData.y,
                                     trackpadData.scroll,
                                     0);
            }
            else
            {
                // end of touch event assumed. only send button data
                tud_hid_mouse_report(REPORT_ID_MOUSE, trackpadData.buttons, 0, 0, 0, 0);
            }
        } break;

        case REPORT_ID_CONSUMER_CONTROL: {
            // use to avoid send multiple consecutive zero report
            static bool has_consumer_key = false;

            if (freshData) // MODIFIED for using the CirquePinnacle library
            {
                // volume down
                uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
                has_consumer_key = true;
            }
            else
            {
                // send empty key report (release key) if previously has key pressed
                uint16_t empty_key = 0;
                if (has_consumer_key)
                    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
                has_consumer_key = false;
            }
        } break;

        case REPORT_ID_GAMEPAD: {
            // use to avoid send multiple consecutive zero report for keyboard
            static bool has_gamepad_key = false;

            hid_gamepad_report_t report = {
                .x = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0, .hat = 0, .buttons = 0};

            if (freshData) // MODIFIED for using the CirquePinnacle library
            {
                report.hat = GAMEPAD_HAT_UP;
                report.buttons = GAMEPAD_BUTTON_A;
                tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

                has_gamepad_key = true;
            }
            else
            {
                report.hat = GAMEPAD_HAT_CENTERED;
                report.buttons = 0;
                if (has_gamepad_key)
                    tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
                has_gamepad_key = false;
            }
        } break;

        default: break;
    }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    bool const freshData = trackpad.available(); // MODIFIED for using the CirquePinnacle library

    // Remote wakeup
    if (tud_suspended() && freshData) // MODIFIED for using the CirquePinnacle library
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }
    else
    {
        // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
        send_hid_report(REPORT_ID_MOUSE, freshData); // MODIFIED for using the CirquePinnacle library
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void)instance;
    (void)len;

    // MODIFIED because this example is only for the mouse device.
    // uint8_t next_report_id = report[0] + 1;

    if (report[0] < REPORT_ID_COUNT)
    {
        send_hid_report(report[0], isDataReady); // MODIFIED for using the CirquePinnacle library
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void)instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if (bufsize < 1)
                return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                // Capslock On: disable blink, turn led on
                blink_interval_ms = 0;
                board_led_write(true);
            }
            else
            {
                // Caplocks Off: back to normal blink
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // blink is disabled
    if (!blink_interval_ms)
        return;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}
