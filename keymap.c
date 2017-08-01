#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "action_macro.h"

#define BASE 0 // default layer
#define ALPH 1 // alpha layer
#define SYMB 2 // symbols (unused)
#define NAV 3 // nav layer with OS+Emacs bindings
#define MDIA 4 // media layer
#define FN 4 // varous FNs

// Extra Space-Cadet shifts. Ref: https://docs.qmk.fm/space_cadet_shift.html
// KC_LEFT_CURLY_BRACE
#ifndef LCCO_KEY // Left Cadet Curly Open, LCtrl / {
  #define LCCO_KEY KC_LBRACKET // must be used with shift mod to get {
#endif
// KC_RIGHT_CURLY_BRACE
#ifndef RCCC_KEY // Right Cadet Curly Close, RCtrl / }
  #define RCCC_KEY KC_RBRACKET // must be used with shift-mod to get }
#endif
#ifndef LCBO_KEY // Left Cadet Bracket Open, LAlt / [
  #define LCBO_KEY KC_LBRACKET
#endif
#ifndef RCBC_KEY // Right Cadet Bracket Close, RAlt / ]
  #define RCBC_KEY KC_RBRACKET
#endif
// KC_LEFT_ANGLE_BRACKET
#ifndef LCLT_KEY // Left Cadet Left Angle Bracket, LCmd / <
  #define LCLT_KEY KC_LBRACKET
#endif
// KC_RIGHT_ANGLE_BRACKET
#ifndef RCGT_KEY // Right Cadet Right Angle Bracket, RCmd / >
  #define RCGT_KEY KC_RBRACKET
#endif

// MACRO DEFINITIONS

// all "Window" operations require Spectacle on OSX
#define M_WMAX M(0) // Window maximize; requires Spectacle on OSX
#define M_WLEFT M(1) // Window to left display
#define M_WRGHT M(2) // Window to right display

enum custom_keycodes {
  KC_LCCO,
  KC_RCCC,
  KC_LCBO,
  KC_RCBC,
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD,
};

//Tap Dance Declarations
enum {
  TD_X_QUOT = 0,
  TD_V_MINS = 1
};

//Tap Dance Definitions
/*qk_tap_dance_action_t tap_dance_actions[] = {
  //Tap once for Esc, twice for Caps Lock
  [TD_X_QUOT]  = ACTION_TAP_DANCE_DOUBLE(KC_X, KC_QUOT),
  [TD_V_MINS]  = ACTION_TAP_DANCE_DOUBLE(KC_V, KC_MINS)
// Other declarations would go here, separated by commas, if you have them
};*/

//In Layer declaration, add tap dance item in place of a key code
//TD(TD_X_QUOT)
//TD(TD_V_MINS)

/* TODO and misc:
REAMINING LAYOUT QUESTIONS
* How to hit backslash?
* I want colon : easy to hit on left hand
* Dashes on left hand are hard to hit
* Do cadet shift of LCMD/< and RCMD/>

Thoughts:
Make the current - (the upper right most button of the left hand) into a "tap for dash, double tab for underscore
                 - do the same thing for the "plus" on the right side
                 - turn the leader into a "tap vs hold" button that does "<" and ">", maintaining cadet shifts on left side
                 - this is consistent with the rest of the space cadet shifts and easy to hit on one hand
                 - add xtra functionlty to right hand ,./; keys bc some of these will be covered by other binds*/

/*  TODO: M-x forward-sexp (along with backward, down, and up) would be good to have on right hand
          Create "hold backspace" layer on left hand for NAV
          Turn current NAV into MEDIA layer?
          Put the pageup/pagedown style buttons on the lefthand NAV layer

Also install https://github.com/purcell/exec-path-from-shell to ensure consistent path;
  this sould obviate a fix in the below init.el that sets OSX specific shell stuff
 */

// TODO: make "hold LEAD" into another alpha shift? (is this the best use of it?)
// TODO: Get = hittable for the love of god. and what about backtick/tilde
// TODO: better mousekeys support? https://docs.qmk.fm/mouse_keys.html
// TODO: double tap Z, X for " and -? might be better to leave Z as punctuation shift tho
//       ref: https://docs.qmk.fm/tap_dance.html
// TODO: more useful double taps? Y, and the punctuation on the bottom right ergodox could be repurposed?
// TODO: maybe just use the current planned "HYPER" (on backspace) to instead be "Hold Z"
//       it would be easy to make movement buttons that are ergonomic on ergodox & otherwise (using ESDF)

