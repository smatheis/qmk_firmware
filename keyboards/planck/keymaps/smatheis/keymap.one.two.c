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
  _OH_BASE,
  _OH_LOWER,
  _OH_RAISE,
  _OH_NUMBERS,
  _OH_CODE,
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
#define ONEHAND     TO(_OH_BASE)
#define BASE        TO(_BASE)

#define OH_LOWER    TT(_OH_LOWER)
#define OH_RAISE    TT(_OH_RAISE)
#define OH_NUMBERS  TT(_OH_NUMBERS)

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
#define CC_SPC_SFT  SFT_T(KC_SPACE)

#define CC_L_GUI    GUI_T(KC_L)
#define CC_H_GUI    GUI_T(KC_H)
#define CC_O_SFT    SFT_T(KC_O)
#define CC_E_SFT    SFT_T(KC_E)
#define CC_D_ALT    ALT_T(KC_D)
#define CC_T_ALT    ALT_T(KC_T)
#define CC_F_CTL    CTL_T(KC_F)
#define CC_A_CTL    CTL_T(KC_A)
#define CC_I_CDE    LT(_OH_CODE, KC_I)

// one handed combos
#define OH_00 CC_L_GUI
#define OH_01 CC_O_SFT
#define OH_02 CC_D_ALT
#define OH_03 CC_F_CTL
#define OH_04 KC_G
#define OH_10 CC_H_GUI
#define OH_11 CC_E_SFT
#define OH_12 CC_T_ALT
#define OH_13 CC_A_CTL
#define OH_14 KC_S
#define OH_20 OH_RAISE
#define OH_21 KC_K
#define OH_22 KC_C
#define OH_23 CC_I_CDE
#define OH_24 KC_U

#define OHL_00 CC_AMPER
#define OHL_01 CC_UNDRSCR
#define OHL_02 CC_LSBRC
#define OHL_03 CC_RSBRC
#define OHL_04 CC_EXCL
#define OHL_10 CC_COLON
#define OHL_11 CC_SLSH
#define OHL_12 CC_LCBRC
#define OHL_13 CC_RCBRC
#define OHL_14 CC_QUESTNM
#define OHL_20 OH_RAISE
#define OHL_21 CC_HASH
#define OHL_22 CC_LRBRC
#define OHL_23 CC_RRBRC
#define OHL_24 CC_SMCOLON

const uint16_t PROGMEM C_OPEN[]  = {OH_00, OH_01, OH_02, OH_03, COMBO_END};
const uint16_t PROGMEM C_B[]     = {OH_00, OH_10, COMBO_END};
const uint16_t PROGMEM C_CTLTB[] = {OH_01, OH_02, OH_03, COMBO_END};
const uint16_t PROGMEM C_UE[]    = {OH_01, OH_02, OH_10, COMBO_END};
const uint16_t PROGMEM C_SCTTB[] = {OH_01, OH_02, OH_13, COMBO_END};
const uint16_t PROGMEM C_OE[]    = {OH_01, OH_03, OH_10, COMBO_END};
const uint16_t PROGMEM C_SS[]    = {OH_01, OH_04, OH_10, COMBO_END};
const uint16_t PROGMEM C_P[]     = {OH_01, OH_11, COMBO_END};
const uint16_t PROGMEM C_REDO[]  = {OH_02, OH_03, OH_04, COMBO_END};
const uint16_t PROGMEM C_AE[]    = {OH_02, OH_03, OH_10, COMBO_END};
const uint16_t PROGMEM C_M[]     = {OH_02, OH_12, COMBO_END};
const uint16_t PROGMEM C_X[]     = {OH_03, OH_04, COMBO_END};
const uint16_t PROGMEM C_COMMA[] = {OH_03, OH_13, COMBO_END};
const uint16_t PROGMEM C_J[]     = {OH_04, OH_14, COMBO_END};
const uint16_t PROGMEM C_SPOTL[] = {OH_10, OH_11, OH_12, OH_13, COMBO_END};
const uint16_t PROGMEM C_N[]     = {OH_10, OH_20, COMBO_END};
const uint16_t PROGMEM C_R[]     = {OH_10, OH_21, COMBO_END};
const uint16_t PROGMEM C_GUITB[] = {OH_11, OH_12, OH_13, COMBO_END};
const uint16_t PROGMEM C_SGUTB[] = {OH_11, OH_12, OH_23, COMBO_END};
const uint16_t PROGMEM C_V[]     = {OH_11, OH_21, COMBO_END};
const uint16_t PROGMEM C_UNDO[]  = {OH_12, OH_13, OH_14, COMBO_END};
const uint16_t PROGMEM C_W[]     = {OH_12, OH_22, COMBO_END};
const uint16_t PROGMEM C_Y[]     = {OH_13, OH_14, COMBO_END};
const uint16_t PROGMEM C_DOT[]   = {OH_13, OH_23, COMBO_END};
const uint16_t PROGMEM C_Z[]     = {OH_14, OH_24, COMBO_END};
const uint16_t PROGMEM C_Q[]     = {OH_23, OH_24, COMBO_END};

