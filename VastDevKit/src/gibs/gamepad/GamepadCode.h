#ifndef _VAST_GIBS_DEVICE_GAMEPADCODE_H_
#define _VAST_GIBS_DEVICE_GAMEPADCODE_H_

#include "SDL2/SDL.h"

namespace hd {
    namespace input {
        namespace gamepad {
            enum class Control {
                UNKNOWN_CONTROL,
                LEFT_THUMBSTICK,
                RIGHT_THUMBSTICK,
                LEFT_TRIGGER,
                RIGHT_TRIGGER,
                BACK_BUTTON,
                START_BUTTON,
                LEFT_DPAD,
                RIGHT_DPAD,
                LEFT_BUMPER,
                RIGHT_BUMPER,
                XBOX_A,
                XBOX_B,
                XBOX_X,
                XBOX_Y,
                PLAYSTATION_CROSS,
                PLAYSTATION_CIRCLE,
                PLAYSTATION_SQUARE,
                PLAYSTATION_TRIANGLE
            };


            typedef uint8_t ButtonId;
            typedef uint8_t AxisName;
            typedef uint8_t AxisId;
        }
        
        //
        // Controls
        //
        /*
        extern const gamepad::ControlName UNKNOWN_CONTROL;
        
        extern const gamepad::ControlName LEFT_THUMBSTICK;
        extern const gamepad::ControlName RIGHT_THUMBSTICK;
        
        extern const gamepad::ControlName LEFT_TRIGGER;
        extern const gamepad::ControlName RIGHT_TRIGGER;
        
        extern const gamepad::ControlName START_BUTTON;
        */
        
        //
        // Axis Name
        //
        extern const gamepad::AxisName HORIZONTAL_AXIS;
        extern const gamepad::AxisName VERTICAL_AXIS;
        
        
        namespace sdl {
            
            //
            // Button values
            //
            extern const gamepad::ButtonId  XBOX_A;
            extern const gamepad::ButtonId  XBOX_B;
            extern const gamepad::ButtonId  XBOX_X;
            extern const gamepad::ButtonId  XBOX_Y;
            
            extern const gamepad::ButtonId  XBOX_LEFT_BUMPER;
            extern const gamepad::ButtonId  XBOX_RIGHT_BUMPER;
            
            extern const gamepad::ButtonId  XBOX_LEFT_THUMBSTICK;
            extern const gamepad::ButtonId  XBOX_RIGHT_THUMBSTICK;
            
            extern const gamepad::ButtonId  XBOX_START;
            extern const gamepad::ButtonId  XBOX_BACK;

            extern const gamepad::ButtonId  XBOX_DPAD_UP;
            extern const gamepad::ButtonId  XBOX_DPAD_DOWN;
            extern const gamepad::ButtonId  XBOX_DPAD_LEFT;
            extern const gamepad::ButtonId  XBOX_DPAD_RIGHT;
            
            //
            // Axis values
            //
            extern const gamepad::AxisId XBOX_LEFT_TRIGGER;
            extern const gamepad::AxisId XBOX_RIGHT_TRIGGER;
            
            extern const gamepad::AxisId XBOX_LEFT_THUMB_HORIZONTAL;
            extern const gamepad::AxisId XBOX_LEFT_THUMB_VERTICAL;
            
            extern const gamepad::AxisId XBOX_RIGHT_THUMB_HORIZONTAL;
            extern const gamepad::AxisId XBOX_RIGHT_THUMB_VERTICAL;


            ///
            /// PS3 Dual shock 3 controller
            ///
            extern const gamepad::ButtonId  PS3_DUALSHOCK_CROSS;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_SQUARE;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_TRIANGLE;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_CIRCLE;

            extern const gamepad::ButtonId  PS3_DUALSHOCK_START;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_SELECT;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_DPAD_UP;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_DPAD_LEFT;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_DPAD_RIGHT;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_DPAD_DOWN;

            extern const gamepad::ButtonId  PS3_DUALSHOCK_LEFT_THUMBSTICK;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_RIGHT_THUMBSTICK;
            
            extern const gamepad::ButtonId  PS3_DUALSHOCK_LEFT_BUMPER;
            extern const gamepad::ButtonId  PS3_DUALSHOCK_RIGHT_BUMPER;

            ///
            /// PS3 Dual shock 3 axis values
            ///
            extern const gamepad::AxisId PS3_DUALSHOCK_LEFT_TRIGGER;
            extern const gamepad::AxisId PS3_DUALSHOCK_RIGHT_TRIGGER;

            extern const gamepad::AxisId PS3_DUALSHOCK_LEFT_THUMB_HORIZONTAL;
            extern const gamepad::AxisId PS3_DUALSHOCK_LEFT_THUMB_VERTICAL;

            extern const gamepad::AxisId PS3_DUALSHOCK_RIGHT_THUMB_HORIZONTAL;
            extern const gamepad::AxisId PS3_DUALSHOCK_RIGHT_THUMB_VERTICAL;
            
        }
    }
};

#endif
