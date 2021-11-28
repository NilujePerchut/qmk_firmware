/* Copyright 2021 niluje
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kbd75mockv2.h"

void board_init(void) {
	/* JTAG-DP Disabled and SW-DP Enabled */
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
}

#ifdef RGBLIGHT_ENABLE

/* WS2812 custom driver for RGB-LIGHTNING feature
 * Basically just clone the WS2812 driver (bitbang version).
 * Duplicates everything necessary but simplifies any unwanted feature
 * Limitations:
 *	- Does not support rgblight_layer
 *	- Does not support rgblight_map*/

#define NOP_FUDGE 0.4
#define WS2812_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#define NUMBER_NOPS 6
#define CYCLES_PER_SEC (STM32_SYSCLK / NUMBER_NOPS * NOP_FUDGE)
#define NS_PER_SEC (1000000000L)
#define NS_PER_CYCLE (NS_PER_SEC / CYCLES_PER_SEC)
#define NS_TO_CYCLES(n) ((n) / NS_PER_CYCLE)

#define rgblight_wait_ns(x)                        \
    do {                                            \
        for (int i = 0; i < NS_TO_CYCLES(x); i++) { \
            __asm__ volatile("nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t");            \
        }                                           \
    } while (0)

#define T1H 900
#define T1L (1250 - T1H)
#define T0H 350
#define T0L (1250 - T0H)
#define RES (1000 * WS2812_TRST_US)  // Width of the low gap between bits to cause a frame to latch

static void rgblight_sendByte(uint8_t byte) {
    for (unsigned char bit = 0; bit < 8; bit++) {
        bool is_one = byte & (1 << (7 - bit));
        if (is_one) {
            writePinHigh(RGBLIGHT_DI_PIN);
            rgblight_wait_ns(T1H);
            writePinLow(RGBLIGHT_DI_PIN);
            rgblight_wait_ns(T1L);
        } else {
            writePinHigh(RGBLIGHT_DI_PIN);
            rgblight_wait_ns(T0H);
            writePinLow(RGBLIGHT_DI_PIN);
            rgblight_wait_ns(T0L);
        }
    }
}

static void rgblight_ws2812_init(void) {
	palSetLineMode(RGBLIGHT_DI_PIN, WS2812_OUTPUT_MODE);
}

void rgblight_call_driver(LED_TYPE *led, uint8_t led_num) {
    static bool s_init = false;
    if (!s_init) {
        rgblight_ws2812_init();
        s_init = true;
    }

    chSysLock();

    for (uint8_t i = 0; i < led_num; i++) {
#if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
        rgblight_sendByte(led[i].g);
        rgblight_sendByte(led[i].r);
        rgblight_sendByte(led[i].b);
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
        rgblight_sendByte(led[i].r);
        rgblight_sendByte(led[i].g);
        rgblight_sendByte(led[i].b);
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
        rgblight_sendByte(led[i].b);
        rgblight_sendByte(led[i].g);
        rgblight_sendByte(led[i].r);
#endif

    }

    rgblight_wait_ns(RES);

    chSysUnlock();
}

extern rgblight_ranges_t rgblight_ranges;

/* Just duplicate the one in rgblight main code */
void rgblight_set(void) {
    LED_TYPE *start_led;
    uint8_t   num_leds = rgblight_ranges.clipping_num_leds;

    start_led = led + rgblight_ranges.clipping_start_pos;
    rgblight_call_driver(start_led, num_leds);
}

#endif
