//
// This class stores button state.  The goal of this component
// is to represent the button state of a particular device at a particular frame.
//
// The component is explicitly updated when a button event is registered for a particular
// device.
//
#ifndef _VAST_GIBS_DEVICE_GAMEPAD_H_
#define _VAST_GIBS_DEVICE_GAMEPAD_H_

#include <map>

#include <Vast/VastMath.h>
#include "core/device/Device.h"
#include "core/component/Component.h"

#include "gibs/Constants.h"
#include "gibs/gamepad/GamepadCode.h"
#include "gibs/gamepad/Axis.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace device {
            class Gamepad : public Component {
                
                int m_deviceId;
                
                std::map<hd::input::gamepad::Control, int> m_controls;
                
                std::map<hd::input::gamepad::Control, hd::input::gamepad::Axis> m_axisControls;
                
                
            public:
                
                int GetDeviceId() const;
                
                bool IsControlActive(hd::input::gamepad::Control control) const;
                
                const hd::input::gamepad::Axis * GetAxis(hd::input::gamepad::Control control) const;
                
                
            protected:
                
                friend class ComponentPool<Gamepad>;
                
                Gamepad(unsigned int ID);
                
                
            private:
                
                friend class hd::input::EventQueue;
                
                void Reset();
                
                void Update();
                
                void SetDeviceId(unsigned int deviceId);

                ///
                /// UpdateAxis is called when a value changes for a specific axis on a control.  It
                /// will be called, for example, when the player changes the up-down axis on the
                /// thumbstick control.  It is guaranteed to be called at least ONCE for
                /// each axis of the control, but it possibly may be called more often per frame.
                ///
                void UpdateAxis(hd::input::gamepad::Control control,
                                hd::input::gamepad::AxisName axis,
                                const float value);
                
                void SetAxis(hd::input::gamepad::Control control,
                             const Vec2 &axisValue);
                
                void RemoveAxis(hd::input::gamepad::Control control);
                
                void AddControl(hd::input::gamepad::Control control);
                
                void RemoveControl(hd::input::gamepad::Control control);
                
                std::map<hd::input::gamepad::Control, int> GetControls() const { return m_controls; }
                
                std::map<hd::input::gamepad::Control, hd::input::gamepad::Axis> GetAxisControls() const { return m_axisControls; }
                
                void CopyGamepad(const Gamepad *gamepad);
                
            };
        }
    }
}

#endif
