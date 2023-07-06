/* Copyright 2015-2017 Jack Humbert, Steffen Matheis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "muse.h"
#include "sendstring_german.h"

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
  _NAVI,
  _MOUSE,
  _CODE,
  _GIT,
};

enum planck_keycodes {
  TIMER = EZ_SAFE_RANGE,
  MS_SCRL,
  BASE_MAC,
  BASE_WIN,
  RSW_OFF,
  CC_GUI_TAB,
  CC_SGU_TAB,
  CC_CTL_TAB,
  CC_SCT_TAB,
};

bool is_app_switching = false;
bool is_tab_switching = false;
bool is_staying_awake = false;
bool blink_light_on = false;

uint16_t stay_awake_timer = 0;
uint16_t blink_light_timer = 0;

/** switch between mac and other keycodes */
bool is_mac = true;
/** turn off (most) rgb */
bool is_rgb = true;
/** currently active rgb layer */
int current_rgb_layer = _BASE;
/** rgb brightness 0...8 */
int brightness = 8;
/** left swap key */
bool left_swap = false;
/** right swap key */
bool right_swap = false;

#define CAPS_KEY   42

// Layers
#define LOWER       TT(_LOWER)
#define RAISE       TT(_RAISE)

// Custom keycodes
#define CC_ATSIGN   A(KC_L)
#define CC_LRBRC    S(KC_8)
#define CC_RRBRC    S(KC_9)
#define CC_LSBRC    A(KC_5)
#define CC_RSBRC    A(KC_6)
#define CC_LCBRC    A(KC_8)
#define CC_RCBRC    A(KC_9)
#define CC_LPBRC    KC_NUBS
#define CC_RPBRC    S(KC_NUBS)
#define CC_PIPE     A(KC_7)
#define CC_TILDE    A(KC_N)
#define CC_BSLASH   LSA(KC_7)
#define CC_SLSH     S(KC_7)
#define CC_UNDRSCR  S(KC_SLSH)
#define CC_GRAVE    KC_GRV
#define CC_CIRCLE   S(KC_GRV)
#define CC_EURO     RALT(KC_E)
#define CC_EXCL     S(KC_1)
#define CC_EQUAL    S(KC_0)
#define CC_AMPER    S(KC_6)
#define CC_ASTERISK S(KC_RBRC)
#define CC_TICK     KC_EQL
#define CC_BTICK    S(KC_EQL)
#define CC_QUESTNM  S(KC_MINS)
#define CC_DASH     KC_SLSH
#define CC_HASH     KC_NUHS
#define CC_DOLLAR   S(KC_4)
#define CC_PLUS     KC_RBRC
#define CC_PERCENT  S(KC_5)
#define CC_QUOT     S(KC_NUHS)
#define CC_DQUOT    S(KC_2)
#define CC_COLON    S(KC_DOT)
#define CC_SMCOLON  S(KC_COMM)
#define CC_HOME     G(KC_LEFT)
#define CC_END      G(KC_RIGHT)

#define CC_SPTLGHT  G(KC_SPACE)
#define CC_SEL_ALL  G(KC_A)
#define CC_CUT      G(KC_X)
#define CC_COPY     G(KC_C)
#define CC_PASTE    G(KC_V)
#define CD_FLIP     LAG(KC_0)
#define CD_SAVE     G(KC_S)
#define CD_CSR_UP   G(A(KC_UP))
#define CD_CSR_DN   G(A(KC_DOWN))
#define CC_UNDO     G(KC_Y)
#define CC_REDO     S(G(KC_Y))

// Custom dual function keycodes
#define CC_FULLNXT  LT(0, KC_0)
#define CC_CLOSE    LT(0, KC_1)
#define CD_COMMAND  LT(0, KC_2)
#define CD_COMMENT  LT(0, KC_3)
#define CD_SEARCH   LT(0, KC_4)
#define CD_EXPLRER  LT(0, KC_5)
#define CD_LEFT     LT(0, KC_6)
#define CD_RIGHT    LT(0, KC_7)
#define CD_UP       LT(0, KC_8)
#define CD_DOWN     LT(0, KC_9)
#define CD_MV_LEFT  LT(0, KC_A)
#define CD_MV_RGHT  LT(0, KC_B)
#define CD_MV_UP    LT(0, KC_C)
#define CD_MV_DOWN  LT(0, KC_D)
#define CD_SVNOFMT  LT(0, KC_E)
#define CD_LIGHT    LT(0, KC_F)
#define CD_DARK     LT(0, KC_G)
#define CD_BACK     LT(0, KC_H)
#define CD_FORWARD  LT(0, KC_I)
#define SWAP_ESC    LT(0, KC_J)
#define SWAP_ENTER  LT(0, KC_L)

