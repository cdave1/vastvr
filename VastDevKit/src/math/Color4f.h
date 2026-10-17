#ifndef _VAST_MATH_COLOR4F_H_
#define _VAST_MATH_COLOR4F_H_

typedef struct Color4f {
	Color4f() {
        Set(1.0f, 1.0f, 1.0f, 1.0f);
    }
	
    
	Color4f (float rf, float gf, float bf, float af) {
        Set(rf, gf, bf, af);
    }
	
    
	Color4f (uint8_t rb, uint8_t gb, uint8_t bb, uint8_t ab) {
        Setb(rb, gb, bb, ab);
    }
	
    
	Color4f (int ri, int gi, int bi, int ai) {
        Seti(ri, gi, bi, ai);
    }
    
    
    inline float& operator[](unsigned int index) {
        return f[index];
    }
    
    
    inline const float& operator[](unsigned int index) const {
        return f[index];
    }
    
    
	void Set(float r_, float g_, float b_, float a_) {
		f[0] = r_;
		f[1] = g_;
		f[2] = b_;
		f[3] = a_;
	}
    
    
    void Setb(uint8_t ur, uint8_t ug, uint8_t ub, uint8_t ua) {
        f[0] = ur / float(255);
        f[1] = ug / float(255);
        f[2] = ub / float(255);
        f[3] = ua / float(255);
    }
    
    
    void Seti(int ur, int ug, int ub, int ua) {
        f[0] = float(ur) / 255.0f;
		f[1] = float(ug) / 255.0f;
		f[2] = float(ub) / 255.0f;
		f[3] = float(ua) / 255.0f;
    }
    
	
	void Set(Color4f color) {
		f[0] = color[0];
		f[1] = color[1];
		f[2] = color[2];
		f[3] = color[3];
	}
	
    
	float f[4];
} Color4f;

#endif