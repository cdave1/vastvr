#ifndef _VAST_MATH_FUNCTIONS_H_
#define _VAST_MATH_FUNCTIONS_H_

#include "math/Color4f.h"
#include "math/Color4ub.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Size2.h"
#include "math/Size3.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Vec4.h"
#include "math/AABB.h"

namespace hd {
    inline Vec3 unproject(const Vec3 &window_pos, const Matrix &modelView, const Matrix &projection,
                          const AABB &viewport) {
        Matrix inverse, view;
        Vec4 a, b;
        Vec3 result;
        
        view = projection * modelView;
        Matrix::InverseLong(inverse, view);
        
        a[0] = (window_pos[0] - viewport.lower[0]) / viewport.Width() * 2.0f - 1.0f;
        a[1] = (window_pos[1] - viewport.lower[1]) / viewport.Height() * 2.0f - 1.0f;
        a[2] = 2.0f * window_pos[2] - 1.0f;
        a[3] = 1.0f;
        
        Matrix::Vec4Multiply(b, a, inverse);
        
        if (b[3] == 0.0f) {
            return result;
        }
        
        b[3] = 1.0f / b[3];
        result[0] = b[0] * b[3];
        result[1] = b[1] * b[3];
        result[2] = b[2] * b[3];
        return result;
    }
    
    
    inline Vec3 cross(const Vec3& a, const Vec3& b) {
        return Vec3((a[1] * b[2]) - (a[2] * b[1]),
                    (a[2] * b[0]) - (a[0] * b[2]),
                    (a[0] * b[1]) - (a[1] * b[0]));
    }
    
    
    inline Vec3 normalize(const Vec3 &vec) {
        float len = vec.Length();
        if (len == 0) {
            return Vec3();
        }
        float f = 1.0f / len;
        
        return Vec3(vec[0] * f, vec[1] * f, vec[2] * f);
    }


    /*
    inline Quaternion normalize(const Quaternion &quat) {
        Quaternion q = quat;
        q.Normalize();
        return q;
    }*/
    
    
    inline Vec2 normalize(const Vec2& vec) {
        float length = vec.Length();
        //if (length < FLT_EPSILON)
        //{
        //	return vec;
        //}
        float invLength = 1.0f / length;
        float x = vec[0] * invLength;
        float y = vec[1] * invLength;
        
        return Vec2(x,y);
    }
    
    
    inline float sin(const float rads) {
        return ::sin(rads);
    }
    
    
    inline float cos(const float rads) {
        return ::cos(rads);
    }
    
    
    template <typename T>
    inline T min(T a, T b) {
        return a < b ? a : b;
    }


    inline Size2 min(const Size2 &a, const Size2 &b) {
        Size2 c;
        c[0] = hd::min(a[0], b[0]);
        c[1] = hd::min(a[1], b[1]);
        return c;
    }


    inline Size3 min(const Size3 &a, const Size3 &b) {
        Size3 c;
        c[0] = hd::min(a[0], b[0]);
        c[1] = hd::min(a[1], b[1]);
        c[2] = hd::min(a[2], b[2]);
        return c;
    }
    
    
    inline Vec2 min(const Vec2& a, const Vec2& b) {
        Vec2 c;
        c[0] = hd::min(a[0], b[0]);
        c[1] = hd::min(a[1], b[1]);
        return c;
    }
    
    
    inline Vec3 min(const Vec3& a, const Vec3& b) {
        Vec3 c;
        c[0] = hd::min(a[0], b[0]);
        c[1] = hd::min(a[1], b[1]);
        c[2] = hd::min(a[2], b[2]);
        return c;
    }
    
    
    template <typename T>
    inline T max(T a, T b) {
        return a > b ? a : b;
    }


    inline Size2 max(const Size2 &a, const Size2 &b) {
        Size2 c;
        c[0] = hd::max(a[0], b[0]);
        c[1] = hd::max(a[1], b[1]);
        return c;
    }


    inline Size3 max(const Size3 &a, const Size3 &b) {
        Size3 c;
        c[0] = hd::max(a[0], b[0]);
        c[1] = hd::max(a[1], b[1]);
        c[2] = hd::max(a[2], b[2]);
        return c;
    }

    
    inline Vec2 max(const Vec2& a, const Vec2& b) {
        Vec2 c;
        c[0] = hd::max(a[0], b[0]);
        c[1] = hd::max(a[1], b[1]);
        return c;
    }
    
    
    inline Vec3 max(const Vec3& a, const Vec3& b) {
        Vec3 c;
        c[0] = hd::max(a[0], b[0]);
        c[1] = hd::max(a[1], b[1]);
        c[2] = hd::max(a[2], b[2]);
        return c;
    }
    
    
    template <typename T>
    inline T clamp(T a, T floor, T ceiling) {
        return hd::min(ceiling, hd::max(floor, a));
    }
    
    
    inline float abs(float a) {
        return a > 0.0f ? a : -a;
    }


    ///
    /// Scalar interpolation -- see http://en.wikipedia.org/wiki/Smoothstep
    ///
    template <typename T>
    inline T smoothstep(T edge0, T edge1, T a) {
        T x = hd::clamp((a - edge0)/(edge1 - edge0), T(0), T(1));
        return x*x*(3 - 2 * x);
    }


    ///
    /// Linear interpolation: http://en.wikipedia.org/wiki/Linear_interpolation
    ///
    template <typename T>
    inline T lerp(T a, T b, float t) {
        hdAssert(t >= 0.0f && t <= 1.0f);
        return a * (1.0f - t) + b * t;
    }


    inline Vec3 lerp(const Vec3 &a, const Vec3 &b, float t) {
        hdAssert(t >= 0.0f && t <= 1.0f);
        return Vec3(a[0] * (1.0f - t) + b[0] * t,
                    a[1] * (1.0f - t) + b[1] * t,
                    a[2] * (1.0f - t) + b[2] * t);
    }


    /*
     * from rgba demo crew - l4m3r copy and paste.
     */
    static unsigned int mirand = 1;
    inline float sf_rand( void ) {
        unsigned int a;
        mirand *= 16807;
        a = (mirand&0x007fffff) | 0x40000000;
        return( *((float*)&a) - 3.0f );
    }
    
    
    inline float hRand() {
        return fabs(sf_rand());
    }
    
    
#define	RAND_LIMIT	32767
    inline float randomf(float lo, float hi) {
        float r = fabs(sf_rand());
        r = (hi - lo) * r + lo;
        return r;
    }
    
    
    /**
     * via djb2 (no link, google it).
     */
    inline unsigned long hash(unsigned char *str) {
        unsigned long hash = 5381;
        int c;
        
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        
        return hash;
    }
    
    
    /*
     Vector3 CalculateBezierPoint(float t,
     Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
     {
     float u = 1 – t;
     float tt = t*t;
     float uu = u*u;
     float uuu = uu * u;
     float ttt = tt * t;
     
     Vector3 p = uuu * p0; //first term
     p += 3 * uu * t * p1; //second term
     p += 3 * u * tt * p2; //third term
     p += ttt * p3; //fourth term
     
     return p;
     */
    inline Vec3 BezierPoint(float t, Vec3 p1, Vec3 p2, Vec3 c1, Vec3 c2) {
        float u = 1.0f - t;
        Vec3 res = (u * u * u) * p1;
        res += (3 * u * u * t) * c1;
        res += (3 * u * t * t) * c2;
        res += (t * t * t) * p2;
        return res;
    }
}

#endif