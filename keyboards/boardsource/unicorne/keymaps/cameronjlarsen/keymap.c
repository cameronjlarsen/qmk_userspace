/* Copyright 2024 Cameron Larsen <@cameronjlarsen>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
#include <stdio.h>
#include "features/oneshot.h"

enum layers {
    _QWERTY = 0,
    SYM,
    NAV,
    FUN,
};
//  Aliases for readability
#define QWERTY DF(_QWERTY)
#define LA_SYM LT(SYM, KC_TAB)
#define LA_NAV MO(NAV)

// One shot mods
enum keycodes {
    OS_SHFT = QK_USER,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
};

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * Inspiration:
 *
 * https://github.com/serebrov/qmk_firmware/blob/custom/keyboards/kyria/keymaps/kyria-mini/keymap.c
 *
 * Notes:
 * - F & J enables CAPSWORD, disables after 5 seconds
 * - Left thumb CTRL and SHIFT are one shot
 * - Enter is moved to ; location and ; is moved to Sym layer
 * - ESC can be accessed by NAV and G
 * - BKSP is accessed by NAV and Enter
 * - Tab is accessed by tapping SYM layer
 * - FUN layer is accessed by holding NAV and SYM layers at the same time
 *
 * ,-----------------------------------------.                       ,-----------------------------------------.
 * |      |   Q  |   W  |   E  |   R  |   T  |                       |   Y  |   U  |   I  |   O  |   P  |      |
 * |------+------+------+------+------+------|                       |------+------+------+------+------+------|
 * |      |   A  |   S  |   D  |   F  |   G  |                       |   H  |   J  |   K  |   L  | Enter|      |
 * |------+------+------+------+------+------.                       ,------+------+------+------+------+------|
 * |      |   Z  |   X  |   C  |   V  |   B  |                       |   N  |   M  | ,  < | . >  | /  ? |      |
 * `------+------+------+------+------+------+------.         ,------+------+------+------+--------------------'
 *                            |  OSM  |  OSM | Nav  |         | Sym  | Space| GUI  |
 *                            |  Ctrl | Shift|      |         | Tab  |      |      |
 *                            `---------------------'         `--------------------'
 */
    [_QWERTY] = LAYOUT_split_3x6_3(
      _______, KC_Q  ,  KC_W  ,  KC_E  ,   KC_R ,   KC_T ,                           KC_Y,  KC_U ,  KC_I ,   KC_O ,   KC_P ,_______,
      _______, KC_A  ,  KC_S  ,  KC_D  ,   KC_F ,   KC_G ,                           KC_H,  KC_J ,  KC_K ,   KC_L , KC_ENT ,_______,
      _______, KC_Z  ,  KC_X  ,  KC_C  ,   KC_V ,   KC_B ,                           KC_N,  KC_M ,KC_COMM, KC_DOT , KC_SLSH,_______,
                                OS_CTRL, OS_SHFT, LA_NAV ,       LA_SYM , KC_SPC, OS_GUI
    ),

/*
 * Sym Layer: Numbers and symbols
 *
 * Notes:
 * - Symbols are grouped together and shifted symbols from middle row are on bottom row
 * - Exception is angle brackets
 *
 * ,----------------------------------------.                       ,-----------------------------------------.
 * |      | 1 ! |  2 @ |  3 # |  4 $ |  5 % |                       |  6 ^ |  7 & |  8 * |  9 ( |  0 ) |      |
 * |------+-----+------+------+------+------|                       |------+------+------+------+------+------|
 * |      | `   |  (   |  )   |  '   |  =   |                       |   \  |  -   |  [   |  ]   |  ;   |      |
 * |------+-----+------+------+------+------+                       +------+------+------+------+------+------|
 * |      | ~   |  <   |  >   |  "   |  +   |                       |   |  |  _   |  {   |  }   |  :   |      |
 * `------+------------+------+------+------+------.         ,------+------+------+------+--------------------'
 *                            |      |      |      |         |      |      |      |
 *                            |      |      |      |         |      |      |      |
 *                            `--------------------'         `--------------------'
 */
    [SYM] = LAYOUT_split_3x6_3(
      _______,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                          KC_6 ,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 ,_______,
      _______, KC_GRV , KC_LPRN, KC_RPRN, KC_QUOT, KC_EQL ,                        KC_BSLS, KC_MINS, KC_LBRC, KC_RBRC, KC_SCLN,_______,
      _______, KC_TILD, KC_LABK, KC_RABK, KC_DQUO, KC_PLUS,                        KC_PIPE, KC_UNDS, KC_LCBR, KC_RCBR, KC_COLN,_______,
                                 _______, _______, _______,      _______, _______, _______
    ),

