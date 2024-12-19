#include QMK_KEYBOARD_H
#include "sendstring_german.h"

/* Base
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Tab    │     Q     │     W     │     E     │     R     │     T     │     Z     │     U     │     I     │     O     │     P     │     Ä     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Shift   │     A     │     S     │     D     │     F     │     G     │     H     │     J     │     K     │     L     │     Ö     │     Ü     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Esc    │     Y     │     X     │     C     │     V     │     B     │     N     │     M     │     ,     │     .     │     -     │   Enter   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Swap    │   Ctrl    │    Alt    │    OS     │   Lower   │         Space         │   Raise   │   Left    │   Down    │    Up     │   Right   │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  Lower
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │     €     │     @     │     _     │     [     │     ]     │     ^     │     !     │     <     │     >     │     =     │     &     │     °     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Backspace │     \     │     /     │     {     │     }     │     *     │     ?     │     (     │     )     │     -     │     :     │     ß     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Esc    │     #     │     $     │     |     │     ~     │     `     │     +     │     %     │     "     │     '     │     ;     │     ´     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Swap    │   Ctrl    │    Alt    │    OS     │   Lower   │         Space         │   Raise   │   Left    │   Down    │    Up     │   Right   │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  Raise
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │  S_Base   │  Page up  │ Backspace │    Up     │  Delete   │ Page down │ Backspace │     1     │     2     │     3     │     +     │     -     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  S_Base   │   Home    │   Left    │   Down    │   Right   │    End    │    Tab    │     4     │     5     │     6     │     0     │     *     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  S_Base   │Select all │    Cut    │   Copy    │   Paste   │   Enter   │     :     │     7     │     8     │     9     │     ;     │     /     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Swap    │   Ctrl    │    Alt    │    OS     │   Lower   │         Space         │   Raise   │     .     │     ,     │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  S_Base
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Tab    │     F     │     U     │     M     │     V     │ Spotlight │           │     Q     │     L     │     B     │     .     │     Z     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │     A     │     C     │     I     │     T     │     D     │           │           │     R     │     N     │     H     │     O     │     S     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Raise   │     P     │     Y     │     G     │     J     │           │           │     X     │     W     │     K     │     ,     │   Shift   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Swap    │           │           │   Lower   │     E     │                       │   Space   │   Raise   │           │           │   Enter   │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/

/* 100% most painful thing in this config - keyboard has eight "rows" of six keys not all of which are in order */
const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    {{5, 4}, {4, 4}, {3, 4}, {2, 4}, {1, 4}, {0, 4}},
    {{5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}},
    {{5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}},
    {{4, 3}, {3, 3}, {2, 7}, {1, 3}, {0, 3}, {5, 7}},
    {{5, 4}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}},
    {{5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}},
    {{5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}},
    {{4, 7}, {3, 7}, {2, 3}, {1, 7}, {0, 7}, {5, 7}}
};

enum planck_layers {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST,
    _CODE,
    _MOUSEGIT,
    _NAVI,
    _QWERTZ,
    _QLOWER,
};

enum planck_keycodes {
    /* OS Settings */
    TIMER = SAFE_RANGE,
    BASE_MAC,
    BASE_WIN,
    /* Trackball keys */
    MS_SCRL,
    MS_DPI,
    MS_RESET,
    /* Turn off all RGB */
    RSW_OFF,
    /* Swap Hands keycode */
    SWAP_HANDS,
    /* Tab switching keys */
    CC_GUI_TAB,
    CC_SGU_TAB,
    CC_CTL_TAB,
    CC_SCT_TAB,
    /* Some secret macro */
    CC_PASSWRD,
};

/** hold ctl / gui while true */
bool is_app_switching = false;
/** hold ctl while true */
bool is_tab_switching = false;
/** wiggle mouse every 60 seconds while true */
bool is_staying_awake = false;
/** blinking keys state indicator */
bool blink_light_on = false;
/** blinking keys because macro is recorded */
bool blink_macro_on = false;

uint16_t stay_awake_timer  = 0;
uint16_t blink_light_timer = 0;

/** switch between mac and other keycodes */
bool is_mac = true;
/** turn off (most) rgb */
bool is_rgb = true;
/** currently active rgb layer */
int current_rgb_layer = _BASE;
/** rgb brightness 0...8 */
int brightness = 8;

/** which LED indicates CAPS_LOCK / CAPS_WORD? */
#define CAPS_KEY 42

/* Layers */
#define LOWER TT(_LOWER)
#define RAISE TT(_RAISE)
#define QWERTZ TG(_QWERTZ)

/* Custom keycodes */
#define CC_ATSIGN A(KC_L)
#define CC_LRBRC S(KC_8)
#define CC_RRBRC S(KC_9)
#define CC_LSBRC A(KC_5)
#define CC_RSBRC A(KC_6)
#define CC_LCBRC A(KC_8)
#define CC_RCBRC A(KC_9)
#define CC_LPBRC KC_NUBS
#define CC_RPBRC S(KC_NUBS)
#define CC_PIPE A(KC_7)
#define CC_TILDE A(KC_N)
#define CC_BSLASH LSA(KC_7)
#define CC_SLSH S(KC_7)
#define CC_UNDRSCR S(KC_SLSH)
#define CC_GRAVE KC_GRV
#define CC_CIRCLE S(KC_GRV)
#define CC_EURO RALT(KC_E)
#define CC_EXCL S(KC_1)
#define CC_EQUAL S(KC_0)
#define CC_AMPER S(KC_6)
#define CC_ASTERISK S(KC_RBRC)
#define CC_TICK KC_EQL
#define CC_BTICK S(KC_EQL)
#define CC_QUESTNM S(KC_MINS)
#define CC_DASH KC_SLSH
#define CC_HASH KC_NUHS
#define CC_DOLLAR S(KC_4)
#define CC_PLUS KC_RBRC
#define CC_PERCENT S(KC_5)
#define CC_QUOT S(KC_NUHS)
#define CC_DQUOT S(KC_2)
#define CC_COLON S(KC_DOT)
#define CC_SMCOLON S(KC_COMM)
#define CC_HOME G(KC_LEFT)
#define CC_END G(KC_RIGHT)

