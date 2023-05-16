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
#include QMK_KEYBOARD_H
#include "keymap_steno.h"

/* Default, minimalist ISO keyboard, please select another one for real jobs */

enum layer_names {
	_BASE = 0,
	_MOUSE,
#ifdef RGBLIGHT_ENABLE
	_RGBLIGHTING,
#endif
#ifdef RGB_MATRIX_ENABLE
	_RGB_MATRIX,
#endif
	_UPDATE, /* This should be the last layer */
};

/* Use Custom keycodes in the case RGBLIGHT + RGBMATRIX is able to work
   in the same time with WS2812 */
enum custom_keycodes {
#ifdef RGBLIGHT_ENABLE
	RGBLIGHT_TOGGLE = SAFE_RANGE,
	RGBLIGHT_MODE,
	RGBLIGHT_RMODE,
	RGBLIGHT_INCREASE_HUE,
	RGBLIGHT_DECREASE_HUE,
	RGBLIGHT_INCREASE_SAT,
	RGBLIGHT_DECREASE_SAT,
	RGBLIGHT_INCREASE_VAL,
	RGBLIGHT_DECREASE_VAL,
#endif

#ifdef RGB_MATRIX_ENABLE
#ifdef RGBLIGHT_ENABLE
	RGB_MATRIX_TOGGLE,
#else
	RGB_MATRIX_TOGGLE = SAFE_RANGE,
#endif
	RGB_MATRIX_MODE,
	RGB_MATRIX_RMODE,
	RGB_MATRIX_INCREASE_HUE,
	RGB_MATRIX_DECREASE_HUE,
	RGB_MATRIX_INCREASE_SAT,
	RGB_MATRIX_DECREASE_SAT,
	RGB_MATRIX_INCREASE_VAL,
	RGB_MATRIX_DECREASE_VAL,
	RGB_MATRIX_INCREASE_SPD,
	RGB_MATRIX_DECREASE_SPD,
#endif
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
    KC_ESC, KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11,  KC_F12,   KC_PSCREEN, KC_DELETE,
  KC_GRAVE, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS, KC_EQL,   KC_PAUSE,   KC_HOME,
    KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC, KC_RBRC,  KC_BSPACE,  KC_PGUP,
   KC_CAPS, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT, KC_NUHS,  KC_ENT,     KC_PGDOWN,
 KC_LSHIFT, KC_NUBS,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_RSHIFT,KC_UP,      KC_END,
   KC_LCTL, KC_LGUI,KC_LALT,KC_NO,  KC_NO,  KC_NO,  KC_SPC, KC_NO,  KC_NO,  KC_RALT,TO(_BASE+1),   KC_RCTL, KC_LEFT, KC_DOWN,    KC_RIGHT),
    [_MOUSE] = LAYOUT(
     TO(0), KC_MPRV,    KC_MSTP,    KC_MPLY,     KC_NO,  KC_NO,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_NO,   KC_NO,        KC_NO,  KC_NO,        KC_NO,        KC_NO,
     KC_NO, KC_NO,      KC_NO,      KC_NO,       KC_NO,  KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,        KC_NO,  KC_NO,        KC_NO,        KC_NO,
     KC_NO, KC_NO,      KC_MS_UP,   KC_NO,       KC_NO,  KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,        KC_NO,  KC_NO,        KC_NO,        KC_MS_WH_UP,
     KC_NO, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, KC_NO,  KC_NO,  KC_NO,    KC_BTN1,  KC_BTN2,  KC_BTN3, KC_NO,        KC_NO,  KC_NO,        KC_NO,        KC_MS_WH_DOWN,
     KC_NO, KC_NO,      KC_NO,      KC_NO,       KC_NO,  KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,        KC_NO,  KC_NO,        KC_NO,        KC_NO,
     KC_NO, KC_NO,      KC_NO,      KC_NO,       KC_NO,  KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,   TO(_MOUSE+1), KC_NO,  KC_MS_ACCEL0, KC_MS_ACCEL1, KC_MS_ACCEL2),
#ifdef RGBLIGHT_ENABLE
    [_RGBLIGHTING] = LAYOUT(
          TO(0), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_TOGGLE,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  RGBLIGHT_MODE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_INCREASE_VAL,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_DECREASE_VAL,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_INCREASE_HUE, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TO(_RGBLIGHTING+1), KC_NO, RGBLIGHT_INCREASE_SAT, RGBLIGHT_DECREASE_HUE, RGBLIGHT_DECREASE_SAT),