// Dual Function keycodes
#define CC_A_GUI    GUI_T(KC_A)
#define CC_S_GUI    GUI_T(KC_S)
#define CC_C_SFT    SFT_T(KC_C)
#define CC_O_SFT    SFT_T(KC_O)
#define CC_I_ALT    ALT_T(KC_I)
#define CC_H_ALT    ALT_T(KC_H)
#define CC_T_CTL    CTL_T(KC_T)
#define CC_N_CTL    CTL_T(KC_N)
#define CC_P_MSE    LT(_MOUSE,  KC_P)
#define CC_COMM_MSE LT(_MOUSE,  KC_COMM)
#define CC_Y_GIT    LT(_GIT,  KC_Z)
#define CC_G_CDE    LT(_CODE,  KC_G)
#define CC_W_CDE    LT(_CODE,  KC_W)
#define CC_SPC_NAV  LT(_NAVI,   KC_SPACE)


const uint16_t PROGMEM C_AE[]  = {KC_F, KC_U, KC_M, COMBO_END};
const uint16_t PROGMEM C_OE[]  = {CC_C_SFT, CC_I_ALT, CC_T_CTL, COMBO_END};
const uint16_t PROGMEM C_UE[]  = {CC_P_MSE, CC_Y_GIT, CC_G_CDE, COMBO_END};
const uint16_t PROGMEM C_SS[]  = {CC_I_ALT, CC_T_CTL, KC_D, COMBO_END};


