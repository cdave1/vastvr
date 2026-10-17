/**
 * Key Release -- use releases a key on the keyboard
 */

#ifndef _VAST_INPUT_KEYBOARD_KEY_RELEASE_H_
#define _VAST_INPUT_KEYBOARD_KEY_RELEASE_H_

#include <Vast/VastMath.h>
#include "core/device/Device.h"
#include "core/component/Component.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace keyboard {
            class KeyRelease : public Component {
            private:
                
                int m_deviceId;
                
                wchar_t m_buttonValue;
                
                hd::Device::Keycode m_keyCode;
                
            public:
                
                int GetDeviceId() const;
                
                wchar_t GetButtonValue() const;
                
                hd::Device::Keycode GetKeyCode() const;
                
                void PrintDebugInfo() const;
                
                
            protected:
                
                friend class ComponentPool<KeyRelease>;
                
                KeyRelease(unsigned int ID);
                
                
            private:
                
                friend class hd::input::EventQueue;
                
                void SetDeviceId(unsigned int deviceId);
                
                template<typename T>
                void SetButtonValue(T buttonValue);
                
            };
            
            template<typename T>
            void KeyRelease::SetButtonValue(T buttonValue) {
                m_buttonValue = static_cast<wchar_t>(buttonValue);
                m_keyCode = hd::Device::GetKeyboardCode(static_cast<int>(buttonValue));
            }
        }
    }
}

#endif
