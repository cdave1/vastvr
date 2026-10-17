//
//  glimp.h
//  skeleton
//
//  Created by David Petrie on 24/03/11.
//  Copyright 2011 n/a. All rights reserved.
//

#ifndef GLIMP_H
#define GLIMP_H

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <math.h>
#include "hdMath.h"

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t Vec3[3];
typedef vec_t vec4_t[4];
typedef vec4_t color4_t;


#define vec2Set(__v, __x, __y) __v[0] = __x; __v[1] = __y;
#define vec3Set(__v, __x, __y, __z) __v[0] = __x; __v[1] = __y; __v[2] = __z;
#define vec4Set(__v, __x, __y, __z, __u) __v[0] = __x; __v[1] = __y; __v[2] = __z; __v[3] = __u;


#define __11 0
#define __21 1
#define __31 2
#define __41 3
#define __12 4
#define __22 5
#define __32 6
#define __42 7
#define __13 8
#define __23 9
#define __33 10
#define __43 11
#define __14 12
#define __24 13
#define __34 14
#define __44 15


typedef struct camera_s
{
    Vec3 eye;
    Vec3 center;
    Vec3 up;
} camera_t;


#ifdef __cplusplus
extern "C" {
#endif
    
    //void aglBindTextureAttribute(GLint attributeHandle);
    
    void aglCross3(Vec3 vOut, const Vec3 a, const Vec3 b);
    
    void aglNormalize3(Vec3 vOut, const Vec3 vec);
    
    void aglMatrixMultiply(float *mOut,
                           const float *mA,
                           const float *mB);
    
    void aglBegin(GLenum prim);
    
    void aglVertex3f(float x, float y, float z);
    
    void aglColor4f(float r, float g, float b, float a);
    
    void aglTexCoord2f(float s, float t);
    
    void aglEnd();
    
    void aglError(const char *source);
    
    void aglMatrixTranslation(
                              float *mOut,
                              const float fX,
                              const float fY,
                              const float fZ);
    
    void aglMatrixRotationZ(float *mOut,
                            const float fAngle);
    
#ifdef __cplusplus
}
#endif
#endif