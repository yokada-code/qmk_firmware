
# Build Options
#   change to "no" to disable the options, or define them in the Makefile in
#   the appropriate keymap folder that will get included automatically
#
#RGBLIGHT_ENABLE = yes       # Enable WS2812 RGB underlight.
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
WPM_ENABLE = yes
#MOUSEKEY_ENABLE = yes

EXTRAFLAGS += -flto

SRC += oled_bongo.c

#CONSOLE_ENABLE = yes
RGBLIGHT_ENABLE = no
MOUSEKEY_ENABLE = no
#OLED_ENABLE = no
#WPM_ENABLE = no
