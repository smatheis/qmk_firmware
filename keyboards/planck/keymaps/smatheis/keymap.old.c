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
  _BASE_NO_HOLD,
  _LOWER,
  _RAISE,
  _ADJUST,
  _NAVI,
  _MOUSE,
  _CODE,
  _GIT,
  _KEYPAD,
  _SYSTEM
};

enum planck_keycodes {
  TIMER = EZ_SAFE_RANGE,
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
#define LOWER      TT(_LOWER)
#define RAISE      TT(_RAISE)
#define SYSTEM     TT(_SYSTEM)

// Custom keycodes
#define CC_SPTLGHT G(KC_SPACE)
#define CC_ATSIGN  A(KC_L)
#define CC_LSBRC   A(KC_5)
#define CC_RSBRC   A(KC_6)
#define CC_LCBRC   A(KC_8)
#define CC_RCBRC   A(KC_9)
#define CC_PIPE    A(KC_7)
#define CC_TILDE   A(KC_N)
#define CC_BSLASH  LSA(KC_7)
#define CC_HOME    G(KC_LEFT)
#define CC_END     G(KC_RIGHT)
#define CC_SEL_ALL G(KC_A)
#define CC_CUT     G(KC_X)
#define CC_COPY    G(KC_C)
#define CC_PASTE   G(KC_V)
#define CD_FLIP    LAG(KC_0)
#define CD_SAVE    G(KC_S)
#define CD_CSR_UP  G(A(KC_UP))
#define CD_CSR_DN  G(A(KC_DOWN))
#define CC_UNDO    G(KC_Y)
#define CC_REDO    S(G(KC_Y))

// Custom dual function keycodes
#define CC_FULLNXT LT(0, KC_0)
#define CC_CLOSE   LT(0, KC_1)
#define CD_COMMAND LT(0, KC_2)
#define CD_COMMENT LT(0, KC_3)
#define CD_SEARCH  LT(0, KC_4)
#define CD_EXPLRER LT(0, KC_5)
#define CD_LEFT    LT(0, KC_6)
#define CD_RIGHT   LT(0, KC_7)
#define CD_UP      LT(0, KC_8)
#define CD_DOWN    LT(0, KC_9)
#define CD_MV_LEFT LT(0, KC_A)
#define CD_MV_RGHT LT(0, KC_B)
#define CD_MV_UP   LT(0, KC_C)
#define CD_MV_DOWN LT(0, KC_D)
#define CD_SVNOFMT LT(0, KC_E)
#define CD_LIGHT   LT(0, KC_F)
#define CD_DARK    LT(0, KC_G)
#define CD_BACK    LT(0, KC_H)
#define CD_FORWARD LT(0, KC_I)
#define SWAP_TAB   LT(0, KC_J)
#define SWAP_ENTER LT(0, KC_L)

// Dual Function keycodes
#define CC_GUI_A   GUI_T(KC_A)
#define CC_GUI_S   GUI_T(KC_S)
#define CC_SFT_C   SFT_T(KC_C)
#define CC_SFT_O   SFT_T(KC_O)
#define CC_ALT_I   ALT_T(KC_I)
#define CC_ALT_H   ALT_T(KC_H)
#define CC_CTL_T   CTL_T(KC_T)
#define CC_CTL_N   CTL_T(KC_N)
#define CC_LWR_Z   LT(_LOWER,  KC_Y)
#define CC_MSE_P   LT(_MOUSE,  KC_P)
#define CC_GIT_F   LT(_GIT,  KC_F)
#define CC_CDE_G   LT(_CODE,  KC_G)
#define CC_CDE_W   LT(_CODE,  KC_W)
#define CC_NAV_SPC LT(_NAVI,   KC_SPACE)
#define CC_SYS_BSP LT(_SYSTEM, KC_BSPC)

#define H          HYPR

const uint16_t PROGMEM ae[] = {KC_ESC, CC_GIT_F, COMBO_END};
const uint16_t PROGMEM oe[] = {KC_ESC, KC_U, COMBO_END};
const uint16_t PROGMEM ue[] = {KC_ESC, KC_M, COMBO_END};
const uint16_t PROGMEM ss[] = {KC_ESC, KC_Z, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
    COMBO(ae, KC_QUOT),
    COMBO(oe, KC_SCLN),
    COMBO(ue, KC_LBRC),
    COMBO(ss, KC_MINS),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Esc    │  Git / F  │     U     │     M     │     Y     │ Spotlight │   Close   │     B     │     L     │     K     │     .     │ Sys / Bsp │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  OS / A   │ Shift / C │ Alt / I   │ Ctrl / T  │     D     │   Redo    │    Up     │     R     │ Ctrl / N  │ Alt / H   │ Shift / O │  OS / S   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Raise   │ Mouse / P │     J     │ Code / G  │     X     │   Undo    │   Down    │     Q     │ Code / W  │     V     │     ,     │ Lower / Z │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap/ Tab │           │           │   Lower   │     E     │      Navi / Space     │   Shift   │   Raise   │   Left    │   Right   │ Swap/Enter│
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_BASE] = LAYOUT_planck_grid(
    KC_ESC,     CC_GIT_F,   KC_U,       KC_M,       KC_Z,       CC_SPTLGHT, CD_COMMAND, KC_B,       KC_L,       KC_K,       KC_DOT,     CC_SYS_BSP,
    CC_GUI_A,   CC_SFT_C,   CC_ALT_I,   CC_CTL_T,   KC_D,       CC_REDO,    KC_UP,      KC_R,       CC_CTL_N,   CC_ALT_H,   CC_SFT_O,   CC_GUI_S,
    RAISE,      CC_MSE_P,   KC_J,       CC_CDE_G,   KC_X,       CC_UNDO,    KC_DOWN,    KC_Q,       CC_CDE_W,   KC_V,       KC_COMM,    CC_LWR_Z,
    SWAP_TAB,   XXXXXXX,    XXXXXXX,    LOWER,      KC_E,       CC_NAV_SPC, CC_NAV_SPC, KC_LSHIFT,  RAISE,      KC_LEFT,    KC_RIGHT,   SWAP_ENTER
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │     €     │     _     │     [     │     ]     │     ^     │     °     │     ´     │     !     │     <     │     >     │     =     │     &     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │     \     │     /     │     {     │     }     │     *     │     @     │    Up     │     ?     │     (     │     )     │     -     │     :     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Adjust   │     #     │     $     │     |     │     ~     │     `     │   Down    │     +     │     %     │     "     │     '     │   Lower   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap/ Tab │           │           │   Lower   │     ;     │      Navi / Space     │   Shift   │  Adjust   │   Left    │   Right   │ Swap/Enter│
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_LOWER] = LAYOUT_planck_grid(
    RALT(KC_E), S(KC_SLSH), CC_LSBRC,   CC_RSBRC,   KC_GRV,     S(KC_GRV),  KC_EQL,     S(KC_1),    KC_NUBS,    S(KC_NUBS), S(KC_0),    S(KC_6),
    CC_BSLASH,  S(KC_7),    CC_LCBRC,   CC_RCBRC,   S(KC_RBRC), CC_ATSIGN,  _______,    S(KC_MINS), S(KC_8),    S(KC_9),    KC_SLSH,    S(KC_DOT),
    _______,    KC_NUHS,    S(KC_4),    CC_PIPE,    CC_TILDE,   S(KC_EQL),  _______,    KC_RBRC,    S(KC_5),    S(KC_2),    S(KC_NUHS), _______,
    _______,    _______,    _______,    _______,    S(KC_COMM), _______,    _______,    _______,    _______,    _______,    _______,    _______
),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │  Page Up  │ Backspace │    Up     │   Down    │ Page Down │           │     /     │     -     │     1     │     2     │     3     │ Backspace │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Home    │   Left    │   Down    │   Right   │    End    │           │     *     │     +     │     4     │     5     │     6     │     0     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │   Raise   │    Cut    │   Copy    │   Paste   │   Enter   │ SelectAll │    Tab    │     :     │     7     │     8     │     9     │  Adjust   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap/ Tab │           │           │   Lower   │     ;     │      Navi / Space     │     ;     │   Raise   │     .     │     ,     │ Swap/Enter│
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_RAISE] = LAYOUT_planck_grid(
    KC_PGUP,    KC_BSPC,    KC_UP,      KC_DEL,     KC_PGDN,    XXXXXXX,    S(KC_7),    KC_SLSH,    KC_1,       KC_2,       KC_3,       KC_BSPACE,
    CC_HOME,    KC_LEFT,    KC_DOWN,    KC_RGHT,    CC_END,     XXXXXXX,    S(KC_RBRC), KC_RBRC,    KC_4,       KC_5,       KC_6,       KC_0,
    _______,    CC_CUT,     CC_COPY,    CC_PASTE,   KC_ENTER,   CC_SEL_ALL, KC_TAB,     S(KC_DOT),  KC_7,       KC_8,       KC_9,       LOWER,
    _______,    _______,    _______,    _______,    S(KC_COMM), _______,    _______,    S(KC_COMM), _______,    KC_DOT,     KC_COMM,    _______
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │ Previous  │ PlayPause │   Next    │           │           │           │    F1     │    F2     │    F3     │    F4     │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │   Mute    │Volume Down│ Volume Up │           │           │           │    F5     │    F6     │    F7     │    F8     │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Adjust   │           │ Brgt Down │  Brgt Up  │           │  Numlock  │PrintScreen│    F9     │    F10    │    F11    │    F12    │  Adjust   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap /Esc │    Tab    │   Shift   │  Adjust   │     ;     │       Caps Lock       │     ;     │  Adjust   │           │           │ Swap/Enter│
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_ADJUST] = LAYOUT_planck_grid(
    XXXXXXX,    KC_MPRV,    KC_MPLY,    KC_MNXT,    XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      XXXXXXX,
    XXXXXXX,    KC_MUTE,    KC_VOLD,    KC_VOLU,    XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_F5,      KC_F6,      KC_F7,      KC_F8,      XXXXXXX,
    _______,    XXXXXXX,    KC_BRID,    KC_BRIU,    XXXXXXX,    KC_NLCK,    KC_PSCR,    KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,
    _______,    _______,    _______,    _______,    S(KC_COMM), KC_CAPS,    KC_CAPS,    S(KC_COMM), _______,    XXXXXXX,    XXXXXXX,    _______
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │           │  App -->  │  App <--  │ Side Top  │ Left Top  │ MiddleTop │ Right Top │  Hyper I  │  Hyper J  │  Hyper K  │  Hyper L  │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │  Tab -->  │  Tab <--  │   Side    │   Left    │  Middle   │   Right   │  Hyper A  │  Hyper B  │  Hyper C  │  Hyper D  │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │Left Middle│MiddleRight│ Side Btm  │ Left Btm  │ MiddleBtm │ Right Btm │  Hyper E  │  Hyper F  │  Hyper G  │  Hyper H  │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap /Esc │           │           │  Center   │NxtWin/Full│         Navi          │   Close   │           │           │           │ Swap/Enter│
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_NAVI] = LAYOUT_planck_grid(
    XXXXXXX,    XXXXXXX,    CC_SGU_TAB, CC_GUI_TAB, MEH(KC_A),  MEH(KC_B),  MEH(KC_C),  MEH(KC_D),  HYPR(KC_I), HYPR(KC_J), HYPR(KC_K), HYPR(KC_L),
    XXXXXXX,    XXXXXXX,    CC_SCT_TAB, CC_CTL_TAB, MEH(KC_E),  MEH(KC_F),  MEH(KC_G),  MEH(KC_H),  HYPR(KC_A), HYPR(KC_B), HYPR(KC_C), HYPR(KC_D),
    XXXXXXX,    XXXXXXX,    MEH(KC_M),  MEH(KC_N),  MEH(KC_I),  MEH(KC_J),  MEH(KC_K),  MEH(KC_L),  HYPR(KC_E), HYPR(KC_F), HYPR(KC_G), HYPR(KC_H),
    _______,    XXXXXXX,    XXXXXXX,    MEH(KC_O),  CC_FULLNXT, _______,    _______,    CC_CLOSE,   XXXXXXX,    XXXXXXX,    XXXXXXX,    _______
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │           │Rightclick │    Up     │ Leftclick │ Scroll Up │ Accel. 0  │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │    <--    │   Down    │    -->    │Scroll Down│ Accel. 1  │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │   Mouse   │ Scroll <- │Middleclick│ Scroll -> │           │ Accel. 2  │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap /Esc │           │           │   Ctrl    │    OS     │                       │           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_MOUSE] = LAYOUT_planck_grid(
    XXXXXXX,    XXXXXXX,    KC_BTN2,    KC_MS_UP,   KC_BTN1,    KC_WH_U,    KC_ACL0,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT,KC_WH_D,    KC_ACL1,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    _______,    KC_WH_L,    KC_BTN3,    KC_WH_R,    XXXXXXX,    KC_ACL2,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    KC_LCTRL,   KC_LGUI,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │Flip Layout│  Move Up  │    Up     │ Move Down │2x2 Layout │           │           │           │           │ Cursor Up │Peek Defntn│           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Move Left │   Left    │   Down    │   Right   │Move Right │           │Light Mode │           │   Back    │Cursor Down│  Forward  │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Search   │ Explorer  │  Comment  │   Code    │SaveNoFrmat│           │ Dark Mode │           │   Code    │ Line Wrap │Definition │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap /Esc │           │           │           │  Format   │         Save          │ Terminal  │           │           │           │ Swap/Enter│
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_CODE] = LAYOUT_planck_grid(
    CD_FLIP,    CD_MV_UP,   CD_UP,      CD_MV_DOWN, LSA(KC_2),  XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    CD_CSR_UP,  A(KC_F12),  XXXXXXX,
    CD_MV_LEFT, CD_LEFT,    CD_DOWN,    CD_RIGHT,   CD_MV_RGHT, XXXXXXX,    CD_LIGHT,   XXXXXXX,    CD_BACK,    CD_CSR_DN,  CD_FORWARD, XXXXXXX,
    CD_SEARCH,  CD_EXPLRER, CD_COMMENT, _______,    CD_SVNOFMT, XXXXXXX,    CD_DARK,    XXXXXXX,    _______,    A(KC_Z),    KC_F12,     XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    S(A(KC_F)), CD_SAVE,    XXXXXXX,    XXXXXXX,    C(KC_SCLN), XXXXXXX,    XXXXXXX,    XXXXXXX,    _______
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │    Git    │   Quit    │ Not Stage │   Stage   │   Edit    │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │  Not All  │ Stage All │   Split   │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │PrvUndecHnk│NxtUndecHnk│ Prev Hunk │ Next Hunk │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap /Esc │           │           │           │    ^C     │         Enter         │           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_GIT] = LAYOUT_planck_grid(
    XXXXXXX,    _______,    KC_Q,       KC_N,       KC_Z,       KC_E,       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_D,       KC_A,       KC_S,       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    KC_K,       KC_J,       S(KC_K),    S(KC_J),    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    C(KC_C),    KC_ENTER,   KC_ENTER,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │   Reset   │           │           │    Mac    │           │           │           │           │           │           │           │  System   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │   Timer   │           │           │           │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │           │           │           │  Windows  │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Swap /Esc │           │           │Next Effect│Tog Effects│       LED Level       │  RGB Off  │           │ RGB Down  │  RGB Up   │ Swap/Enter│
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_SYSTEM] = LAYOUT_planck_grid(
    RESET,      XXXXXXX,    XXXXXXX,    BASE_MAC,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,
    XXXXXXX,    XXXXXXX,    XXXXXXX,    TIMER,      XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    BASE_WIN,   XXXXXXX,    XXXXXXX,    XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    RGB_MOD,    RGB_TOG,    LED_LEVEL,  LED_LEVEL,  RSW_OFF,    XXXXXXX,    RGB_VAD,    RGB_VAI,    _______
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
        { RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_CDE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF },
        { RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_ARR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE },
        { RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_ARR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_LYR, RGB_BSE,     RGB_BSE,      RGB_MDF, RGB_LYR, RGB_ARR, RGB_ARR, RGB_MDF },
    },
    [_LOWER] = {
        { RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM },
        { RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_ARR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM },
        { RGB_LYR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_ARR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_LYR },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_LYR, RGB_SYM,     RGB_BSE,      RGB_MDF, RGB_LYR, RGB_ARR, RGB_ARR, RGB_MDF },
    },
    [_RAISE] = {
        { RGB_MDF, RGB_MDF, RGB_ARR, RGB_MDF, RGB_MDF, RGB_BLK, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_MDF },
        { RGB_MDF, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MDF, RGB_BLK, RGB_SYM, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM },
        { RGB_LYR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_LYR },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_LYR, RGB_SYM,     RGB_BSE,      RGB_SYM, RGB_LYR, RGB_SYM, RGB_SYM, RGB_MDF },
    },
    [_ADJUST] = {
        { RGB_BLK, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BSE, RGB_BLK, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_BLK },
        { RGB_BLK, RGB_MKY, RGB_MKY, RGB_MKY, RGB_BSE, RGB_BLK, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_BLK },
        { RGB_LYR, RGB_BSE, RGB_MDF, RGB_MDF, RGB_BSE, RGB_MDF, RGB_MDF, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY, RGB_LYR },
        { RGB_MDF, RGB_MDF, RGB_MDF, RGB_LYR, RGB_BLK,     RGB_MDF,      RGB_BLK, RGB_LYR, RGB_BLK, RGB_BLK, RGB_MDF },
    },
    [_NAVI] = {
        { RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_BLK, RGB_BLK, RGB_NAV, RGB_NAV, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_BLK, RGB_BLK, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_MDF, RGB_NAV,     RGB_LYR,      RGB_NAV, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MDF },
    },
    [_MOUSE] = {
        { RGB_BLK, RGB_BLK, RGB_MSE, RGB_ARR, RGB_MSE, RGB_MSE, RGB_MSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MSE, RGB_MSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_LYR, RGB_MSE, RGB_MSE, RGB_MSE, RGB_BLK, RGB_MSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_MDF, RGB_MDF,     RGB_BLK,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
    [_CODE] = {
        { RGB_CDE, RGB_SYM, RGB_ARR, RGB_SYM, RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_ARR, RGB_CDE, RGB_BLK },
        { RGB_SYM, RGB_ARR, RGB_ARR, RGB_ARR, RGB_SYM, RGB_BLK, RGB_CDE, RGB_BLK, RGB_ARR, RGB_ARR, RGB_ARR, RGB_BLK },
        { RGB_CDE, RGB_CDE, RGB_CDE, RGB_LYR, RGB_CDE, RGB_BLK, RGB_CDE, RGB_BLK, RGB_LYR, RGB_CDE, RGB_CDE, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_CDE,     RGB_CDE,      RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MDF },
    },
    [_GIT] = {
        { RGB_BLK, RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_MDF,     RGB_MDF,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
    [_KEYPAD] = {
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_NUM, RGB_NUM, RGB_NUM, RGB_BLK, RGB_MDF },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_NUM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_NUM, RGB_NUM, RGB_NUM, RGB_BLK, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK,     RGB_LYR,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
    [_SYSTEM] = {
        { RGB_SYS, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_LYR },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_SYS, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BSE, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_MDF, RGB_BLK, RGB_BLK, RGB_SYS, RGB_SYS,     RGB_SYS,      RGB_SYS, RGB_BLK, RGB_SYS, RGB_SYS, RGB_MDF },
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
  if (!is_rgb && layername != _SYSTEM) {
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
    case _LOWER:  planck_ez_left_led_on(); break;
    case _RAISE:  planck_ez_right_led_on(); break;
    case _ADJUST: planck_ez_left_led_on(); planck_ez_right_led_on(); break;
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
      case CC_PIPE:    return custom_tap(record, ALGR(KC_NUBS));
      case CC_TILDE:   return custom_tap(record, ALGR(KC_RBRC));
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
    case CC_FULLNXT: return custom_tap_hold(record, is_mac ? G(S(KC_EQL)) : H(KC_Z),    is_mac ? G(C(KC_F)) : KC_F11);
    // close window, close application
    case CC_CLOSE:   return custom_tap_hold(record, is_mac ? G(KC_W)      : C(KC_W),    is_mac ? G(KC_Q)    : A(KC_F4));
    // vs code
    case CD_COMMAND: return custom_tap_hold(record, is_mac ? G(KC_P)      : C(KC_P),    is_mac ? S(G(KC_P)) : S(C(KC_P)));
    // vs code line comment, vs code block comment
    case CD_COMMENT: return custom_tap_hold(record, is_mac ? G(S(KC_7))   : C(KC_NUHS), S(A(KC_A)));
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
    case SWAP_TAB:
      if (record->event.pressed) {
        if (record->tap.count) register_code(KC_TAB);
        else {
          left_swap = true;
          swap_hands = true;
          rgb_set_all(current_rgb_layer);
        }
      } else {
        if (record->tap.count) unregister_code(KC_TAB);
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
  }
  return true;
}


void rgb_matrix_indicators_kb(void) {
    if (host_keyboard_led_state().caps_lock) rgb_matrix_set_array(CAPS_KEY, RGB_SYS);
}


void matrix_scan_user(void) {
  // blink keys in system layer
  if (current_rgb_layer == _SYSTEM && timer_elapsed(blink_light_timer) > 1000) {
    blink_light_timer = timer_read();
    blink_light_on = !blink_light_on;
    if (blink_light_on) {
      rgb_matrix_set_array(3,  RGB_BSE);
      rgb_matrix_set_array(32, RGB_BSE);
      rgb_matrix_set_array(15, RGB_SYS);
    } else {
      if (is_staying_awake) rgb_matrix_set_array(15, RGB_BLK);
      rgb_matrix_set_array(is_mac ? 3 : 32, RGB_BLK);
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
  │Swap Hands │           │           │           │           │                       │           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