const uint16_t PROGMEM C_PLUS[]  = {OHL_00, OHL_02, COMBO_END};
const uint16_t PROGMEM C_PRCNT[] = {OHL_00, OHL_03, COMBO_END};
const uint16_t PROGMEM C_QUOT[]  = {OHL_00, OHL_10, COMBO_END};
const uint16_t PROGMEM C_DQUOT[] = {OHL_01, OHL_11, COMBO_END};
const uint16_t PROGMEM C_EQUAL[] = {OHL_02, OHL_10, COMBO_END};
const uint16_t PROGMEM C_LPBRC[] = {OHL_02, OHL_12, COMBO_END};
const uint16_t PROGMEM C_GRAVE[] = {OHL_03, OHL_04, COMBO_END};
const uint16_t PROGMEM C_DASH[]  = {OHL_03, OHL_10, COMBO_END};
const uint16_t PROGMEM C_CIRCL[] = {OHL_03, OHL_14, COMBO_END};
const uint16_t PROGMEM C_RPBRC[] = {OHL_03, OHL_13, COMBO_END};
const uint16_t PROGMEM C_TICK[]  = {OHL_04, OHL_10, COMBO_END};
const uint16_t PROGMEM C_BTICK[] = {OHL_04, OHL_14, COMBO_END};
const uint16_t PROGMEM C_BSLSH[] = {OHL_11, OHL_21, COMBO_END};
const uint16_t PROGMEM C_DLLR[]  = {OHL_12, OHL_22, COMBO_END};
const uint16_t PROGMEM C_ASTRK[] = {OHL_13, OHL_14, COMBO_END};
const uint16_t PROGMEM C_PIPE[]  = {OHL_13, OHL_23, COMBO_END};
const uint16_t PROGMEM C_EURO[]  = {OHL_13, OHL_24, COMBO_END};
const uint16_t PROGMEM C_TILDE[] = {OHL_14, OHL_24, COMBO_END};
const uint16_t PROGMEM C_ATSGN[] = {OHL_23, OHL_24, COMBO_END};


