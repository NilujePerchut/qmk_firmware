/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"
#include "rgb.h"
#include "rgb_matrix_types.h"

//#include "pwm.c"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

const led_config_t g_led_config = { {
	// Key Matrix to Led Index
		{15 , 14 , 13 , 12 , 11 , 10 , 9 ,  8 ,  7 ,  6 ,  5 ,  4 ,  3 ,  2 ,  0},
		{16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 1  , 30},
		{44 , 43 , 42 , 41 , 40 , 39 , 38 , 37 , 36 , 35 , 34 , 33 , 32 , 29 , 31},
		{45 , 46 , 47 , 48 , 49 , 50 , 51 , 52 , 53 , 54 , 55 , 56 , 57 , 58 , 59},
		{74 , 73 , 72 , 71 , 70 , 69 , 68 , 67 , 66 , 65 , 64 , 63 , 62 , 61 , 60},
		{75 , 76 , 77 , NO_LED , NO_LED , NO_LED , 78 , NO_LED , NO_LED , 79 , 80 , 81 , 82 , 83 , 84},
	}, {
		// LED Index to Physical Position
		// Delete, Pause, PrintScreen, F12, F11, F10, F9, F8, F7, F6, F5, F4, F3, F2, F1, Escape
		{224, 0}, {210,0}, {195,0}, {180,0}, {165,0}, {150,0}, {135,0}, {120,0}, {105,0}, {90,0}, {75,0}, {60,0}, {45,0}, {30,0}, {15,0}, {0,0},

		// Squared, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, Degree, Plus, Backspace, Home
		{0,13}, {15,13}, {30,13}, {45,13}, {60,13}, {75,13}, {90,13}, {105,13}, {120,13}, {135,13}, {150,13}, {165,13}, {180, 13}, {201, 13}, {224,13},

		// Page Up, Pound, Trema, P, O, I, U, Y, T, R, E, Z, A, Tab
		{224,26}, {186,26}, {171,26}, {156,26}, {142,26}, {127,26}, {112,26}, {97,26}, {82,26}, {67,26}, {52,26}, {37,26}, {22, 26}, {4,26},

		// Caps Lock, Q, S, D, F, G, H, J, K, L, M, Percent, Micro, Enter, Page Down
		{6,38}, {26,38}, {41,38}, {56,38}, {71,38}, {86,38}, {101,38}, {115,38}, {130,38}, {145,38}, {160,38}, {175,38}, {190,38}, {209,26}, {224,38},

		// End, Up, Right Shift, Paragraph, Slash, Dot, Question, N, B, V, C, X, W, Chevron, Left Shift
		{224,51}, {210,51}, {188,51}, {168,51}, {153,51}, {138,51}, {123,51}, {108,51}, {93,51}, {78,51}, {63,51}, {48,51}, {33,51}, {19,51}, {2,51},

		// Left Control, Win, Alt, Space, AltGr, Menu, Right Control, Left, Down, Right
		{2,64}, {20,64}, {39,64}, {95,64}, {150,64}, {165,64}, {180,64}, {195,64}, {210,64}, {224,64},
	}, {
		// LED Index to Flag
		1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1,
		1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
		1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1,
		1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
		1, 1, 1, 4, 1, 1, 1, 1, 1, 1,
	}
};

inline uint8_t matrix_rows(void){
  return MATRIX_ROWS;
}

inline uint8_t matrix_cols(void){
  return MATRIX_COLS;
}

__attribute__ ((weak))
void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
    matrix_scan_user();
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

void matrix_init(void)
{
  // initialize row and col
  unselect_rows();
  init_cols();
  // initialize matrix state: all keys off
  for (uint8_t i=0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
    matrix_debouncing[i] = 0;
  }
  //debug
  debug_matrix = true;
  matrix_init_quantum();
}

uint8_t matrix_scan(void){
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    select_row(i);
    wait_us(30);  // without this wait read unstable value.
    matrix_row_t cols = read_cols();
    if (matrix_debouncing[i] != cols) {
      matrix_debouncing[i] = cols;
      if (debouncing) {
        debug("bounce!: "); debug_hex(debouncing); debug("\n");
      }
    debouncing = DEBOUNCE;
    }
    unselect_rows();
  }

  if (debouncing) {
    if (--debouncing) {
      wait_ms(1);
    } else {
      for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = matrix_debouncing[i];
      }
    }
  }
  matrix_scan_quantum();
  return 1;
}

inline bool matrix_is_on(uint8_t row, uint8_t col){
  return (matrix[row] & ((matrix_row_t)1<<col));
}

inline matrix_row_t matrix_get_row(uint8_t row){
  return matrix[row];
}

void matrix_print(void){
  print("\nr/c 0123456789ABCDEF\n");
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    phex(row); print(": ");
    pbin_reverse16(matrix_get_row(row));
    print("\n");
  }
}

