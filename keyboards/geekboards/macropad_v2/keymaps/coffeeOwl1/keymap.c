/* Copyright 2020 Geekboards ltd. (geekboards.ru / geekboards.de)
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
#include "print.h"

#define _BL 0
#define _SL 1
#define _TL 2

#define TAB_CLOSE LGUI(KC_W)
#define TAB_UNCLOSE LGUI(LSFT(KC_T))
#define SCREEN_LEFT LCTL(KC_LEFT)
#define SCREEN_RIGHT LCTL(KC_RGHT)
#define TAB_LEFT LGUI(LSFT(KC_LBRC))
#define TAB_RIGHT LGUI(LSFT(KC_RBRC))
#define FIND LGUI(KC_F)
#define WINDOW_LEFT_HALF LALT(LGUI(KC_LEFT))
#define WINDOW_RIGHT_HALF LALT(LGUI(KC_RGHT))
#define WINDOW_CENTER LCTL(LALT(LSFT(LGUI(KC_C))))
#define SUPER_TAB LGUI(KC_TAB)
#define MISSION_CONTROL LCTL(KC_UP)

enum {
    INTERN_CODE_SEARCH,
    DEL_LINE
};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BL] = LAYOUT_ortho_2x4(
        OSL(_SL), TAB_CLOSE, FIND, MT(MOD_LGUI, KC_ENT),
        SCREEN_LEFT, TAB_LEFT, TAB_RIGHT, SCREEN_RIGHT
    ),
    [_SL] = LAYOUT_ortho_2x4(
        KC_ESC, TAB_UNCLOSE, INTERN_CODE_SEARCH, KC_BSPC,
        WINDOW_LEFT_HALF, SUPER_TAB, MISSION_CONTROL, WINDOW_RIGHT_HALF
    ),
    [_TL] = LAYOUT_ortho_2x4(
        KC_TRNS, KC_TRNS, KC_TRNS, DEL_LINE,
        KC_TRNS, WINDOW_LEFT_HALF, WINDOW_CENTER, WINDOW_RIGHT_HALF
    )
};
// TODO add delete line

layer_state_t layer_state_set_user(layer_state_t state) {
    #ifdef CONSOLE_ENABLE
    uprintf("L: %u\n", get_highest_layer(state));
    #endif
    switch (get_highest_layer(state)) {
    case _BL:
        rgblight_disable();
        // rgblight_setrgb (0x00,  0x00, 0xFF); //Blue
        break;
    case _SL:
        rgblight_enable();
        rgblight_setrgb (0x00,  0x00, 0xFF); //Blue
        // rgblight_setrgb (0xFF,  0x00, 0x00); //Red
        // rgblight_sethsv(255, 255, 50);
        break;
    case _TL:
        rgblight_enable();
        rgblight_setrgb (0x00,  0xFF, 0x00); //Green?
        // rgblight_sethsv(255, 255, 50);
        break;
    }
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    #ifdef CONSOLE_ENABLE
    // uprintf("KL: kc: %u, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    #endif
    switch (keycode) {
        case INTERN_CODE_SEARCH:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_T);
                unregister_code(KC_LGUI);
                unregister_code(KC_T);
                SEND_STRING("xbgs ");
                register_code(KC_LGUI);
                register_code(KC_V);
                unregister_code(KC_LGUI);
                unregister_code(KC_V);
                register_code(KC_ENT);
                unregister_code(KC_ENT);
            }
            return false;
        case DEL_LINE:
            if (record->event.pressed) {
                // Move to the end of line
                register_code(KC_LGUI);
                register_code(KC_RGHT);
                unregister_code(KC_RGHT);
                unregister_code(KC_LGUI);
                // Select to the start of the line
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                register_code(KC_LEFT);
                register_code(KC_LEFT);
                unregister_code(KC_LEFT);
                unregister_code(KC_LEFT);
                unregister_code(KC_LSFT);
                unregister_code(KC_LGUI);
                // Delete
                tap_code(KC_BSPC);
            }
            return false;
    }
    return true;
};

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
//   debug_enable=true;
//   debug_matrix=true;
//   debug_keyboard=true;
  //debug_mouse=true;
}