#define CC_SPTLGHT G(KC_SPACE)
#define CC_SEL_ALL G(KC_A)
#define CC_CUT G(KC_X)
#define CC_COPY G(KC_C)
#define CC_PASTE G(KC_V)
#define CD_FLIP LAG(KC_0)
#define CD_SAVE G(KC_S)
#define CD_CSR_UP G(A(KC_UP))
#define CD_CSR_DN G(A(KC_DOWN))
#define CD_RENAME KC_F2
#define CD_CH_ALL G(KC_F2)
#define CC_UNDO G(KC_Y)
#define CC_REDO S(G(KC_Y))

/* Dual function keycodes */
#define CC_A_GUI GUI_T(KC_A)
#define CC_S_GUI GUI_T(KC_S)
#define CC_C_SFT SFT_T(KC_C)
#define CC_F_SFT SFT_T(KC_F)
#define CC_O_SFT SFT_T(KC_O)
#define CC_I_ALT ALT_T(KC_I)
#define CC_U_ALT ALT_T(KC_U)
#define CC_H_ALT ALT_T(KC_H)
#define CC_T_CTL CTL_T(KC_T)
#define CC_M_CTL CTL_T(KC_M)
#define CC_N_CTL CTL_T(KC_N)
#define CC_P_MSG LT(_MOUSEGIT, KC_P)
#define CC_COMM_MSG LT(_MOUSEGIT, KC_COMM)
#define CC_G_CDE LT(_CODE, KC_G)
#define CC_W_CDE LT(_CODE, KC_W)
#define CC_SPC_NAV LT(_NAVI, KC_SPACE)
#define CC_HSH_RSE LT(_RAISE, CC_HASH)

/* Custom dual function keycodes */
#define CC_FULLNXT LT(0, KC_0)
#define CC_CLOSE LT(0, KC_1)
#define CC_AE_ESC LT(0, KC_2)
#define CC_UE_TAB LT(0, KC_3)
#define CD_COMMAND LT(0, KC_4)
#define CD_COMMENT LT(0, KC_5)
#define CD_SEARCH LT(0, KC_6)
#define CD_EXPLRER LT(0, KC_7)
#define CD_LEFT LT(0, KC_8)
#define CD_RIGHT LT(0, KC_9)
#define CD_UP LT(0, KC_A)
#define CD_DOWN LT(0, KC_B)
#define CD_MV_LEFT LT(0, KC_C)
#define CD_MV_RGHT LT(0, KC_D)
#define CD_MV_UP LT(0, KC_E)
#define CD_MV_DOWN LT(0, KC_F)
#define CD_SVNOFMT LT(0, KC_G)
#define CD_LIGHT LT(0, KC_H)
#define CD_DARK LT(0, KC_I)
#define CD_BACK LT(0, KC_J)
#define CD_FORWARD LT(0, KC_K)
#define CC_MSE_ENT LT(0, KC_L)

/* Combos */
const uint16_t PROGMEM C_SCROLL[] = {KC_BTN1, CC_MSE_ENT, COMBO_END};
const uint16_t PROGMEM C_DPI[]    = {KC_LEFT, KC_RIGHT, COMBO_END};

const uint16_t PROGMEM C_AE[] = {CC_T_CTL, CC_M_CTL, COMBO_END};
const uint16_t PROGMEM C_OE[] = {CC_I_ALT, CC_U_ALT, COMBO_END};
const uint16_t PROGMEM C_UE[] = {CC_C_SFT, CC_F_SFT, COMBO_END};
const uint16_t PROGMEM C_SS[] = {KC_D, KC_V, COMBO_END};

const uint16_t PROGMEM C_RESET[]   = {KC_F1, KC_F2, KC_F3, KC_F4, COMBO_END};
const uint16_t PROGMEM C_PASSWRD[] = {KC_F5, KC_F6, KC_F7, KC_F8, COMBO_END};
const uint16_t PROGMEM C_MSRESET[] = {KC_F9, KC_F10, KC_F11, KC_F12, COMBO_END};
const uint16_t PROGMEM C_ESC[]     = {KC_TAB, CC_F_SFT, CC_U_ALT, CC_M_CTL, COMBO_END};

