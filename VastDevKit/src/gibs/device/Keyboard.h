//
// This class stores button state.  The goal of this component
// is to represent the button state of a particular device at a particular frame.
//
// The component is explicitly updated when a button event is registered for a particular
// device.
//
#ifndef _VAST_GIBS_DEVICE_KEYBOARD_H_
#define _VAST_GIBS_DEVICE_KEYBOARD_H_

#include <Vast/VastMath.h>
#include "core/device/Device.h"
#include "core/component/Component.h"
#include "gibs/Constants.h"
#include <map>

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace device {

            class Keyboard : public Component {
                
                int m_deviceId;
                
                std::map<hd::Device::Keycode, int> m_keys;
                
                
            public:
                
                int GetDeviceId() const;
                
                bool IsKeyDown(const hd::Device::Keycode keycode) const;
                
                
            protected:
                
                friend class ComponentPool<Keyboard>;
                
                Keyboard(unsigned int ID);
                
                
            private:
                
                friend class hd::input::EventQueue;
                
                void Reset();
                
                void Update();
                
                void SetDeviceId(unsigned int deviceId);
                
                void AddKey(hd::Device::Keycode keycode);
                
                void RemoveKey(hd::Device::Keycode keycode);
                
                std::map<hd::Device::Keycode, int> GetKeys() const { return m_keys; }
                
                void CopyKeyboard(const Keyboard *keyboard);
                
            };
        }
    }
}

#endif