combo_t key_combos[COMBO_COUNT] = {
    COMBO(C_AE, KC_QUOT),
    COMBO(C_OE, KC_SCLN),
    COMBO(C_UE, KC_LBRC),
    COMBO(C_SS, KC_MINS),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_planck_grid(
    KC_TAB,     KC_F,       KC_U,       KC_M,       KC_V,       CC_SPTLGHT, KC_BSPACE,  KC_Q,       KC_L,       KC_B,       KC_DOT,     KC_Y,
    CC_A_GUI,   CC_C_SFT,   CC_I_ALT,   CC_T_CTL,   KC_D,       CC_UNDO,    KC_UP,      KC_R,       CC_N_CTL,   CC_H_ALT,   CC_O_SFT,   CC_S_GUI,
    RAISE,      CC_P_MSE,   CC_Y_GIT,   CC_G_CDE,   KC_J,       CC_REDO,    KC_DOWN,    KC_X,       CC_W_CDE,   KC_K,       CC_COMM_MSE,LOWER,
    SWAP_ESC,   CC_CLOSE,   CD_COMMAND, LOWER,      KC_E,       CC_SPC_NAV, CC_SPC_NAV, KC_LSHIFT,  RAISE,      KC_LEFT,    KC_RIGHT,   SWAP_ENTER
  ),
  [_LOWER] = LAYOUT_planck_grid(
    CC_ATSIGN,  CC_UNDRSCR, CC_LSBRC,   CC_RSBRC,   CC_GRAVE,   CC_CIRCLE,  CC_EURO,    CC_EXCL,    CC_LPBRC,   CC_RPBRC,   CC_EQUAL,   CC_AMPER,
    CC_BSLASH,  CC_SLSH,    CC_LCBRC,   CC_RCBRC,   CC_ASTERISK,CC_TICK,    _______,    CC_QUESTNM, CC_LRBRC,   CC_RRBRC,   CC_DASH,    CC_COLON,
    _______,    CC_HASH,    CC_DOLLAR,  CC_PIPE,    CC_TILDE,   CC_BTICK,   _______,    CC_PLUS,    CC_PERCENT, CC_DQUOT,   CC_QUOT,    _______,
    _______,    _______,    _______,    _______,    CC_SMCOLON, _______,    _______,    _______,    _______,    _______,    _______,    _______
),
  [_RAISE] = LAYOUT_planck_grid(
    KC_PGUP,    KC_BSPC,    KC_UP,      KC_DEL,     KC_PGDN,    XXXXXXX,    CC_SLSH,    CC_DASH,    KC_1,       KC_2,       KC_3,       KC_BSPACE,
    CC_HOME,    KC_LEFT,    KC_DOWN,    KC_RGHT,    CC_END,     XXXXXXX,    CC_ASTERISK,CC_PLUS,    KC_4,       KC_5,       KC_6,       KC_0,
    _______,    CC_CUT,     CC_COPY,    CC_PASTE,   KC_ENTER,   XXXXXXX,    KC_TAB,     CC_COLON,   KC_7,       KC_8,       KC_9,       _______,
    _______,    CC_SEL_ALL, XXXXXXX,    _______,    CC_SMCOLON, _______,    _______,    CC_SMCOLON, _______,    KC_DOT,     KC_COMM,    _______
  ),
  [_ADJUST] = LAYOUT_planck_grid(
    XXXXXXX,    KC_MPRV,    KC_MPLY,    KC_MNXT,    XXXXXXX,    RESET,      RSW_OFF,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_PSCR,
    TIMER,      KC_MUTE,    KC_VOLD,    KC_VOLU,    XXXXXXX,    BASE_MAC,   RGB_VAI,    KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_NLCK,
    _______,    XXXXXXX,    KC_BRID,    KC_BRIU,    XXXXXXX,    BASE_WIN,   RGB_VAD,    KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,
    _______,    _______,    _______,    _______,    CC_SMCOLON, LED_LEVEL,  LED_LEVEL,  KC_CAPS,    _______,    RGB_TOG,    RGB_MOD,    _______
  ),
  [_NAVI] = LAYOUT_planck_grid(
    XXXXXXX,    XXXXXXX,    CC_SGU_TAB, CC_GUI_TAB, MEH(KC_A),  MEH(KC_B),  MEH(KC_C),  MEH(KC_D),  HYPR(KC_I), HYPR(KC_J), HYPR(KC_K), HYPR(KC_L),
    XXXXXXX,    XXXXXXX,    CC_SCT_TAB, CC_CTL_TAB, MEH(KC_E),  MEH(KC_F),  MEH(KC_G),  MEH(KC_H),  HYPR(KC_A), HYPR(KC_B), HYPR(KC_C), HYPR(KC_D),
    XXXXXXX,    XXXXXXX,    CC_FULLNXT, CC_CLOSE,   MEH(KC_I),  MEH(KC_J),  MEH(KC_K),  MEH(KC_L),  HYPR(KC_E), HYPR(KC_F), HYPR(KC_G), HYPR(KC_H),
    _______,    XXXXXXX,    XXXXXXX,    MEH(KC_O),  MEH(KC_M),  _______,    _______,    MEH(KC_N),  XXXXXXX,    XXXXXXX,    XXXXXXX,    _______
  ),
  [_MOUSE] = LAYOUT_planck_grid(
    XXXXXXX,    KC_BTN2,    KC_MS_UP,   KC_BTN1,    KC_WH_U,    KC_ACL0,    KC_ACL0,    KC_WH_U,    KC_BTN1,    KC_MS_UP,   KC_BTN2,    XXXXXXX,
    XXXXXXX,    KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT,KC_WH_D,    KC_ACL1,    KC_ACL1,    KC_WH_D,    KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT,XXXXXXX,
    XXXXXXX,    _______,    KC_WH_L,    KC_BTN3,    KC_WH_R,    KC_ACL2,    KC_ACL2,    KC_WH_L,    KC_BTN3,    KC_WH_R,    _______,    XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    KC_LCTRL,   KC_LGUI,    XXXXXXX,    XXXXXXX,    KC_LGUI,    KC_LCTRL,   XXXXXXX,    XXXXXXX,    XXXXXXX
  ),
  [_CODE] = LAYOUT_planck_grid(
    CD_FLIP,    CD_MV_UP,   CD_UP,      CD_MV_DOWN, LSA(KC_2),  XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    CD_CSR_UP,  A(KC_F12),  XXXXXXX,
    CD_MV_LEFT, CD_LEFT,    CD_DOWN,    CD_RIGHT,   CD_MV_RGHT, XXXXXXX,    CD_LIGHT,   XXXXXXX,    CD_BACK,    CD_CSR_DN,  CD_FORWARD, XXXXXXX,
    CD_SEARCH,  CD_EXPLRER, CD_COMMENT, _______,    CD_SVNOFMT, XXXXXXX,    CD_DARK,    XXXXXXX,    _______,    A(KC_Z),    KC_F12,     XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    S(A(KC_F)), CD_SAVE,    XXXXXXX,    XXXXXXX,    C(KC_SCLN), XXXXXXX,    XXXXXXX,    XXXXXXX,    _______
  ),
  [_GIT] = LAYOUT_planck_grid(
    XXXXXXX,    KC_Q,       KC_N,       KC_Z,       KC_E,       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    KC_D,       KC_A,       KC_S,       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    KC_K,       _______,    KC_J,       S(KC_K),    S(KC_J),    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    C(KC_C),    KC_ENTER,   KC_ENTER,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
  )
};


