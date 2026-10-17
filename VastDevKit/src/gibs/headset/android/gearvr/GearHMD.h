//
// On update, looks for new devices.
//

#ifndef _VAST_GIBS_HEADSET_ANDROID_GEAR_H_
#define _VAST_GIBS_HEADSET_ANDROID_GEAR_H_

#include <string>
#include <Vast/VastMath.h>
#include "core/component/ComponentPool.h"
#include "gibs/headset/HMD.h"

#include "VrApi.h"
#include "VrApi_Helpers.h"
#include "VrApi_Android.h"

namespace hd {
    class Texture;
    
    namespace input {
        namespace android {
            class Gear : public HMD {
            private:
                
                std::string m_displayName;
                
                float m_width;
                
                float m_height;
                
                long m_displayId;

                ovrJava *m_java;

                ovrMobile *m_ovrMobile;

                int m_frameIndex;
                
                
            public:
                
                GearHMD(ovrJava *java);

                void InitDevice();

                void InitForRendering();
                
                Quaternion GetDeviceOrientation();

                void RecenterDevice();

                Vec3 GetDeviceTrackingOffset();
                
                AABB GetViewport(hd::input::EyePosition eye);
                
                Matrix GetProjectionMatrix(hd::input::EyePosition eye);

                Vec3 GetIPDOffset(hd::input::EyePosition eye = hd::input::EyePosition::Center);

                float GetIPDRadius() const;

                Matrix GetInverseProjectionMatrix(hd::input::EyePosition eye);

                void BeginTiming();

                void UpdateTiming();

                void EndTiming();

                void Render(const hd::Texture *leftTexture, const hd::Texture *rightTexture);

                
            };
        }
    }
}

#endif