combo_t key_combos[COMBO_COUNT] = {
    COMBO(C_B,     KC_B),
    COMBO(C_J,     KC_J),
    COMBO(C_M,     KC_M),
    COMBO(C_N,     KC_N),
    COMBO(C_P,     KC_P),
    COMBO(C_Q,     KC_Q),
    COMBO(C_R,     KC_R),
    COMBO(C_V,     KC_V),
    COMBO(C_W,     KC_W),
    COMBO(C_X,     KC_X),
    COMBO(C_Y,     KC_Z),
    COMBO(C_Z,     KC_Y),
    COMBO(C_AE,    KC_QUOT),
    COMBO(C_OE,    KC_SCLN),
    COMBO(C_UE,    KC_LBRC),
    COMBO(C_SS,    KC_MINS),
    COMBO(C_COMMA, KC_COMMA),
    COMBO(C_DOT,   KC_DOT),
    COMBO(C_UNDO,  CC_UNDO),
    COMBO(C_REDO,  CC_REDO),
    
    COMBO(C_ASTRK, CC_ASTERISK),
    COMBO(C_ATSGN, CC_ATSIGN),
    COMBO(C_BSLSH, CC_BSLASH),
    COMBO(C_BTICK, CC_BTICK),
    COMBO(C_CIRCL, CC_CIRCLE),
    COMBO(C_DASH,  CC_DASH),
    COMBO(C_DLLR,  CC_DOLLAR),
    COMBO(C_DQUOT, CC_DQUOT),
    COMBO(C_EURO,  CC_EURO),
    COMBO(C_EQUAL, CC_EQUAL),
    COMBO(C_GRAVE, CC_GRAVE),
    COMBO(C_LPBRC, CC_LPBRC),
    COMBO(C_PIPE,  CC_PIPE),
    COMBO(C_PLUS,  CC_PLUS),
    COMBO(C_PRCNT, CC_PERCENT),
    COMBO(C_QUOT,  CC_QUOT),
    COMBO(C_RPBRC, CC_RPBRC),
    COMBO(C_TICK,  CC_TICK),
    COMBO(C_TILDE, CC_TILDE),

    COMBO(C_OPEN,  CD_COMMAND),
    COMBO(C_CTLTB, CC_CTL_TAB),
    COMBO(C_SCTTB, CC_SCT_TAB),
    COMBO(C_GUITB, CC_GUI_TAB),
    COMBO(C_SGUTB, CC_SGU_TAB),
    COMBO(C_SPOTL, CC_SPTLGHT),
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
    KC_PGUP,    KC_BSPC,    KC_UP,      KC_DEL,     KC_PGDN,    KC_SCLN,    CC_SLSH,    CC_DASH,    KC_1,       KC_2,       KC_3,       KC_BSPACE,
    CC_HOME,    KC_LEFT,    KC_DOWN,    KC_RGHT,    CC_END,     KC_QUOT,    CC_ASTERISK,CC_PLUS,    KC_4,       KC_5,       KC_6,       KC_0,
    _______,    CC_CUT,     CC_COPY,    CC_PASTE,   KC_ENTER,   KC_LBRC,    KC_TAB,     CC_COLON,   KC_7,       KC_8,       KC_9,       _______,
    _______,    CC_SEL_ALL, KC_MINS,    _______,    CC_SMCOLON, _______,    _______,    CC_SMCOLON, _______,    KC_DOT,     KC_COMM,    _______
  ),
  [_ADJUST] = LAYOUT_planck_grid(
    ONEHAND,    KC_MPRV,    KC_MPLY,    KC_MNXT,    XXXXXXX,    RESET,      RSW_OFF,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_PSCR,
    TIMER,      KC_MUTE,    KC_VOLD,    KC_VOLU,    CMB_ON,     BASE_MAC,   RGB_VAI,    KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_NLCK,
    _______,    XXXXXXX,    KC_BRID,    KC_BRIU,    CMB_OFF,    BASE_WIN,   RGB_VAD,    KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,
    _______,    _______,    _______,    _______,    CC_SMCOLON, LED_LEVEL,  LED_LEVEL,  KC_CAPS,    _______,    RGB_TOG,    RGB_MOD,    _______
  ),
  [_NAVI] = LAYOUT_planck_grid(
    XXXXXXX,    XXXXXXX,    CC_SGU_TAB, CC_GUI_TAB, MEH(KC_A),  MEH(KC_B),  MEH(KC_C),  MEH(KC_D),  HYPR(KC_I), HYPR(KC_J), HYPR(KC_K), HYPR(KC_L),
    XXXXXXX,    XXXXXXX,    CC_SCT_TAB, CC_CTL_TAB, MEH(KC_E),  MEH(KC_F),  MEH(KC_G),  MEH(KC_H),  HYPR(KC_A), HYPR(KC_B), HYPR(KC_C), HYPR(KC_D),
    XXXXXXX,    XXXXXXX,    CC_FULLNXT, CC_CLOSE,   MEH(KC_I),  MEH(KC_J),  MEH(KC_K),  MEH(KC_L),  HYPR(KC_E), HYPR(KC_F), HYPR(KC_G), HYPR(KC_H),
    _______,    CC_CLOSE,   CC_FULLNXT, MEH(KC_O),  MEH(KC_M),  _______,    _______,    MEH(KC_N),  XXXXXXX,    XXXXXXX,    XXXXXXX,    _______
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
  ),
  /* one handed mode */
  [_OH_BASE] = LAYOUT_planck_grid(
    OH_00,      OH_01,      OH_02,      OH_03,      OH_04,      KC_ESCAPE,  KC_BRIU,    KC_MUTE,    KC_VOLD,    KC_VOLU,    XXXXXXX,    XXXXXXX,
    OH_10,      OH_11,      OH_12,      OH_13,      OH_14,      KC_TAB,     KC_BRID,    KC_MPRV,    KC_MPLY,    KC_MNXT,    XXXXXXX,    XXXXXXX,
    OH_20,      OH_21,      OH_22,      OH_23,      OH_24,      KC_ENTER,   XXXXXXX,    XXXXXXX,    KC_UP,      XXXXXXX,    XXXXXXX,    MS_SCRL,
    KC_LCTRL,   KC_LALT,    KC_LGUI,    OH_NUMBERS, OH_LOWER,   CC_SPC_SFT, CC_SPC_SFT, KC_LEFT,    KC_DOWN,    KC_RIGHT,   KC_BTN2,    KC_BTN1
  ),
  [_OH_LOWER] = LAYOUT_planck_grid(
    OHL_00,     OHL_01,     OHL_02,     OHL_03,     OHL_04,     _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      XXXXXXX,    XXXXXXX,
    OHL_10,     OHL_11,     OHL_12,     OHL_13,     OHL_14,     _______,    KC_F5,      KC_F6,      KC_F7,      KC_F8,      XXXXXXX,    XXXXXXX,
    OHL_20,     OHL_21,     OHL_22,     OHL_23,     OHL_24,     _______,    KC_F9,      KC_F10,     KC_F11,     KC_F12,     XXXXXXX,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,    _______
  ),
  [_OH_RAISE] = LAYOUT_planck_grid(
    KC_PGUP,    KC_BSPC,    KC_UP,      KC_DEL,     KC_PGDN,    _______,    XXXXXXX,    RESET,      XXXXXXX,    BASE,       BASE_MAC,   XXXXXXX,
    CC_HOME,    KC_LEFT,    KC_DOWN,    KC_RIGHT,   CC_END,     _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    BASE_WIN,   XXXXXXX,
    _______,    CC_CUT,     CC_COPY,    CC_PASTE,   KC_ENTER,   CC_SEL_ALL, XXXXXXX,    RGB_TOG,    RGB_VAI,    LED_LEVEL,  TIMER,      _______,
    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RSW_OFF,    RGB_VAD,    RGB_MOD,    _______,    _______
  ),
  [_OH_NUMBERS] = LAYOUT_planck_grid(
    KC_BSPC,    KC_3,       KC_2,       KC_1,       CC_DASH,    CC_SLSH,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    KC_0,       KC_6,       KC_5,       KC_4,       CC_PLUS,    CC_ASTERISK,XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    KC_TAB,     KC_9,       KC_8,       KC_7,       CC_COLON,   KC_ENTER,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,
    KC_ESC,     KC_COMM,    KC_DOT,     _______,    CC_SMCOLON, _______,    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,    _______
  ),
  [_OH_CODE] = LAYOUT_planck_grid(
    CD_FLIP,    LSA(2),     CD_CSR_UP,  KC_F12,     A(KC_F12),  CD_LIGHT,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    A(KC_Z),    CD_BACK,    CD_CSR_DN,  CD_FORWARD, CD_MV_RGHT, CD_DARK,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    CD_SEARCH,  CD_EXPLRER, CD_COMMENT, _______,    CD_SVNOFMT, C(KC_SCLN), XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    S(A(KC_F)), CD_SAVE,    CD_SAVE,    C(KC_SCLN), XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
  )
