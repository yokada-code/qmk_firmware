#include QMK_KEYBOARD_H
#include "bmp.h"
#include "oled.h"
#include "print.h"
#include <string.h>
#include "apidef.h"

uint8_t display_flags = 0;
static bool is_ble_advertising = false;

uint8_t ble_con_status[CON_STATUS_STR_LEN];
uint8_t ble_con_hostname[CON_STATUS_STR_LEN];

static char get_hex_char(uint8_t i){
    if (i<10) {
        return '0' + (char) i;
    } else if (i<16) {
        return 'a' + (char) i-10;
    } else {
        return '!';
    }
}

void update_bt_connection_status_str(void){
    bmp_api_bonding_info_t peers[8];
    uint32_t peer_cnt = sizeof(peers)/sizeof(peers[0]);
    uint8_t bonding_map = 0;

    BMPAPI->ble.get_bonding_info(peers, &peer_cnt);
    for (int i = 0; i < peer_cnt; i++) {
         bonding_map |= 1 << peers[i].id;
    }

    if (is_keyboard_master()) {
        if (get_ble_enabled()){
            uint8_t *hostname_full = NULL;
            uint16_t stat = BMPAPI->ble.get_connection_status();
            int i;

            log_info("Ble connection status: 0x%04x\n", stat);

            if ((stat >> 8) == 0) {
                ble_con_status[0] = '-';
                memcpy(ble_con_hostname, "----", CON_STATUS_STR_LEN);
            } else {
                ble_con_status[0] = get_hex_char(stat & 0xff);

                for (i = 0; i < peer_cnt; i++) {
                     if (peers[i].id == (stat & 0xff)) {
                         hostname_full = peers[i].name;
                         break;
                     }
                }

                memcpy(ble_con_hostname, hostname_full, CON_STATUS_STR_LEN);
                ble_con_hostname[CON_STATUS_STR_LEN-1] = '\0';   // chop the string with the display length
            }
        } else if (get_usb_enabled()){
            ble_con_status[0] = '-';
            memcpy(ble_con_hostname, " USB", CON_STATUS_STR_LEN);
        } else {
            ble_con_status[0] = '!';
            memcpy(ble_con_hostname, " ERR", CON_STATUS_STR_LEN);
        }
    } else {
        ble_con_status[0] = 'M';
        ble_con_hostname[0] = '\0';
    }
    ble_con_status[1] = ':';
    ble_con_status[2] = get_hex_char(bonding_map >> 4);
    ble_con_status[3] = get_hex_char(bonding_map & 0xf);
    ble_con_status[4] = '\0';
}

void bmp_select_changed_user_cb() {
    update_bt_connection_status_str();
}

uint32_t bmp_con_state_changed_timer = 0;
bool bmp_con_state_changed = false;

__attribute__((weak)) void bmp_state_change_cb_user(bmp_api_event_t event) {}

void bmp_state_change_cb_kb(bmp_api_event_t event) {
    char *event_str = "";
    uint16_t stat = BMPAPI->ble.get_connection_status();

    switch (event) {
        case USB_CONNECTED:
            event_str = "USB_CONNECTED";
            break;
        case USB_DISCONNECTED:
            event_str = "USB_DISCONNECTED";
            break;
        case BLE_CONNECTED:
            event_str = "BLE_CONNECTED";
            is_ble_advertising = false;
            break;
        case BLE_DISCONNECTED:
            event_str = "BLE_DISCONNECTED";
            break;
        case BLE_ADVERTISING_START:
            event_str = "BLE_ADVERTISING_START";
            is_ble_advertising = true;
            break;
        case BLE_ADVERTISING_STOP:
            event_str = "BLE_ADVERTISING_STOP";
            is_ble_advertising = false;
            break;
        case USB_CDC_ACM_OPEND:
            event_str = "USB_CDC_ACM_OPEND";
            break;
        case USB_CDC_ACM_CLOSED:
            event_str = "USB_CDC_ACM_CLOSED";
            break;
        case USB_HID_READY:
            event_str = "USB_HID_READY";
            break;
        default:
            event_str = "UNKNOWN";
            break;
    }

    log_info("State changed event: %s, connection status: 0x%04x\n", event_str, stat);
    bmp_con_state_changed_timer = timer_read32();
    bmp_con_state_changed = true;
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
