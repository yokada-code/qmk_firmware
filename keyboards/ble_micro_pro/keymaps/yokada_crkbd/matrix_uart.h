// Baudrate list
// https://infocenter.nordicsemi.com/index.jsp?topic=%2Fps_nrf52840%2Fuart.html
#define Baud1200    0x0004F000  // 1200 baud (actual rate: 1205)
#define Baud2400    0x0009D000  // 2400 baud (actual rate: 2396)
#define Baud4800    0x0013B000  // 4800 baud (actual rate: 4808)
#define Baud9600    0x00275000  // 9600 baud (actual rate: 9598)
#define Baud14400   0x003B0000  // 14400 baud (actual rate: 14414)
#define Baud19200   0x004EA000  // 19200 baud (actual rate: 19208)
#define Baud28800   0x0075F000  // 28800 baud (actual rate: 28829)
#define Baud31250   0x00800000  // 31250 baud
#define Baud38400   0x009D5000  // 38400 baud (actual rate: 38462)
#define Baud56000   0x00E50000  // 56000 baud (actual rate: 55944)
#define Baud57600   0x00EBF000  // 57600 baud (actual rate: 57762)
#define Baud76800   0x013A9000  // 76800 baud (actual rate: 76923)
#define Baud115200  0x01D7E000  // 115200 baud (actual rate: 115942)
#define Baud230400  0x03AFB000  // 230400 baud (actual rate: 231884)
#define Baud250000  0x04000000  // 250000 baud
#define Baud460800  0x075F7000  // 460800 baud (actual rate: 470588)
#define Baud921600  0x0EBED000  // 921600 baud (actual rate: 941176)
#define Baud1M      0x10000000  // 1Mega baud

typedef union {
    struct {
        uint32_t count;
        bmp_api_key_event_t key_state[16];
    };
    uint8_t dat[128];
} bmp_uart_keystate_t;

extern bmp_uart_keystate_t uart_buf;
extern uint8_t uart_buf_p;

// defined in tmk_core/protocol/nrf/main_master.c
extern uint8_t MAINTASK_INTERVAL;
