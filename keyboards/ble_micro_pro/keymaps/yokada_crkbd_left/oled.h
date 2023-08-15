#pragma once
#include QMK_KEYBOARD_H

enum crkbd_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

void render_bongocat(void);
bool process_record_user_luna(uint16_t keycode, keyrecord_t *record);
void print_status_luna(void);