/* Custom Row and column indexes

Initial plan (according to Schematics and PCB)
----------------------------------------------
    B13  B14  B15  A8   A10  A11  A12  A15  B3   B4   B5   B7   B8      B9     A1
B6  ESC| F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8 | F9 | F10| F11| F12| PSC| PAU| DEL
A9  GRA| 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 0  | '  | ¡  |   BCKS  | HOM
B12 TAB| Q  | W  | E  | R  | T  | Y  | U  | I  | O  | P  | `  | +  |         | PGU
B11 CAP| A  | S  | D  | F  | G  | H  | J  | K  | L  | Ñ  | ´  | Ç  |   ENTER | PGD
B10 LSI| <  | Z  | X  | C  | V  | B  | N  | M  | ,  | .  | -  |   RSHIF | UP | END
B1  LCT| WIN| ALT|               ESPACE             | ALG| FN | RCT| LEF| DOW| RIG

Problematic pins are:
---------------------
A11, A12 are used for USB

Remap
-----
A11 -> PB0
A12 -> PA7

Final mapping (after remap)
----------------------------------------------
    B13  B14  B15  A8   A10  B0   A7   A15  B3   B4   B5   B7   B8      B9     A1
B6  ESC| F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8 | F9 | F10| F11| F12| PSC| PAU| DEL
A9  GRA| 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 0  | '  | ¡  |   BCKS  | HOM
B12 TAB| Q  | W  | E  | R  | T  | Y  | U  | I  | O  | P  | `  | +  |         | PGU
B11 CAP| A  | S  | D  | F  | G  | H  | J  | K  | L  | Ñ  | ´  | Ç  |   ENTER | PGD
B10 LSI| <  | Z  | X  | C  | V  | B  | N  | M  | ,  | .  | -  |   RSHIF | UP | END
B1  LCT| WIN| ALT|               ESPACE             | ALG| FN | RCT| LEF| DOW| RIG

* */

/* Column pin configuration
 */
//  Modified by Xydane
static void  init_cols(void){
  palSetPadMode(GPIOB, 13, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 8, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 10, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 0, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 3, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 4, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 5, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 7, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 8, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 9, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_PULLUP);
}

/* Returns status of switches(1:on, 0:off) */
//  Modified by Xydane
static matrix_row_t read_cols(void){
  return ((palReadPad(GPIOB, 13)==PAL_HIGH) ? 0 : (1<<0))
    | ((palReadPad(GPIOB, 14)==PAL_HIGH) ? 0 : (1<<1))
    | ((palReadPad(GPIOB, 15)==PAL_HIGH) ? 0 : (1<<2))
    | ((palReadPad(GPIOA, 8)==PAL_HIGH) ? 0 : (1<<3))
    | ((palReadPad(GPIOA, 10)==PAL_HIGH) ? 0 : (1<<4))
    | ((palReadPad(GPIOB, 0)==PAL_HIGH) ? 0 : (1<<5))
    | ((palReadPad(GPIOA, 7)==PAL_HIGH) ? 0 : (1<<6))
    | ((palReadPad(GPIOA, 15)==PAL_HIGH) ? 0 : (1<<7))
    | ((palReadPad(GPIOB, 3)==PAL_HIGH) ? 0 : (1<<8))
    | ((palReadPad(GPIOB, 4)==PAL_HIGH) ? 0 : (1<<9))
    | ((palReadPad(GPIOB, 5)==PAL_HIGH) ? 0 : (1<<10))
    | ((palReadPad(GPIOB, 7)==PAL_HIGH) ? 0 : (1<<11))
    | ((palReadPad(GPIOB, 8)==PAL_HIGH) ? 0 : (1<<12))
    | ((palReadPad(GPIOB, 9)==PAL_HIGH) ? 0 : (1<<13))
    | ((palReadPad(GPIOA, 1)==PAL_HIGH) ? 0 : (1<<14));
}

/* Row pin configuration
 */
//  Modified by Xydane
static void unselect_rows(void){
  palSetPadMode(GPIOB, 6, PAL_MODE_INPUT);
  palSetPadMode(GPIOA, 9, PAL_MODE_INPUT);
  palSetPadMode(GPIOB, 12, PAL_MODE_INPUT);
  palSetPadMode(GPIOB, 11, PAL_MODE_INPUT);
  palSetPadMode(GPIOB, 10, PAL_MODE_INPUT);
  palSetPadMode(GPIOB, 1, PAL_MODE_INPUT);
}

//  Modified by Xydane
static void select_row(uint8_t row){
  (void)row;
  switch (row) {
    case 0:
      palSetPadMode(GPIOB, 6, PAL_MODE_OUTPUT_PUSHPULL);
      palClearPad(GPIOB, 6);
      break;
    case 1:
      palSetPadMode(GPIOA, 9, PAL_MODE_OUTPUT_PUSHPULL);
      palClearPad(GPIOA, 9);
      break;
    case 2:
      palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL);
      palClearPad(GPIOB, 12);
      break;
    case 3:
      palSetPadMode(GPIOB, 11, PAL_MODE_OUTPUT_PUSHPULL);
      palClearPad(GPIOB, 11);
      break;
    case 4:
      palSetPadMode(GPIOB, 10, PAL_MODE_OUTPUT_PUSHPULL);
      palClearPad(GPIOB, 10);
      break;
    case 5:
      palSetPadMode(GPIOB, 1, PAL_MODE_OUTPUT_PUSHPULL);
      palClearPad(GPIOB, 1);
      break;
  }
}
