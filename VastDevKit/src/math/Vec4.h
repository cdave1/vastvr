#ifndef _VAST_MATH_VEC4_H_
#define _VAST_MATH_VEC4_H_

#include "math/Vec3.h"

typedef struct Vec4 {
	Vec4 () {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
        v[3] = 0.0f;
    }
	
	
    Vec4 (float x, float y) {
        v[0] = x;
        v[1] = y;
        v[2] = 0.0f;
        v[3] = 0.0f;
    }
	
	
    Vec4 (float x, float y, float z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = 0.0f;
    }
	

    Vec4 (float x, float y, float z, float w) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }


    Vec4 (Vec3 vec, float w) {
        v[0] = vec[0];
        v[1] = vec[1];
        v[2] = vec[2];
        v[3] = w;
    }
    
    
	void Set(float x, float y, float z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }
    
    
	void Set(float x, float y, float z, float w) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }
    
    
    void Set(const Vec4 &vec) {
        Set(vec[0], vec[1], vec[2], vec[3]);
    }
	
    
	float Length() const {
        return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}
    
    
    inline float& operator[](unsigned int index) {
        return v[index];
    }
    
    
    inline const float& operator[](unsigned int index) const {
        return v[index];
    }
	
    
	Vec4 operator -() const {
        return Vec4(-v[0], -v[1], -v[2], -v[3]);
    }
	
    
	void operator += (const Vec4 &a) {
        v[0] += a[0];
        v[1] += a[1];
        v[2] += a[2];
        v[3] += a[3];
    }
	
	
	void operator -= (const Vec4 &a) {
        v[0] -= a[0];
        v[1] -= a[1];
        v[2] -= a[2];
        v[3] -= a[3];
	}
	
	
	void operator *= (float s) {
        v[0] *= s;
        v[1] *= s;
        v[2] *= s;
        v[3] *= s;
	}
	
	
    // Dot product
	float operator * (const Vec4 &a) const {
        return v[0] * a[0] + v[1] * a[1] + v[2] * a[2] + v[3] * a[3];
	}
    
	float v[4];
} Vec4;


inline Vec4 operator + (const Vec4 &a, const Vec4 &b) {
	return Vec4(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
}


inline Vec4 operator - (const Vec4 &a, const Vec4 &b) {
    return Vec4(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
}


inline Vec4 operator * (float s, const Vec4 &a) {
	return Vec4(s * a[0], s * a[1], s * a[2], s * a[3]);
}


inline bool operator == (const Vec4 &a, const Vec4 &b) {
	return a[0] == b[0] && a[1] == b[1]  && a[2] == b[2] && a[3] == b[3];
}

#endif