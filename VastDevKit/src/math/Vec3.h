#ifndef _VAST_MATH_VEC3_H_
#define _VAST_MATH_VEC3_H_

#include <math.h>

typedef struct Vec3 {
	Vec3 () {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }


    Vec3 (float x) {
        v[0] = x;
        v[1] = x;
        v[2] = x;
    }
	
	
    Vec3 (float x, float y) {
        v[0] = x;
        v[1] = y;
        v[2] = 0.0f;
    }
	
	
    Vec3 (float x, float y, float z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }
	
	
	void Set(float x, float y, float z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }
    
    
    void Set(const Vec3 &vec) {
        Set(vec[0], vec[1], vec[2]);
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


    Vec3 abs() const {
        return Vec3(fabsf(v[0]), fabsf(v[1]), fabsf(v[2]));
    }
	
    
	Vec3 operator -() const {
        return Vec3(-v[0], -v[1], -v[2]);
    }
	
    
	void operator += (const Vec3 &a) {
        v[0] += a[0];
        v[1] += a[1];
        v[2] += a[2];
    }
	
	
	void operator -= (const Vec3 &a) {
        v[0] -= a[0];
        v[1] -= a[1];
        v[2] -= a[2];
	}
	
	
	void operator *= (float s) {
        v[0] *= s;
        v[1] *= s;
        v[2] *= s;
	}
	
	
    // Dot product
	float operator * (const Vec3 &a) const {
        return v[0] * a[0] + v[1] * a[1] + v[2] * a[2];
	}
    
	float v[3];
} Vec3;


inline Vec3 operator + (const Vec3 &a, const Vec3 &b) {
	return Vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}


inline Vec3 operator - (const Vec3 &a, const Vec3 &b) {
    return Vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}


inline Vec3 operator * (float s, const Vec3 &a) {
	return Vec3(s * a[0], s * a[1], s * a[2]);
}


inline Vec3 operator / (const Vec3 &a, float d) {
    return Vec3(a[0] / d, a[1] / d, a[2] / d);
}


inline Vec3 operator * (const Vec3 &a, float s) {
    return Vec3(s * a[0], s * a[1], s * a[2]);
}

inline bool operator == (const Vec3 &a, const Vec3 &b) {
	return a[0] == b[0] && a[1] == b[1]  && a[2] == b[2];
}

#endif