//
// Key Select -- user selects a key on a keyboard
//
#ifndef _VAST_INPUT_KEYBOARD_KEY_SELECT_H_
#define _VAST_INPUT_KEYBOARD_KEY_SELECT_H_

#include <Vast/VastMath.h>
#include "core/device/Device.h"
#include "core/component/Component.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace keyboard {
            class KeySelect : public Component {
            private:
                
                int m_deviceId;

				int m_scanCode;
                
                wchar_t m_buttonValue;
                
                hd::Device::Keycode m_keyCode;
                
                
            public:
                
                int GetDeviceId() const;
                
                wchar_t GetButtonValue() const;
                
                hd::Device::Keycode GetKeyCode() const;
                
                void PrintDebugInfo() const;
                
                
            protected:
                
                friend class ComponentPool<KeySelect>;
                
                KeySelect(unsigned int ID);
                
                
            private:
                
                friend class hd::input::EventQueue;
                
                void SetDeviceId(unsigned int deviceId);
                
                template<typename T>
                void SetButtonValue(T buttonValue);
                
            };
            
            template<typename T>
            void KeySelect::SetButtonValue(T buttonValue) {
                m_buttonValue = static_cast<wchar_t>(buttonValue);
				m_scanCode = static_cast<int>(buttonValue);
                m_keyCode = hd::Device::GetKeyboardCode(m_scanCode);
            }
        }
    }
}

#endif