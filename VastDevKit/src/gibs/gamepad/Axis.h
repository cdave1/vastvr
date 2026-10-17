//
// Axis values for game pad device.
//
// An axis is a control with a direction in at least one axis.  Examples:
// - Left and right thumbsticks on the Xbox 360 controller.
// - Left and right triggers on the Xbox 360 controller.
//
#ifndef _VAST_GIBS_GAMEPAD_AXIS_H_
#define _VAST_GIBS_GAMEPAD_AXIS_H_

#include <vast/VastMath.h>
#include "gibs/gamepad/GamepadCode.h"

namespace hd {
    namespace input {
        namespace device {
            class Gamepad;
        }
    }
}

namespace hd {
    namespace input {
        namespace gamepad {
            enum class AxisState {
                Inactive,
                Activating,
                Active,
                Deactivating,
            };
            
            class Axis {
            private:
                
                hd::input::gamepad::Control m_control;
                
                Vec2 m_direction;
                
                hd::input::gamepad::AxisState m_state;
                
                
            public:

                Axis();

                Axis(hd::input::gamepad::Control, const Vec2 &direction);
                
                ~Axis();
                
                hd::input::gamepad::Control GetControl() const;
                
                const Vec2 & GetDirection() const;
                
                hd::input::gamepad::AxisState GetState() const;
                
                void SetDirection(const Vec2 &direction);
                
                void SetState(hd::input::gamepad::AxisState);
                
            };
        }
    }
}

#endif