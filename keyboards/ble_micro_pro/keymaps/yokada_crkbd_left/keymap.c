/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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

#include QMK_KEYBOARD_H
#include "config.h"
#include "oled.h"
#include "debug.h"

#include "bmp.h"
#include "bmp_custom_keycode.h"
#include "keycode_str_converter.h"

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    LOWER = BMP_SAFE_RANGE,
    RAISE,
};

const key_string_map_t custom_keys_user =
{
    .start_kc = LOWER,
    .end_kc = RAISE,
    .key_strings = "LOWER\0RAISE\0"
};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    {{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I,
        KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    }}
};

uint32_t keymaps_len() {
  return 19;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    layer_state_t new;
    new = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
#ifdef CONSOLE_ENABLE
    dprintf("state: %u, new: %u\n", state, new);
#endif
    return new;
}

bool is_keyboard_master(void) {
    return IS_LEFT_HAND;
}

///////////////////////////
// keyboard initialization
///////////////////////////
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    } else {
        return OLED_ROTATION_0;
    }
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_luna();
    } else {
        render_bongocat();
    }
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool continue_process = process_record_user_bmp(keycode, record);
    process_record_user_luna(keycode, record);
    if (continue_process == false) {
        return false;
    }
    return true;
}

