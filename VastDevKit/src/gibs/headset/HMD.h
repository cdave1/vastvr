#ifndef _VAST_GIBS_HMD_H_
#define _VAST_GIBS_HMD_H_

#include <string>
#include <Vast/VastMath.h>

namespace hd {
    class Texture;
    
    namespace input {
        enum class EyePosition {
            Center,
            Left,
            Right,
        };
        
        class HMD {
        public:
            
            HMD();

            virtual int InitDevice();

            virtual void InitForRendering();
            
            virtual Quaternion GetDeviceOrientation();

            virtual void RecenterDevice();

            virtual Vec3 GetDeviceTrackingOffset();
            
            virtual AABB GetViewport(hd::input::EyePosition eye);
            
            virtual Matrix GetProjectionMatrix(hd::input::EyePosition eye);

            virtual Vec3 GetEyeOffset(hd::input::EyePosition eye = hd::input::EyePosition::Center);

			virtual Quaternion GetEyeOrientation(hd::input::EyePosition eye = hd::input::EyePosition::Center);

            virtual float GetIPDRadius() const;

            virtual Matrix GetInverseProjectionMatrix(hd::input::EyePosition eye);

            virtual void BeginTiming();

            virtual void UpdateTiming();

            virtual void EndTiming();

			virtual void PreRender(hd::input::EyePosition eye);

            virtual void Render();

			virtual void PostRender(hd::input::EyePosition eye);

			virtual void RenderMirror();
            
        };
    }
}

#endif
