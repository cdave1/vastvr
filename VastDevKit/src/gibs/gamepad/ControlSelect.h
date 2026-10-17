//
// Player selects a control: pushes a button, or begins using a trigger or thumbstick.
//
#ifndef _VAST_INPUT_GAMEPAD_CONTROL_SELECT_H_
#define _VAST_INPUT_GAMEPAD_CONTROL_SELECT_H_

#include <Vast/VastMath.h>
#include "core/device/Device.h"
#include "core/component/Component.h"
#include "gibs/gamepad/GamepadCode.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;

        namespace gamepad {
            class ControlSelect : public Component {
            private:

                int m_deviceId;

                hd::input::gamepad::Control m_control;
                

            public:

                int GetDeviceId() const;

                hd::input::gamepad::Control GetControl() const;


            protected:

                friend class ComponentPool<ControlSelect>;

                ControlSelect(unsigned int ID);


            private:

                friend class hd::input::EventQueue;

                void SetDeviceId(unsigned int deviceId);

                void SetControl(hd::input::gamepad::Control control);

            };
        }
    }
}

#endif