// Ref: https://docs.qmk.fm/quantum_keycodes.html
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | LEAD   |   1  |   2  |   3  |   4  |   5  |  -   |           |  +   |   6  |   7  |   8  |   9  |   0  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | LAlt/[ |   Q  |   W  |   E  |   R  |   T  | Tab  |           |  -   |   Y  |   U  |   I  |   O  |   P  | RAlt/] |
 * |--------+------+------+------+------+------|/ALPH |           |      |------+------+------+------+------+--------|
 * | LCtrl/{|   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  | RCtrl/}|
 * |--------+------+------+------+------+------| LCmd |           | RCmd |------+------+------+------+------+--------|
 * |LShift/(|Z/SYMB|   X  |   C  |   V  |B/SYMB|      |           |      |   N  |   M  |   ,  |   .  |   /  |RShift/)|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |LCmd/<|  '   |  -   | Left | Right|                                       | Down |  Up  |   \  |   `  |RCmd/>|
 *   `----------------------------------'                                       `----------------------------------'
 * TODO: implement the LCmd/< and RCmd/>
 *                                        ,-------------.       ,---------------.
 *                                        |      |      |       |      |        |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      | Back |      |       |      |        |      |
 *                                 | Space| Space|------|       |------|  Del   |Enter |
 *                                 |/Alpha|      | Esc  |       |      | /Nav   |/Alpha|
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // Left Hand   |             |       |       |       |       |             |
        KC_LEAD,        KC_1,         KC_2,   KC_3,   KC_4,   KC_5,      KC_MINS,
        KC_LCBO,        KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,      LT(ALPH, KC_TAB),
        KC_LCCO,        KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        KC_LSPO,LT(SYMB,KC_Z),        KC_X,   KC_C,   KC_V,LT(SYMB,KC_B),KC_LGUI,
        KC_LGUI,        KC_QUOT,      KC_MINS,KC_LEFT,KC_RGHT,
	/*         Left Hand Island START ->       */ KC_TRNS,KC_TRNS,
                                                              KC_TRNS,
	LT(ALPH, KC_SPC),LT(ALL_T(KC_NO), KC_BSPC),KC_ESC,
        // Right Hand    |       |      |       |       |                 |
	     KC_PLUS,     KC_6,   KC_7,  KC_8,   KC_9,   KC_0,        ALL_T(KC_NO),
	     KC_MINS,     KC_Y,   KC_U,  KC_I,   KC_O,   KC_P,             KC_RCBC,
	                  KC_H,   KC_J,  KC_K,   KC_L,   KC_SCLN,          KC_RCCC,
             KC_RGUI,     KC_N,   KC_M,  KC_COMM,KC_DOT,LT(SYMB,KC_SLSH),  KC_RSPC,
                                  KC_DOWN,KC_UP, KC_BSLS,KC_GRV,           KC_RGUI,
	     KC_TRNS,     KC_TRNS, //  <- Right Hand Island START
             KC_TRNS,
             KC_TRNS,LT(MDIA, KC_DELT),LT(ALPH, KC_ENT)
    ),
// note: GUI_T(KC_QUOT) gives you ' / Cmd, might be useful...
//       CTL_T(KC_SLSH) - gives //Ctrl
//       LT(NAV, KC_SCLN) - guessing, but probably is a nav modifier press with a semicolon tap?
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
	                             KC_ENT,LT(MDIA, KC_DELT),KC_TRNS,
        // Right Hand    |       |      |       |       |                 |
	     KC_TRNS,     KC_1,   KC_2,  KC_3,   KC_4,   KC_5,             KC_TRNS,
	     KC_TAB,      KC_Q,   KC_W,  KC_E,   KC_R,   KC_T,             KC_LCBO,
                          KC_A,   KC_S,  KC_D,   KC_F,   KC_G,             KC_LCCO,
             KC_LGUI,     KC_Z,   KC_X,  KC_C,   KC_V,   KC_B,             KC_LSPO,
                                  KC_LEFT,KC_RIGHT,KC_MINS,KC_QUOT,        KC_RGUI,
	     KC_TRNS,     KC_TRNS, //  <- Right Hand Island START
             KC_TRNS,
             KC_TRNS,LT(NAV, KC_BSPC),KC_SPC
    ),