/* rgb handling */
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
// TODO find way to write once that still compiles
/** currently active colors with brightess subtracted */
int colors[][3] = {
    [RGB_BSE] = { RGB_CYAN },
    [RGB_SYM] = { RGB_ORANGE },
    [RGB_NAV] = { RGB_GREEN },
    [RGB_MSE] = { RGB_AZURE },
    [RGB_CDE] = { RGB_YELLOW },
    [RGB_GIT] = { RGB_TURQUOISE },
    [RGB_SYS] = { RGB_RED },

    [RGB_MDF] = { RGB_RED },
    [RGB_LYR] = { RGB_BLUE },
    [RGB_ARR] = { RGB_GREEN },
    [RGB_NUM] = { RGB_WHITE },
    [RGB_MKY] = { RGB_PINK },
    [RGB_FKY] = { RGB_MAGENTA },
    [RGB_BLK] = { RGB_OFF },
};
/** full brightness initial colors */
int initial_colors[][3] = {
    [RGB_BSE] = { RGB_CYAN },
    [RGB_SYM] = { RGB_ORANGE },
    [RGB_NAV] = { RGB_GREEN },
    [RGB_MSE] = { RGB_AZURE },
    [RGB_CDE] = { RGB_YELLOW },
    [RGB_GIT] = { RGB_TURQUOISE },
    [RGB_SYS] = { RGB_RED },

    [RGB_MDF] = { RGB_RED },
    [RGB_LYR] = { RGB_BLUE },
    [RGB_ARR] = { RGB_GREEN },
    [RGB_NUM] = { RGB_WHITE },
    [RGB_MKY] = { RGB_PINK },
    [RGB_FKY] = { RGB_MAGENTA },
    [RGB_BLK] = { RGB_OFF },
};
/** map every key in every layer to a color from rgb_colors */
static uint8_t rgbmaps[][4][12] = {
    [_BASE] = {
        { RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE },
        { RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_ARR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE },
        { RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_ARR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_LYR },
        { RGB_MDF, RGB_NAV, RGB_CDE, RGB_LYR, RGB_BSE,     RGB_BSE,      RGB_MDF, RGB_LYR, RGB_ARR, RGB_ARR, RGB_MDF },
    },
    [_LOWER] = {
        { RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM },
        { RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_ARR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM },
        { RGB_LYR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_ARR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_LYR },
        { RGB_MDF, RGB_NAV, RGB_CDE, RGB_LYR, RGB_SYM,     RGB_BSE,      RGB_MDF, RGB_LYR, RGB_ARR, RGB_ARR, RGB_MDF },
    },
    [_RAISE] = {
        { RGB_MDF, RGB_MDF, RGB_ARR, RGB_MDF, RGB_MDF, RGB_BLK, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_MDF },
        { RGB_MDF, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MDF, RGB_BLK, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM },
        { RGB_LYR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_BLK, RGB_MDF, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_LYR },
        { RGB_MDF, RGB_MDF, RGB_BLK, RGB_LYR, RGB_SYM,     RGB_BSE,      RGB_SYM, RGB_LYR, RGB_SYM, RGB_SYM, RGB_MDF },
    },
    [_ADJUST] = {
        { RGB_BSE, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BLK, RGB_SYS, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_MDF },
        { RGB_SYS, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BLK, RGB_BSE, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_MDF },
        { RGB_LYR, RGB_BLK, RGB_MDF, RGB_MDF, RGB_BLK, RGB_BSE, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_LYR },
        { RGB_MDF, RGB_MDF, RGB_MDF, RGB_LYR, RGB_SYM,     RGB_SYS,      RGB_MDF, RGB_LYR, RGB_SYS, RGB_SYS, RGB_MDF },
    },
    [_NAVI] = {
        { RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE,     RGB_LYR,      RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MDF },
    },
    [_MOUSE] = {
        { RGB_BLK, RGB_MSE, RGB_ARR, RGB_MSE, RGB_MSE, RGB_MSE, RGB_MSE, RGB_MSE, RGB_MSE, RGB_ARR, RGB_MSE, RGB_BLK },
        { RGB_BLK, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MSE, RGB_MSE, RGB_MSE, RGB_MSE, RGB_ARR, RGB_ARR, RGB_ARR, RGB_BLK },
        { RGB_BLK, RGB_LYR, RGB_MSE, RGB_MSE, RGB_MSE, RGB_BLK, RGB_MSE, RGB_MSE, RGB_BLK, RGB_MSE, RGB_LYR, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_MDF, RGB_MDF,     RGB_BLK,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
    [_CODE] = {
        { RGB_CDE, RGB_SYM, RGB_ARR, RGB_SYM, RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_ARR, RGB_CDE, RGB_BLK },
        { RGB_SYM, RGB_ARR, RGB_ARR, RGB_ARR, RGB_SYM, RGB_BLK, RGB_CDE, RGB_BLK, RGB_ARR, RGB_ARR, RGB_ARR, RGB_BLK },
        { RGB_CDE, RGB_CDE, RGB_CDE, RGB_LYR, RGB_CDE, RGB_BLK, RGB_CDE, RGB_BLK, RGB_LYR, RGB_CDE, RGB_CDE, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_CDE,     RGB_CDE,      RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MDF },
    },
    [_GIT] = {
        { RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BSE, RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MDF,     RGB_MDF,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
};
/** set key at idx to color from colors */
void rgb_matrix_set_array(int idx, int name) {
  rgb_matrix_set_color(idx, colors[name][0], colors[name][1], colors[name][2]);
}
/** set all keys to layername, supports inverting layout */
void rgb_set_all(int layername) {
  current_rgb_layer = layername;
  // only show capslock in no rgb mode
  if (!is_rgb && layername != _ADJUST) {
    rgb_matrix_set_color_all(RGB_OFF);
    if (host_keyboard_led_state().caps_lock) rgb_matrix_set_array(CAPS_KEY, RGB_SYS);
    return;
  }

  if (!swap_hands) {
    for (int i = 0; i < 3; i++) for (int j = 0; j < 12; j++) {
      rgb_matrix_set_array((i * 12) + j, rgbmaps[layername][i][j]);
    }
    for (int j = 0; j < 11; j++) rgb_matrix_set_array(36 + j, rgbmaps[layername][3][j]);
  } else {
    /*
    if (left_swap && right_swap) {
    // full color invert
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 12; j++) {
          rgb_matrix_set_color((i * 12) + j,
            rgbmaps[layername][i][(11 - j) * 3],
            rgbmaps[layername][i][((11 - j) * 3) + 1],
            rgbmaps[layername][i][((11 - j) * 3) + 2]);
        }
      }
      for (int j = 0; j < 11; j++) {
        rgb_matrix_set_color(36 + j,
          rgbmaps[layername][3][(10 - j) * 3],
          rgbmaps[layername][3][((10 - j) * 3) + 1],
          rgbmaps[layername][3][((10 - j) * 3) + 2]);
      }
    } else */
    if (left_swap) {
      // left hand only
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++)  rgb_matrix_set_array((i * 12) + j, rgbmaps[layername][i][11 - j]);
        for (int j = 6; j < 12; j++) rgb_matrix_set_array((i * 12) + j, RGB_BLK);
      }
      for (int j = 0; j < 6; j++)  rgb_matrix_set_array(36 + j, rgbmaps[layername][3][10 - j]);
      for (int j = 6; j < 11; j++) rgb_matrix_set_array(36 + j, RGB_BLK);
      rgb_matrix_set_array(36, RGB_SYS);
    } else {
      // right hand only
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++)  rgb_matrix_set_array((i * 12) + j, RGB_BLK);
        for (int j = 6; j < 12; j++) rgb_matrix_set_array((i * 12) + j, rgbmaps[layername][i][11-j]);
      }
      for (int j = 0; j < 5; j++)  rgb_matrix_set_array(36 + j, RGB_BLK);
      for (int j = 5; j < 11; j++) rgb_matrix_set_array(36 + j, rgbmaps[layername][3][10 - j]);
      rgb_matrix_set_array(46, RGB_SYS);
    }
  }
  // override key for caps lock / caps word
  if (host_keyboard_led_state().caps_lock) rgb_matrix_set_array(CAPS_KEY, RGB_SYS);
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
    for (int i = 0; i <= 2; i++) colors[name][i] = (initial_colors[name][i] * brightness) / 8;
  }
  rgb_set_all(current_rgb_layer);
}