#endif
#ifdef RGB_MATRIX_ENABLE
    [_RGB_MATRIX] = LAYOUT(
          TO(0), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MATRIX_TOGGLE,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MATRIX_DECREASE_SPD, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MATRIX_INCREASE_SPD, KC_NO, KC_NO,
  RGB_MATRIX_MODE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MATRIX_INCREASE_VAL,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MATRIX_DECREASE_VAL,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MATRIX_INCREASE_HUE, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TO(_RGB_MATRIX+1), KC_NO, RGB_MATRIX_INCREASE_SAT, RGB_MATRIX_DECREASE_HUE, RGB_MATRIX_DECREASE_SAT),
#endif
    [_UPDATE] = LAYOUT(
          TO(0), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RESET, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TO(0), KC_NO, KC_NO, KC_NO, KC_NO),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
#ifdef RGBLIGHT_ENABLE
		case RGBLIGHT_TOGGLE:
			if (record->event.pressed)
				rgblight_toggle();
			return false;
		case RGBLIGHT_MODE:
			if (record->event.pressed)
				rgblight_step();
			return false;
		case RGBLIGHT_RMODE:
			if (record->event.pressed)
				rgblight_step_reverse();
			return false;
		case RGBLIGHT_INCREASE_HUE:
			if (record->event.pressed)
				rgblight_increase_hue();
			return false;
		case RGBLIGHT_DECREASE_HUE:
			if (record->event.pressed)
				rgblight_decrease_hue();
			return false;
		case RGBLIGHT_INCREASE_SAT:
			if (record->event.pressed)
				rgblight_increase_sat();
			return false;
		case RGBLIGHT_DECREASE_SAT:
			if (record->event.pressed)
				rgblight_decrease_sat();
			return false;
		case RGBLIGHT_INCREASE_VAL:
			if (record->event.pressed)
				rgblight_increase_val();
			return false;
		case RGBLIGHT_DECREASE_VAL:
			if (record->event.pressed)
				rgblight_increase_val();
			return false;
#endif
#ifdef RGB_MATRIX_ENABLE
		case RGB_MATRIX_TOGGLE:
			if (record->event.pressed)
				rgb_matrix_toggle();
			return false;
		case RGB_MATRIX_MODE:
			if (record->event.pressed)
				rgb_matrix_step();
			return false;
		case RGB_MATRIX_RMODE:
			if (record->event.pressed)
				rgb_matrix_step_reverse();
			return false;
		case RGB_MATRIX_INCREASE_HUE:
			if (record->event.pressed)
				rgb_matrix_increase_hue();
			return false;
		case RGB_MATRIX_DECREASE_HUE:
			if (record->event.pressed)
				rgb_matrix_decrease_hue();
			return false;
		case RGB_MATRIX_INCREASE_SAT:
			if (record->event.pressed)
				rgb_matrix_increase_sat();
			return false;
		case RGB_MATRIX_DECREASE_SAT:
			if (record->event.pressed)
				rgb_matrix_decrease_sat();
			return false;
		case RGB_MATRIX_INCREASE_VAL:
			if (record->event.pressed)
				rgb_matrix_increase_val();
			return false;
		case RGB_MATRIX_DECREASE_VAL:
			if (record->event.pressed)
				rgb_matrix_increase_val();
			return false;
		case RGB_MATRIX_INCREASE_SPD:
			if (record->event.pressed)
				rgb_matrix_increase_speed();
			return false;
		case RGB_MATRIX_DECREASE_SPD:
			if (record->event.pressed)
				rgb_matrix_increase_speed();
			return false;
#endif
	}
	return true;
}

