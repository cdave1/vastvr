#ifndef _VAST_MATH_FRUSTUM_H_
#define _VAST_MATH_FRUSTUM_H_

#include "math/Vec3.h"
#include "math/Plane.h"
#include "math/AABB.h"
#include "math/Matrix.h"


struct Frustum {
	Frustum (Matrix &mvp) {
        planes[0] = Plane(mvp[3] - mvp[0], mvp[7] - mvp[4], mvp[11] - mvp[8], mvp[15] - mvp[12]);
        planes[1] = Plane(mvp[3] + mvp[0], mvp[7] + mvp[4], mvp[11] + mvp[8], mvp[15] + mvp[12]);

        planes[2] = Plane(mvp[3] + mvp[1], mvp[7] + mvp[5], mvp[11] + mvp[9], mvp[15] + mvp[13]);
        planes[3] = Plane(mvp[3] - mvp[1], mvp[7] - mvp[5], mvp[11] - mvp[9], mvp[15] - mvp[13]);

        planes[4] = Plane(mvp[3] - mvp[2], mvp[7] - mvp[6], mvp[11] - mvp[10], mvp[15] - mvp[14]);
        planes[5] = Plane(mvp[3] + mvp[2], mvp[7] + mvp[6], mvp[11] + mvp[10], mvp[15] + mvp[14]);

        for (unsigned i = 0; i < 6; ++i) {
            planes[i].Normalize();
        }
    }


    inline bool Contains(const Vec3 &center, const float radius) {
        for (int16_t i = 5; i >= 0; --i) {
            float dot = planes[i].DotCoord(center);
            if (dot < 0.0f) {
                if (fabsf(dot) < radius) {
                    return true;
                } else {
                    return false;
                }
            }
        }

        return true;
    }


    inline bool AABBInside(const Vec3 &origin, const Vec3 &scale, const AABB &aabb) {
        float longest = 0.4f * std::max(scale[0] * aabb.Width(), std::max(scale[1] * aabb.Height(), scale[2] * aabb.Depth()));
        return Contains(origin, longest);
    }
	
    Plane planes[6];
};

#endif