layer_state_t layer_state_set_user(layer_state_t state) {
  // unregister gui / alt / ctrl from GUI/ALT/CTL-Tab
  if (is_app_switching) {
    unregister_code(is_mac ? KC_LGUI : KC_LALT);
    is_app_switching = false;
  }
  if (is_tab_switching) {
    unregister_code(KC_LCTRL);
    is_tab_switching = false;
  }

  state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);

  planck_ez_left_led_off();
  planck_ez_right_led_off();

  switch (get_highest_layer(state)) {
    case _LOWER:
      planck_ez_left_led_on(); break;
    case _RAISE:
      planck_ez_right_led_on(); break;
    case _ADJUST:
      planck_ez_left_led_on(); planck_ez_right_led_on(); break;
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
      unregister_code(KC_LCTRL);
      is_tab_switching = false;
    }
  }

  // change some keycodes for anything not mac
  if (!is_mac) {
    switch (keycode) {
      case CC_HOME:    return custom_tap(record, KC_HOME);
      case CC_END:     return custom_tap(record, KC_END);
      case CC_SEL_ALL: return custom_tap(record, C(KC_A));
      case CC_CUT:     return custom_tap(record, C(KC_X));
      case CC_COPY:    return custom_tap(record, C(KC_C));
      case CC_PASTE:   return custom_tap(record, C(KC_V));
      case CC_ATSIGN:  return custom_tap(record, ALGR(KC_Q));
      case CC_LSBRC:   return custom_tap(record, ALGR(KC_8));
      case CC_RSBRC:   return custom_tap(record, ALGR(KC_9));
      case CC_LCBRC:   return custom_tap(record, ALGR(KC_7));
      case CC_RCBRC:   return custom_tap(record, ALGR(KC_0));
      case CC_PIPE:    return custom_tap(record, ALGR(CC_LPBRC));
      case CC_TILDE:   return custom_tap(record, ALGR(CC_PLUS));
      case CC_BSLASH:  return custom_tap(record, ALGR(KC_MINS));
      case CC_UNDO:    return custom_tap(record, C(KC_Y));
      case CC_REDO:    return custom_tap(record, C(KC_Z));
      // vs code
      case CD_FLIP:    return custom_tap(record, LCA(KC_0));
      case CD_SAVE:    return custom_tap(record, C(KC_S));
      case CD_CSR_UP:  return custom_tap(record, C(A(KC_UP)));
      case CD_CSR_DN:  return custom_tap(record, C(A(KC_DOWN)));
    }
  }

  switch (keycode) {
    // custom tap / hold keycodes
    // next window, fullscreen
    case CC_FULLNXT: return custom_tap_hold(record, is_mac ? G(CC_BTICK) : HYPR(KC_Z), is_mac ? G(C(KC_F)) : KC_F11);
    // close window, close application
    case CC_CLOSE:   return custom_tap_hold(record, is_mac ? G(KC_W)      : C(KC_W),    is_mac ? G(KC_Q)    : A(KC_F4));
    // vs code
    case CD_COMMAND: return custom_tap_hold(record, is_mac ? G(KC_P)      : C(KC_P),    is_mac ? S(G(KC_P)) : S(C(KC_P)));
    // vs code line comment, vs code block comment
    case CD_COMMENT: return custom_tap_hold(record, is_mac ? G(CC_SLSH)   : C(CC_HASH), S(A(KC_A)));
    // vs code toggle sidebar, vs code toggle explorer
    case CD_EXPLRER: return custom_tap_hold(record, is_mac ? G(KC_B)      : C(KC_B),    is_mac ? G(S(KC_E)) : C(S(KC_E)));
    // search, vs code global search
    case CD_SEARCH:  return custom_tap_hold(record, is_mac ? G(KC_F)      : C(KC_F),    is_mac ? G(S(KC_F)) : C(S(KC_F)));
    // vs code save without formatting
    case CD_SVNOFMT: return custom_sendstring(record, is_mac ? SS_LCMD("k")"s" : SS_LCTL("k"SS_LSFT("S")));
    // vs code light mode
    case CD_LIGHT:   return custom_sendstring(record, is_mac ? SS_LGUI(SS_LSFT("p"))"color theme\nlight+\n" : SS_LCTL(SS_LSFT("p"))"color theme\nlight+\n");
    // vs code dark mode
    case CD_DARK:    return custom_sendstring(record, is_mac ? SS_LGUI(SS_LSFT("p"))"color theme\ndark+\n" : SS_LCTL(SS_LSFT("p"))"color theme\ndark+\n");
    // vs code back
    case CD_BACK:    return custom_sendstring(record, is_mac ? SS_LCTL("-") : SS_LALT(SS_TAP(X_LEFT)));
    // vs code forward
    case CD_FORWARD: return custom_sendstring(record, is_mac ? SS_LCTL(SS_LSFT("-")) : SS_LALT(SS_TAP(X_RIGHT)));
    // vs code movement
    case CD_UP:      return custom_sendstring_hold(record, is_mac ? SS_LGUI("k"SS_TAP(X_UP)) : SS_LCTL("k"SS_TAP(X_UP)),
                                                    is_mac ? SS_LGUI("k")SS_LALT(SS_TAP(X_UP)) : SS_LCTL("k")SS_LALT(SS_TAP(X_UP)));
    case CD_MV_UP:   return custom_sendstring_hold(record, is_mac ? SS_LGUI("k")SS_TAP(X_UP) : SS_LCTL("k")SS_TAP(X_UP),
                                                    is_mac ? SS_LGUI("k"SS_LSFT(SS_LALT(SS_TAP(X_UP)))) : SS_LCTL("k"SS_LSFT(SS_LALT(SS_TAP(X_UP)))));
    case CD_DOWN:    return custom_sendstring_hold(record, is_mac ? SS_LGUI("k"SS_TAP(X_DOWN)) : SS_LCTL("k"SS_TAP(X_DOWN)),
                                                    is_mac ? SS_LGUI("k")SS_LALT(SS_TAP(X_DOWN)) : SS_LCTL("k")SS_LALT(SS_TAP(X_DOWN)));
    case CD_MV_DOWN: return custom_sendstring_hold(record, is_mac ? SS_LGUI("k")SS_TAP(X_DOWN) : SS_LCTL("k")SS_TAP(X_DOWN),
                                                    is_mac ? SS_LGUI("k"SS_LSFT(SS_LALT(SS_TAP(X_DOWN)))) : SS_LCTL("k"SS_LSFT(SS_LALT(SS_TAP(X_DOWN)))));
    case CD_LEFT:    return custom_sendstring_hold(record, is_mac ? SS_LGUI("k"SS_TAP(X_LEFT)) : SS_LCTL("k"SS_TAP(X_LEFT)),
                                                    is_mac ? SS_LGUI("k")SS_LALT(SS_TAP(X_LEFT)) : SS_LCTL("k")SS_LALT(SS_TAP(X_LEFT)));
    case CD_MV_LEFT: return custom_sendstring_hold(record, is_mac ? SS_LGUI("k")SS_TAP(X_LEFT) : SS_LCTL("k")SS_TAP(X_LEFT),
                                                    is_mac ? SS_LGUI("k"SS_LSFT(SS_LALT(SS_TAP(X_LEFT)))) : SS_LCTL("k"SS_LSFT(SS_LALT(SS_TAP(X_LEFT)))));
    case CD_RIGHT:   return custom_sendstring_hold(record, is_mac ? SS_LGUI("k"SS_TAP(X_RIGHT)) : SS_LCTL("k"SS_TAP(X_RIGHT)),
                                                    is_mac ? SS_LGUI("k")SS_LALT(SS_TAP(X_RIGHT)) : SS_LCTL("k")SS_LALT(SS_TAP(X_RIGHT)));
    case CD_MV_RGHT: return custom_sendstring_hold(record, is_mac ? SS_LGUI("k")SS_TAP(X_RIGHT) : SS_LCTL("k")SS_TAP(X_RIGHT),
                                                    is_mac ? SS_LGUI("k"SS_LSFT(SS_LALT(SS_TAP(X_RIGHT)))) : SS_LCTL("k"SS_LSFT(SS_LALT(SS_TAP(X_RIGHT)))));

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
    case SWAP_ESC:
      if (record->event.pressed) {
        if (record->tap.count) register_code(KC_ESC);
        else {
          left_swap = true;
          swap_hands = true;
          rgb_set_all(current_rgb_layer);
        }
      } else {
        if (record->tap.count) unregister_code(KC_ESC);
        else {
          left_swap = false;
          swap_hands = right_swap;
          rgb_set_all(current_rgb_layer);
        }
      }
      return false;
    case SWAP_ENTER:
      if (record->event.pressed) {
        if (record->tap.count) register_code(KC_ENTER);
        else {
          right_swap = true;
          swap_hands = true;
          rgb_set_all(current_rgb_layer);
        }
      } else {
        if (record->tap.count) unregister_code(KC_ENTER);
        else {
          right_swap = false;
          swap_hands = left_swap;
          rgb_set_all(current_rgb_layer);
        }
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
    case MS_SCRL:
      if (record->event.pressed) {
        tap_code16(KC_SCROLLLOCK);
        tap_code16(KC_SCROLLLOCK);
      }
      return false;
  }
  return true;
}


