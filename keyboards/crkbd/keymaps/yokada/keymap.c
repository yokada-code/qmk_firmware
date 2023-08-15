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
#include "print.h"

///////////////////////////
// keymap related items
///////////////////////////
#define QK_BOOT RESET
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x6_3(KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_INT1, MO(1), KC_INT5, KC_SPC, MO(2), KC_INT4, KC_RALT),
	[1] = LAYOUT_split_3x6_3(KC_ESC, KC_BTN2, KC_UP, KC_NO, KC_PGUP, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_NO, KC_NO, KC_BSPC, KC_ENT, KC_DEL, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[2] = LAYOUT_split_3x6_3(KC_ESC, KC_NO, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_HOME, KC_PSCR, KC_END, KC_MINS, KC_NO, KC_TRNS, KC_NO, KC_4, KC_5, KC_6, KC_RBRC, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_EQL, KC_ENT, KC_TRNS, KC_NO, KC_1, KC_2, KC_3, KC_BSLS, KC_BSPC, KC_NO, KC_COMM, KC_DOT, KC_SLSH, KC_INT3, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[3] = LAYOUT_split_3x6_3(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_MOD, RGB_HUD, RGB_HUD, RGB_VAD, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)

layer_state_t layer_state_set_user(layer_state_t state) {
    layer_state_t new;
    new = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
#ifdef CONSOLE_ENABLE
    uprintf("state: %u, new: %u\n", state, new);
#endif 
    return new;
}

///////////////////////////
// keyboard initialization
///////////////////////////
void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    // Customise these values to desired behaviour
    debug_enable=true;
    debug_matrix=true;
    //debug_keyboard=true;
    //debug_mouse=true;
#endif 
}

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
    if (continue_process == false) {
        return false;
    }
    process_record_user_luna(keycode, record);
    return true;
}

