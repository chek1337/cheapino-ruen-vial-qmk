# Switch between pure-QMK build (no) and Vial build (yes).
# Vial build pulls in qmk_settings.c overlay which hijacks tap-hold callbacks.
VIA_ENABLE = no
VIAL_ENABLE = no
ENCODER_MAP_ENABLE = yes
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
SRC += ruen.c
