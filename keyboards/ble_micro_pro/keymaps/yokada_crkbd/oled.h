#pragma once

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
void update_bt_connection_status_str(void);

#define CON_STATUS_STR_LEN 5
extern uint8_t ble_con_status[CON_STATUS_STR_LEN];
extern uint8_t ble_con_hostname[CON_STATUS_STR_LEN];

#define BMP_USER_DATA_WPM     1
#define BMP_USER_DATA_FLAGS   2

extern uint8_t display_flags;

#define BMP_USER_FLAG_OLED_ON 1

extern uint32_t bmp_con_state_changed_timer;
extern bool bmp_con_state_changed;

#define BMP_CON_STATE_CHANGED_DULATION  1500
