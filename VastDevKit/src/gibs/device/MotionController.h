//
// This class represents the state of a handheld pointer device such as an oculus touch controller.
//
// Note: depending on the EventQueue's internal policy, input from other real devices (such as a mouse) may be
// interpreted as pointer input.
//
#ifndef _VAST_GIBS_DEVICE_MOTION_CONTROLLER_H_
#define _VAST_GIBS_DEVICE_MOTION_CONTROLLER_H_

#include <Vast/VastMath.h>
#include "core/device/Device.h"
#include "core/component/Component.h"
#include "gibs/Constants.h"

using namespace hd::input;

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace device {

            class MotionController : public Component {
                
                int m_deviceId;

                Vec3 m_position;

                Vec3 m_trackingOffset;

                Quaternion m_orientation;

                Quaternion m_orientationOffset;
                
                
            public:

                int GetDeviceId() const;
                
            protected:
                
                friend class ComponentPool<MotionController>;
                
                MotionController(unsigned int ID);

                const Vec3 GetPosition() const;

                const Vec3 GetTrackingOffset() const;

                const Vec3 GetWorldPosition() const;

                const Quaternion GetOrientation() const;

                const Quaternion GetOrientationOffset() const;

                const Quaternion GetWorldOrientation() const;

                
                
            private:
                
                friend class hd::input::EventQueue;
                
                void Reset();
                
                void Update();
                
                void SetDeviceId(unsigned int deviceId);

                void SetPosition(const Vec3 &position);

                void SetTrackingOffset(const Vec3 &trackingOffset);

                void SetOrientation(const Quaternion &orientation);

                void SetOrientationOffset(const Quaternion &orientationOffset);

                void RotateBy(const Quaternion &delta);

                const Matrix GetLookAtMatrix() const;

                void CopyMotionController(const MotionController *motionController);
                
            };
        }
    }
}

#endif