const uint16_t PROGMEM C_HEX_A[] = {KC_1, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM C_HEX_B[] = {KC_2, KC_3, KC_TAB, COMBO_END};
const uint16_t PROGMEM C_HEX_C[] = {KC_4, KC_5, KC_6, COMBO_END};
const uint16_t PROGMEM C_HEX_D[] = {KC_5, KC_6, KC_0, COMBO_END};
const uint16_t PROGMEM C_HEX_E[] = {KC_7, KC_8, KC_9, COMBO_END};
const uint16_t PROGMEM C_HEX_F[] = {KC_8, KC_9, CC_SMCOLON, COMBO_END};
const uint16_t PROGMEM C_HEX_X[] = {CC_COLON, KC_7, KC_8, COMBO_END};

const uint16_t PROGMEM C_MACRO_E[] = {KC_V, CC_SPTLGHT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM C_MACRO_1[] = {KC_D, CC_UNDO, KC_UP, COMBO_END};
const uint16_t PROGMEM C_MACRO_2[] = {KC_J, CC_REDO, KC_DOWN, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(C_SCROLL, MS_SCRL),
    COMBO(C_DPI, MS_DPI),

    COMBO(C_AE, KC_QUOT),
    COMBO(C_OE, KC_SCLN),
    COMBO(C_UE, KC_LBRC),
    COMBO(C_SS, KC_MINS),

    COMBO(C_RESET, QK_BOOTLOADER),
    COMBO(C_PASSWRD, CC_PASSWRD),
    COMBO(C_MSRESET, MS_RESET),
    COMBO(C_ESC, KC_ESC),

    COMBO(C_HEX_A, KC_A),
    COMBO(C_HEX_B, KC_B),
    COMBO(C_HEX_C, KC_C),
    COMBO(C_HEX_D, KC_D),
    COMBO(C_HEX_E, KC_E),
    COMBO(C_HEX_F, KC_F),
    COMBO(C_HEX_X, KC_X),

    COMBO(C_MACRO_E, DM_RSTP),
    COMBO(C_MACRO_1, DM_REC1),
    COMBO(C_MACRO_2, DM_REC2)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base
      ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
      │    Tab    │ Sft / F   │ Alt / U   │ Ctl / M   │     V     │ Spotlight │ Backspace │     Q     │     L     │     B     │     .     │     Z     │
      ├───────────┼─────Ü─────┼─────Ö─────┼─────Ä─────┼─────ß─────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │  OS / A   │ Sft / C   │ Alt / I   │ Ctl / T   │     D     │   Undo    │    Up     │     R     │ Ctl / N   │ Alt / H   │ Sft / O   │  OS / S   │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │   Raise   │ MSG / P   │     Y     │ Cde / G   │     J     │   Redo    │   Down    │     X     │ Cde / W   │     K     │ MSG / ,   │MS2 / Enter│
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼──Scroll───┤
      │ SwapHands │   Close   │  Command  │   Lower   │     E     │      Navi / Space     │   Shift   │   Raise   │   Left   DPI  Right   │    MS1    │
      └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
    */
    [_BASE] = LAYOUT_planck_grid(
        KC_TAB, CC_F_SFT, CC_U_ALT, CC_M_CTL, KC_V, CC_SPTLGHT, KC_BSPC, KC_Q, KC_L, KC_B, KC_DOT, KC_Y,
        CC_A_GUI, CC_C_SFT, CC_I_ALT, CC_T_CTL, KC_D, CC_UNDO, KC_UP, KC_R, CC_N_CTL, CC_H_ALT, CC_O_SFT, CC_S_GUI,
        RAISE, CC_P_MSG, KC_Z, CC_G_CDE, KC_J, CC_REDO, KC_DOWN, KC_X, CC_W_CDE, KC_K, CC_COMM_MSG, CC_MSE_ENT,
        SWAP_HANDS, CC_CLOSE, CD_COMMAND, LOWER, KC_E, CC_SPC_NAV, CC_SPC_NAV, OSM(MOD_LSFT), RAISE, KC_LEFT, KC_RIGHT, KC_BTN1
    ),
    /* Lower
      ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
      │     @     │     _     │     [     │     ]     │     ^     │     °     │ Backspace │     !     │     <     │     >     │     =     │     &     │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │     \     │     /     │     {     │     }     │     *     │     €     │    Up     │     ?     │     (     │     )     │     -     │     :     │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │     #     │     $     │     |     │     ~     │     `     │     ´     │   Down    │     +     │     %     │     "     │     '     │     ;     │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │ SwapHands │           │           │   Lower   │           │         Space         │           │  Adjust   │   Left    │   Right   │    M1     │
      └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
    */
    [_LOWER] = LAYOUT_planck_grid(
        CC_ATSIGN, CC_UNDRSCR, CC_LSBRC, CC_RSBRC, CC_GRAVE, CC_CIRCLE, _______, CC_EXCL, CC_LPBRC, CC_RPBRC, CC_EQUAL, CC_AMPER,
        CC_BSLASH, CC_SLSH, CC_LCBRC, CC_RCBRC, CC_ASTERISK, CC_EURO, _______, CC_QUESTNM, CC_LRBRC, CC_RRBRC, CC_DASH, CC_COLON,
        CC_HSH_RSE, CC_DOLLAR, CC_PIPE, CC_TILDE, CC_BTICK, CC_TICK, _______, CC_PLUS, CC_PERCENT, CC_DQUOT, CC_QUOT, CC_SMCOLON,
        _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, KC_SPACE, KC_SPACE, XXXXXXX, _______, _______, _______, _______
    ),
    /* Raise
      ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
      │  Page Up  │ Backspace │    Up     │   Down    │ Page Down │ Spotlight │ Backspace │     -     │     1     │     2     │     3     │    Tab    │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │   Home    │   Left    │   Down    │   Right   │    End    │   Undo    │     *     │     +     │     4     │     5     │     6     │     0     │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │   Raise   │    Cut    │   Copy    │   Paste   │   Enter   │   Redo    │     /     │     :     │     7     │     8     │     9     │     ;     │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │ SwapHands │    Tab    │ SelectAll │  Adjust   │           │         Space         │   Enter   │   Raise   │     .     │     ,     │    M1     │
      └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
    */
    [_RAISE] = LAYOUT_planck_grid(
        KC_PGUP, KC_BSPC, KC_UP, KC_DEL, KC_PGDN, _______, _______, CC_DASH, KC_1, KC_2, KC_3, KC_TAB,
        CC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, CC_END, _______, CC_ASTERISK, CC_PLUS, KC_4, KC_5, KC_6, KC_0,
        _______, CC_CUT, CC_COPY, CC_PASTE, KC_ENTER, _______, CC_SLSH, CC_COLON, KC_7, KC_8, KC_9, CC_SMCOLON,
        _______, KC_TAB, CC_SEL_ALL, _______, XXXXXXX, KC_SPACE, KC_SPACE, KC_ENTER, _______, KC_DOT, KC_COMM, _______
    ),
    /* Adjust
      ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
      │  RGB Tog  │   RGB +   │   Next    │ Volume Up │Brightness+│    Mac    │           │PrintScreen│    F1     │    F2     │    F3     │    F4     │ Reset
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │ Next RGB  │  RGB Off  │ PlayPause │   Mute    │Brightness-│  QWERTZ   │  Macro 1  │  NumLock  │    F5     │    F6     │    F7     │    F8     │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │  Adjust   │   RGB -   │ Previous  │Volume Down│           │   Linux   │  Macro 2  │           │    F9     │    F10    │    F11    │    F12    │ Reset Mouse
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │ SwapHands │           │           │  Adjust   │ LED Level │         Timer         │ CapsLock  │  Adjust   │           │           │    M1     │
      └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
    */
    [_ADJUST] = LAYOUT_planck_grid(
        RGB_TOG, RGB_VAI, KC_MNXT, KC_VOLU, KC_BRIU, BASE_MAC, XXXXXXX, KC_PSCR, KC_F1, KC_F2, KC_F3, KC_F4,
        RGB_MOD, RSW_OFF, KC_MPLY, KC_MUTE, KC_BRID, QWERTZ, DM_PLY1, KC_NUM_LOCK, KC_F5, KC_F6, KC_F7, KC_F8,
        _______, RGB_VAD, KC_MPRV, KC_VOLD, XXXXXXX, BASE_WIN, DM_PLY2, XXXXXXX, KC_F9, KC_F10, KC_F11, KC_F12,
        _______, XXXXXXX, XXXXXXX, _______, LED_LEVEL, TIMER, TIMER, KC_CAPS, _______, XXXXXXX, XXXXXXX, _______
    ),
    /* Code
      ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
      │   Flip    │  MoveUp   │    Up     │ MoveDown  │           │           │           │           │           │ Cursor Up │Definition │           │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │ MoveLeft  │   Left    │   Down    │   Right   │ MoveRight │           │           │ LightMode │   Back    │ CursrDown │  Forward  │  Rename   │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │  Search   │ Explorer  │  Comment  │   Code    │ SaveNoFmt │           │           │ Dark Mode │   Code    │ LineBreak │Definition │ ChangeAll │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │ SwapHands │           │           │  Format   │   Save    │                       │ Terminal  │           │           │           │    M1     │
      └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
    */
    [_CODE] = LAYOUT_planck_grid(
        CD_FLIP, CD_MV_UP, CD_UP, CD_MV_DOWN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, CD_CSR_UP, A(KC_F12), XXXXXXX,
        CD_MV_LEFT, CD_LEFT, CD_DOWN, CD_RIGHT, CD_MV_RGHT, XXXXXXX, XXXXXXX, CD_LIGHT, CD_BACK, CD_CSR_DN, CD_FORWARD, CD_RENAME,
        CD_SEARCH, CD_EXPLRER, CD_COMMENT, _______, CD_SVNOFMT, XXXXXXX, XXXXXXX, CD_DARK, _______, A(KC_Z), KC_F12, CD_CH_ALL,
        _______, XXXXXXX, XXXXXXX, S(A(KC_F)), CD_SAVE, XXXXXXX, XXXXXXX, C(KC_SCLN), XXXXXXX, XXXXXXX, XXXXXXX, _______
    ),
    /* Mouse / Git
      ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
      │           │           │ Scroll Up │  Up Left  │ Up Right  │           │           │           │           │           │           │           │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │           │           │Scroll Down│ Down Left │Down Right │           │           │           │           │           │           │           │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │           │   Mouse   │Scroll Left│ScrollRight│    M3     │           │           │           │           │           │           │           │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │           │           │           │    M2     │    M1     │                       │           │           │           │           │           │
      └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
    */
    [_MOUSEGIT] = LAYOUT_planck_grid(
        XXXXXXX, XXXXXXX, KC_MS_UP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_Q, S(KC_J), S(KC_K), XXXXXXX, XXXXXXX,
        KC_LCTL, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, KC_WH_U, XXXXXXX, XXXXXXX, KC_E, KC_A, KC_D, XXXXXXX, XXXXXXX,
        KC_LGUI, _______, KC_WH_L, KC_WH_R, KC_WH_D, XXXXXXX, XXXXXXX, KC_S, KC_Z, KC_N, _______, _______,
        _______, _______, MS_SCRL, KC_BTN2, KC_BTN1, _______, _______, KC_ENTER, C(KC_C), C(KC_X), XXXXXXX, _______
    ),
    /* Navi
      ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
      │           │           │ SGui Tab  │  Gui Tab  │   Meh A   │   Meh B   │   Meh C   │   Meh D   │  Hyper I  │  Hyper J  │  Hyper K  │  Hyper L  │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │           │ CtrlSpace │ SCtrl Tab │ Ctrl Tab  │   Meh E   │   Meh F   │   Meh G   │   Meh H   │  Hyper A  │  Hyper B  │  Hyper C  │  Hyper D  │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │           │           │Full/NxtWin│   Close   │   Meh I   │   Meh J   │   Meh K   │   Meh L   │  Hyper E  │  Hyper F  │  Hyper G  │  Hyper H  │
      ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
      │           │           │           │   Meh O   │   Meh M   │          Nav          │   Meh N   │   Meh P   │           │           │           │
      └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
    */
    [_NAVI]   = LAYOUT_planck_grid(
        XXXXXXX, XXXXXXX, CC_SGU_TAB, CC_GUI_TAB, MEH(KC_A), MEH(KC_B), MEH(KC_C), MEH(KC_D), HYPR(KC_I), HYPR(KC_J), HYPR(KC_K), HYPR(KC_L),
        XXXXXXX, C(KC_SPC), CC_SCT_TAB, CC_CTL_TAB, MEH(KC_E), MEH(KC_F), MEH(KC_G), MEH(KC_H), HYPR(KC_A), HYPR(KC_B), HYPR(KC_C), HYPR(KC_D),
        XXXXXXX, XXXXXXX, CC_FULLNXT, CC_CLOSE, MEH(KC_I), MEH(KC_J), MEH(KC_K), MEH(KC_L), HYPR(KC_E), HYPR(KC_F), HYPR(KC_G), HYPR(KC_H),
        _______, _______, XXXXXXX, MEH(KC_O), MEH(KC_M), _______, _______, MEH(KC_N), MEH(KC_P), XXXXXXX, XXXXXXX, _______
    ),
    [_QWERTZ] = LAYOUT_planck_grid(
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,
        KC_LCTL,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,
        KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     CC_DASH,    KC_ENTER,
        KC_BSPC,    KC_ESC,     KC_LALT,    KC_LGUI,    MO(_QLOWER),KC_SPACE,   KC_SPACE,   MO(_QLOWER),KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT
    ),
    [_QLOWER] = LAYOUT_planck_grid(
        KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_PGUP,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,
        KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_PGDN,    KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,
        _______,    S(KC_1),    S(KC_2),    S(KC_3),    S(KC_4),    S(KC_5),    S(KC_6),    S(KC_7),    S(KC_8),    S(KC_9),    S(KC_0),    QWERTZ,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_MUTE,    KC_VOLD,    KC_VOLU,    CC_END
    )
};

/* RGB handling */
/** all colors used in keymap - must start with RGB_BSE, end with RGB_BLK for iterability */
enum rgb_colors {
    RGB_BSE,
    RGB_SYM,
    RGB_NAV,
    RGB_MSE,
    RGB_CDE,
    RGB_GIT,
    RGB_SYS,

    RGB_MDF,
    RGB_LYR,
    RGB_ARR,
    RGB_NUM,
    RGB_MKY,
    RGB_FKY,
    RGB_BLK
};
/** currently active colors with brightess subtracted */
int colors[][3] = {
    [RGB_BSE] = {RGB_CYAN},
    [RGB_SYM] = {RGB_ORANGE},
    [RGB_NAV] = {RGB_GREEN},
    [RGB_MSE] = {RGB_AZURE},
    [RGB_CDE] = {RGB_YELLOW},
    [RGB_GIT] = {RGB_TURQUOISE},
    [RGB_SYS] = {RGB_RED},

    [RGB_MDF] = {RGB_RED},
    [RGB_LYR] = {RGB_BLUE},
    [RGB_ARR] = {RGB_GREEN},
    [RGB_NUM] = {RGB_WHITE},
    [RGB_MKY] = {RGB_PINK},
    [RGB_FKY] = {RGB_MAGENTA},
    [RGB_BLK] = {RGB_OFF},
};
/** full brightness initial colors */
int initial_colors[][3] = {
    [RGB_BSE] = {RGB_CYAN},
    [RGB_SYM] = {RGB_ORANGE},
    [RGB_NAV] = {RGB_GREEN},
    [RGB_MSE] = {RGB_AZURE},
    [RGB_CDE] = {RGB_YELLOW},
    [RGB_GIT] = {RGB_TURQUOISE},
    [RGB_SYS] = {RGB_RED},

    [RGB_MDF] = {RGB_RED},
    [RGB_LYR] = {RGB_BLUE},
    [RGB_ARR] = {RGB_GREEN},
    [RGB_NUM] = {RGB_WHITE},
    [RGB_MKY] = {RGB_PINK},
    [RGB_FKY] = {RGB_MAGENTA},
    [RGB_BLK] = {RGB_OFF},
};
/** map every key in every layer to a color from rgb_colors */
static uint8_t rgbmaps[][4][12] =
    {
        [_BASE] =
            {
                {RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE},
                {RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE},
                {RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF},
                {RGB_SYS, RGB_MDF, RGB_CDE, RGB_LYR, RGB_BSE, RGB_BSE, RGB_MDF, RGB_LYR, RGB_NAV, RGB_NAV, RGB_MSE},
            },
        [_LOWER] =
            {
                {RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM},
                {RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_NAV, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM},
                {RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_NAV, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM},
                {RGB_SYS, RGB_BLK, RGB_BLK, RGB_LYR, RGB_BLK, RGB_BSE, RGB_BLK, RGB_LYR, RGB_NAV, RGB_NAV, RGB_MSE},
            },
        [_RAISE] =
            {
                {RGB_MDF, RGB_MDF, RGB_ARR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_MDF},
                {RGB_MDF, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MDF, RGB_MDF, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM},
                {RGB_LYR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_SYM},
                {RGB_SYS, RGB_MDF, RGB_MDF, RGB_LYR, RGB_BLK, RGB_BSE, RGB_MDF, RGB_LYR, RGB_SYM, RGB_SYM, RGB_MSE},
            },
        [_ADJUST] =
            {
                {RGB_SYS, RGB_FKY, RGB_MKY, RGB_MKY, RGB_FKY, RGB_BSE, RGB_BLK, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY},
                {RGB_SYS, RGB_SYS, RGB_MKY, RGB_MKY, RGB_FKY, RGB_BSE, RGB_SYS, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY},
                {RGB_LYR, RGB_FKY, RGB_MKY, RGB_MKY, RGB_BLK, RGB_BSE, RGB_SYS, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY},
                {RGB_SYS, RGB_BLK, RGB_BLK, RGB_LYR, RGB_BLK, RGB_SYS, RGB_MDF, RGB_LYR, RGB_BLK, RGB_BLK, RGB_MSE},
            },
        [_CODE] =
            {
                {RGB_CDE, RGB_SYM, RGB_ARR, RGB_SYM, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_ARR, RGB_CDE, RGB_BLK},
                {RGB_SYM, RGB_ARR, RGB_ARR, RGB_ARR, RGB_SYM, RGB_BLK, RGB_BLK, RGB_CDE, RGB_ARR, RGB_ARR, RGB_ARR, RGB_CDE},
                {RGB_CDE, RGB_CDE, RGB_CDE, RGB_LYR, RGB_CDE, RGB_BLK, RGB_BLK, RGB_CDE, RGB_LYR, RGB_CDE, RGB_CDE, RGB_CDE},
                {RGB_SYS, RGB_BLK, RGB_BLK, RGB_CDE, RGB_CDE, RGB_BLK, RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MSE},
            },
        [_MOUSEGIT] =
            {
                {RGB_BLK, RGB_BLK, RGB_ARR, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK},
                {RGB_MDF, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MSE, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK},
                {RGB_MDF, RGB_LYR, RGB_MSE, RGB_MSE, RGB_MSE, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_LYR, RGB_MSE},
                {RGB_SYS, RGB_MDF, RGB_MSE, RGB_MSE, RGB_MSE, RGB_BSE, RGB_MDF, RGB_MDF, RGB_MDF, RGB_BLK, RGB_MSE},
            },
        [_NAVI] =
            {
                {RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV},
                {RGB_BLK, RGB_MDF, RGB_NAV, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV},
                {RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV},
                {RGB_SYS, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE, RGB_LYR, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK, RGB_MSE},
            },
        [_QWERTZ] =
            {
                {RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE},
                {RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE},
                {RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF},
                {RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_LYR, RGB_BSE, RGB_LYR, RGB_ARR, RGB_ARR, RGB_ARR, RGB_ARR},
            },
        [_QLOWER] =
            {
                {RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY},
                {RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY},
                {RGB_MDF, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYS},
                {RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_LYR,      RGB_BSE,     RGB_LYR, RGB_MKY, RGB_MKY, RGB_MKY, RGB_MDF},
            },
};
/** set key at idx to color from colors */
void rgb_matrix_set_array(int idx, int name) {
    rgb_matrix_set_color(idx, colors[name][0], colors[name][1], colors[name][2]);
}
/** set all keys to layername, supports inverting layout */
void rgb_set_all(int layername) {
    current_rgb_layer = layername;
    // only show system layer in no rgb mode
    if (!is_rgb && layername != _ADJUST) {
        rgb_matrix_set_color_all(RGB_OFF);
        return;
    }

    if (!swap_hands) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 12; j++) {
                rgb_matrix_set_array((i * 12) + j, rgbmaps[layername][i][j]);
            }
        for (int j = 0; j < 11; j++)
            rgb_matrix_set_array(36 + j, rgbmaps[layername][3][j]);
    } else {
        // left hand only
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 6; j++)
                rgb_matrix_set_array((i * 12) + j, rgbmaps[layername][i][11 - j]);
            for (int j = 6; j < 12; j++)
                rgb_matrix_set_array((i * 12) + j, RGB_BLK);
        }
        for (int j = 0; j < 6; j++)
            rgb_matrix_set_array(36 + j, rgbmaps[layername][3][10 - j]);
        for (int j = 6; j < 11; j++)
            rgb_matrix_set_array(36 + j, RGB_BLK);
        // set swap key color
        rgb_matrix_set_array(36, RGB_SYS);
    }
    // override key for caps lock / caps word
    if (host_keyboard_led_state().caps_lock || is_caps_word_on()) rgb_matrix_set_array(CAPS_KEY, RGB_FKY);
}
/** pass true to increase brightness, false to decrease brightness */
void update_rgb_brightness(bool b) {
    if (b) {
        if (brightness == 8) return;
        brightness++;
    } else {
        if (brightness == 0) return;
        brightness--;
    }
    for (int name = RGB_BSE; name < RGB_BLK; name++) {
        for (int i = 0; i <= 2; i++)
            colors[name][i] = (initial_colors[name][i] * brightness) / 8;
    }
    rgb_set_all(current_rgb_layer);
}
/* rgb handling end */

