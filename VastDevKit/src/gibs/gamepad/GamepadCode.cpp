#include "GamepadCode.h"

//
// Fixed control names
//
/*
const hd::input::gamepad::ControlName hd::input::UNKNOWN_CONTROL = 0;

const hd::input::gamepad::ControlName hd::input::LEFT_THUMBSTICK = 1;
const hd::input::gamepad::ControlName hd::input::RIGHT_THUMBSTICK = 2;

const hd::input::gamepad::ControlName hd::input::LEFT_TRIGGER = 3;
const hd::input::gamepad::ControlName hd::input::RIGHT_TRIGGER = 4;

const hd::input::gamepad::ControlName hd::input::START_BUTTON = 5;
*/

//
// Axis names
//
const hd::input::gamepad::AxisName hd::input::HORIZONTAL_AXIS = 0;
const hd::input::gamepad::AxisName hd::input::VERTICAL_AXIS = 1;

//
// Gamepad Button Ids
//
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_A = 0;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_B = 1;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_X = 2;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_Y = 3;

const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_LEFT_BUMPER = 4;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_RIGHT_BUMPER = 5;

const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_LEFT_THUMBSTICK = 6;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_RIGHT_THUMBSTICK = 7;

const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_START = 8;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_BACK = 9;

const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_DPAD_UP = 11;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_DPAD_DOWN = 12;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_DPAD_LEFT = 13;
const hd::input::gamepad::ButtonId hd::input::sdl::XBOX_DPAD_RIGHT = 14;

//
// Gamepad Axis Ids
//
const hd::input::gamepad::AxisId hd::input::sdl::XBOX_LEFT_TRIGGER = 2;
const hd::input::gamepad::AxisId hd::input::sdl::XBOX_RIGHT_TRIGGER = 5;

const hd::input::gamepad::AxisId hd::input::sdl::XBOX_LEFT_THUMB_HORIZONTAL = 0;
const hd::input::gamepad::AxisId hd::input::sdl::XBOX_LEFT_THUMB_VERTICAL = 1;

const hd::input::gamepad::AxisId hd::input::sdl::XBOX_RIGHT_THUMB_HORIZONTAL = 3;
const hd::input::gamepad::AxisId hd::input::sdl::XBOX_RIGHT_THUMB_VERTICAL = 4;


///
/// PS3 Dual shock 3 controller
///
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_CROSS = 14;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_SQUARE = 15;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_TRIANGLE = 12;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_CIRCLE = 13;

const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_START = 3;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_SELECT = 0;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_DPAD_UP = 4;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_DPAD_LEFT = 7;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_DPAD_RIGHT = 5;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_DPAD_DOWN = 6;

const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_LEFT_THUMBSTICK = 1;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_RIGHT_THUMBSTICK = 2;

const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_LEFT_BUMPER = 10;
const hd::input::gamepad::ButtonId hd::input::sdl::PS3_DUALSHOCK_RIGHT_BUMPER = 11;

///
/// PS3 Dual shock 3 axis values
///

const hd::input::gamepad::AxisId hd::input::sdl::PS3_DUALSHOCK_LEFT_TRIGGER = 8;
const hd::input::gamepad::AxisId hd::input::sdl::PS3_DUALSHOCK_RIGHT_TRIGGER = 9;

const hd::input::gamepad::AxisId hd::input::sdl::PS3_DUALSHOCK_LEFT_THUMB_HORIZONTAL = 0;
const hd::input::gamepad::AxisId hd::input::sdl::PS3_DUALSHOCK_LEFT_THUMB_VERTICAL = 1;

const hd::input::gamepad::AxisId hd::input::sdl::PS3_DUALSHOCK_RIGHT_THUMB_HORIZONTAL = 2;
const hd::input::gamepad::AxisId hd::input::sdl::PS3_DUALSHOCK_RIGHT_THUMB_VERTICAL = 3;