/* Base
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │ 00  L     │ 01  O     │ 02  D     │ 03  F     X 04  G     │    Esc    │Brightness+│   Mute    │ Volume -  │ Volume +  │           │           │
  ├─────B─────┼─────P─────┼─────M─────┼─────,─────┼─────J─────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ 10  H     │ 11  E     │ 12  T     │ 13  A     Y 14  S     │    Tab    │Brightness-│ Previous  │ PlayPause │   Next    │           │           │
  ├─────N────R┼─────V─────┼─────W─────┼─────.─────┼─────Z─────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ 20 Raise  │ 21  K     │ 22  C     │ 23  I     Q 24  U     │   Enter   │           │           │    Up     │           │           │  Scroll   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Control  │    Alt    │  Command  │  Numbers  │   Lower   │     Space / Shift     │   Left    │   Down    │   Right   │    M2     │    M1     │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  Lower
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │ 00  &     │ 01  _     │ 02  [     │ 03  ]     ^ 04  !     │    Esc    │    F1     │    F2     │    F3     │    F4     │           │           │
  ├─────'─────┼─────"─────┼─────<─────┼─────>─────┼─────`─────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ 10  :     │ 11  /     │ 12  {     │ 13  }     * 14  ?     │    Tab    │    F5     │    F6     │    F7     │    F8     │           │           │
  ├───────────┼─────\─────┼─────$─────┼─────|─────┼─────~─────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ 20 Adjust │ 21  #     │ 22  (     │ 23  )     @ 24  ;     │   Enter   │    F9     │    F10    │    F11    │    F12    │           │  Scroll   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Control  │    Alt    │  Command  │  Numbers  │   Lower   │     Space / Shift     │           │           │           │    M2     │    M1     │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  Raise
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │  Page Up  │ Backspace │    Up     │  Delete   │ Page Down │    Esc    │           │   Reset   │           │ Two Hands │    Mac    │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Home   │   Left    │   Down    │   Right   │    End    │    Tab    │           │           │           │           │  Windows  │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Raise   │    Cut    │   Copy    │   Paste   │   Enter   │ SelectAll │           │  RGB Tog  │ More RGB  │ LED Level │   Timer   │  Scroll   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Control  │    Alt    │  Command  │  Numbers  │   Lower   │     Space / Shift     │  RGB Off  │ Less RGB  │Next Effect│    M2     │    M1     │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  Numbers
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │ Backspace │     3     │     2     │     1     │     -     │     /     │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │     0     │     6     │     5     │     4     │     +     │     *     │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Tab    │     9     │     8     │     7     │     :     │   Enter   │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Esc    │     ,     │     .     │  Numbers  │     ;     │     Space / Shift     │           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
  Combos / more Keycodes

  Assume all adjacent combos are unavailable
  
  missing:
  - ctl tab x2                01 02 03, 01 02 13
  - app switcher x2           11 12 13, 11 12 23
  - spotlight                 10 11 12 13
  - osm ctl sft alt cmd       
  - undo                      12 13 14
  - redo                      02 03 04
  - open file / commands      00 01 02 03
  - code:                     New Layer
    - up down left right x2
    - cursor up down
    - back
    - forward
    - line break
    - comments
    - find
    - replace
    - light dark mode
    - (peek) definition
    - save
    - format
    - save no format      
  - ctl c
  ? macros:                   
    - emails
    - function
    - class
    - const
    - let
    - return
    - caps word
    - if
    - for
    - for in
    - for of
    - Object
    - map
    - reduce
    - Promise
    - then
    - catch
    - while
*/
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
        { RGB_MDF, RGB_MDF, RGB_ARR, RGB_MDF, RGB_MDF, RGB_BSE, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_MDF },
        { RGB_MDF, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MDF, RGB_BSE, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM },
        { RGB_LYR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_BSE, RGB_MDF, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_LYR },
        { RGB_MDF, RGB_MDF, RGB_BSE, RGB_LYR, RGB_SYM,     RGB_BSE,      RGB_SYM, RGB_LYR, RGB_SYM, RGB_SYM, RGB_MDF },
    },
    [_ADJUST] = {
        { RGB_BSE, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BLK, RGB_SYS, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_MDF },
        { RGB_SYS, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BLK, RGB_BSE, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_MDF },
        { RGB_LYR, RGB_BLK, RGB_MDF, RGB_MDF, RGB_BLK, RGB_BSE, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_LYR },
        { RGB_MDF, RGB_MDF, RGB_MDF, RGB_LYR, RGB_SYM,     RGB_SYS,      RGB_MDF, RGB_LYR, RGB_SYS, RGB_SYS, RGB_MDF },
    },
    [_NAVI] = {
        { RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_BLK, RGB_BLK, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_MDF, RGB_NAV,     RGB_LYR,      RGB_NAV, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MDF },
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
    [_OH_BASE] = {
        { RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_SYS, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BLK, RGB_BLK },
        { RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_SYS, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BLK, RGB_BLK },
        { RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_BLK, RGB_BLK, RGB_ARR, RGB_BLK, RGB_BLK, RGB_MDF },
        { RGB_MDF, RGB_MDF, RGB_MDF, RGB_LYR, RGB_LYR,     RGB_BSE,      RGB_ARR, RGB_ARR, RGB_ARR, RGB_MSE, RGB_MSE },
    },
    [_OH_LOWER] = {
        { RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_BLK, RGB_BLK },
        { RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_BLK, RGB_BLK },
        { RGB_LYR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_BLK, RGB_MDF },
        { RGB_MDF, RGB_MDF, RGB_MDF, RGB_LYR, RGB_LYR,     RGB_BSE,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_MSE, RGB_MSE },
    },
    [_OH_RAISE] = {
        { RGB_MDF, RGB_MDF, RGB_ARR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_BLK, RGB_SYS, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BLK },
        { RGB_MDF, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MDF, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BLK },
        { RGB_LYR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_BLK, RGB_SYS, RGB_SYS, RGB_SYS, RGB_SYS, RGB_MDF },
        { RGB_BLK, RGB_BLK, RGB_MDF, RGB_LYR, RGB_LYR,     RGB_BSE,      RGB_SYS, RGB_SYS, RGB_SYS, RGB_MSE, RGB_MSE },
    },
    [_OH_NUMBERS] = {
        { RGB_MDF, RGB_NUM, RGB_NUM, RGB_NUM, RGB_SYM, RGB_SYM, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_SYM, RGB_SYM, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_SYM, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_MDF, RGB_SYM, RGB_SYM, RGB_LYR, RGB_SYM,     RGB_BSE,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
    [_OH_CODE] = {
        { RGB_CDE, RGB_CDE, RGB_ARR, RGB_CDE, RGB_CDE, RGB_ARR, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_CDE, RGB_ARR, RGB_ARR, RGB_ARR, RGB_BLK, RGB_ARR, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_CDE, RGB_CDE, RGB_CDE, RGB_LYR, RGB_CDE, RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_CDE,     RGB_CDE,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
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
    case _OH_LOWER:
      planck_ez_left_led_on(); break;
    case _RAISE:
    case _OH_RAISE:
      planck_ez_right_led_on(); break;
    case _ADJUST:
    case _OH_NUMBERS:
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
    case TO(_OH_BASE):
      if (record->event.pressed) combo_enable();
      return true;
    case TO(_BASE):
      if (record->event.pressed) combo_disable();
      return true;
    case MS_SCRL:
      if (record->event.pressed) {
        tap_code(KC_SCROLLLOCK);
        tap_code(KC_SCROLLLOCK);
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
  if ((current_rgb_layer == _ADJUST || current_rgb_layer == _OH_RAISE) && timer_elapsed(blink_light_timer) > 1000) {
    blink_light_timer = timer_read();
    blink_light_on = !blink_light_on;
    if (blink_light_on) {
      rgb_matrix_set_array(current_rgb_layer == _ADJUST ? 17 : 10, RGB_BSE);
      rgb_matrix_set_array(current_rgb_layer == _ADJUST ? 29 : 22, RGB_BSE);
      rgb_matrix_set_array(current_rgb_layer == _ADJUST ? 12 : 34, RGB_SYS);
    } else {
      if (is_staying_awake) rgb_matrix_set_array(current_rgb_layer == _ADJUST ? 12 : 34, RGB_BLK);
      rgb_matrix_set_array(current_rgb_layer == _ADJUST ? is_mac ? 17 : 29 : is_mac ? 10 : 22, RGB_BLK);
    }
  }

  // amphetamine for attached device
  if (is_staying_awake && timer_elapsed(stay_awake_timer) > 60000) {
    stay_awake_timer = timer_read();
    SEND_STRING(SS_TAP(X_MS_L)SS_TAP(X_MS_R));
  }
}

/*
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
