#ifndef _VAST_GIBS_OCULUS_TEXTURE_TARGET_H_
#define _VAST_GIBS_OCULUS_TEXTURE_TARGET_H_

#include <Vast/Vast.h>
//#include "Kernel/OVR_System.h"

// Include the Oculus SDK
//#include "OVR_CAPI_GL.h"

namespace hd {
    namespace input {
    typedef  GLuint ovrTextureSwapChain;
    typedef int ovrSession;
        class OculusTextureTarget {
        private:
            
            GLuint m_frameBufferId;

			//ovrTextureSwapChain m_textureSwapChain;
            ovrTextureSwapChain m_textureSwapChain;

			GLuint m_depthBufferTextureId;
            
            Size2 m_size;
            
        public:

            OculusTextureTarget();
            
			void Create(ovrSession hmd, const Size2 &size);

            void Clear();
            
            void Start(ovrSession hmd);
            
            void End();

			ovrTextureSwapChain GetOvrTextureSwapChain() const;
            
            Size2 GetSize() const;
            
            
        };
    }
}

#endif
