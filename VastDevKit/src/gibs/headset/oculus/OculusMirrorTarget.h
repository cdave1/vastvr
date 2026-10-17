#ifndef _VAST_GIBS_OCULUS_MIRROR_TARGET_H_
#define _VAST_GIBS_OCULUS_MIRROR_TARGET_H_

#include <Vast/Vast.h>
//#include "Kernel/OVR_System.h"

// Include the Oculus SDK
//#include "OVR_CAPI_GL.h"

namespace hd {
    namespace input {
    typedef int ovrMirrorTexture;
    typedef int ovrSession;
    class OculusMirrorTarget {
        private:
            
            GLuint m_frameBufferId;

			ovrMirrorTexture m_mirrorTexture;
            
            Size2 m_size;
            
        public:

			OculusMirrorTarget();
            
			void Create(ovrSession hmd, const Size2 &size);

			void Render();

			ovrMirrorTexture GetOvrGLTexture() const;
            
            Size2 GetSize() const;
            
            
        };
    }
}

#endif
