#ifndef _MUFFIN_PLATFORM_INPUT_H_
#define _MUFFIN_PLATFORM_INPUT_H_

#include "muffin_core/common.h"
#include "muffin_core/math.h"
#include "muffin_platform/window.h"

typedef enum MufKeyCode_e {
    MUF_KEYCODE_SPACE           = 32,
    MUF_KEYCODE_COMMA           = 44,
    MUF_KEYCODE_MINUS           = 46,
    MUF_KEYCODE_SLASH           = 47,

    MUF_KEYCODE_NUM0            = 48,
    MUF_KEYCODE_NUM1            = 49,
    MUF_KEYCODE_NUM2            = 50,
    MUF_KEYCODE_NUM3            = 51,
    MUF_KEYCODE_NUM4            = 52,
    MUF_KEYCODE_NUM5            = 53,
    MUF_KEYCODE_NUM6            = 54,
    MUF_KEYCODE_NUM7            = 55,
    MUF_KEYCODE_NUM8            = 56,
    MUF_KEYCODE_NUM9            = 57,

    MUF_KEYCODE_SEMICOLON       = 59,
    MUF_KEYCODE_EQUAL           = 61,

    MUF_KEYCODE_A               = 65,
    MUF_KEYCODE_B               = 66,
    MUF_KEYCODE_C               = 67,
    MUF_KEYCODE_D               = 68,
    MUF_KEYCODE_E               = 69,
    MUF_KEYCODE_F               = 70,
    MUF_KEYCODE_G               = 71,
    MUF_KEYCODE_H               = 72,
    MUF_KEYCODE_I               = 73,
    MUF_KEYCODE_J               = 74,
    MUF_KEYCODE_K               = 75,
    MUF_KEYCODE_L               = 76,
    MUF_KEYCODE_M               = 77,
    MUF_KEYCODE_N               = 78,
    MUF_KEYCODE_O               = 79,
    MUF_KEYCODE_P               = 80,
    MUF_KEYCODE_Q               = 81,
    MUF_KEYCODE_R               = 82,
    MUF_KEYCODE_S               = 83,
    MUF_KEYCODE_T               = 84,
    MUF_KEYCODE_U               = 85,
    MUF_KEYCODE_V               = 86,
    MUF_KEYCODE_W               = 87,
    MUF_KEYCODE_X               = 88,
    MUF_KEYCODE_Y               = 89,
    MUF_KEYCODE_Z               = 90,

    MUF_KEYCODE_LEFT_BRACKET    = 91,
    MUF_KEYCODE_BACKSLASH       = 92,
    MUF_KEYCODE_RIGHT_BRACKET   = 93,
    MUF_KEYCODE_GRAVE_ACCENT    = 96,

    MUF_KEYCODE_ESCAPE          = 256,
    MUF_KEYCODE_ENTER           = 257,
    MUF_KEYCODE_TAB             = 258,
    MUF_KEYCODE_BACKSPACE       = 259,
    MUF_KEYCODE_INSERT          = 260,
    MUF_KEYCODE_DELETE          = 261,
    MUF_KEYCODE_RIGHT           = 262,
    MUF_KEYCODE_LEFT            = 263,
    MUF_KEYCODE_DOWN            = 264,
    MUF_KEYCODE_UP              = 265,
    MUF_KEYCODE_PAGEUP          = 266,
    MUF_KEYCODE_PAGEDOWN        = 267,
    MUF_KEYCODE_HOME            = 268,
    MUF_KEYCODE_END             = 269,
    MUF_KEYCODE_CAPS_LOCK       = 280,
    MUF_KEYCODE_SCROLL_LOCK     = 283,
    MUF_KEYCODE_PAUSE           = 284,

    MUF_KEYCODE_F1              = 186,
    MUF_KEYCODE_F2              = 187,
    MUF_KEYCODE_F3              = 188,
    MUF_KEYCODE_F4              = 189,
    MUF_KEYCODE_F5              = 190,
    MUF_KEYCODE_F6              = 191,
    MUF_KEYCODE_F7              = 192,
    MUF_KEYCODE_F8              = 193,
    MUF_KEYCODE_F9              = 194,
    MUF_KEYCODE_F10             = 195,
    MUF_KEYCODE_F11             = 196,
    MUF_KEYCODE_F12             = 197,

    MUF_KEYCODE_LEFT_SHIFT      = 340,
    MUF_KEYCODE_LEFT_CONTROL    = 341,
    MUF_KEYCODE_LEFT_ALT        = 342,
    MUF_KEYCODE_LEFT_SUPER      = 343,
    MUF_KEYCODE_RIGHT_SHIFT     = 344,
    MUF_KEYCODE_RIGHT_CONTROL   = 345,
    MUF_KEYCODE_RIGHT_ALT       = 346,
    MUF_KEYCODE_RIGHT_SUPER     = 347
} MufKeyCode;

typedef enum MufKeyModeFlags_e {
    MUF_KEYMODE_FLAGS_SHIFT     = 0x0001,
    MUF_KEYMODE_FLAGS_CONTROL   = 0x0002,
    MUF_KEYMODE_FLAGS_ALT       = 0x0004,
    MUF_KEYMODE_FLAGS_SUPER     = 0x0008,
    MUF_KEYMODE_FLAGS_CAPS_LOCK = 0x0010,
    MUF_KEYMODE_FLAGS_NUM_LOCK  = 0x0020,
} MufKeyModeFlags;

typedef enum MufMouseButton_e {
    MUF_MOUSE_BUTTON_0,
    MUF_MOUSE_BUTTON_1,
    MUF_MOUSE_BUTTON_2,
    MUF_MOUSE_BUTTON_3,

    MUF_MOUSE_BUTTON_RIGHT = MUF_MOUSE_BUTTON_0,
    MUF_MOUSE_BUTTON_MIDDLE = MUF_MOUSE_BUTTON_1,
    MUF_MOUSE_BUTTON_LEFT = MUF_MOUSE_BUTTON_2
} MufMouseButton;

muf_bool mufIsKeyPressed(MufKeyCode keyCode);
MufKeyModeFlags mufGetKeyMode();

MufVec2 mufGetCursorPosition();

void mufSetCursorPosition(MufVec2 position);

typedef void(*MufKeyPressedCallback)(MufWindow *window);

#endif