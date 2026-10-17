#ifndef _VAST_MATH_QUATERNION_H_
#define _VAST_MATH_QUATERNION_H_

#include "math/Constants.h"
#include "math/Vec3.h"
#include "math/Functions.h"
#include <cmath>

enum Axis
{
    Axis_X = 0, Axis_Y = 1, Axis_Z = 2
};

// RotateDirection describes the rotation direction around an axis, interpreted as follows:
//  CW  - Clockwise while looking "down" from positive axis towards the origin.
//  CCW - Counter-clockwise while looking from the positive axis towards the origin,
//        which is in the negative axis direction.
//  CCW is the default for the RHS coordinate system. Oculus standard RHS coordinate
//  system defines Y up, X right, and Z back (pointing out from the screen). In this
//  system Rotate_CCW around Z will specifies counter-clockwise rotation in XY plane.
enum RotateDirection
{
    Rotate_CCW = 1,
    Rotate_CW  = -1
};


typedef struct Quaternion {
	Quaternion () {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
        v[3] = 1.0f;
    }
    
    
    Quaternion(const Vec3& axis, float theta) {
        float halfAngle = theta * 0.5f;
        Vec3 unitAxis = hd::normalize(axis);
        float sinHalfAngle = sinf(halfAngle);
        
        v[0] = unitAxis[0] * sinHalfAngle;
        v[1] = unitAxis[1] * sinHalfAngle;
        v[2] = unitAxis[2] * sinHalfAngle;
        v[3] = cosf(halfAngle);
    }
    
    
    Quaternion(Axis A, float angle, RotateDirection d) {
        const float RHS = 1.0f;
        
        float sinHalfAngle = RHS * d * sin(angle * 0.5f);
        Vec3 vec;
        vec[0] = vec[1] = vec[2] = 0.0f;
        vec[A] = sinHalfAngle;
        
        v[0] = vec[0];
        v[1] = vec[1];
        v[2] = vec[2];
        v[3] = cosf(angle * 0.5f);
    }
	

    Quaternion (float x, float y, float z, float w) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }



    static Quaternion LookAt(const Vec3 &eye, const Vec3 &at, const Vec3 &up) {
        Vec3 direction = hd::normalize(at - eye);
        Vec3 forward(0, 0, 1);
        float dot = forward * direction;

        if (fabs(dot - (-1.0f)) < 0.000001f) {
            return Quaternion(up, hd_pi);
        }
        if (fabs(dot - (1.0f)) < 0.000001f) {
            return Quaternion();
        }

        float rotationAngle = acosf(dot);
        Vec3 rotationAxis = hd::cross(forward, direction);
        rotationAxis = hd::normalize(rotationAxis);
        return Quaternion(rotationAxis, rotationAngle);
    }


    // Original axis-angle construction. Zero/nonfinite directions yield identity.
    // Normalize with double intermediates so large finite float inputs stay valid.
    static Quaternion FromTwoVectors(const Vec3 &u, const Vec3 &v) {
        const double sourceLength = std::sqrt(double(u[0]) * u[0] + double(u[1]) * u[1] + double(u[2]) * u[2]);
        const double targetLength = std::sqrt(double(v[0]) * v[0] + double(v[1]) * v[1] + double(v[2]) * v[2]);
        if (!(sourceLength > 0) || !(targetLength > 0) ||
            !std::isfinite(sourceLength) || !std::isfinite(targetLength)) return Quaternion();

        const Vec3 source(u[0] / sourceLength, u[1] / sourceLength, u[2] / sourceLength);
        const Vec3 target(v[0] / targetLength, v[1] / targetLength, v[2] / targetLength);
        Vec3 axis = hd::cross(source, target);
        const float sine = axis.Length();
        const float cosine = std::max(-1.0f, std::min(1.0f, source * target));
        if (sine > 0.000001f) return Quaternion(axis, std::atan2(sine, cosine));
        if (cosine >= 0) return Quaternion();

        // Opposite directions need an axis perpendicular to the source.
        unsigned int smallest = 0;
        for (unsigned int component = 1; component < 3; ++component) {
            if (std::fabs(source[component]) < std::fabs(source[smallest])) smallest = component;
        }
        Vec3 basis;
        basis[smallest] = 1.0f;
        return Quaternion(hd::cross(source, basis), hd_pi);
    }


    void SetIdentity() {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
        v[3] = 1.0f;
    }
    
    
	void Set(float x, float y, float z, float w) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }
    
    
    void Set(const Quaternion &quat) {
        Set(quat[0], quat[1], quat[2], quat[3]);
    }


    void Invert() {
        v[0] = -v[0];
        v[1] = -v[1];
        v[2] = -v[2];
        v[3] = v[3];
    }

/*
    void Normalize() {
        float len = Length();
        if (len == 0.0f) {
            Set(0.0f, 0.0f, 0.0f, 1.0f);
        } else {
            len = 1.0f / len;
            Set(v[0] * len, v[1] * len, v[2] * len, v[3] * len);
        }
    }*/


    Quaternion operator *(const Quaternion &b) const {
        Quaternion t;
        t[0] = v[3] * b[0] + v[0] * b[3] + v[1] * b[2] - v[2] * b[1];
        t[1] = v[3] * b[1] - v[0] * b[2] + v[1] * b[3] + v[2] * b[0];
        t[2] = v[3] * b[2] + v[0] * b[1] - v[1] * b[0] + v[2] * b[3];
        t[3] = v[3] * b[3] - v[0] * b[0] - v[1] * b[1] - v[2] * b[2];
        return t;
    }


    ///
    /// Multiply vector by this quaternion.  Equivalent to Matrix::Vec3Multiply.
    /// via http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
    /// Copyright (c) 2012-2017 Stefan Reinalter, MIT.
    /// Complete notice: docs/licenses/Stefan-Reinalter-MIT.txt.
    ///
    Vec3 operator *(const Vec3 &vec) const {
        Vec3 q(v[0], v[1], v[2]);
        Vec3 t = 2 * hd::cross(q, vec);
        Vec3 ret = vec + v[3] * t + hd::cross(q, t);
        return ret;
    }


    inline float Length() const {
        return sqrt(v[0] * v[0] +
                    v[1] * v[1] +
                    v[2] * v[2] +
                    v[3] * v[3]);
    }
    
    
    inline float& operator[](unsigned int index) {
        return v[index];
    }
    
    
    inline const float& operator[](unsigned int index) const {
        return v[index];
    }
    
	float v[4];
} Quaternion;


inline float diff(const Quaternion &a, const Quaternion &b) {
    return a[0] == b[0] && a[1] == b[1]  && a[2] == b[2] && a[3] == b[3];
}


inline bool operator == (const Quaternion &a, const Quaternion &b) {
    return a[0] == b[0] && a[1] == b[1]  && a[2] == b[2] && a[3] == b[3];
}

#endif
