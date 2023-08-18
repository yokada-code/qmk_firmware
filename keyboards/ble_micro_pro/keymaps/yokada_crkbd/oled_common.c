#include QMK_KEYBOARD_H
#include "bmp.h"
#include "oled.h"

static bool is_ble_advertising = false;

void set_bt_connection_status_str(void){
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
                ble_con_status[0] = '-';
                ble_con_hostname[0] = '-';
                ble_con_hostname[1] = '-';
                ble_con_hostname[2] = '-';
                ble_con_hostname[3] = '-';
                ble_con_hostname[4] = '\0';
            } else {
                ble_con_status[0] = '0' + (stat & 0xff);

                for (i = 0; i < peer_cnt; i++) {
                     if (peers[i].id == (stat & 0xff)) {
                         hostname_full = peers[i].name;
                         break;
                     }
                }

                for (i=0; i<CON_STATUS_STR_LEN-1 && hostname_full[i] != '\0'; i++) {
                    ble_con_hostname[i] = (char)hostname_full[i];
                }
                ble_con_hostname[i] = '\0';
            }
        } else if (is_usb_connected()){
            ble_con_status[0] = '-';
            ble_con_hostname[0] = ' ';
            ble_con_hostname[1] = 'U';
            ble_con_hostname[2] = 'S';
            ble_con_hostname[3] = 'B';
            ble_con_hostname[4] = '\0';
        } else {
            ble_con_status[0] = '-';
            ble_con_hostname[0] = '-';
            ble_con_hostname[1] = '-';
            ble_con_hostname[2] = '-';
            ble_con_hostname[3] = '-';
            ble_con_hostname[4] = '\0';
        }
    } else {
        ble_con_status[0] = 'M';
        ble_con_hostname[0] = '\0';
    }
    ble_con_status[1] = ':';
    ble_con_status[2] = '0' + (char) (bonding_map >> 8);
    ble_con_status[3] = '0' + (char) (bonding_map & 0xff);
    ble_con_status[4] = '\0';
}

__attribute__((weak)) void bmp_state_change_cb_user(bmp_api_event_t event) {}

void bmp_state_change_cb_kb(bmp_api_event_t event) {

    switch (event) {
        case BLE_ADVERTISING_START:
            is_ble_advertising = true;
            break;

        case BLE_ADVERTISING_STOP:
            is_ble_advertising = false;
            set_bt_connection_status_str();
            break;

        case BLE_CONNECTED:
            is_ble_advertising = false;
            set_bt_connection_status_str();
        case BLE_DISCONNECTED:
            //is_ble_advertising = false;
            break;

        default:
            break;
    }
    bmp_state_change_cb_user(event);
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