layer_state_t layer_state_set_user(layer_state_t state) {
    // unregister gui / alt / ctrl from GUI/ALT/CTL-Tab
    if (is_app_switching) {
        unregister_code(is_mac ? KC_LGUI : KC_LALT);
        is_app_switching = false;
    }
    if (is_tab_switching) {
        unregister_code(KC_LCTL);
        is_tab_switching = false;
    }

    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);

    planck_ez_left_led_off();
    planck_ez_right_led_off();

    switch (get_highest_layer(state)) {
        case _LOWER:
            planck_ez_left_led_on();
            break;
        case _RAISE:
            planck_ez_right_led_on();
            break;
        case _ADJUST:
            planck_ez_left_led_on();
            planck_ez_right_led_on();
            break;
    }
    rgb_set_all(get_highest_layer(state));

    return state;
}

/* various helpers to extract repeating code */
bool custom_sendstring_hold(keyrecord_t *record, const char *tapstr, const char *holdstr) {
    if (record->event.pressed) {
        if (record->tap.count) {
            SEND_STRING(tapstr);
        } else {
            SEND_STRING(holdstr);
        }
    }
    return false;
}

bool custom_sendstring(keyrecord_t *record, const char *tapstr) {
    if (record->event.pressed) SEND_STRING(tapstr);
    return false;
}

