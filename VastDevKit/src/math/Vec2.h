#ifndef _VAST_MATH_VEC2_H_
#define _VAST_MATH_VEC2_H_

typedef struct Vec2 {
	Vec2 () {
        _v[0] = 0.0f;
        _v[1] = 0.0f;
    }
    
    
    Vec2 (const Vec2 &vec) {
        _v[0] = vec[0];
        _v[1] = vec[1];
    }


    Vec2 (float x) {
        _v[0] = x;
        _v[1] = x;
    }
	
	
    Vec2 (float x, float y) {
        _v[0] = x;
        _v[1] = y;
    }
	
	
	void Set(float x, float y) {
        _v[0] = x;
        _v[1] = y;
    }
    
    
    void Set(const Vec2 &vec) {
        Set(vec[0], vec[1]);
    }
	
    
	float Length() const {
        return sqrtf(_v[0] * _v[0] + _v[1] * _v[1]);
	}
    
    
    inline float& operator[](unsigned int index) {
        return _v[index];
    }
    
    
    inline const float& operator[](unsigned int index) const {
        return _v[index];
    }


    Vec2 abs() const {
        return Vec2(fabsf(_v[0]), fabsf(_v[1]));
    }

    
	Vec2 operator -() const { 
        return Vec2(-_v[0], -_v[1]);
    }
	
    
	void operator += (const Vec2& a) {
        _v[0] += a[0];
        _v[1] += a[1];
    }
	
	
	void operator -= (const Vec2& a) {
        _v[0] -= a[0];
        _v[1] -= a[1];
	}
	
	
	void operator *= (float s) {
        _v[0] *= s;
        _v[1] *= s;
	}
	
	
    // Dot product
	float operator * (const Vec2& a) const {
        return _v[0] * a[0] + _v[1] * a[1];
	}
    
	float _v[2];
} Vec2;


inline Vec2 operator + (const Vec2& a, const Vec2& b) {
	return Vec2(a[0] + b[0], a[1] + b[1]);
}


inline Vec2 operator - (const Vec2& a, const Vec2& b) {
    return Vec2(a[0] - b[0], a[1] - b[1]);
}


inline Vec2 operator * (float s, const Vec2& a) {
	return Vec2(s * a[0], s * a[1]);
}


inline bool operator == (const Vec2& a, const Vec2& b) {
	return a[0] == b[0] && a[1] == b[1];
}

#endif