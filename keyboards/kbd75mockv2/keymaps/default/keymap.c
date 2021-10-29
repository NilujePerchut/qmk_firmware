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
	_BASE,
	_RGB_LIGHTING,
};

/* Custom KEYCODES */
enum custom_keycodes {
	/* RGB_LIGHT */
	RGBLIGHT_TOGGLE = SAFE_RANGE,
	RGBLIGHT_MODE,
	RGBLIGHT_RMODE,
	RGBLIGHT_INCREASE_HUE,
	RGBLIGHT_DECREASE_HUE,
	RGBLIGHT_INCREASE_SAT,
	RGBLIGHT_DECREASE_SAT,
	RGBLIGHT_INCREASE_VAL,
	RGBLIGHT_DECREASE_VAL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
    KC_ESC, KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11,  KC_F12,   KC_PSCREEN, KC_DELETE,
  KC_GRAVE, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS, KC_EQL,   KC_PAUSE,   KC_HOME,
    KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC, KC_RBRC,  KC_BSPACE,  KC_PGUP,
   KC_CAPS, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT, KC_NUHS,  KC_ENT,     KC_PGDOWN,
 KC_LSHIFT, KC_NUBS,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_RSHIFT,KC_UP,      KC_END,
   KC_LCTL, KC_LGUI,KC_LALT,KC_NO,  KC_NO,  KC_NO,  KC_SPC, KC_NO,  KC_NO,  KC_RALT,TO(1),   KC_RCTL, KC_LEFT, KC_DOWN,    KC_RIGHT),
    [_RGB_LIGHTING] = LAYOUT(
          TO(0), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_TOGGLE,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  RGBLIGHT_MODE, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_INCREASE_VAL,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_DECREASE_VAL,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGBLIGHT_INCREASE_HUE, KC_NO,
          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TO(0), KC_NO, RGBLIGHT_INCREASE_SAT, RGBLIGHT_DECREASE_HUE, RGBLIGHT_DECREASE_SAT),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		/* RGB_LIGHT */
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
	}
	return true;
}

