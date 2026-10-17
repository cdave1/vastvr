#ifndef _VAST_NOISE_H
#define _VAST_NOISE_H

#include "graphics/GL/GraphicsInterface.h"

#include "graphics/Texture/Texture.h"


#ifdef __cplusplus
extern "C" {
#endif

    extern void hdCalculateNoise(float * &noise, const unsigned int width, const unsigned int height, int type, float z);
    
    extern void hdMakeNoiseTexture(const unsigned int width, const unsigned int height, int type, float z, hd::Texture * tex);

#ifdef __cplusplus
}
#endif

#endif