/*
 * Nav Layer: Navigation, Media
 *
 * Notes:
 * - Vim style navigation keys
 * - Volume and Media Keys
 * - BKSP on Enter
 * - DEL on /
 * - Esc on G
 *
 * ,-----------------------------------------.                       ,-----------------------------------------.
 * |RGBVai|RGBHui|RGBMod|RGBTog| BriUp| BriDn|                       | Home | PgDn | PgUp | End  |PrtScr|      |
 * |------+------+------+------+------+------|                       |------+------+------+------+------+------|
 * |      |  GUI |  Alt | Ctrl | Shift| Esc  |                       |   ←  |  ↓   |   ↑  |   →  | Bksp |      |
 * |------+------+------+------+------+------+                       +------+------+------+------+------+------|
 * |      |      | Vol- | Mute | Vol+ |NumLck|                       | MPrev| MPlay| MStop| MNext|Delete|      |
 * `--------------------+------+------+------+------.         ,------+------+------+------+--------------------'
 *                             |      |      |      |         |      |      |      |
 *                             |      |      |      |         |      |      |      |
 *                             `--------------------'         `--------------------'
 */
    [NAV] = LAYOUT_split_3x6_3(
        RGB_VAI, RGB_HUI, RGB_MOD, RGB_TOG, KC_BRIU,KC_BRID,                       KC_HOME, KC_PGDN, KC_PGUP, KC_END , KC_PSCR, _______,
        _______, OS_GUI , OS_ALT , OS_CTRL, OS_SHFT, KC_ESC,                       KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, KC_BSPC, _______,
        _______, _______, KC_VOLD, KC_MUTE, KC_VOLU, KC_NUM,                       KC_MPRV, KC_MPLY, KC_MSTP, KC_MNXT, KC_DEL , _______,
                                  _______, _______, _______,     _______, _______, _______
    ),

/*
 * Function Layer: Function keys
 *
 * Notes:
 * - F1-F10 on bottom row
 * - F11-F12 on index finger inner row
 * - Homerow mods
 * - Num keys on top row
 *
 * ,-----------------------------------------.                       ,-----------------------------------------.
 * | Boot |  1 ! |  2 @ |  3 # |  4 $ |  5 % |                       |  6 ^ |  7 & |  8 * |  9 ( |  0 ) |      |
 * |------+------+------+------+------+------|                       |------+------+------+------+------+------|
 * |EEPROM|  GUI |  Alt | Ctrl | Shift| F11  |                       | F12  | Shift| Ctrl |  Alt |  GUI |      |
 * |------+------+------+------+------+------+                       +------+------+------+------+------+------|
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                       |  F6  |  F7  |  F8  |  F9  | F10  |      |
 * `--------------------+------+------+------+------.         ,------+------+------+------+--------------------'
 *                             |      |      |      |         |      |      |      |
 *                             |      |      |      |         |      |      |      |
 *                             `--------------------'         `--------------------'
 */
    [FUN] = LAYOUT_split_3x6_3(
       QK_BOOT,   KC_1 ,   KC_2 ,   KC_3 ,   KC_4 ,   KC_5 ,                         KC_6 ,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 ,_______,
       EE_CLR , OS_GUI , OS_ALT , OS_CTRL, OS_SHFT, KC_F11 ,                       KC_F12 , OS_SHFT, OS_CTRL, OS_ALT , OS_GUI ,_______,
       _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 ,  KC_F5 ,                        KC_F6 ,  KC_F7 ,  KC_F8 ,  KC_F9 , KC_F10 ,_______,
                                 _______, _______, _______,     _______, _______, _______
    ),

};

enum combo_events {
    CAPS_COMBO
};

const uint16_t PROGMEM caps_combo[] = {KC_F, KC_J, COMBO_END};

combo_t key_combos[] = {
  [CAPS_COMBO] = COMBO_ACTION(caps_combo),
  // Other combos...C
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case CAPS_COMBO:
      if (pressed) {
        caps_word_on();  // Activate Caps Word!
      }
      break;

    // Other combos...
  }
}

bool is_oneshot_cancel_key(uint16_t keycode){
  switch (keycode) {
    case LA_NAV:
      return true;
    default:
      return false;
  }
}

bool is_oneshot_ignored_key(uint16_t keycode){
  switch (keycode) {
    case LA_NAV:
    case LA_SYM:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
      return true;
    default:
      return false;
  }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;



bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
    case KC_MINS:
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}


layer_state_t layer_state_set_user(layer_state_t state){
  return update_tri_layer_state(state, SYM, NAV, FUN);
}


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_caps_word(keycode, record)) { return false; }
  // Your macros ...
  update_oneshot(
      &os_shft_state, KC_LSFT, OS_SHFT,
      keycode, record
    );

  update_oneshot(
      &os_ctrl_state, KC_LCTL, OS_CTRL,
      keycode, record
    );

  update_oneshot(
      &os_alt_state, KC_LALT, OS_ALT,
      keycode, record
    );

  update_oneshot(
      &os_cmd_state, KC_LGUI, OS_GUI,
      keycode, record
    );
  return true;
}

