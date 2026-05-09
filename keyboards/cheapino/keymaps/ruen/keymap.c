/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "ruen.h"

enum cheapino_layers {
    _BASE = 0,
    _SYM,
    _NUM_NAV,
    _NAV,
    _ADJ,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_record_ruen(keycode, record);
}

void keyboard_post_init_user(void) {
    ruen_init();
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_NO,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_NO,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    KC_P,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_LBRC, KC_RBRC, KC_NO,
                          KC_TAB,  KC_SPC,  MO(_NAV),         MO(_SYM),MO(_NUM_NAV), RUEN_TOGGLE
    ),

    [_SYM] = LAYOUT_split_3x5_3(
        RUEN_GRAVE, RUEN_LT,    RUEN_GT,   RUEN_DQUOTE, RUEN_QUOTE, KC_NO, RUEN_TILDE, RUEN_AMP,  RUEN_LCBR, RUEN_RCBR, RUEN_HASH,
        KC_EXLM,    RUEN_SLASH, KC_MINS,   KC_EQL,      RUEN_PIPE,         KC_PERC,    KC_UNDS,   KC_LPRN,   KC_RPRN,   RUEN_QST,
        RUEN_BSLS,  KC_ASTR,    KC_PLUS,   RUEN_AT,     RUEN_DOLLAR,       RUEN_COMMA, RUEN_DOT,  RUEN_LBRC, RUEN_RBRC, RUEN_CARET,
                                RUEN_SCLN, RUEN_COLON,  MO(_ADJ),          KC_NO,      KC_NO,     KC_NO
    ),

    [_NUM_NAV] = LAYOUT_split_3x5_3(
        KC_9,    KC_8,    KC_7,    KC_6,    KC_5,    KC_NO,    KC_NO,      RSFT(KC_ENT),  RCTL(KC_DEL),  KC_DEL,  KC_PSCR,
        KC_4,    KC_3,    KC_2,    KC_1,    KC_0,              KC_NO,      KC_ENT,        RCTL(KC_BSPC), KC_BSPC, KC_ESC,
        KC_PGDN, KC_PGUP, KC_HOME, KC_END,  KC_NO,             RUEN_COMMA, RUEN_DOT,      KC_LBRC,       KC_RBRC, KC_NO,
                          KC_NO,   OSM(MOD_LSFT), KC_NO,       KC_NO,      KC_NO,         KC_NO
    ),

    [_NAV] = LAYOUT_split_3x5_3(
        KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_NO, KC_NO,  RSFT(RCTL(KC_LEFT)), RSFT(RCTL(KC_DOWN)), RSFT(RCTL(KC_UP)), RSFT(RCTL(KC_RGHT)), KC_NO,
        KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_NO,         KC_LEFT,             KC_DOWN,             KC_UP,             KC_RGHT,             KC_NO,
        KC_F9,  KC_F10, KC_F11, KC_F12, KC_NO,         RCTL(KC_LEFT),       RCTL(KC_DOWN),       RCTL(KC_UP),       RCTL(KC_RGHT),       KC_NO,
                        KC_NO,  KC_NO,  KC_NO,         MO(_ADJ),            KC_NO,               KC_NO
    ),

    [_ADJ] = LAYOUT_split_3x5_3(
        QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_VOLD, KC_MUTE, KC_VOLU, KC_PSCR,
        KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,        KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,
                        KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO,   KC_NO
    ),
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE]    = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [_SYM]     = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [_NUM_NAV] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NAV]     = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
    [_ADJ]     = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
};
