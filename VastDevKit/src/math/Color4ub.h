#ifndef _VAST_MATH_COLOR4UB_H_
#define _VAST_MATH_COLOR4UB_H_

#include "math/Color4f.h"

struct Color4ub
{
	Color4ub() 
    {
        SetZero();
    }
	
    
	Color4ub (float rf, float gf, float bf, float af)
    {
        Setf(rf, gf, bf, af);
    }
	
    
	Color4ub (uint8_t rb, uint8_t gb, uint8_t bb, uint8_t ab)
    {
        Set(rb, gb, bb, ab);
    }
    
	
	Color4ub (int ri, int gi, int bi, int ai)
    {
        r = (uint8_t)ri;
        g = (uint8_t)gi;
        b = (uint8_t)bi;
        a = (uint8_t)ai;        
    }
	
    
    void SetZero() 
    { 
        r = 0; 
        g = 0;
        b = 0;
        a = 0;
    }


    void Set(const Color4f &color) {
        Setf(color[0], color[1], color[2], color[3]);
    }
    
    
	void Setf(float rf, float gf, float bf, float af)
	{
		r = (uint8_t)(rf * 255);
		g = (uint8_t)(gf * 255); 
		b = (uint8_t)(bf * 255); 
		a = (uint8_t)(af * 255);
	}
	
    
	void Set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}
	
    
	uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

#endif