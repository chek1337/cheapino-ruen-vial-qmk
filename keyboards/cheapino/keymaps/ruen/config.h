
#define VIAL_KEYBOARD_UID {0x79, 0xED, 0x40, 0xBB, 0x09, 0x8B, 0xC1, 0x9E}
// Unlock combo is two inner thumb keys.
#define VIAL_UNLOCK_COMBO_ROWS { 6, 2 }
#define VIAL_UNLOCK_COMBO_COLS { 11, 5 }

#define DYNAMIC_KEYMAP_LAYER_COUNT 14

// Tap-hold tuning. Only applied in pure-QMK builds; Vial overlay (qmk_settings.c)
// hijacks these callbacks and reads values from runtime QS settings instead, so
// defining them here in Vial mode either gets ignored (PERMISSIVE_HOLD) or
// triggers redefinition errors (CHORDAL_HOLD / HOLD_ON_OTHER_KEY_PRESS_PER_KEY).
#ifndef QMK_SETTINGS
#    define PERMISSIVE_HOLD
#    define CHORDAL_HOLD
#    define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// ruen.c references get_tapping_term(), which only exists when this is set.
// Vial enables it transitively via TAP_DANCE_ENABLE.
#    define TAPPING_TERM_PER_KEY
#endif

#define COMBO_TERM 40

#define ONESHOT_TIMEOUT 1500
#define ONESHOT_TAP_TOGGLE 2

// OS layout switch shortcuts — configure your OS keyboard settings to match.
// Defaults: Ctrl+Shift+1 → English, Ctrl+Shift+2 → Russian
#define RUEN_KEY_EN LCTL(LSFT(KC_1))
#define RUEN_KEY_RU LCTL(LSFT(KC_2))
