#ifndef _VAST_GIBS_HARDWARE_OCULUS_HMD_H_
#define _VAST_GIBS_HARDWARE_OCULUS_HMD_H_

#include <string>
#include <Vast/VastMath.h>
#include "core/component/ComponentPool.h"
#include "gibs/headset/HMD.h"
#include "gibs/headset/oculus/OculusTextureTarget.h"
#include "gibs/headset/oculus/OculusMirrorTarget.h"

//#include "Kernel/OVR_System.h"

// Include the Oculus SDK
//#include "OVR_CAPI_GL.h"

namespace hd {
    class Texture;

    namespace graphics {
        class Shader;
    }
    
    namespace input {
        class DistortionMesh;
        
        class OculusHMD : public HMD {
        private:
            
            std::string m_displayName;
            
            float m_width;
            
            float m_height;
            
            long m_displayId;

            OculusTextureTarget * m_renderTargets[2];
            
			OculusMirrorTarget * m_mirrorTarget;

            
        public:
            
            OculusHMD();

            int InitDevice();

            void InitForRendering();
            
            Quaternion GetDeviceOrientation();

            void RecenterDevice();

            Vec3 GetDeviceTrackingOffset();
            
            AABB GetViewport(hd::input::EyePosition eye);
            
            Matrix GetProjectionMatrix(hd::input::EyePosition eye);

            Vec3 GetEyeOffset(hd::input::EyePosition eye = hd::input::EyePosition::Center);

			Quaternion GetEyeOrientation(hd::input::EyePosition eye = hd::input::EyePosition::Center);

            float GetIPDRadius() const;

            Matrix GetInverseProjectionMatrix(hd::input::EyePosition eye);

            void BeginTiming();

            void UpdateTiming();

			void EndTiming();

			void PreRender(hd::input::EyePosition eye);

            void Render();

			void PostRender(hd::input::EyePosition eye);

			void RenderMirror();

            
        private:

            void CreateDistortionMeshes();
            
        };
    }
}

#endif
