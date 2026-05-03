/* SPDX-License-Identifier: GPL-2.0-or-later */

/*
 * RuEn port for Cheapino v2.
 *
 * Logic ported from silakka54-with-ruen-port (Win/Linux variant).
 * Sends Win+Space (or Ctrl+Space when keymap_config.swap_lctl_lgui is set)
 * to switch the OS-level keyboard layout, and provides custom keycodes
 * for layout-independent punctuation/symbols.
 *
 * Custom keycodes defined here MUST be mirrored in vial.json
 * under "customKeycodes".
 */

#include QMK_KEYBOARD_H

enum cheapino_layers {
    _BASE = 0,
    _SYM,
    _NUM_NAV,
    _NAV,
    _ADJ,
};

enum custom_keycodes {
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
};

static bool ruen_is_russian = false;
static bool ruen_saved_state = false;
static bool ruen_mod_restore_state = false;
static uint16_t ruen_mod_timer = 0;
static bool ruen_word_active = false;

static void ruen_send_layout_switch(void) {
    uint8_t mods = get_mods();

    if (mods != 0) {
        del_mods(mods);
    }

    if (keymap_config.swap_lctl_lgui) {
        register_code(KC_LCTL);
        tap_code(KC_SPACE);
        wait_ms(50);
        unregister_code(KC_LCTL);
        wait_ms(50);
    } else {
        register_code(KC_LGUI);
        tap_code(KC_SPACE);
        wait_ms(50);
        unregister_code(KC_LGUI);
        wait_ms(50);
    }

    if (mods != 0) {
        add_mods(mods);
    }
}

static void ruen_set_layout(bool russian) {
    if (ruen_is_russian == russian) {
        return;
    }

    ruen_send_layout_switch();
    ruen_is_russian = russian;
}

static void ruen_toggle_layout(void) {
    ruen_set_layout(!ruen_is_russian);
}

static void ruen_set_en(void) {
    ruen_set_layout(false);
}
static void ruen_set_ru(void) {
    ruen_set_layout(true);
}

static void ruen_send_layout_symbol(uint16_t en_keycode, uint16_t ru_keycode) {
    tap_code16(ruen_is_russian ? ru_keycode : en_keycode);
}

static void ruen_send_temporary_symbol(bool russian_layout, uint16_t keycode) {
    bool previous_layout = ruen_is_russian;

    ruen_set_layout(russian_layout);
    tap_code16(keycode);
    ruen_set_layout(previous_layout);
}

static void ruen_send_us_symbol(uint16_t keycode) {
    ruen_send_temporary_symbol(false, keycode);
}

static void ruen_send_ru_symbol(uint16_t keycode) {
    ruen_send_temporary_symbol(true, keycode);
}

static void ruen_store(void) {
    ruen_saved_state = ruen_is_russian;
}

static void ruen_revert(void) {
    if (ruen_is_russian != ruen_saved_state) {
        ruen_toggle_layout();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (ruen_word_active && record->event.pressed) {
        switch (keycode) {
            case KC_SPC:
            case KC_ENT:
            case KC_ESC:
            case KC_MINS:
                ruen_set_ru();
                ruen_word_active = false;
                caps_word_off();
                break;
            default:
                break;
        }
    }

    if (record->event.pressed) {
        switch (keycode) {
            case RUEN_TOGGLE:
                ruen_toggle_layout();
                return false;
            case RUEN_EN:
                ruen_set_en();
                return false;
            case RUEN_RU:
                ruen_set_ru();
                return false;
            case RUEN_DOT:
                ruen_send_layout_symbol(KC_DOT, KC_SLSH);
                return false;
            case RUEN_COMMA:
                ruen_send_layout_symbol(KC_COMM, LSFT(KC_SLSH));
                return false;
            case RUEN_SCLN:
                ruen_send_layout_symbol(KC_SCLN, LSFT(KC_4));
                return false;
            case RUEN_COLON:
                ruen_send_layout_symbol(LSFT(KC_SCLN), LSFT(KC_6));
                return false;
            case RUEN_DQUOTE:
                ruen_send_layout_symbol(LSFT(KC_QUOT), LSFT(KC_2));
                return false;
            case RUEN_QST:
                ruen_send_layout_symbol(LSFT(KC_SLSH), LSFT(KC_7));
                return false;
            case RUEN_SLASH:
                ruen_send_layout_symbol(KC_SLSH, LSFT(KC_BSLS));
                return false;
            case RUEN_LBRC:
                ruen_send_us_symbol(KC_LBRC);
                return false;
            case RUEN_RBRC:
                ruen_send_us_symbol(KC_RBRC);
                return false;
            case RUEN_LCBR:
                ruen_send_us_symbol(LSFT(KC_LBRC));
                return false;
            case RUEN_RCBR:
                ruen_send_us_symbol(LSFT(KC_RBRC));
                return false;
            case RUEN_LT:
                ruen_send_us_symbol(LSFT(KC_COMM));
                return false;
            case RUEN_GT:
                ruen_send_us_symbol(LSFT(KC_DOT));
                return false;
            case RUEN_GRAVE:
                ruen_send_us_symbol(KC_GRV);
                return false;
            case RUEN_TILDE:
                ruen_send_us_symbol(LSFT(KC_GRV));
                return false;
            case RUEN_AT:
                ruen_send_us_symbol(LSFT(KC_2));
                return false;
            case RUEN_HASH:
                ruen_send_us_symbol(LSFT(KC_3));
                return false;
            case RUEN_DOLLAR:
                ruen_send_us_symbol(LSFT(KC_4));
                return false;
            case RUEN_CARET:
                ruen_send_us_symbol(LSFT(KC_6));
                return false;
            case RUEN_AMP:
                ruen_send_us_symbol(LSFT(KC_7));
                return false;
            case RUEN_PIPE:
                ruen_send_us_symbol(LSFT(KC_BSLS));
                return false;
            case RUEN_NUM:
                ruen_send_ru_symbol(LSFT(KC_3));
                return false;
            case RUEN_SYNC:
                ruen_is_russian = !ruen_is_russian;
                return false;
            case RUEN_MOD:
                ruen_toggle_layout();
                ruen_mod_restore_state = !ruen_is_russian;
                ruen_mod_timer = timer_read();
                return false;
            case RUEN_WORD:
                if (ruen_is_russian && !ruen_word_active) {
                    bool shift_active =
                        (get_mods() | get_oneshot_mods() | get_weak_mods()) &
                        MOD_MASK_SHIFT;

                    if (get_oneshot_mods() & MOD_MASK_SHIFT) {
                        clear_oneshot_mods();
                    }

                    ruen_set_en();
                    ruen_word_active = true;

                    if (shift_active) {
                        caps_word_on();
                    }
                }
                return false;
            case RUEN_STORE:
                ruen_store();
                return false;
            case RUEN_REVERT:
                ruen_revert();
                return false;
            case RUEN_QUOTE:
                ruen_send_us_symbol(KC_QUOT);
                return false;
            case RUEN_BSLS:
                ruen_send_us_symbol(KC_BSLS);
                return false;
            default:
                break;
        }
    } else {
        switch (keycode) {
            case RUEN_MOD:
                if (timer_elapsed(ruen_mod_timer) >=
                    get_tapping_term(keycode, record)) {
                    ruen_set_layout(ruen_mod_restore_state);
                }
                return false;
            default:
                break;
        }
    }
    return true;
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