bool custom_tap_hold(keyrecord_t *record, uint16_t tapcode, uint16_t holdcode) {
    if (record->event.pressed) {
        if (record->tap.count) {
            tap_code16(tapcode);
        } else {
            tap_code16(holdcode);
        }
    }
    return false;
}

bool custom_tap(keyrecord_t *record, uint16_t tapcode) {
    if (record->event.pressed) tap_code16(tapcode);
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // unregister gui / alt / ctrl from GUI/ALT/CTL-Tab
    if (record->event.pressed) {
        if (is_app_switching && keycode != CC_GUI_TAB && keycode != CC_SGU_TAB) {
            unregister_code(is_mac ? KC_LGUI : KC_LALT);
            is_app_switching = false;
        }
        if (is_tab_switching && keycode != CC_CTL_TAB && keycode != CC_SCT_TAB) {
            unregister_code(KC_LCTL);
            is_tab_switching = false;
        }
    }

    // change some keycodes for anything not mac
    if (!is_mac) {
        switch (keycode) {
            case CC_HOME:
                return custom_tap(record, KC_HOME);
            case CC_END:
                return custom_tap(record, KC_END);
            case CC_SEL_ALL:
                return custom_tap(record, C(KC_A));
            case CC_CUT:
                return custom_tap(record, C(KC_X));
            case CC_COPY:
                return custom_tap(record, C(KC_C));
            case CC_PASTE:
                return custom_tap(record, C(KC_V));
            case CC_ATSIGN:
                return custom_tap(record, ALGR(KC_Q));
            case CC_LSBRC:
                return custom_tap(record, ALGR(KC_8));
            case CC_RSBRC:
                return custom_tap(record, ALGR(KC_9));
            case CC_LCBRC:
                return custom_tap(record, ALGR(KC_7));
            case CC_RCBRC:
                return custom_tap(record, ALGR(KC_0));
            case CC_PIPE:
                return custom_tap(record, ALGR(CC_LPBRC));
            case CC_TILDE:
                return custom_tap(record, ALGR(CC_PLUS));
            case CC_BSLASH:
                return custom_tap(record, ALGR(KC_MINS));
            case CC_UNDO:
                return custom_tap(record, C(KC_Y));
            case CC_REDO:
                return custom_tap(record, C(KC_Z));
            // vs code
            case CD_FLIP:
                return custom_tap(record, LCA(KC_0));
            case CD_SAVE:
                return custom_tap(record, C(KC_S));
            case CD_CSR_UP:
                return custom_tap(record, C(A(KC_UP)));
            case CD_CSR_DN:
                return custom_tap(record, C(A(KC_DOWN)));
            case CD_CH_ALL:
                return custom_tap(record, C(KC_F2));
            case CD_RENAME:
                return custom_tap(record, S(KC_ENTER));
        }
    }

    switch (keycode) {
        // custom tap / hold keycodes
        // next window, fullscreen
        case CC_FULLNXT:
            return custom_tap_hold(record, is_mac ? G(CC_BTICK) : HYPR(KC_Z), is_mac ? G(C(KC_F)) : KC_F11);
        // close window, close application
        case CC_CLOSE:
            return custom_tap_hold(record, is_mac ? G(KC_W) : C(KC_W), is_mac ? G(KC_Q) : A(KC_F4));
        // Left Mouse, Right Mouse
        case CC_MSE_ENT:
            return custom_tap_hold(record, KC_ENTER, KC_BTN2);
        // vs code
        case CD_COMMAND:
            return custom_tap_hold(record, is_mac ? G(KC_P) : C(KC_P), is_mac ? S(G(KC_P)) : S(C(KC_P)));
        // vs code line comment, vs code block comment
        case CD_COMMENT:
            return custom_tap_hold(record, is_mac ? G(CC_SLSH) : C(CC_HASH), S(A(KC_A)));
        // vs code toggle sidebar, vs code toggle explorer
        case CD_EXPLRER:
            return custom_tap_hold(record, is_mac ? G(KC_B) : C(KC_B), is_mac ? G(S(KC_E)) : C(S(KC_E)));
        // search, vs code global search
        case CD_SEARCH:
            return custom_tap_hold(record, is_mac ? G(KC_F) : C(KC_F), is_mac ? G(S(KC_F)) : C(S(KC_F)));
        // vs code save without formatting
        case CD_SVNOFMT:
            return custom_sendstring(record, is_mac ? SS_LCMD("k") "s" : SS_LCTL("k" SS_LSFT("S")));
        // vs code light mode
        case CD_LIGHT:
            return custom_sendstring(record, is_mac ? SS_LGUI(SS_LSFT("p")) "color theme\nlight modern\n" : SS_LCTL(SS_LSFT("p")) "color theme\nlight modern\n");
        // vs code dark mode
        case CD_DARK:
            return custom_sendstring(record, is_mac ? SS_LGUI(SS_LSFT("p")) "color theme\ndark modern\n" : SS_LCTL(SS_LSFT("p")) "color theme\ndark modern\n");
        // vs code back
        case CD_BACK:
            return custom_sendstring(record, is_mac ? SS_LCTL("-") : SS_LALT(SS_TAP(X_LEFT)));
        // vs code forward
        case CD_FORWARD:
            return custom_sendstring(record, is_mac ? SS_LCTL(SS_LSFT("-")) : SS_LALT(SS_TAP(X_RIGHT)));
        // vs code movement
        case CD_UP:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k" SS_TAP(X_UP)) : SS_LCTL("k" SS_TAP(X_UP)), is_mac ? SS_LGUI("k") SS_LALT(SS_TAP(X_UP)) : SS_LCTL("k") SS_LALT(SS_TAP(X_UP)));
        case CD_MV_UP:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k") SS_TAP(X_UP) : SS_LCTL("k") SS_TAP(X_UP), is_mac ? SS_LGUI("k" SS_LSFT(SS_LALT(SS_TAP(X_UP)))) : SS_LCTL("k" SS_LSFT(SS_LALT(SS_TAP(X_UP)))));
        case CD_DOWN:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k" SS_TAP(X_DOWN)) : SS_LCTL("k" SS_TAP(X_DOWN)), is_mac ? SS_LGUI("k") SS_LALT(SS_TAP(X_DOWN)) : SS_LCTL("k") SS_LALT(SS_TAP(X_DOWN)));
        case CD_MV_DOWN:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k") SS_TAP(X_DOWN) : SS_LCTL("k") SS_TAP(X_DOWN), is_mac ? SS_LGUI("k" SS_LSFT(SS_LALT(SS_TAP(X_DOWN)))) : SS_LCTL("k" SS_LSFT(SS_LALT(SS_TAP(X_DOWN)))));
        case CD_LEFT:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k" SS_TAP(X_LEFT)) : SS_LCTL("k" SS_TAP(X_LEFT)), is_mac ? SS_LGUI("k") SS_LALT(SS_TAP(X_LEFT)) : SS_LCTL("k") SS_LALT(SS_TAP(X_LEFT)));
        case CD_MV_LEFT:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k") SS_TAP(X_LEFT) : SS_LCTL("k") SS_TAP(X_LEFT), is_mac ? SS_LGUI("k" SS_LSFT(SS_LALT(SS_TAP(X_LEFT)))) : SS_LCTL("k" SS_LSFT(SS_LALT(SS_TAP(X_LEFT)))));
        case CD_RIGHT:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k" SS_TAP(X_RIGHT)) : SS_LCTL("k" SS_TAP(X_RIGHT)), is_mac ? SS_LGUI("k") SS_LALT(SS_TAP(X_RIGHT)) : SS_LCTL("k") SS_LALT(SS_TAP(X_RIGHT)));
        case CD_MV_RGHT:
            return custom_sendstring_hold(record, is_mac ? SS_LGUI("k") SS_TAP(X_RIGHT) : SS_LCTL("k") SS_TAP(X_RIGHT), is_mac ? SS_LGUI("k" SS_LSFT(SS_LALT(SS_TAP(X_RIGHT)))) : SS_LCTL("k" SS_LSFT(SS_LALT(SS_TAP(X_RIGHT)))));

        // password macro
        case CC_PASSWRD:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LEFT) "Sta" SS_TAP(X_RIGHT) "bi3robot");
            }
            return false;
        // application and tab switching
        case CC_GUI_TAB:
            if (record->event.pressed) {
                if (!is_app_switching) {
                    is_app_switching = true;
                    register_code(is_mac ? KC_LGUI : KC_LALT);
                }
                tap_code(KC_TAB);
            }
            return false;
        case CC_SGU_TAB:
            if (record->event.pressed) {
                if (!is_app_switching) {
                    is_app_switching = true;
                    register_code(is_mac ? KC_LGUI : KC_LALT);
                }
                tap_code16(S(KC_TAB));
            }
            return false;
        case CC_CTL_TAB:
            if (record->event.pressed) {
                if (!is_tab_switching) {
                    is_tab_switching = true;
                    register_code(KC_LCTL);
                }
                tap_code(KC_TAB);
            }
            return false;
        case CC_SCT_TAB:
            if (record->event.pressed) {
                if (!is_tab_switching) {
                    is_tab_switching = true;
                    register_code(KC_LCTL);
                }
                tap_code16(S(KC_TAB));
            }
            return false;

        // system keycodes
        case RGB_VAI:
            if (record->event.pressed) update_rgb_brightness(true);
            return true;
        case RGB_VAD:
            if (record->event.pressed) update_rgb_brightness(false);
            return true;
        case TIMER:
            if (record->event.pressed) {
                is_staying_awake = !is_staying_awake;
                stay_awake_timer = timer_read();
            }
            return false;
        case SWAP_HANDS:
            if (record->event.pressed) {
                if (!record->tap.count) {
                    swap_hands = true;
                    rgb_set_all(current_rgb_layer);
                }
            } else {
                swap_hands = false;
                rgb_set_all(current_rgb_layer);
            }
            return false;
        case BASE_MAC:
            if (record->event.pressed) is_mac = true;
            return false;
        case BASE_WIN:
            if (record->event.pressed) is_mac = false;
            return false;
        case RSW_OFF:
            if (record->event.pressed) is_rgb = !is_rgb;
            return false;
        // Special trackball keys
        case MS_SCRL:
            if (record->event.pressed) {
                tap_code16(KC_NUM_LOCK);
                tap_code16(KC_NUM_LOCK);
            }
            return false;
        case MS_DPI:
            if (record->event.pressed) {
                tap_code16(KC_CAPS_LOCK);
                tap_code16(KC_CAPS_LOCK);
            }
            return false;
        case MS_RESET:
            if (record->event.pressed) {
                tap_code16(KC_NUM_LOCK);
                tap_code16(KC_NUM_LOCK);
                tap_code16(KC_CAPS_LOCK);
                tap_code16(KC_CAPS_LOCK);
            }
            return false;
        case DM_REC1:
        case DM_REC2:
            if (!record->event.pressed) {
                layer_off(_RAISE);
                layer_off(_LOWER);
            }
    }
    return true;
}

