// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum helix_layers {
    _BASE_L,
    _LOWER_L,
    _RAISE_L,
    _ADJUST_L,
    _CODE_L,
    _MOUSE_L,
    _NAVI_L,
    _QWERTZ_L,
    _BASE_R,
    _LOWER_R,
    _RAISE_R,
    _ADJUST_R,
    _CODE_R,
    _MOUSE_R,
    _NAVI_R,
    _QWERTZ_R,
};

enum helix_keycodes {
    BASE_MAC,
    BASE_WIN,
    /* Swap Hands keycode */
    SWAP_HANDS,
    /* Tab switching keys */
    CC_GUI_TAB,
    CC_SGU_TAB,
    CC_CTL_TAB,
    CC_SCT_TAB,
};

/** hold ctl / gui while true */
bool is_app_switching = false;
/** hold ctl while true */
bool is_tab_switching = false;
/** switch between mac and other keycodes */
bool is_mac = true;

/* Layers */
#define LOWER_L TT(_LOWER_L)
#define LOWER_R TT(_LOWER_R)
#define RAISE_L TT(_RAISE_L)
#define RAISE_R TT(_RAISE_R)
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
#define CC_P_MSG LT(_MOU, KC_P)
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

#define _________________QWERTY_L1_________________ KC_Q, KC_W, KC_E, KC_R, KC_T

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┐
     * │Tab│ Q │ W │ E │ R │ T │
     * ├───┼───┼───┼───┼───┼───┤
     * │Bsp│ A │ S │ D │ F │ G │
     * ├───┼───┼───┼───┼───┼───┤
     * │Sft│ Z │ X │ C │ V │ B │
     * ├───┼───┼───┼───┼───┼───┤
     * │Ctl│App│GUI│Alt│Spc│Spc│
     * └───┴───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT(KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_BSPC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LCTL, KC_MENU, KC_LGUI, KC_LALT, KC_SPC, KC_SPC)};