#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = {
{
	/* Key Matrix to LED Index */
	/* Row0
	   ESC    F1  F2  F3  F4  F5  F6  F7  F8  F9  F10  F11  F12  PScr  Del*/
	{   87,   86, 85, 84, 83, 82, 81, 80, 79, 78, 77,  76,  75,  74,   72},
	/* Row1
	  BKQ    1   2   3   4   5   6   7   8   9   0   -   =   Pause  Home*/
	{  57,   58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,   73,   71},
	/* Row2
	  Tab    Q   W   E   R   T   Y   U   I   O   P   [   ]   BSPACE PgUp*/
	{  56,   55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44,   70,   41},
	/* Row3
	CapsLck A   S   D   F   G   H   J   K   L   ;   '   #   ISOEnt PgDn*/
	{   26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,   42,   40},
	/* Row4
	LShft    \   Z   X   C   V   B   N   M  Comma Period Slash RShft  UArrow  End */
	{   25,  23, 22, 21, 20, 19, 18, 17, 16,  15,   14,    13,   12,    11,   10},
	/* Row5
	  LCtrl Win Alt                           SPACE                   AltGr Menu RCtrl RArrow DArrow RArrow */
	{   0,   1,  2,  NO_LED, NO_LED, NO_LED,    3,   NO_LED, NO_LED,    4,    5,   6,    7,     8,     9},
}, {
	/* LED Index to Physical Position */
	//   0        1        2        3         4         5        6         7         8          9
	// LCtrl     Win      Alt     Space     AltGr     Menu     RtCtrl     Left      Down      Right
	  {2,64},  {20,64}, {39,64}, {95,64}, {150,64}, {165,64}, {180,64}, {195,64}, {210,64}, {224,64},

	//   10       11        12        13        14       15         16        17        18       19       20       21       22       23      24        25
	//  End       Up      RShift      /         :         ,         M         N         B        V        C        X        Z,       \       NA      LShift
	 {224,51}, {210,51}, {188,51}, {168,51}, {153,51}, {138,51}, {123,51}, {108,51}, {93,51}, {78,51}, {63,51}, {48,51}, {33,51}, {19,51}, {11,51},  {2,51},

	//    26         27       28       29       30       31        32        33        34        35        36        37        38      39             40
	// Caps Lock     A,       S        D        F        G         H         J         K         L         ;         '         #     Enter ANSI,   Page Down
	    {6,38},   {26,38}, {41,38}, {56,38}, {71,38}, {86,38}, {101,38}, {115,38}, {130,38}, {145,38}, {160,38}, {175,38}, {190,38}, {200,38},     {224,38},

	//   41        42         43       44        45        46        47        48        49        50       51       52       53       54       55       56
	// Page Up  EnterISO   AinsiBSl     ]        (         P         O         I         U         Y        T        R        E        Z,       A        Tab
	  {224,26}, {206,32},  {204,26}, {186,26}, {171,26}, {156,26}, {142,26}, {127,26}, {112,26}, {97,26}, {82,26}, {67,26}, {52,26}, {37,26}, {22, 26}, {4,26},

	//   57      58       59       60       61       62       63       64        65        66        67        68        69          70        71
	//  BQK      1        2        3        4        5        6        7         8         9,        0,        -         +        Backspace,  Home
	  {0,13}, {15,13}, {30,13}, {45,13}, {60,13}, {75,13}, {90,13}, {105,13}, {120,13}, {135,13}, {150,13}, {165,13}, {180, 13}, {201, 13}, {224,13},

	//   72        73       74       75       76       77       78       79       80      81      82      83      84      85      86     87
	// Delete    Pause   PrntScr    F12      F11      F10       F9       F8       F7      F6      F5      F4      F3      F2      F1     Esc
	  {224, 0}, {210,0}, {195,0}, {180,0}, {165,0}, {150,0}, {135,0}, {120,0}, {105,0}, {90,0}, {75,0}, {60,0}, {45,0}, {30,0}, {15,0}, {0,0},
}, {
	/* LED Index to Flag by index*/
	//   0    1    2     3     4     5     6     7     8     9
	// LCtrl Win  Alt  Space AltGr Menu  RtCtrl Left  Down Right
	    0x1, 0x4, 0x1,  0x4,  0x1,  0x4,  0x1,  0x4,  0x4,  0x4,

	//   10  11     12    13   14   15   16    17  18   19   20   21   22   23   24    25
	//  End  Up   RShift  /    :    ,    M     N   B    V    C    X    Z,   \    NA   LShift
	    0x4, 0x4,  0x1,   0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0,  0x1,

	//    26      27   28   29   30   31   32   33   34   35   36   37   38      39           40
	// Caps Lock  A,   S    D    F    G    H    J    K    L    ;    '    #    Enter ANSI,  Page Down
	    0xD,      0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,    0x4,         0x4,

	//   41        42        43     44    45   46   47   48   49   50   51   52   53   54   55   56
	// Page Up  EnterISO  AinsiBSl  ]     (    P    O    I    U    Y    T    R    E    Z,   A    Tab
	     0x4,      0x4,     0x0,    0x4,  0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,

	//   57   58  59   60   61   62   63   64   65   66   67   68   69      70        71
	//  BQK   1   2    3    4    5    6    7    8    9,   0,   -    +    Backspace,  Home
	    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,    0x4,     0x4,

	//   72     73      74   75   76   77   78   79   80   81   82   83   84   85   86   87
	// Delete Pause  PrntScr F12  F11  F10  F9   F8   F7   F6   F5   F4   F3   F2   F1   Esc
	   0x4,   0x4,   0x4,    0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
} };
#endif

