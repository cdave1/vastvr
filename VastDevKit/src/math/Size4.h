#ifndef _VAST_MATH_SIZE4_H_
#define _VAST_MATH_SIZE4_H_

typedef struct Size4 {
	Size4 () {
        _v[0] = 0;
        _v[1] = 0;
        _v[2] = 0;
        _v[3] = 0;
    }
    
    
    Size4 (const Size4 &s) {
        _v[0] = s[0];
        _v[1] = s[1];
        _v[2] = s[2];
        _v[3] = s[3];
    }
	
	
    Size4 (uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
        _v[0] = a;
        _v[1] = b;
        _v[2] = c;
        _v[3] = d;
    }
	
	
	void Set(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
        _v[0] = a;
        _v[1] = b;
        _v[2] = c;
        _v[3] = d;
    }
    
    
    void Set(const Size4 &sz) {
        Set(sz[0], sz[1], sz[2], sz[3]);
    }
    
    
    inline uint32_t& operator[](unsigned int index) {
        return _v[index];
    }
    
    
    inline const uint32_t& operator[](unsigned int index) const {
        return _v[index];
    }
	
    
	void operator += (const Size4& a) {
        _v[0] += a[0];
        _v[1] += a[1];
        _v[2] += a[2];
        _v[3] += a[3];
    }
	
	
	void operator -= (const Size4& a) {
        _v[0] -= a[0];
        _v[1] -= a[1];
        _v[2] -= a[2];
        _v[3] -= a[3];
	}
	
	
	void operator *= (uint32_t s) {
        _v[0] *= s;
        _v[1] *= s;
        _v[2] *= s;
        _v[3] *= s;
	}
    
	uint32_t _v[4];
} Size4;


inline Size4 operator + (const Size4& a, const Size4& b) {
	return Size4(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
}


inline Size4 operator - (const Size4& a, const Size4& b) {
    return Size4(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
}


inline Size4 operator * (uint32_t s, const Size4& a) {
	return Size4(s * a[0], s * a[1], s * a[2], s * a[3]);
}


inline bool operator == (const Size4& a, const Size4& b) {
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}

#endif