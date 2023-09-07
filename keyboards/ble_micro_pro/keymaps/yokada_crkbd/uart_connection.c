#include QMK_KEYBOARD_H
#include "bmp_pin_def.h"
#include "uart_connection.h"
#include "oled.h"
#include "print.h"

#define INNER_BUF_LEN 128
#define UART_SEND_DATA_MAX 125
#define BUF_LEN 256

static uint8_t buf[BUF_LEN];
static uint8_t buf_p = 0;
static uart_cb callback;

typedef enum {
    WAIT_PREAMBLE,
    WAIT_LEN,
    RECV_DATA,
    RECV_SUM,
} recv_state_t;

static void data_recv_cb(uint8_t d) {
    static recv_state_t state = WAIT_PREAMBLE;
    static uint8_t len;
    static uint8_t checksum = 0;
    switch (state) {
        case WAIT_PREAMBLE:
            if (d == 'a') {
                state = WAIT_LEN;
            }
            break;
        case WAIT_LEN:
            len = d;
            buf_p = 0;
            state = RECV_DATA;
            break;
        case RECV_DATA:
            buf[buf_p++] = d;
            checksum ^= d;
            len--;
            if (len == 0){
                state = RECV_SUM;
            }
            break;
        case RECV_SUM:
            if (d != checksum) {
                log_info("recv failed, invalid checksum %x, expected: %x\n", d, checksum);
            } else {
                dprintf("recv data, len %d, checksum %x\n", buf_p, checksum);
                callback(buf, buf_p);
            }
            checksum = 0;
            state = WAIT_PREAMBLE;
            break;
    }
}

static char calc_checksum(uint8_t* data, uint8_t len) {
    uint8_t checksum = 0;
    for (uint8_t i=0; i<len; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

static void send_char_low(uint8_t* data, uint8_t len) {
    BMPAPI->uart.send(data, len);
}

void uart_send(uint8_t* data, uint8_t len) {
    uint8_t flush[INNER_BUF_LEN] = {0};
    uint8_t buf[INNER_BUF_LEN];
    uint8_t checksum;

    if (len > UART_SEND_DATA_MAX) {
        log_info("Assertion ERROR: uart send data is longer than INNER_BUF_LEN\n");
        return;
    }

    checksum = calc_checksum(data, len);
    buf[0] = 'a';     //preamble
    buf[1] = len;
    memcpy(&buf[2], data, len);
    buf[len+2] = checksum;

    send_char_low(buf, len+3);
    dprintf("Send data: len: %d, 0x%x\n", len, checksum);
    send_char_low(flush, INNER_BUF_LEN);
}

void uart_init(uart_cb f) {
    bmp_uart_config_t uconf = {
        .tx_pin      = (is_keyboard_master()) ? D2 : B6,
        .rx_pin      = (is_keyboard_master()) ? B6 : D2,
        .baudrate    = Baud1200,
        .rx_callback = data_recv_cb,
        .rx_protocol = 1
    };

    callback = f;
    BMPAPI->uart.init(&uconf);
}
