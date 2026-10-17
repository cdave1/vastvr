#ifndef _VAST_MATH_SIZE2_H_
#define _VAST_MATH_SIZE2_H_

typedef struct Size2 {
	Size2 () {
        _v[0] = 0;
        _v[1] = 0;
    }
    
    
    Size2 (const Size2 &s) {
        _v[0] = s[0];
        _v[1] = s[1];
    }
	
	
    Size2 (uint32_t x, uint32_t y) {
        _v[0] = x;
        _v[1] = y;
    }
	
	
	void Set(uint32_t x, uint32_t y) {
        _v[0] = x;
        _v[1] = y;
    }
    
    
    void Set(const Size2 &vec) {
        Set(vec[0], vec[1]);
    }
	
    
	double Length() const {
        return sqrt(_v[0] * _v[0] + _v[1] * _v[1]);
	}
    
    
    inline uint32_t& operator[](unsigned int index) {
        return _v[index];
    }
    
    
    inline const uint32_t& operator[](unsigned int index) const {
        return _v[index];
    }
	
    
	void operator += (const Size2& a) {
        _v[0] += a[0];
        _v[1] += a[1];
    }
	
	
	void operator -= (const Size2& a) {
        _v[0] -= a[0];
        _v[1] -= a[1];
	}
	
	
	void operator *= (uint32_t s) {
        _v[0] *= s;
        _v[1] *= s;
	}
	
	
    // Dot product
	uint32_t operator * (const Size2& a) const {
        return _v[0] * a[0] + _v[1] * a[1];
	}
    
	uint32_t _v[2];
} Size2;


inline Size2 operator + (const Size2& a, const Size2& b) {
	return Size2(a[0] + b[0], a[1] + b[1]);
}


inline Size2 operator - (const Size2& a, const Size2& b) {
    return Size2(a[0] - b[0], a[1] - b[1]);
}


inline Size2 operator * (uint32_t s, const Size2& a) {
	return Size2(s * a[0], s * a[1]);
}


inline bool operator == (const Size2& a, const Size2& b) {
	return a[0] == b[0] && a[1] == b[1];
}

#endif