void rgb_matrix_indicators_kb(void) {
    if (host_keyboard_led_state().caps_lock) rgb_matrix_set_array(CAPS_KEY, RGB_SYS);
}


void matrix_scan_user(void) {
  // blink keys in system layer
  if (current_rgb_layer == _ADJUST && timer_elapsed(blink_light_timer) > 1000) {
    blink_light_timer = timer_read();
    blink_light_on = !blink_light_on;
    if (blink_light_on) {
      rgb_matrix_set_array(17, RGB_BSE);
      rgb_matrix_set_array(29, RGB_BSE);
      rgb_matrix_set_array(12, RGB_SYS);
    } else {
      if (is_staying_awake) rgb_matrix_set_array(12, RGB_BLK);
      rgb_matrix_set_array(is_mac ? 17 : 29, RGB_BLK);
    }
  }

  // amphetamine for attached device
  if (is_staying_awake && timer_elapsed(stay_awake_timer) > 60000) {
    stay_awake_timer = timer_read();
    SEND_STRING(SS_TAP(X_MS_L)SS_TAP(X_MS_R));
  }
}

/*
Features:
-* Lefthand onehand mode (Swap key on left thumb, no important functionality on swap key equivalent)
-* All german letters easily available including umlauts
-* Home row mods + secondary mods for onehand mode
-* Symbol layer
-* Numpad + . , : ; Space Tab SftTab Backspace
-* Hexpad
-* Arrow keys + Home End PgUp PgDn Cut Copy Paste Enter SelectAll
-* Esc Tab SftTab
-* Media keys, Brightness
-* Mac & Win/Linux mode
-* Timer
-* Navigation layer (app / tab switching, hyper keys, meh keys)
-* VS Code Layer
-* Mouse keys + Trackball scroll toggle
-? Git Layer?
? QWERTZ Layer + Lower + Raise
* CAPS_WORD with double shift

Base
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Tab    │ SFT F     │ ALT U     │ CTL M     │     V     │                       │     Q     │     L     │     B     │     .     │     Z     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │ GUI A     │ SFT C     │ ALT I     │ CTL T     │     D     │                       │     R     │ CTL N     │ ALT H     │ SFT O     │ GUI S     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Raise   │ Mouse P   │     Y     │ Code G    │     J     │                       │     X     │ Code W    │     K     │ GIT ,     │  Hexpad   │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │   Swap    │   Lower   │ Nav E     │                       │ Nav Space │   Raise   │   Shift   │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
Lower - ° € ´  -> Adjust
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │     @     │     _     │     [     │     ]     │     ^     │                       │     !     │     <     │     >     │     =     │     &     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │     \     │     /     │     {     │     }     │     *     │                       │     ?     │     (     │     )     │     -     │     :     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Adjust   │     #     │     $     │     |     │     ~     │                       │     +     │     %     │     "     │     '     │     ;     │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │   Swap    │   Lower   │     `     │                       │ Nav Space │  Adjust   │   Shift   │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
Raise - Tab ShiftTab Esc -> Adjust
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │  Page Up  │ Backspace │    Up     │  Delete   │ Page Down │                       │     .  /  │  -  1  /  │  -  2  /  │  -  3     │ Backspace │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Home    │   Left    │   Down    │   Right   │    End    │                       │     ,  *  │  +  4  *  │  +  5  *  │  +  6     │     0     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Raise   │    Cut    │   Copy    │   Paste   │   Enter   │                       │ Esc :     │ Esc 7 Tab │ Esc 8 Tab │     9 Tab │     ;     │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │   Swap    │  Adjust   │ SelectAll │                       │ Nav Space │   Raise   │   Shift   │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
Adjust
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Tab    │   Mute    │   Vol -   │   Vol +   │     °     │                       │   Print   │    F1     │    F2     │    F3     │    F4     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Shift Tab │   Prev    │ PlayPause │   Next    │     €     │                       │  NumLock  │    F5     │    F6     │    F7     │    F8     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Adjust   │    Esc    │   Bgt -   │   Bgt +   │     ´     │                       │ CapsLock  │    F9     │    F10    │    F11    │    F12    │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │   Swap    │  Adjust   │  System   │                       │ Nav Space │  Adjust   │   Shift   │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
Hexpad
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │     B     │     A     │     b     │     a     │           │                       │     .     │     1     │     2     │     3     │ Backspace │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │     D     │     C     │     d     │     c     │           │                       │     ,     │     4     │     5     │     6     │     0     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │     F     │     E     │     f     │     e     │           │                       │     :     │     7     │     8     │     9     │  Hexpad   │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │   Swap    │           │     x     │                       │     ;     │           │           │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
Code
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │   Flip    │  Move Up  │    Up     │ Move Down │           │                       │           │           │ Cursor Up │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Move Left │   Left    │   Down    │   Right   │ MoveRight │                       │   Light   │   Back    │Cursor Down│  Forward  │ RenameSym │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Search   │ Explorer  │  Comment  │   Code    │ SaveNoFmt │                       │   Dark    │   Code    │ Line Wrap │Definition │ ChangeAll │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │   Swap    │  Format   │   Save    │                       │ Terminal  │           │           │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
MouseGit
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │           │    Up     │           │           │                       │   Quit    │   Next    │ Previous  │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │    CTL    │   Left    │   Down    │   Right   │ Scroll Up │                       │   Edit    │ Stage All │!Stage All │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │    GUI    │   Mouse   │Scroll Left│ScrollRight│Scroll Down│                       │   Split   │   Stage   │ Not Stage │    Git    │           │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │ ScrollTog │Right Click│Left Click │                       │   Enter   │    C-C    │    C-X    │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
Navigation
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │    M-A    │    M-B    │ S App Tab │ App Tab   │    M-C    │                       │    M-D    │    H-I    │    H-J    │    H-K    │    H-L    │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │    M-E    │    M-F    │ S Ctl Tab │ Ctl Tab   │    M-G    │                       │    M-H    │    H-A    │    H-B    │    H-C    │    H-D    │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │    M-I    │    M-J    │ Full/Next │   Close   │    M-K    │                       │    M-L    │    H-E    │    H-F    │    H-G    │    H-H    │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │   Swap    │    M-M    │    Nav    │                       │    Nav    │    M-N    │    M-O    │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
System
  ┌───────────┬───────────┬───────────┬───────────┬───────────┐                       ┌───────────┬───────────┬───────────┬───────────┬───────────┐
  │   Reset   │           │           │    Mac    │           │                       │           │ Led Level │           │   RGB +   │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │   Timer   │           │                       │           │           │           │  RGB Off  │  RGB Tog  │
  ├───────────┼───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │                       │           │    Win    │           │   RGB -   │  RGB Mod  │
  └───────────┴───────────┼───────────┼───────────┼───────────┤                       ├───────────┼───────────┼───────────┼───────────┴───────────┘
                          │           │           │  System   │                       │           │           │           │
                          └───────────┴───────────┴───────────┘                       └───────────┴───────────┴───────────┘
*/
