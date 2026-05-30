#include "ruen.h"

static bool ruen_is_russian = false;
static bool ruen_saved_state = false;
static bool ruen_mod_restore_state = false;
static uint16_t ruen_mod_timer = 0;
static bool ruen_word_active = false;

static void ruen_send_os_shortcut(uint16_t keycode) {
    uint8_t mods = get_mods();

    if (mods != 0) {
        del_mods(mods);
    }

    tap_code16(keycode);
    wait_ms(50);

    if (mods != 0) {
        add_mods(mods);
    }
}

static void ruen_set_layout(bool russian) {
    if (ruen_is_russian == russian) {
        return;
    }

    ruen_send_os_shortcut(russian ? RUEN_KEY_RU : RUEN_KEY_EN);
    ruen_is_russian = russian;
}

static void ruen_toggle_layout(void) {
    ruen_set_layout(!ruen_is_russian);
}

void ruen_force_en(void) {
    ruen_set_layout(false);
}

void ruen_force_ru(void) {
    ruen_set_layout(true);
}

bool ruen_is_ru(void) {
    return ruen_is_russian;
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

void ruen_tap_in_en(uint16_t keycode) {
    ruen_send_us_symbol(keycode);
}

static void ruen_store(void) {
    ruen_saved_state = ruen_is_russian;
}

static void ruen_revert(void) {
    if (ruen_is_russian != ruen_saved_state) {
        ruen_toggle_layout();
    }
}

void ruen_init(void) {
    ruen_is_russian = false;
    ruen_word_active = false;
}

bool process_record_ruen(uint16_t keycode, keyrecord_t *record) {
    if (ruen_word_active && record->event.pressed) {
        switch (keycode) {
            case KC_SPC:
            case KC_ENT:
            case KC_ESC:
            case KC_MINS:
                ruen_force_ru();
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
                ruen_force_en();
                return false;
            case RUEN_RU:
                ruen_force_ru();
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

                    ruen_force_en();
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
            case RUEN_CSC:
                ruen_send_us_symbol(LSFT(LCTL(KC_C)));
                return false;
            case RUEN_CSV:
                ruen_send_us_symbol(LSFT(LCTL(KC_V)));
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
