#ifndef _VAST_MATH_COLLISION3_H_
#define _VAST_MATH_COLLISION3_H_

#include "math/Vec2.h"
#include "math/Vec3.h"

///
/// Worldspace vectors for a collision:
///
/// @origin the worldspace location of the collision
/// @normal the surface normal at origin
/// @incidentRay the NORMALIZED direction of the ray to create the collision
/// @reflection the direction of the force returned by surface.  Where incidentRay == 1,
/// reflectedRay will have the range: 0 <= reflectedRay <= incidentRay
///
struct Collision3 {
	Collision3 () {
        origin.Set(0, 0, 0);
        normal.Set(0, 0, 0);
        incidentRay.Set(0, 0, 0);
        reflectedRay.Set(0, 0, 0);
    }

    Collision3(float x, float y) {
        Reset();
        origin.Set(x, y, 0.0f);
    }

    Collision3(float x, float y, float z) {
        Reset();
        origin.Set(x, y, z);
    }

    Collision3(Vec3 vec) {
        Reset();
        origin.Set(vec);
    }
    
    void Reset() {
        origin.Set(0, 0, 0);
        normal.Set(0, 0, 0);
        incidentRay.Set(0, 0, 0);
        reflectedRay.Set(0, 0, 0);
    }
	
    Vec3 origin;
    Vec3 normal;
    Vec3 incidentRay;
    Vec3 reflectedRay;
};

#endif