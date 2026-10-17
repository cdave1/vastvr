#ifndef _HD_INPUT_DISTORTION_VERTEX_H_
#define _HD_INPUT_DISTORTION_VERTEX_H_

#include "math/Vec2.h"
#include "math/Color4f.h"

namespace hd {
    namespace input {
        struct DistortionVertex {
            DistortionVertex () {
                position.Set(0.0f, 0.0f, 0.0f, 0.0f);
                tanEyeAnglesR.Set(0.0f, 0.0f);
                tanEyeAnglesG.Set(0.0f, 0.0f);
                tanEyeAnglesB.Set(0.0f, 0.0f);
            }
            
            Vec4 position;
            Vec2 tanEyeAnglesR;
            Vec2 tanEyeAnglesG;
            Vec2 tanEyeAnglesB;
        };
    }
}

#endif
