// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

/*
* define layers: left hand, right hand with _ALT
* redefine SWAP behaviour: set var, if true change keycode at start of process_record_user
    -> never actually change into _ALT layer, only read keycodes from those
* permanent hand swap: swap left hand / right hand layers themselves
*/

enum helix_layers {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST,
    _CODE,
    _MOUSE,
    _NAVI,
    _QWERTZ,

    _BASE_ALT,
    _LOWER_ALT,
    _RAISE_ALT,
    _ADJUST_ALT,
    _CODE_ALT,
    _MOUSE_ALT,
    _NAVI_ALT,
    _QWERTZ_ALT,
};

enum helix_keycodes {
    BASE_MAC = SAFE_RANGE,
    BASE_WIN,
    BASE_LEFT,
    BASE_RIGHT,
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
/** use _ALT keycodes instead */
bool swap_hands = false;
/** currently active layer */
int current_layer = _BASE;

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

/* Combos */
const uint16_t PROGMEM C_SCROLL[] =  {KC_BTN1,  CC_MSE_ENT, COMBO_END};
const uint16_t PROGMEM C_DPI[] =     {KC_LEFT,  KC_RIGHT,   COMBO_END};

const uint16_t PROGMEM C_AE[] =      {CC_T_CTL, CC_M_CTL,   COMBO_END};
const uint16_t PROGMEM C_OE[] =      {CC_I_ALT, CC_U_ALT,   COMBO_END};
const uint16_t PROGMEM C_UE[] =      {CC_C_SFT, CC_F_SFT,   COMBO_END};
const uint16_t PROGMEM C_SS[] =      {KC_D,     KC_V,       COMBO_END};

const uint16_t PROGMEM C_ESC[] =     {KC_TAB,   CC_F_SFT,   CC_U_ALT, CC_M_CTL, COMBO_END};

const uint16_t PROGMEM C_HEX_A[] =   {KC_1,     KC_2,       KC_3,     COMBO_END};
const uint16_t PROGMEM C_HEX_B[] =   {KC_2,     KC_3,       KC_TAB,   COMBO_END};
const uint16_t PROGMEM C_HEX_C[] =   {KC_4,     KC_5,       KC_6,     COMBO_END};
const uint16_t PROGMEM C_HEX_D[] =   {KC_5,     KC_6,       KC_0,     COMBO_END};
const uint16_t PROGMEM C_HEX_E[] =   {KC_7,     KC_8,       KC_9,     COMBO_END};
const uint16_t PROGMEM C_HEX_F[] =   {KC_8,     KC_9,      CC_SMCOLON,COMBO_END};
const uint16_t PROGMEM C_HEX_X[] =   {CC_COLON, KC_7,       KC_8,     COMBO_END};

combo_t key_combos[] = {
  COMBO(C_SCROLL, MS_SCRL),
  COMBO(C_DPI,    MS_DPI),

  COMBO(C_AE, KC_QUOT),
  COMBO(C_OE, KC_SCLN),
  COMBO(C_UE, KC_LBRC),
  COMBO(C_SS, KC_MINS),

  COMBO(C_ESC, KC_ESC),
  
  COMBO(C_HEX_A, KC_A),
  COMBO(C_HEX_B, KC_B),
  COMBO(C_HEX_C, KC_C),
  COMBO(C_HEX_D, KC_D),
  COMBO(C_HEX_E, KC_E),
  COMBO(C_HEX_F, KC_F),
  COMBO(C_HEX_X, KC_X),
};


#define ___________________NOTHING___________________ XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX

#define ___________________BASE_L1___________________ KC_TAB,     CC_F_SFT,   CC_U_ALT,   CC_M_CTL,   KC_V,       CC_SPTLGHT
#define ___________________BASE_L2___________________ CC_A_GUI,   CC_C_SFT,   CC_I_ALT,   CC_T_CTL,   KC_D,       CC_UNDO
#define ___________________BASE_L3___________________ RAISE,      CC_P_MSG,   KC_Z,       CC_G_CDE,   KC_J,       CC_REDO
#define ___________________BASE_L4___________________ SWAP_HANDS, CC_CLOSE,   CD_COMMAND, LOWER,      KC_E,       CC_SPC_NAV
#define ___________________BASE_R1___________________ KC_BSPC,    KC_Q,       KC_L,       KC_B,       KC_DOT,     KC_Y
#define ___________________BASE_R2___________________ KC_UP,      KC_R,       CC_N_CTL,   CC_H_ALT,   CC_O_SFT,   CC_S_GUI
#define ___________________BASE_R3___________________ KC_DOWN,    KC_X,       CC_W_CDE,   KC_K,       CC_COMM_MSG,CC_MSE_ENT
#define ___________________BASE_R4___________________ CC_SPC_NAV, OSM(MOD_LSFT),RAISE,    KC_LEFT,    KC_RIGHT,   KC_BTN1

#define __________________LOWER_L1___________________ CC_ATSIGN,  CC_UNDRSCR, CC_LSBRC,   CC_RSBRC,   CC_GRAVE,   CC_CIRCLE
#define __________________LOWER_L2___________________ CC_BSLASH,  CC_SLSH,    CC_LCBRC,   CC_RCBRC,   CC_ASTERISK,CC_EURO
#define __________________LOWER_L3___________________ CC_HSH_RSE, CC_DOLLAR,  CC_PIPE,    CC_TILDE,   CC_BTICK,   CC_TICK
#define __________________LOWER_L4___________________ _______,    XXXXXXX,    XXXXXXX,    _______,    XXXXXXX,    KC_SPACE
#define __________________LOWER_R1___________________ _______,    CC_EXCL,    CC_LPBRC,   CC_RPBRC,   CC_EQUAL,   CC_AMPER
#define __________________LOWER_R2___________________ _______,    CC_QUESTNM, CC_LRBRC,   CC_RRBRC,   CC_DASH,    CC_COLON
#define __________________LOWER_R3___________________ _______,    CC_PLUS,    CC_PERCENT, CC_DQUOT,   CC_QUOT,    CC_SMCOLON
#define __________________LOWER_R4___________________ KC_SPACE,   XXXXXXX,    _______,    _______,    _______,    _______

#define __________________RAISE_L1___________________ KC_PGUP,    KC_BSPC,    KC_UP,      KC_DEL,     KC_PGDN,    _______
#define __________________RAISE_L2___________________ CC_HOME,    KC_LEFT,    KC_DOWN,    KC_RGHT,    CC_END,     _______
#define __________________RAISE_L3___________________ _______,    CC_CUT,     CC_COPY,    CC_PASTE,   KC_ENTER,   _______
#define __________________RAISE_L4___________________ _______,    KC_TAB,     CC_SEL_ALL, _______,    XXXXXXX,    KC_SPACE
#define __________________RAISE_R1___________________ _______,    CC_DASH,    KC_1,       KC_2,       KC_3,       KC_TAB
#define __________________RAISE_R2___________________ CC_ASTERISK,CC_PLUS,    KC_4,       KC_5,       KC_6,       KC_0
#define __________________RAISE_R3___________________ CC_SLSH,    CC_COLON,   KC_7,       KC_8,       KC_9,       CC_SMCOLON
#define __________________RAISE_R4___________________ KC_SPACE,   KC_ENTER,   _______,    KC_DOT,     KC_COMM,    _______

#define _________________ADJUST_L1___________________ RGB_TOG,    RGB_VAI,    KC_MNXT,    KC_VOLU,    KC_BRIU,    BASE_MAC
#define _________________ADJUST_L2___________________ RGB_MOD,    RSW_OFF,    KC_MPLY,    KC_MUTE,    KC_BRID,    QWERTZ
#define _________________ADJUST_L3___________________ _______,    RGB_VAD,    KC_MPRV,    KC_VOLD,    XXXXXXX,    BASE_WIN
#define _________________ADJUST_L4___________________ _______,    XXXXXXX,    XXXXXXX,    _______,    LED_LEVEL,  TIMER
#define _________________ADJUST_R1___________________ XXXXXXX,    KC_PSCR,    KC_F1,      KC_F2,      KC_F3,      KC_F4
#define _________________ADJUST_R2___________________ DM_PLY1,    KC_NUM_LOCK,KC_F5,      KC_F6,      KC_F7,      KC_F8
#define _________________ADJUST_R3___________________ DM_PLY2,    XXXXXXX,    KC_F9,      KC_F10,     KC_F11,     KC_F12
#define _________________ADJUST_R4___________________ TIMER,      KC_CAPS,    _______,    XXXXXXX,    XXXXXXX,    _______

#define ___________________CODE_L1___________________ CD_FLIP,    CD_MV_UP,   CD_UP,      CD_MV_DOWN, XXXXXXX,    XXXXXXX
#define ___________________CODE_L2___________________ CD_MV_LEFT, CD_LEFT,    CD_DOWN,    CD_RIGHT,   CD_MV_RGHT, XXXXXXX
#define ___________________CODE_L3___________________ CD_SEARCH,  CD_EXPLRER, CD_COMMENT, _______,    CD_SVNOFMT, XXXXXXX
#define ___________________CODE_L4___________________ _______,    XXXXXXX,    XXXXXXX,    S(A(KC_F)), CD_SAVE,    XXXXXXX
#define ___________________CODE_R1___________________ XXXXXXX,    XXXXXXX,    XXXXXXX,    CD_CSR_UP,  A(KC_F12),  XXXXXXX
#define ___________________CODE_R2___________________ XXXXXXX,    CD_LIGHT,   CD_BACK,    CD_CSR_DN,  CD_FORWARD, CD_RENAME
#define ___________________CODE_R3___________________ XXXXXXX,    CD_DARK,    _______,    A(KC_Z),    KC_F12,     CD_CH_ALL
#define ___________________CODE_R4___________________ XXXXXXX,    C(KC_SCLN), XXXXXXX,    XXXXXXX,    XXXXXXX,    _______

#define __________________MOUSE_L1___________________ XXXXXXX,    XXXXXXX,    KC_MS_UP,    XXXXXXX,   XXXXXXX,    XXXXXXX
#define __________________MOUSE_L2___________________ KC_LCTL,    KC_MS_LEFT, KC_MS_DOWN,  KC_MS_RIGHT,KC_WH_U,   XXXXXXX
#define __________________MOUSE_L3___________________ KC_LGUI,    _______,    KC_WH_L,     KC_WH_R,   KC_WH_D,    XXXXXXX
#define __________________MOUSE_L4___________________ _______,    _______,    MS_SCRL,     KC_BTN2,   KC_BTN1,    _______
#define __________________MOUSE_R1___________________ XXXXXXX,    KC_Q,       S(KC_J),    S(KC_K),    XXXXXXX,    XXXXXXX
#define __________________MOUSE_R2___________________ XXXXXXX,    KC_E,       KC_A,       KC_D,       XXXXXXX,    XXXXXXX
#define __________________MOUSE_R3___________________ XXXXXXX,    KC_S,       KC_Z,       KC_N,       _______,    _______
#define __________________MOUSE_R4___________________ _______,    KC_ENTER,   C(KC_C),    C(KC_X),    XXXXXXX,    _______

#define ___________________NAVI_L1___________________ XXXXXXX,    XXXXXXX,    CC_SGU_TAB, CC_GUI_TAB, MEH(KC_A),  MEH(KC_B)
#define ___________________NAVI_L2___________________ XXXXXXX,    C(KC_SPC),  CC_SCT_TAB, CC_CTL_TAB, MEH(KC_E),  MEH(KC_F)
#define ___________________NAVI_L3___________________ XXXXXXX,    XXXXXXX,    CC_FULLNXT, CC_CLOSE,   MEH(KC_I),  MEH(KC_J)
#define ___________________NAVI_L4___________________ _______,    _______,    XXXXXXX,    MEH(KC_O),  MEH(KC_M),  _______
#define ___________________NAVI_R1___________________ MEH(KC_C),  MEH(KC_D),  HYPR(KC_I), HYPR(KC_J), HYPR(KC_K), HYPR(KC_L)
#define ___________________NAVI_R2___________________ MEH(KC_G),  MEH(KC_H),  HYPR(KC_A), HYPR(KC_B), HYPR(KC_C), HYPR(KC_D)
#define ___________________NAVI_R3___________________ MEH(KC_K),  MEH(KC_L),  HYPR(KC_E), HYPR(KC_F), HYPR(KC_G), HYPR(KC_H)
#define ___________________NAVI_R4___________________ _______,    MEH(KC_N),  MEH(KC_P),  XXXXXXX,    XXXXXXX,    _______

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        ___________________NOTHING___________________,
        ___________________BASE_L1___________________,
        ___________________BASE_L2___________________,
        ___________________BASE_L3___________________, BASE_LEFT,
        ___________________BASE_L4___________________, BASE_RIGHT
    ),
    [_LOWER] = LAYOUT(
        ___________________NOTHING___________________,
        __________________LOWER_L1___________________,
        __________________LOWER_L2___________________,
        __________________LOWER_L3___________________, BASE_LEFT,
        __________________LOWER_L4___________________, BASE_RIGHT
    ),
    [_RAISE] = LAYOUT(
        ___________________NOTHING___________________,
        __________________RAISE_L1___________________,
        __________________RAISE_L2___________________,
        __________________RAISE_L3___________________, BASE_LEFT,
        __________________RAISE_L4___________________, BASE_RIGHT
    ),
    [_ADJUST] = LAYOUT(
        ___________________NOTHING___________________,
        _________________ADJUST_L1___________________,
        _________________ADJUST_L2___________________,
        _________________ADJUST_L3___________________, BASE_LEFT,
        _________________ADJUST_L4___________________, BASE_RIGHT
    ),
    [_CODE] = LAYOUT(
        ___________________NOTHING___________________,
        ___________________CODE_L1___________________,
        ___________________CODE_L2___________________,
        ___________________CODE_L3___________________, BASE_LEFT,
        ___________________CODE_L4___________________, BASE_RIGHT
    ),
    [_MOUSE] = LAYOUT(
        ___________________NOTHING___________________,
        __________________MOUSE_L1___________________,
        __________________MOUSE_L2___________________,
        __________________MOUSE_L3___________________, BASE_LEFT,
        __________________MOUSE_L4___________________, BASE_RIGHT
    ),
    [_NAVI] = LAYOUT(
        ___________________NOTHING___________________,
        ___________________NAVI_L1___________________,
        ___________________NAVI_L2___________________,
        ___________________NAVI_L3___________________, BASE_LEFT,
        ___________________NAVI_L4___________________, BASE_RIGHT
    ),
    [_QWERTZ] = LAYOUT(
        KC_ESC,  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,  
        KC_TAB,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,  
        KC_LCTL, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,
        KC_LSFT, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   BASE_LEFT,
        SWAP_HANDS,KC_LCTL,KC_ALT,KC_GUI,KC_SPACE,KC_SPACE,BASE_RIGHT
    )

    [_BASE_ALT] = LAYOUT(
        ___________________NOTHING___________________,
        ___________________BASE_R1___________________,
        ___________________BASE_R2___________________,
        ___________________BASE_R3___________________, BASE_LEFT,
        ___________________BASE_R4___________________, BASE_RIGHT
    ),
    [_LOWER_ALT] = LAYOUT(
        ___________________NOTHING___________________,
        __________________LOWER_R1___________________,
        __________________LOWER_R2___________________,
        __________________LOWER_R3___________________, BASE_LEFT,
        __________________LOWER_R4___________________, BASE_RIGHT
    ),
    [_RAISE_ALT] = LAYOUT(
        ___________________NOTHING___________________,
        __________________RAISE_R1___________________,
        __________________RAISE_R2___________________,
        __________________RAISE_R3___________________, BASE_LEFT,
        __________________RAISE_R4___________________, BASE_RIGHT
    ),
    [_ADJUST_ALT] = LAYOUT(
        ___________________NOTHING___________________,
        _________________ADJUST_R1___________________,
        _________________ADJUST_R2___________________,
        _________________ADJUST_R3___________________, BASE_LEFT,
        _________________ADJUST_R4___________________, BASE_RIGHT
    ),
    [_CODE_ALT] = LAYOUT(
        ___________________NOTHING___________________,
        ___________________CODE_R1___________________,
        ___________________CODE_R2___________________,
        ___________________CODE_R3___________________, BASE_LEFT,
        ___________________CODE_R4___________________, BASE_RIGHT
    ),
    [_MOUSE_ALT] = LAYOUT(
        ___________________NOTHING___________________,
        __________________MOUSE_R1___________________,
        __________________MOUSE_R2___________________,
        __________________MOUSE_R3___________________, BASE_LEFT,
        __________________MOUSE_R4___________________, BASE_RIGHT
    ),
    [_NAVI_ALT] = LAYOUT(
        ___________________NOTHING___________________,
        ___________________NAVI_R1___________________,
        ___________________NAVI_R2___________________,
        ___________________NAVI_R3___________________, BASE_LEFT,
        ___________________NAVI_R4___________________, BASE_RIGHT
    ),
    [_QWERTZ_ALT] = LAYOUT(
        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,  BASE_LEFT,
        KC_SPACE,KC_SPACE,KC_GUI,  KC_ALT,  KC_RCTL, KC_RSFT, BASE_RIGHT
        
    )
};

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

    current_layer = get_highest_layer(state);

    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
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
    if (swap_hands) {
        keycode = keymaps[current_layer + _BASE_ALT][record->event.key.row][5 - record->event.key.col]
    }
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
        case BASE_LEFT:
        case BASE_RIGHT:
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

void raw_hid_receive(uint8_t *data, uint8_t length) {
    raw_hid_send(data, length);
}