/* Keymap 2: Symbol Layer
 * Wanted keys on left hand: # " ' ` ~
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |  #   |  -   |  "   |  :   |      |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |  _   |  '   |  ;   |      |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
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
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
       KC_TRNS,KC_HASH,KC_MINS, KC_DQT,KC_COLN,KC_TRNS,
       KC_TRNS,KC_TRNS,KC_UNDS,KC_QUOT,KC_SCLN,KC_TRNS,KC_TRNS,
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
/* Keymap 3: Media keys (media, mouse, browser)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      | Mute |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |VolUp |      |      |           |      | Rclk | MsUp | Lclk |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      | Prev | Play | Next |------|           |------|MsLeft|MsDown|MsRigt|      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |VolDn |      |      |           |      |      |      |      |      |      |        |
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
// Media{
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
       KC_TRNS,  KC_BTN2, KC_MS_U, KC_BTN1, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),
/* Keymap 4: nav layer
 *                                
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      | Mute |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |sxp-^ |      |      |           |      |      | MsUp | Lclk |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      | <-sxp|sxp-dn|sxp-> |------|           |------|MsLeft|MsDown|MsRigt|      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           | Rclk |      |      |      |      |      |        |
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
// Nav {
[NAV] = KEYMAP(
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
       KC_TRNS,  KC_BTN2, KC_MS_U, KC_BTN1, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
)
};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(SYMB),                // FN1 - Momentary Layer 1 (Symbols)
};

// Down LGui, Down LAlt, Down+Up F, Up LAlt, Up LGui, Macro END
#define WMAX_MACRO MACRO(D(LGUI), D(LALT), T(F), U(LALT), U(LGUI), END)
#define WLEFT_MACRO MACRO(D(LGUI), D(LALT), D(LCTL), T(LEFT), U(LCTL), U(LALT), U(LGUI), END)
#define WRGHT_MACRO MACRO(D(LGUI), D(LALT), D(LCTL), T(RGHT), U(LCTL), U(LALT), U(LGUI), END)

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
	case 0:
	  return WMAX_MACRO;
        case 1:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
        case 2:
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
  ergodox_led_all_on();
  for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
    ergodox_led_all_set (i);
    wait_ms (5);
  }
  wait_ms(1000);
  for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
    ergodox_led_all_set (i);
    wait_ms (10);
  }
  ergodox_led_all_off();
};

// Stolen from https://docs.qmk.fm/leader_key.html
LEADER_EXTERNS();

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();
    SEQ_ONE_KEY(KC_S) {
      register_mods(MOD_BIT(KC_LSFT));
      register_code(KC_S);
      unregister_code(KC_S);
      unregister_mods(MOD_BIT(KC_LSFT));
    }
    SEQ_ONE_KEY(KC_W) {
      // Reference: https://docs.qmk.fm/macros.html
      action_macro_play(WMAX_MACRO);
    }
    SEQ_TWO_KEYS(KC_W,KC_LEFT) {
      action_macro_play(WLEFT_MACRO);
    }
    SEQ_TWO_KEYS(KC_W,KC_RGHT) {
      action_macro_play(WRGHT_MACRO);
    }
    SEQ_TWO_KEYS(KC_C, KC_C) {
      register_mods(MOD_BIT(KC_LALT));
      register_code(KC_X);
      unregister_code(KC_X);
      unregister_mods(MOD_BIT(KC_LALT));
      SEND_STRING("cider-connect");
      register_code(KC_ENT);
      unregister_code(KC_ENT);
    }
    SEQ_TWO_KEYS(KC_A, KC_S) {
      register_code(KC_H);
      unregister_code(KC_H);
    }
    SEQ_THREE_KEYS(KC_A, KC_S, KC_D) {
      register_code(KC_LGUI);
      register_code(KC_S);
      unregister_code(KC_S);
      unregister_code(KC_LGUI);
    }
  }
  
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
