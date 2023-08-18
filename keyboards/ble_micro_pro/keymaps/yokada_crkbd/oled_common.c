#include QMK_KEYBOARD_H
#include "bmp.h"
#include "oled.h"

static bool is_ble_advertising = false;

__attribute__((weak)) void bmp_state_change_cb_user(bmp_api_event_t event) {}

void bmp_state_change_cb_kb(bmp_api_event_t event) {

    switch (event) {
        case BLE_ADVERTISING_START:
            is_ble_advertising = true;
            break;

        case BLE_ADVERTISING_STOP:
            is_ble_advertising = false;
            break;

        case BLE_CONNECTED:
            is_ble_advertising = false;
        case BLE_DISCONNECTED:
            //is_ble_advertising = false;
            break;

        default:
            break;
    }
    bmp_state_change_cb_user(event);
}

void get_bt_connection_status_str(char *status_str, char *hostname){
    bmp_api_bonding_info_t peers[8];
    uint32_t peer_cnt = sizeof(peers)/sizeof(peers[0]);
    uint16_t bonding_map = 0;

    BMPAPI->ble.get_bonding_info(peers, &peer_cnt);
    for (int i = 0; i < peer_cnt; i++) {
         bonding_map |= 1 << peers[i].id;
    }

    if (is_keyboard_master()) {
        if (is_ble_connected()){
            uint8_t *hostname_full;
            uint16_t stat = BMPAPI->ble.get_connection_status();
            int i;

            if ((stat >> 8) == 0) {
                status_str[0] = '-';
                hostname[0] = ' ';
                hostname[1] = 'N';
                hostname[2] = '/';
                hostname[3] = 'A';
                hostname[4] = '\0';
            } else {
                status_str[0] = '0' + (stat & 0xff);

                for (i = 0; i < peer_cnt; i++) {
                     if (peers[i].id == (stat & 0xff)) {
                         hostname_full = peers[i].name;
                         break;
                     }
                }

                for (i=0; i<OLED_HOSTNAME_MAX-1 && hostname_full[i] != '\0'; i++) {
                    hostname[i] = (char)hostname_full[i];
                }
                hostname[i] = '\0';
            }
        } else if (is_usb_connected()){
            status_str[0] = '-';
            hostname[0] = ' ';
            hostname[1] = 'U';
            hostname[2] = 'S';
            hostname[3] = 'B';
            hostname[4] = '\0';
        }
    } else {
        status_str[0] = 'M';
        hostname[0] = '\0';
    }
    status_str[1] = ':';
    status_str[2] = '0' + (char) (bonding_map >> 8);
    status_str[3] = '0' + (char) (bonding_map & 0xff);
    status_str[4] = '\0';
}

char get_bt_advertisement_status_char(void){
    if (is_ble_advertising){
        uint8_t advertise_to = get_advertise_to();
        if (advertise_to == 255) {
            return 'A';
        }
        return '0' + (char) advertise_to;
    }
    return '-';
}
