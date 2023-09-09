SPLIT_KEYBOARD = yes
OLED_ENABLE = no
SWAP_HANDS_ENABLE = yes
COMBO_ENABLE = yes
CAPS_WORD_ENABLE = yes
COMMAND_ENABLE = no
DYNAMIC_MACRO_ENABLE = no
MOUSEKEY_ENABLE = yes
EXTRAKEYS_ENABLE = yes
LED_BACK_ENABLE = yes

LTO_ENABLE = yes  # if firmware size over limit, try this option

# Helix Spacific Build Options
# you can uncomment and edit follows 7 Variables
#  jp: 以下の7つの変数を必要に応じて編集し、コメントアウトをはずします。
# OLED_ENABLE = no            # OLED_ENABLE
# LOCAL_GLCDFONT = no         # use each keymaps "helixfont.h" insted of "common/glcdfont.c"
# LED_BACK_ENABLE = no        # LED backlight (Enable WS2812 RGB underlight.)
# LED_UNDERGLOW_ENABLE = no   # LED underglow (Enable WS2812 RGB underlight.)
# LED_ANIMATIONS = yes        # LED animations
# IOS_DEVICE_ENABLE = no      # connect to IOS device (iPad,iPhone)

# OLED_ENABLE が yes のとき
#   OLED_SELECT が core ならば QMK 標準の oled_dirver.c を使用します。
#   OLED_SELECT が core 以外ならば従来どおり helix/local_drivers/ssd1306.c を使用します。
# If OLED_ENABLE is 'yes'
#   If OLED_SELECT is 'core', use QMK standard oled_dirver.c.
#   If OLED_SELECT is other than 'core', use helix/local_drivers/ssd1306.c.
#OLED_SELECT = core

#ifeq ($(strip $(OLED_ENABLE)), yes)
#    SRC += oled_display.c
#endif
