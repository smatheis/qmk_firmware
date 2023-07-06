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

enum planck_layers {
  _BASE,
  _SECOND_BASE,
  _RAISE,
  _LOWER,
  _LOWERER,
  _NUMBERS,
  _NAVI,
  _CODE,
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

// Layers
#define LOWER       TT(_LOWER)
#define LOWERER     TT(_LOWERER)
#define NUMBERS     TT(_NUMBERS)
#define RAISE       TT(_RAISE)
#define SECOND_BASE TT(_SECOND_BASE)

#define CC_NAV_SPC  LT(_NAVI, KC_SPACE)
#define CC_CDE_G    LT(_CODE, KC_G)

#define CC_GUI_H    GUI_T(KC_H)
#define CC_SFT_E    SFT_T(KC_E)
#define CC_ALT_T    ALT_T(KC_T)
#define CC_CTL_A    CTL_T(KC_A)
#define CC_GUI_L    GUI_T(KC_L)
#define CC_SFT_O    SFT_T(KC_O)
#define CC_ALT_D    ALT_T(KC_D)
#define CC_CTL_I    CTL_T(KC_I)

// Custom keycodes
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

// Dual Function keycodes

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Esc    │     R     │  GUI / L  │ Shift / O │  Alt / D  │Control / I│     U     │ Backspace │           │   Mute    │ Volume -  │ Volume +  │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Lowerer  │     N     │  GUI / H  │ Shift / E │  Alt / T  │Control / A│     S     │   Undo    │   Redo    │           │    Up     │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Tab    │   Raise   │     B     │     K     │     C     │ Code / G  │     F     │   Enter   │           │   Left    │   Down    │   Right   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Open/Cmds │   Shift   │  Numbers  │     P     │     M     │      Navi / Space     │Second Base│   Lower   │ Previous  │Play/Pause │   Next    │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_BASE] = LAYOUT_planck_grid(
    KC_ESC,     KC_R,       CC_GUI_L,   CC_SFT_O,   CC_ALT_D,   CC_CTL_I,   KC_U,       KC_BSPACE,  XXXXXXX,     KC_MUTE,    KC_VOLD,    KC_VOLU,
    LOWERER,    KC_N,       CC_GUI_H,   CC_SFT_E,   CC_ALT_T,   CC_CTL_A,   KC_S,       CC_UNDO,    CC_REDO,     XXXXXXX,    KC_UP,      XXXXXXX,
    KC_TAB,     RAISE,      KC_B,       KC_K,       KC_C,       CC_CDE_G,   KC_F,       KC_ENTER,   XXXXXXX,     KC_LEFT,    KC_DOWN,    KC_RIGHT,
    CD_COMMAND, KC_LSFT,    NUMBERS,    KC_P,       KC_M,       CC_NAV_SPC, CC_NAV_SPC, SECOND_BASE,LOWER,       KC_MPRV,    KC_MPLY,    KC_MNXT
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Esc    │           │     ß     │     Ü     │     Ö     │     Ä     │           │ Backspace │           │   Mute    │ Volume -  │ Volume +  │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Lowerer  │           │     J     │     .     │     ,     │     Y     │     Z     │   Undo    │   Redo    │           │    Up     │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Tab    │   Raise   │     Q     │     V     │     W     │     X     │           │   Enter   │           │   Left    │   Down    │   Right   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Open/Cmds │   Shift   │  Numbers  │           │           │      Navi / Space     │Second Base│   Lower   │ Previous  │Play/Pause │   Next    │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_SECOND_BASE] = LAYOUT_planck_grid(
    _______,    XXXXXXX,    KC_MINS,    KC_LBRC,    KC_SCLN,    KC_QUOT,    XXXXXXX,    _______,    XXXXXXX,    _______,    _______,    _______,
    _______,    XXXXXXX,    KC_J,       KC_DOT,     KC_COMM,    KC_Z,       KC_Y,       _______,    _______,    XXXXXXX,    _______,    XXXXXXX,
    _______,    _______,    KC_Q,       KC_V,       KC_W,       KC_X,       XXXXXXX,    _______,    XXXXXXX,    _______,    _______,    _______,
    _______,    _______,    _______,    XXXXXXX,    XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______
  ),

