#ifndef _VAST_MATH_PLANE_H_
#define _VAST_MATH_PLANE_H_

#include "math/Vec3.h"
#include "math/Vec4.h"

typedef struct Plane {
	Plane () {
        normal = Vec3();
        d = 0.0f;
    }
	
	
    Plane (float x, float y, float z, float _d) {
        normal = Vec3(x, y, z);
        d = _d;
    }
	
	
    Plane (Vec3 _n, float _d) {
        normal = _n;
        d = _d;
    }


    void Normalize() {
        float dist = normal.Length();
        normal = normal / dist;
        d = d / dist;
    }


    float Dot(const Vec4 &v) {
        return normal[0] * v[0] + normal[1] * v[1] + normal[2] * v[2] + d * v[3];
    }


    float DotCoord(const Vec3 &v) {
        return normal[0] * v[0] + normal[1] * v[1] + normal[2] * v[2] + d;
    }


    float DotNormal(const Vec3 &v) {
        return normal[0] * v[0] + normal[1] * v[1] + normal[2] * v[2];
    }


    Vec3 normal;
    float d;
} Plane;

#endif