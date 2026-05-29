/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "ruen.h"

enum cheapino_layers {
    _BASE_EN = 0,
    _BASE_RU,
    _SYM,
    _NUM_NAV,
    _NAV,
    _ADJ,
};

#define HM_A LGUI_T(KC_A)
#define HM_S LALT_T(KC_S)
#define HM_D LSFT_T(KC_D)
#define HM_F LCTL_T(KC_F)
#define HM_J RCTL_T(KC_J)
#define HM_K RSFT_T(KC_K)
#define HM_L LALT_T(KC_L)
#define HM_P RGUI_T(KC_P)
#define HM_SCLN RGUI_T(KC_SCLN)

#define LT_NAV LT(_NAV, KC_NO)
#define LT_SYM LT(_SYM, KC_NO)
#define LT_NUM LT(_NUM_NAV, KC_NO)
#define LT_ADJ LT(_ADJ, KC_NO)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case RUEN_LAYER_TOGGLE:
                if (layer_state_is(_BASE_RU)) {
                    ruen_force_en();
                    layer_move(_BASE_EN);
                } else {
                    ruen_force_ru();
                    layer_move(_BASE_RU);
                }
                return false;
        }
    }
    return process_record_ruen(keycode, record);
}

void keyboard_post_init_user(void) {
    ruen_init();
}

// HOOKP per-key: instant hold for thumb-LT (so the next key's lookup happens
// on the new layer), normal tap-hold for HRM (so cross-hand typing doesn't
// fire mods eagerly). Vial defines get_hold_on_other_key_press as non-weak,
// so this override only compiles in pure-QMK builds.
#ifndef QMK_SETTINGS
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT_NAV:
        case LT_SYM:
        case LT_NUM:
        case LT_ADJ:
            return true;
    }
    return false;
}
#endif

// Thumb keys marked '*' so any chord with them settles instantly as hold,
// regardless of which hand the other key is on.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_split_3x5_3(
    'L', 'L', 'L', 'L', 'L', '*', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
                   '*', '*', '*', '*', '*', '*'
);

// Static combos for pure-QMK build. In Vial build combos are managed at runtime
// via Vial GUI, so this array would conflict — guard it out there.
#ifndef QMK_SETTINGS
enum combos {
    COMBO_ER_LAYER_TOGGLE,
    COMBO_ZC_CSC,
    COMBO_ZV_CSV,
    COMBO_F_SPC_CTRL_SPC,
    COMBO_D_SPC_CTRL_W,
};

const uint16_t PROGMEM combo_er[]    = {KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM combo_zc[]    = {KC_Z, KC_C, COMBO_END};
const uint16_t PROGMEM combo_zv[]    = {KC_Z, KC_V, COMBO_END};
const uint16_t PROGMEM combo_f_spc[] = {HM_F, KC_SPC, COMBO_END};
const uint16_t PROGMEM combo_d_spc[] = {HM_D, KC_SPC, COMBO_END};

combo_t key_combos[] = {
    [COMBO_ER_LAYER_TOGGLE] = COMBO(combo_er,    RUEN_LAYER_TOGGLE),
    [COMBO_ZC_CSC]          = COMBO(combo_zc,    RUEN_CSC),
    [COMBO_ZV_CSV]          = COMBO(combo_zv,    RUEN_CSV),
    [COMBO_F_SPC_CTRL_SPC]  = COMBO(combo_f_spc, LCTL(KC_SPC)),
    [COMBO_D_SPC_CTRL_W]    = COMBO(combo_d_spc, LCTL(KC_W)),
};
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE_EN] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_NO,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_NO,
        HM_A,    HM_S,    HM_D,    HM_F,    KC_G,             KC_H,    HM_J,    HM_K,    HM_L,    HM_P,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_LBRC, KC_RBRC, KC_NO,
                          KC_TAB,  KC_SPC,  LT_NAV,           LT_SYM,  LT_NUM,       RUEN_LAYER_TOGGLE
    ),

    [_BASE_RU] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_NO,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        HM_A,    HM_S,    HM_D,    HM_F,    KC_G,             KC_H,    HM_J,    HM_K,    HM_L,    HM_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_QUOT,
                          KC_TAB,  KC_SPC,  LT_NAV,           LT_SYM,  LT_NUM,       RUEN_LAYER_TOGGLE
    ),

    [_SYM] = LAYOUT_split_3x5_3(
        RUEN_GRAVE, RUEN_LT,    RUEN_GT,   RUEN_DQUOTE, RUEN_QUOTE, KC_NO, RUEN_AMP, RUEN_TILDE,  RUEN_LCBR, RUEN_RCBR, RUEN_HASH,
        KC_EXLM,    RUEN_SLASH, KC_MINS,   KC_EQL,      RUEN_PIPE,         KC_PERC,    KC_UNDS,   KC_LPRN,   KC_RPRN,   RUEN_QST,
        RUEN_BSLS,  KC_ASTR,    KC_PLUS,   RUEN_AT,     RUEN_DOLLAR,       RUEN_COMMA, RUEN_DOT,  RUEN_LBRC, RUEN_RBRC, RUEN_CARET,
                                RUEN_SCLN, RUEN_COLON,  LT_ADJ,            KC_NO,      KC_NO,     KC_NO
    ),

    [_NUM_NAV] = LAYOUT_split_3x5_3(
        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,    KC_NO,      RSFT(KC_ENT),  RCTL(KC_DEL),  KC_DEL,  KC_PSCR,
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,              KC_NO,      KC_ENT,        RCTL(KC_BSPC), KC_BSPC, KC_ESC,
        KC_PGDN, KC_PGUP, KC_HOME, KC_END,  KC_NO,             RUEN_COMMA, RUEN_DOT,      KC_LBRC,       KC_RBRC, KC_NO,
                          RUEN_WORD, OSM(MOD_LSFT), CW_TOGG,   KC_NO,      KC_NO,         KC_NO
    ),

    [_NAV] = LAYOUT_split_3x5_3(
        KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_NO, KC_NO,  RSFT(RCTL(KC_LEFT)), RSFT(RCTL(KC_DOWN)), RSFT(RCTL(KC_UP)), RSFT(RCTL(KC_RGHT)), KC_NO,
        KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_NO,         KC_LEFT,             KC_DOWN,             KC_UP,             KC_RGHT,             KC_NO,
        KC_F9,  KC_F10, KC_F11, KC_F12, KC_NO,         RCTL(KC_LEFT),       RCTL(KC_DOWN),       RCTL(KC_UP),       RCTL(KC_RGHT),       KC_NO,
                        KC_NO,  KC_NO,  KC_NO,         LT_ADJ,              KC_NO,               KC_NO
    ),

    [_ADJ] = LAYOUT_split_3x5_3(
        QK_BOOT, RGB_TOG, RGB_MOD,  RGB_RMOD, KC_NO,   KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,    KC_NO,
        KC_NO,   RGB_VAD, RGB_VAI,  RGB_HUD,  RGB_HUI,        KC_NO, KC_VOLD, KC_MUTE, KC_VOLU, KC_PSCR,
        EE_CLR,   RGB_SAD, RGB_SAI,  KC_NO,    KC_NO,          KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,
                          KC_NO,    KC_NO,    KC_NO,          KC_NO, KC_NO,   KC_NO
    ),
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE_EN] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [_BASE_RU] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [_SYM]     = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [_NUM_NAV] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NAV]     = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
    [_ADJ]     = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
};
