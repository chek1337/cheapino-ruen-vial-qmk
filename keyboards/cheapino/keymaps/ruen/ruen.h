#pragma once

#include "quantum.h"

enum ruen_keycodes {
    RUEN_TOGGLE = QK_KB_0,
    RUEN_EN,
    RUEN_RU,
    RUEN_DOT,
    RUEN_COMMA,
    RUEN_SCLN,
    RUEN_COLON,
    RUEN_DQUOTE,
    RUEN_QST,
    RUEN_SLASH,
    RUEN_LBRC,
    RUEN_RBRC,
    RUEN_LCBR,
    RUEN_RCBR,
    RUEN_LT,
    RUEN_GT,
    RUEN_GRAVE,
    RUEN_TILDE,
    RUEN_AT,
    RUEN_HASH,
    RUEN_DOLLAR,
    RUEN_CARET,
    RUEN_AMP,
    RUEN_PIPE,
    RUEN_NUM,
    RUEN_SYNC,
    RUEN_MOD,
    RUEN_WORD,
    RUEN_STORE,
    RUEN_REVERT,
    RUEN_QUOTE,
    RUEN_BSLS,
    RUEN_LAYER_TOGGLE,
    RUEN_CSC,
    RUEN_CSV,
};

bool process_record_ruen(uint16_t keycode, keyrecord_t *record);
void ruen_init(void);

void ruen_force_en(void);
void ruen_force_ru(void);
bool ruen_is_ru(void);
void ruen_tap_in_en(uint16_t keycode);
