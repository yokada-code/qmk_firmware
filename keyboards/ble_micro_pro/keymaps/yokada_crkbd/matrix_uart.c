#include QMK_KEYBOARD_H
#include "bmp_pin_def.h"
#include "bmp_matrix.h"
#include "matrix_uart.h"
#include "bmp_debounce.h"

bmp_uart_keystate_t uart_buf;
uint8_t uart_buf_p = 0;

void bmp_uart_cb(uint8_t recv) {
    uart_buf.dat[uart_buf_p++] = recv;
    if (uart_buf_p == sizeof(uart_buf)) {
        uart_buf_p = 0;
    }
}

void matrix_init_user(void) {
    bmp_uart_config_t uconf = {
        .tx_pin      = (is_keyboard_master()) ? D2 : B6,
        .rx_pin      = (is_keyboard_master()) ? B6 : D2,
        .baudrate    = Baud1200,
        .rx_callback = bmp_uart_cb,
        .rx_protocol = 1
    };

    BMPAPI->uart.init(&uconf);
}

#define DEFAULT_MATRIX_ROWS 32
static matrix_row_t matrix_dummy[DEFAULT_MATRIX_ROWS];
static matrix_row_t matrix_debouncing[DEFAULT_MATRIX_ROWS];
extern const bmp_matrix_func_t  matrix_func_row2col;
static const bmp_matrix_func_t *matrix_func = &matrix_func_row2col;

uint8_t matrix_scan_impl(matrix_row_t *_matrix) {
    const bmp_api_config_t *config     = BMPAPI->app.get_config();
    const uint8_t           device_row = matrix_func->get_device_row();
    const uint8_t           device_col = matrix_func->get_device_col();
    uint8_t                 matrix_offset =
        config->matrix.is_left_hand ? 0 : config->matrix.rows - device_row;
    int matrix_changed = 0;

    uint32_t raw_changed = matrix_func->scan(matrix_debouncing);
    bmp_api_key_event_t key_state[16];
    matrix_changed = bmp_debounce(
        matrix_debouncing + matrix_offset, matrix_dummy + matrix_offset,
        device_row, device_col,
        config->matrix.debounce * MAINTASK_INTERVAL, raw_changed, key_state);

    for (uint8_t i; i<matrix_changed; i++) {
        key_state[i].row += matrix_offset;
    }

    if (!is_keyboard_master()) { //SLAVE
        uart_buf.count = matrix_changed;
        memcpy(uart_buf.key_state, key_state, 
               sizeof(bmp_api_key_event_t) * matrix_changed);
    }
    BMPAPI->uart.send(uart_buf.dat, sizeof(uart_buf));

    if (debug_config.keyboard && matrix_changed > 0) {
        dprintf("device rows:\n");
        for (uint8_t idx = 0; idx < device_row; idx++) {
            if (device_col <= 8) {
                dprintf("\tdr%02d:0x%02x\n", idx,
                        matrix_debouncing[idx + matrix_offset]);
            } else if (device_col <= 16) {
                dprintf("\tdr%02d:0x%04x\n", idx,
                        matrix_debouncing[idx + matrix_offset]);
            } else {
                dprintf("\tdr%02d:0x%08x\n", idx,
                        matrix_debouncing[idx + matrix_offset]);
            }
        }
        dprintf("\n");
    }

    uint32_t pop_cnt = matrix_changed;
    if (is_keyboard_master()) { //MASTER
        pop_cnt += uart_buf.count;
        memcpy(&key_state[matrix_changed], uart_buf.key_state,
               sizeof(bmp_api_key_event_t) * uart_buf.count);
    }

    for (uint32_t i = 0; i < pop_cnt; i++) {
        if (key_state[i].state == 0) {
            _matrix[key_state[i].row] &= ~(1 << key_state[i].col);
        } else {
            _matrix[key_state[i].row] |= (1 << key_state[i].col);
        }
    }

    if (debug_config.keyboard && pop_cnt > 0) {
        dprintf("matrix rows:\n");
        for (uint8_t idx = 0; idx < config->matrix.rows; idx++) {
            if (config->matrix.cols <= 8) {
                dprintf("\tr%02d:0x%02x\n", idx, _matrix[idx]);
            } else if (config->matrix.cols <= 16) {
                dprintf("\tr%02d:0x%04x\n", idx, _matrix[idx]);
            } else {
                dprintf("\tr%02d:0x%08x\n", idx, _matrix[idx]);
            }
        }
        dprintf("\n");
    }

    return pop_cnt;
}

