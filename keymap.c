#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#define BASE 0 // default layer
#define ALPH 1 // alpha layer
#define SYMB 2 // symbols
#define MDIA 3 // media keys

#define NAV 5
#define FN 6

// Extra Space-Cadet shifts. Ref: https://docs.qmk.fm/space_cadet_shift.html
#ifndef LCCO_KEY // Left Cadet Curly Open, LCtrl / {
  #define LCCO_KEY KC_LBRACKET // must be used with shift mod to get {
#endif
#ifndef RCCC_KEY // Right Cadet Curly Close, RCtrl / }
  #define RCCC_KEY KC_RBRACKET // must be used with shift-mod to get }
#endif
#ifndef LCBO_KEY // Left Cadet Bracket Open, LAlt / [
  #define LCBO_KEY KC_LBRACKET
#endif
#ifndef RCBC_KEY // Right Cadet Bracket Close, RAlt / ]
  #define RCBC_KEY KC_RBRACKET
#endif

enum custom_keycodes {
  KC_LCCO,
  KC_RCCC,
  KC_LCBO,
  KC_RCBC,
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |   1  |   2  |   3  |   4  |   5  |      |           |  +   |   6  |   7  |   8  |   9  |   0  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | LAlt/[ |   Q  |   W  |   E  |   R  |   T  | Tab  |           | [Nav]|   Y  |   U  |   I  |   O  |   P  | RAlt   |
 * |--------+------+------+------+------+------|/ALPH |           |      |------+------+------+------+------+--------|
 * | LCtrl/{|   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  | RCtrl/}|
 * |--------+------+------+------+------+------| LCmd |           | RCmd |------+------+------+------+------+--------|
 * |LShift/(|  Z   |   X  |   C  |   V  |   B  | /Win |           | /Win |   N  |   M  |   ,  |   .  |   /  |RShift/)|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | LCmd |  '"  |  -   | Left | Right|                                       | Down |  Up  |   \  |   `  | RCmd |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        |      |      |       |      |        |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      | Back |      |       |      |        |      |
 *                                 | Space| Space|------|       |------|  Del   |Enter |
 *                                 |/Alpha|      |      |       |      | /Nav   |/Alpha|
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // Left Hand   |             |       |       |       |       |             |
        KC_LEAD,        KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   KC_TRNS,
        KC_LCBO,        KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   LT(ALPH, KC_TAB),
        KC_LCCO,        KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        KC_LSPO,        KC_Z,         KC_X,   KC_C,   KC_V,   KC_B,   KC_LGUI,
        KC_LGUI,        KC_QUOT,      KC_MINS,        KC_LEFT,KC_RGHT,
	/*         Left Hand Island START ->       */ KC_TRNS,KC_TRNS,
                                                              KC_TRNS,
                                             LT(ALPH, KC_SPC),KC_BSPC,KC_TRNS,
        // Right Hand    |       |      |       |       |                 |
             KC_PLUS,     KC_6,   KC_7,  KC_8,   KC_9,   KC_0,             KC_TRNS,
	     KC_TRNS,     KC_Y,   KC_U,  KC_I,   KC_O,   KC_P,             KC_RCBC,
	                  KC_H,   KC_J,  KC_K,   KC_L,   KC_SCLN,          KC_RCCC,
             KC_RGUI,     KC_N,   KC_M,  KC_COMM,KC_DOT, KC_SLSH,          KC_RSPC,
                                  KC_DOWN,KC_UP, KC_BSLS,KC_GRV,           KC_RGUI,
	     KC_TRNS,     KC_TRNS, //  <- Right Hand Island START
             KC_TRNS,
	     KC_TRNS,     LT(ALPH, KC_DELT),LT(ALPH, KC_ENT)
    ),
// note: GUI_T(KC_QUOT) gives you ' / Cmd, might be useful...
//       CTL_T(KC_SLSH) - gives //Ctrl
//       LT(MDIA, KC_SCLN) - guessing, but probably is a media modifier press with a semicolon tap?
/* Keymap 1: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |   1  |   2  |   3  |   4  |   5  |      |           |  +   |   6  |   7  |   8  |   9  |   0  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | LAlt/[ |   Q  |   W  |   E  |   R  |   T  | Tab  |           |      |   Y  |   U  |   I  |   O  |   P  | RAlt/] |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtrl/{|   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  | RCtrl/}|
 * |--------+------+------+------+------+------| LCmd |           | RCmd |------+------+------+------+------+--------|
 * |LShift/(|  Z   |   X  |   C  |   V  |   B  | /Win |           | /Win |   N  |   M  |   ,  |   .  |   /  |RShift/)|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | LCmd |  '"  |  -   | Left | Right|                                       | Down |  Up  |   \  |   `  | RCmd |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        |      |      |       |      |        |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      | Back |      |       |      |        |      |
 *                                 | Space| Space|------|       |------|  Del   |Enter |
 *                                 |      |      |      |       |      |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[ALPH] = KEYMAP(  // layer 0 : default
        // Left Hand   |             |       |       |       |       |             |
        KC_TRNS,        KC_6,         KC_7,   KC_8,   KC_9,   KC_0,   KC_PLUS,
        KC_RCBC,        KC_Y,         KC_U,   KC_I,   KC_O,   KC_P,   KC_TRNS, // this should become shift+tab
	KC_RCCC,        KC_H,         KC_J,   KC_K,   KC_L,   KC_SCLN,
	KC_RSPC,        KC_N,         KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_RGUI,
        KC_RGUI,        KC_GRV,       KC_BSLS,KC_DOWN,KC_UP,
	/*         Left Hand Island START ->       */ KC_TRNS,KC_TRNS,
                                                              KC_TRNS,
	                                       KC_ENT,KC_DELT,KC_TRNS,
        // Right Hand    |       |      |       |       |                 |
	     KC_TRNS,     KC_1,   KC_2,  KC_3,   KC_4,   KC_5,             KC_TRNS,
	     KC_TAB,      KC_Q,   KC_W,  KC_E,   KC_R,   KC_T,             KC_LCBO,
                          KC_A,   KC_S,  KC_D,   KC_F,   KC_G,             KC_LCCO,
             KC_LGUI,     KC_Z,   KC_X,  KC_C,   KC_V,   KC_B,             KC_LSPO,
                                  KC_LEFT,KC_RIGHT,KC_MINS,KC_QUOT,        KC_RGUI,
	     KC_TRNS,     KC_TRNS, //  <- Right Hand Island START
             KC_TRNS,
             KC_TRNS,     KC_BSPC,KC_SPC
    ),
