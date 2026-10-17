#ifndef _VAST_MATH_SIZE3_H_
#define _VAST_MATH_SIZE3_H_

typedef struct Size3 {
	Size3 () {
        _v[0] = 0;
        _v[1] = 0;
        _v[2] = 0;
    }
    
    
    Size3 (const Size3 &s) {
        _v[0] = s[0];
        _v[1] = s[1];
        _v[2] = s[2];
    }
	
	
    Size3 (uint32_t x, uint32_t y, uint32_t z) {
        _v[0] = x;
        _v[1] = y;
        _v[2] = z;
    }
	
	
	void Set(uint32_t x, uint32_t y, uint32_t z) {
        _v[0] = x;
        _v[1] = y;
        _v[2] = z;
    }
    
    
    void Set(const Size3 &sz) {
        Set(sz[0], sz[1], sz[2]);
    }
    
    
    inline uint32_t& operator[](unsigned int index) {
        return _v[index];
    }
    
    
    inline const uint32_t& operator[](unsigned int index) const {
        return _v[index];
    }
	
    
	void operator += (const Size3& a) {
        _v[0] += a[0];
        _v[1] += a[1];
        _v[2] += a[2];
    }
	
	
	void operator -= (const Size3& a) {
        _v[0] -= a[0];
        _v[1] -= a[1];
        _v[2] -= a[2];
	}
	
	
	void operator *= (uint32_t s) {
        _v[0] *= s;
        _v[1] *= s;
        _v[2] *= s;
	}
	
	
    // Dot product
	uint32_t operator * (const Size3& a) const {
        return _v[0] * a[0] + _v[1] * a[1] + _v[2] * a[2];
	}
    
	uint32_t _v[3];
} Size3;


inline Size3 operator + (const Size3& a, const Size3& b) {
	return Size3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}


inline Size3 operator - (const Size3& a, const Size3& b) {
    return Size3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}


inline Size3 operator * (uint32_t s, const Size3& a) {
	return Size3(s * a[0], s * a[1], s * a[2]);
}


inline bool operator == (const Size3& a, const Size3& b) {
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

#endif