/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Esc    │     @     │     €     │     _     │     [     │     ]     │     ^     │ Backspace │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Lowerer  │     ;     │     \     │     /     │     {     │     }     │     *     │   Undo    │ Wheel Up  │    M1     │ Mouse Up  │    M2     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Tab    │   Raise   │     #     │     $     │     |     │     ~     │     `     │   Enter   │ WheelDown │ Mouse <-- │ MouseDown │ Mouse --> │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Open/Cmds │   Shift   │           │           │           │      Navi / Space     │Second Base│   Lower   │ Wheel <-- │    M3     │ Wheel --> │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_LOWER] = LAYOUT_planck_grid(
    _______,    CC_ATSIGN,  RALT(KC_E), S(KC_SLSH), CC_LSBRC,   CC_RSBRC,   KC_GRV,     _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    _______,    S(KC_COMM), CC_BSLASH,  S(KC_7),    CC_LCBRC,   CC_RCBRC,   S(KC_RBRC), _______,    KC_WH_U,    KC_BTN1,    KC_MS_UP,   KC_BTN2,
    _______,    _______,    KC_NUHS,    S(KC_4),    CC_PIPE,    CC_TILDE,   S(KC_EQL),  _______,    KC_WH_D,    KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT,
    _______,    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,    _______,    _______,    _______,    KC_WH_L,    KC_BTN3,    KC_WH_R
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │    Esc    │     °     │     &     │     =     │     <     │     >     │     !     │ Backspace │           │  RGB Off  │   RGB -   │   RGB +   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │  Lowerer  │     ;     │     :     │     -     │     (     │     )     │     ?     │   Undo    │           │   Timer   │    Mac    │   Reset   │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │    Tab    │   Raise   │     '     │     "     │     %     │     +     │     ´     │   Enter   │           │ LED Level │  Windows  │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │ Open/Cmds │   Shift   │           │           │           │      Navi / Space     │Second Base│   Lower   │           │RGB Toggle │ RGB Next  │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_LOWERER] = LAYOUT_planck_grid(
    _______,    S(KC_GRV),  S(KC_6),    S(KC_0),    KC_NUBS,    S(KC_NUBS), S(KC_1),    _______,    XXXXXXX,    RSW_OFF,    RGB_VAD,    RGB_VAI,
    _______,    S(KC_COMM), S(KC_DOT),  KC_SLSH,    S(KC_8),    S(KC_9),    S(KC_MINS), _______,    XXXXXXX,    TIMER,      BASE_MAC,   RESET,
    _______,    _______,    S(KC_NUHS), S(KC_2),    S(KC_5),    KC_RBRC,    KC_EQL,     _______,    XXXXXXX,    LED_LEVEL,  BASE_WIN,   BASE_WIN,
    _______,    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,    _______,    _______,    _______,    XXXXXXX,    RGB_TOG,    RGB_MOD
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │    Esc    │  Page Up  │ Backspace │    Up     │   Down    │ Page Down │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │    Tab    │   Home    │   Left    │   Down    │   Right   │    End    │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │   Raise   │Select All │    Cut    │   Copy    │   Paste   │   Enter   │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │         Space         │Second Base│   Lower   │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_RAISE] = LAYOUT_planck_grid(
    XXXXXXX,    KC_ESC,     KC_PGUP,    KC_BSPC,    KC_UP,      KC_DEL,     KC_PGDN,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    KC_TAB,     CC_HOME,    KC_LEFT,    KC_DOWN,    KC_RGHT,    CC_END,     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    _______,    CC_SEL_ALL, CC_CUT,     CC_COPY,    CC_PASTE,   KC_ENTER,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_SPACE,   KC_SPACE,   _______,    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │           │     /     │     3     │     2     │     1     │     -     │ Backspace │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │     *     │     6     │     5     │     4     │     +     │           │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │     ;     │     9     │     8     │     7     │     :     │   Enter   │           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │  Numbers  │ Shift Tab │    Tab    │         Space         │     0     │     .     │     ,     │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_NUMBERS] = LAYOUT_planck_grid(
    XXXXXXX,    XXXXXXX,    S(KC_7),    KC_3,       KC_2,       KC_1,       KC_SLSH,    KC_BSPACE,  XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    S(KC_RBRC), KC_6,       KC_5,       KC_4,       KC_RBRC,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    S(KC_COMM), KC_9,       KC_8,       KC_7,       S(KC_DOT),  KC_ENTER,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    _______,    SFT_T(KC_TAB),KC_TAB,   KC_SPACE,   KC_SPACE,   KC_0,       KC_DOT,     KC_COMM,    XXXXXXX,    XXXXXXX
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │  Hyper L  │  Hyper K  │  Hyper J  │  Hyper I  │  App -->  │  <-- App  │           │    F1     │    F2     │    F3     │    F4     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │  Hyper D  │  Hyper C  │  Hyper B  │  Hyper A  │  Tab -->  │  <-- Tab  │           │    F5     │    F6     │    F7     │    F8     │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │  Hyper H  │  Hyper G  │  Hyper F  │  Hyper E  │           │           │           │    F9     │    F10    │    F11    │    F12    │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │           │           │           │         Navi          │           │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_NAVI] = LAYOUT_planck_grid(
    XXXXXXX,    HYPR(KC_L), HYPR(KC_L), HYPR(KC_J), HYPR(KC_I), CC_GUI_TAB, CC_SGU_TAB, XXXXXXX,    KC_F1,      KC_F2,      KC_F3,      KC_F4,
    XXXXXXX,    HYPR(KC_D), HYPR(KC_C), HYPR(KC_B), HYPR(KC_A), CC_CTL_TAB, CC_SCT_TAB, XXXXXXX,    KC_F5,      KC_F6,      KC_F7,      KC_F8,
    XXXXXXX,    HYPR(KC_H), HYPR(KC_G), HYPR(KC_F), HYPR(KC_E), XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_F9,      KC_F10,     KC_F11,     KC_F12,
    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    _______,    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
  ),
