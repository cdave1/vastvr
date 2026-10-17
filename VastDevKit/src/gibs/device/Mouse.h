//
// This class stores device state for a mouse device.
//
#ifndef _VAST_GIBS_DEVICE_MOUSE_H_
#define _VAST_GIBS_DEVICE_MOUSE_H_

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

            class Mouse : public Component {
                
                int m_deviceId;

                Vec3 m_currentScreenPosition;

                
                
            public:
                
                int GetDeviceId() const;

                Vec3 GetCurrentScreenPosition() const {
                    return m_currentScreenPosition;
                }

                
            protected:
                
                friend class ComponentPool<Mouse>;
                
                Mouse(unsigned int ID);
                
                
            private:
                
                friend class hd::input::EventQueue;
                
                void Reset();
                
                void Update();
                
                void SetDeviceId(unsigned int deviceId);

                void SetCurrentScreenPosition(const Vec3 &position) {
                    m_currentScreenPosition = position;
                }

                void CopyMouse(const Mouse *mouse);
                
            };
        }
    }
}

#endif