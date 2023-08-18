#pragma once
#include QMK_KEYBOARD_H

enum crkbd_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

bool process_record_user_luna(uint16_t keycode, keyrecord_t *record);
void print_status_luna(void);
void print_status_bongo(void);

char get_bt_advertisement_status_char(void);
void get_bt_connection_status_str(char *status_str, char *host_name);

#define CON_STATUS_STR_LEN 5
uint8_t ble_con_status[CON_STATUS_STR_LEN];
uint8_t ble_con_hostname[CON_STATUS_STR_LEN];