/*
  ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
  │           │           │Flip Layout│  Move Up  │    Up     │ Move Down │2x2 Layout │Peek Defntn│           │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │ Cursor Up │ Move Left │   Left    │   Down    │   Right   │Move Right │Definition │Light Mode │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │Cursor Down│  Search   │ Explorer  │  Comment  │   Code    │SaveNoFrmat│ Line Wrap │ Dark Mode │           │           │           │
  ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┴───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
  │           │           │   Back    │  Forward  │ Terminal  │         Save          │  Format   │           │           │           │           │
  └───────────┴───────────┴───────────┴───────────┴───────────┴───────────────────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
*/
  [_CODE] = LAYOUT_planck_grid(
    XXXXXXX,    XXXXXXX,    CD_FLIP,    CD_MV_UP,   CD_UP,      CD_MV_DOWN, LSA(KC_2),  A(KC_F12),  XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    CD_CSR_UP,  CD_MV_LEFT, CD_LEFT,    CD_DOWN,    CD_RIGHT,   CD_MV_RGHT, KC_F12,     CD_LIGHT,   XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    CD_CSR_DN,  CD_SEARCH,  CD_EXPLRER, CD_COMMENT, _______,    CD_SVNOFMT, A(KC_Z),    CD_DARK,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    CD_BACK,    CD_FORWARD, C(KC_SCLN), CD_SAVE,    CD_SAVE,    S(A(KC_F)), XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
  )
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

  [_GIT] = LAYOUT_planck_grid(
    XXXXXXX,    _______,    KC_Q,       KC_N,       KC_Z,       KC_E,       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_D,       KC_A,       KC_S,       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    KC_K,       KC_J,       S(KC_K),    S(KC_J),    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    _______,    XXXXXXX,    XXXXXXX,    XXXXXXX,    C(KC_C),    KC_ENTER,   KC_ENTER,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
  ),*/
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
        { RGB_MDF, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_BLK, RGB_MKY, RGB_MKY, RGB_MKY },
        { RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_BLK, RGB_ARR, RGB_BLK },
        { RGB_MDF, RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_MDF, RGB_BLK, RGB_ARR, RGB_ARR, RGB_ARR },
        { RGB_CDE, RGB_MDF, RGB_LYR, RGB_BSE, RGB_BSE,     RGB_BSE,      RGB_LYR, RGB_LYR, RGB_MKY, RGB_MKY, RGB_MKY },
    },
    [_SECOND_BASE] = {
        { RGB_MDF, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_MDF, RGB_BLK, RGB_MKY, RGB_MKY, RGB_MKY },
        { RGB_LYR, RGB_BLK, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_NAV, RGB_NAV, RGB_BLK, RGB_ARR, RGB_BLK },
        { RGB_MDF, RGB_LYR, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BSE, RGB_BLK, RGB_MDF, RGB_BLK, RGB_ARR, RGB_ARR, RGB_ARR },
        { RGB_CDE, RGB_MDF, RGB_LYR, RGB_BLK, RGB_BLK,     RGB_BSE,      RGB_LYR, RGB_LYR, RGB_MKY, RGB_MKY, RGB_MKY },
    },
    [_LOWER] = {
        { RGB_MDF, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_LYR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_NAV, RGB_MSE, RGB_MSE, RGB_ARR, RGB_MSE },
        { RGB_MDF, RGB_LYR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_MSE, RGB_ARR, RGB_ARR, RGB_ARR },
        { RGB_CDE, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK,     RGB_BSE,      RGB_LYR, RGB_LYR, RGB_MSE, RGB_MSE, RGB_MSE },
    },
    [_LOWERER] = {
        { RGB_MDF, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_BLK, RGB_SYS, RGB_SYS, RGB_SYS },
        { RGB_LYR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_NAV, RGB_BLK, RGB_SYS, RGB_BSE, RGB_SYS },
        { RGB_MDF, RGB_LYR, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_SYM, RGB_MDF, RGB_BLK, RGB_SYS, RGB_BSE, RGB_BLK },
        { RGB_CDE, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK,     RGB_BSE,      RGB_LYR, RGB_LYR, RGB_BLK, RGB_SYS, RGB_SYS },
    },
    [_RAISE] = {
        { RGB_BLK, RGB_MDF, RGB_MDF, RGB_MDF, RGB_ARR, RGB_MDF, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_MDF, RGB_MDF, RGB_ARR, RGB_ARR, RGB_ARR, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_LYR, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK,     RGB_BSE,      RGB_LYR, RGB_LYR, RGB_BLK, RGB_BLK, RGB_BLK },
    },
    [_NUMBERS] = {
        { RGB_BLK, RGB_BLK, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_SYM, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_SYM, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_SYM, RGB_NUM, RGB_NUM, RGB_NUM, RGB_SYM, RGB_MDF, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_LYR, RGB_MDF, RGB_MDF,     RGB_BSE,      RGB_NUM, RGB_SYM, RGB_SYM, RGB_BLK, RGB_BLK },
    },
    [_NAVI] = {
        { RGB_BLK, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV, RGB_MDF, RGB_MDF, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY },
        { RGB_BLK, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV, RGB_MDF, RGB_MDF, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY },
        { RGB_BLK, RGB_NAV, RGB_NAV, RGB_NAV, RGB_NAV, RGB_BLK, RGB_BLK, RGB_BLK, RGB_FKY, RGB_FKY, RGB_FKY, RGB_FKY },
        { RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK,     RGB_LYR,      RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
    [_CODE] = {
        { RGB_BLK, RGB_BLK, RGB_CDE, RGB_CDE, RGB_ARR, RGB_CDE, RGB_CDE, RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_CDE, RGB_CDE, RGB_ARR, RGB_ARR, RGB_ARR, RGB_CDE, RGB_CDE, RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_CDE, RGB_CDE, RGB_CDE, RGB_CDE, RGB_LYR, RGB_CDE, RGB_CDE, RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK },
        { RGB_BLK, RGB_BLK, RGB_CDE, RGB_CDE, RGB_CDE,     RGB_CDE,      RGB_CDE, RGB_BLK, RGB_BLK, RGB_BLK, RGB_BLK },
    },
};
/** set key at idx to color from colors */
void rgb_matrix_set_array(int idx, int name) {
  rgb_matrix_set_color(idx, colors[name][0], colors[name][1], colors[name][2]);
}
/** set all keys to layername */
void rgb_set_all(int layername) {
  current_rgb_layer = layername;

  if (!is_rgb) {
    rgb_matrix_set_color_all(RGB_OFF);
    return;
  }

  for (int i = 0; i < 3; i++) for (int j = 0; j < 12; j++) {
    rgb_matrix_set_array((i * 12) + j, rgbmaps[layername][i][j]);
  }
  for (int j = 0; j < 11; j++) rgb_matrix_set_array(36 + j, rgbmaps[layername][3][j]);

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

  planck_ez_left_led_off();
  planck_ez_right_led_off();

  switch (get_highest_layer(state)) {
    case _LOWER:
    case _LOWERER:
      planck_ez_left_led_on(); 
      break;
    case _RAISE:
    case _NUMBERS:
      planck_ez_right_led_on();
      break;
    case _SECOND_BASE: 
      planck_ez_left_led_on();
      planck_ez_right_led_on();
      break;
  }
  rgb_set_all(get_highest_layer(state));

  return state;
}


/* various helpers to extract repeating code */
bool custom_vscode_command_hold(keyrecord_t *record, const char *tapstr, const char *holdstr) {
  if (record->event.pressed) {
    SEND_STRING(is_mac ? SS_LGUI(SS_LSFT("p")) : SS_LCTL(SS_LSFT("p")));
    if (record->tap.count) {
      SEND_STRING(tapstr);
    } else {
      SEND_STRING(holdstr);
    }
    SEND_STRING("\n");
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
    case CC_FULLNXT: return custom_tap_hold(record, is_mac ? G(S(KC_EQL)) : HYPR(KC_Z),    is_mac ? G(C(KC_F)) : KC_F11);
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
    case CD_UP:      return custom_vscode_command_hold(record, "focus editor group above", "split editor into group above");
    case CD_DOWN:    return custom_vscode_command_hold(record, "focus editor group below", "split editor into group below");
    case CD_LEFT:    return custom_vscode_command_hold(record, "focus left editor group",  "split editor into left group");
    case CD_RIGHT:   return custom_vscode_command_hold(record, "focus right editor group", "split editor into right group");
    case CD_MV_UP:   return custom_vscode_command_hold(record, "move editor group up",     "move editor into group above");
    case CD_MV_DOWN: return custom_vscode_command_hold(record, "move editor group down",   "move editor into group below");
    case CD_MV_LEFT: return custom_vscode_command_hold(record, "move editor group left",   "move editor into left group");
    case CD_MV_RGHT: return custom_vscode_command_hold(record, "move editor group right",  "move editor into right group");

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

void matrix_scan_user(void) {
  // blink keys in system layer
  if (current_rgb_layer == _LOWERER && timer_elapsed(blink_light_timer) > 1000) {
    blink_light_timer = timer_read();
    blink_light_on = !blink_light_on;
    if (blink_light_on) {
      rgb_matrix_set_array(22, RGB_BSE);
      rgb_matrix_set_array(34, RGB_BSE);
      rgb_matrix_set_array(21, RGB_SYS);
    } else {
      if (is_staying_awake) rgb_matrix_set_array(21, RGB_BLK);
      rgb_matrix_set_array(is_mac ? 22 : 34, RGB_BLK);
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