bool rgb_matrix_indicators_kb(void) {
    if (host_keyboard_led_state().caps_lock) rgb_matrix_set_array(CAPS_KEY, RGB_FKY);
    return false;
}

void caps_word_set_user(bool active) {
    rgb_matrix_set_array(CAPS_KEY, host_keyboard_led_state().caps_lock || is_caps_word_on() ? RGB_FKY : RGB_MDF);
}

void matrix_scan_user(void) {
    // blink keys in system layer
    if ((current_rgb_layer == _ADJUST || blink_macro_on) && timer_elapsed(blink_light_timer) > 1000) {
        blink_light_timer = timer_read();
        blink_light_on    = !blink_light_on;

        if (blink_macro_on) {
            rgb_matrix_set_array(36, blink_light_on ? RGB_SYS : RGB_BLK);
        }
        if (current_rgb_layer == _ADJUST) {
            if (blink_light_on) {
                rgb_matrix_set_array(5, RGB_BSE);
                rgb_matrix_set_array(29, RGB_BSE);
                rgb_matrix_set_array(41, RGB_SYS);
            } else {
                if (is_staying_awake) rgb_matrix_set_array(41, RGB_BLK);
                rgb_matrix_set_array(is_mac ? 5 : 29, RGB_BLK);
            }
        }
    }

    // amphetamine for attached device
    if (is_staying_awake && timer_elapsed(stay_awake_timer) > 60000) {
        stay_awake_timer = timer_read();
        SEND_STRING(SS_TAP(X_MS_L) SS_TAP(X_MS_R));
    }
}

void dynamic_macro_record_start_user(int8_t direction) {
    blink_macro_on = true;
}

void dynamic_macro_record_end_user(int8_t direction) {
    blink_macro_on = false;
    rgb_matrix_set_array(36, RGB_SYS);
}

/*
 Onehand - Why
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────────────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Tab    │     B     │     P     │     M     │     G     │ Backspace             │           │           │           │           │           │
  ├───────────┼─────J─────┼───────────┼─────.─────┼─────Q─────┼───────────┬───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │     R     │     L     │     O     │     D     │     U     │     F     │           │           │           │           │           │           │
  ├───────────┼─────V─────┼───────────┼─────,─────┼─────X─────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │     N     │     H     │     E     │     T     │     A     │     S     │           │           │           │           │           │           │
  ├───────────┼───────────┼─────Z─────┼─────Y─────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │     W     │   Raise   │     K     │     C     │     I     │   Lower   │Nav / Space│           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────────────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │           │           │           │           │                       │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┬───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │           │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │           │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │           │           │           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │           │           │           │           │           │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │           │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │           │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │                       │           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
