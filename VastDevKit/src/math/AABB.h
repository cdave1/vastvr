#ifndef _VAST_MATH_AABB_H_
#define _VAST_MATH_AABB_H_

#include "math/Vec3.h"
#include "math/Size2.h"

typedef struct AABB {
	AABB() : lower(), upper() {}
    
    
    AABB(float width, float height) {
        lower.Set(0.5f * -width, 0.5f * -height, 0.0f);
        upper.Set(0.5f *  width, 0.5f *  height, 0.0f);
    }
    
    
    AABB(float loX, float loY, float width, float height) {
        lower.Set(loX, loY, 0.0f);
        upper.Set(loX + width, loY + height, 0.0f);
    }
    
    
    AABB(Vec3 lo, Vec3 hi) {
        lower.Set(lo);
        upper.Set(hi);
    }
    
    
    AABB(float width, float height, Vec3 center) {
        lower.Set(center[0] - (0.5f * width),
                  center[1] - (0.5f * height),
                  center[2]);
        upper.Set(center[0] + (0.5f * width),
                  center[1] + (0.5f * height),
                  center[2]);
    }
    
    
    Vec3 Center() const {
        return lower + (0.5f * (upper - lower));
    }
	
    
	float Width() const {
		return upper[0] - lower[0];
	}
	
    
	float Height() const {
		return upper[1] - lower[1];
	}
    
    
    float Depth() const {
		return upper[2] - lower[2];
	}


    Vec3 Half() const {
        return Vec3(0.5f * Width(), 0.5f * Height(), 0.0f);
    }


    void SetCenter(const Vec3 &center) {
        Vec3 size(Width(), Height(), Depth());
        lower.Set(center - (0.5f * size));
        upper.Set(center + (0.5f * size));
    }
    
    
    Size2 GetSize2() const {
        return Size2(uint32_t(Width()), uint32_t(Height()));
    }


    Vec3 Size() const {
        return Vec3(Width(), Height(), Depth());
    }
	
    
	Vec3 lower;
	Vec3 upper;
} AABB;

inline bool operator == (const AABB &a, const AABB &b) {
    return a.lower == b.lower && a.upper == b.upper;
}

#endif