/* Keymap 2: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | EPRM  |      |      |      |      |                                       |      |    . |   0  |   =  |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |Animat|      |       |Toggle|Solid |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |Bright|Bright|      |       |      |Hue-  |Hue+  |
 *                                 |ness- |ness+ |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = KEYMAP(
       // left hand
       VRSN,   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_TRNS,
       KC_TRNS,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,KC_TRNS,
       KC_TRNS,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       KC_TRNS,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,KC_TRNS,
          EPRM,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       RGB_MOD,KC_TRNS,
                                               KC_TRNS,
                               RGB_VAD,RGB_VAI,KC_TRNS,
       // right hand
       KC_TRNS, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_TRNS, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_F12,
                KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, KC_TRNS,
       KC_TRNS, KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS, KC_TRNS,
                         KC_TRNS,KC_DOT,  KC_0,    KC_EQL,  KC_TRNS,
       RGB_TOG, RGB_SLD,
       KC_TRNS,
       KC_TRNS, RGB_HUD, RGB_HUI
),
/* Keymap 3: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      | Mute |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |VolUp |      |      |           |      |      | MsUp |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      | Prev | Play | Next |------|           |------|MsLeft| Lclk |MsRigt|      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |VolDn |      |      |           | Rclk |      |MsDown|      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_MS_L, KC_BTN1, KC_MS_R, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_BTN2,  KC_TRNS, KC_MS_D, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),
};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(SYMB),                // FN1 - Momentary Layer 1 (Symbols)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
        case 1:
        if (record->event.pressed) { // For resetting EEPROM
          eeconfig_init();
        }
        break;
      }
    return MACRO_NONE;
};

// Used for curly-cadet-shift; like space-cadet except it inserts {}
static uint16_t ccs_timer[2] = {0, 0};
// Used for bracket-cadet-shift; inserts []
static uint16_t bcs_timer[2] = {0, 0};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
    case VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
      break;
    case RGB_SLD:
      if (record->event.pressed) {
        #ifdef RGBLIGHT_ENABLE
          rgblight_mode(1);
        #endif
      }
      return false;
      break;
    case KC_LCCO: {  // LCtrl {
      // This space cadet shift implementation is stolen from
      // https://github.com/qmk/qmk_firmware/blob/d1fb8d2296889ee1aaa08988c8951eb5f12d930b/quantum/quantum.c
      // Requires 2x custom keycodes, two timer values ine one array, and two case statements her
      if (record->event.pressed) {
        ccs_timer[0] = timer_read ();
        register_mods(MOD_BIT(KC_LCTL));
      }
      else {
	unregister_mods(MOD_BIT(KC_LCTL));	  
        if (timer_elapsed(ccs_timer[0]) < TAPPING_TERM) {	      
	  register_mods(MOD_BIT(KC_LSFT));
          register_code(LCCO_KEY);
          unregister_code(LCCO_KEY);
	  unregister_mods(MOD_BIT(KC_LSFT));
        }       
      }
    return false;
    }
    case KC_RCCC: { // RCtrl }
      if (record->event.pressed) {
        ccs_timer[1] = timer_read ();
        register_mods(MOD_BIT(KC_RCTL));
      }
      else {
	unregister_mods(MOD_BIT(KC_RCTL));	  
        if (timer_elapsed(ccs_timer[1]) < TAPPING_TERM) {	      
	  register_mods(MOD_BIT(KC_RSFT));
          register_code(RCCC_KEY);
          unregister_code(RCCC_KEY);
	  unregister_mods(MOD_BIT(KC_RSFT));
        }
      }
    return false;
    }
    case KC_LCBO: {  // LAlt [
      if (record->event.pressed) {
        bcs_timer[0] = timer_read ();
        register_mods(MOD_BIT(KC_LALT));
      }
      else {
	unregister_mods(MOD_BIT(KC_LALT));	  
        if (timer_elapsed(bcs_timer[0]) < TAPPING_TERM) {	      
          register_code(LCBO_KEY);
          unregister_code(LCBO_KEY);
        }
      }
    return false;
    }
    case KC_RCBC: {  // RAlt ]
      if (record->event.pressed) {
        bcs_timer[1] = timer_read ();
        register_mods(MOD_BIT(KC_RALT));
      }
      else {
	unregister_mods(MOD_BIT(KC_RALT));	  
        if (timer_elapsed(bcs_timer[1]) < TAPPING_TERM) {	      
          register_code(RCBC_KEY);
          unregister_code(RCBC_KEY);
        }
      }
    return false;
    }
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        default:
            // none
            break;
    }

};
