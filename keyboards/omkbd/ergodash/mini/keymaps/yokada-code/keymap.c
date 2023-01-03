#include QMK_KEYBOARD_H


enum layer_names {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

#define EISU LALT(KC_GRV)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [0] = LAYOUT(KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_MINS, KC_EQL, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_RBRC, KC_NUHS, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_APP, KC_BSPC, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_INT1, KC_ESC, KC_LGUI, KC_LALT, MO(1), KC_INT5, KC_SPC, KC_DEL, KC_ENT, KC_INT4, MO(2), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT),
        [1] = LAYOUT(KC_TRNS, KC_NO, KC_UP, KC_NO, KC_PGUP, KC_NO, KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_NO, KC_NO, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_NO, KC_NO, KC_BSPC, KC_ENT, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_INT3, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(3), KC_HOME, KC_PGDN, KC_PGUP, KC_END),
        [2] = LAYOUT(KC_ESC, KC_NO, KC_7, KC_8, KC_9, KC_0, KC_PSLS, KC_NO, KC_NO, KC_HOME, KC_PSCR, KC_END, KC_MINS, KC_EQL, KC_TRNS, KC_NO, KC_4, KC_5, KC_6, KC_PAST, KC_PMNS, KC_NO, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_RBRC, KC_NUHS, KC_TRNS, KC_NO, KC_1, KC_2, KC_3, KC_PPLS, KC_TRNS, KC_TRNS, KC_BSPC, KC_ENT, KC_COMM, KC_DOT, KC_SLSH, KC_INT3, KC_TRNS, KC_TRNS, KC_TRNS, MO(3), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END),
        [3] = LAYOUT(KC_TRNS, QK_BOOT, RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI, KC_NO, KC_NO, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, KC_NO, KC_NO, KC_TRNS, KC_NO, BL_TOGG, BL_BRTG, BL_UP, BL_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
  }
  return true;
}

void keyboard_post_init_user(void) {
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

