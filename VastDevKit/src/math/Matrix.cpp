/*
Matrix routines include adaptations of Oolong Math/Matrix.cpp.
The project changed the Matrix class/API, storage access, and platform paths;
this file is not the unmodified Oolong source. Other original routines and
formula-based additions retain their respective attribution below.

Copyright (c) 2007-2008 Wolfgang Engel  http://code.google.com/p/oolongengine/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#include "math/Matrix.h"

#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Quaternion.h"
#include "math/Functions.h"
#include "math/Constants.h"


Matrix::Matrix() {
}


Matrix::~Matrix() {
}


float& Matrix::operator[] (unsigned int index) {
    return f[index];
}


const float& Matrix::operator[] (unsigned int index) const {
    return f[index];
}


void Matrix::Identity(Matrix &m) {
	m[ 0] = 1;	m[ 4] = 0;	m[ 8] = 0;	m[12] = 0;
	m[ 1] = 0;	m[ 5] = 1;	m[ 9] = 0;	m[13] = 0;
	m[ 2] = 0;	m[ 6] = 0;	m[10] = 1;	m[14] = 0;
	m[ 3] = 0;	m[ 7] = 0;	m[11] = 0;	m[15] = 1;
}


Matrix Matrix::Identity() {
    Matrix m;
    Matrix::Identity(m);
    return m;
}


Matrix& Matrix::Multiply(Matrix *r,
                         const Matrix &a,
                         const Matrix &b) {
    r->f[ 0] = (a[ 0] * b[ 0]) + (a[ 4] * b[ 1]) + (a[ 8] * b[ 2]) + (a[12] * b[ 3]);
    r->f[ 1] = (a[ 1] * b[ 0]) + (a[ 5] * b[ 1]) + (a[ 9] * b[ 2]) + (a[13] * b[ 3]);
    r->f[ 2] = (a[ 2] * b[ 0]) + (a[ 6] * b[ 1]) + (a[10] * b[ 2]) + (a[14] * b[ 3]);
    r->f[ 3] = (a[ 3] * b[ 0]) + (a[ 7] * b[ 1]) + (a[11] * b[ 2]) + (a[15] * b[ 3]);
    
    r->f[ 4] = (a[ 0] * b[ 4]) + (a[ 4] * b[ 5]) + (a[ 8] * b[ 6]) + (a[12] * b[ 7]);
    r->f[ 5] = (a[ 1] * b[ 4]) + (a[ 5] * b[ 5]) + (a[ 9] * b[ 6]) + (a[13] * b[ 7]);
    r->f[ 6] = (a[ 2] * b[ 4]) + (a[ 6] * b[ 5]) + (a[10] * b[ 6]) + (a[14] * b[ 7]);
    r->f[ 7] = (a[ 3] * b[ 4]) + (a[ 7] * b[ 5]) + (a[11] * b[ 6]) + (a[15] * b[ 7]);
    
    r->f[ 8] = (a[ 0] * b[ 8]) + (a[ 4] * b[ 9]) + (a[ 8] * b[10]) + (a[12] * b[11]);
    r->f[ 9] = (a[ 1] * b[ 8]) + (a[ 5] * b[ 9]) + (a[ 9] * b[10]) + (a[13] * b[11]);
    r->f[10] = (a[ 2] * b[ 8]) + (a[ 6] * b[ 9]) + (a[10] * b[10]) + (a[14] * b[11]);
    r->f[11] = (a[ 3] * b[ 8]) + (a[ 7] * b[ 9]) + (a[11] * b[10]) + (a[15] * b[11]);
    
    r->f[12] = (a[ 0] * b[12]) + (a[ 4] * b[13]) + (a[ 8] * b[14]) + (a[12] * b[15]);
    r->f[13] = (a[ 1] * b[12]) + (a[ 5] * b[13]) + (a[ 9] * b[14]) + (a[13] * b[15]);
    r->f[14] = (a[ 2] * b[12]) + (a[ 6] * b[13]) + (a[10] * b[14]) + (a[14] * b[15]);
    r->f[15] = (a[ 3] * b[12]) + (a[ 7] * b[13]) + (a[11] * b[14]) + (a[15] * b[15]);
    
    return *r;
}


void Matrix::Print(Matrix &matrix) {
    for(int i = 0; i < 4; i++) {
        hd::log::i("%f\t%f\t%f\t%f\n", matrix[i], matrix[i+4], matrix[i+8], matrix[i+12]);
    }
}


bool Matrix::Equals(const Matrix &a, const Matrix &b, const float precision) {
    if (precision == 0) {
        return a[ 0] == b[ 0] && a[ 4] == b[ 4] && a[ 8] == b[ 8] && a[12] == b[12]
            && a[ 1] == b[ 1] && a[ 5] == b[ 5] && a[ 9] == b[ 9] && a[13] == b[13]
            && a[ 2] == b[ 2] && a[ 6] == b[ 6] && a[10] == b[10] && a[14] == b[14]
            && a[ 3] == b[ 3] && a[ 7] == b[ 7] && a[11] == b[11] && a[15] == b[15];
    } else {
        for (int i = 0; i < 16; ++i) {
            if (fabs(a[i] - b[i]) > precision) {
                return false;
            }
        }
        return true;
    }
}


void Matrix::Set(Matrix &m, float *f) {
    m[ 0] = f[ 0];	m[ 4] = f[ 1];	m[ 8] = f[ 2];	m[12] = f[ 3];
	m[ 1] = f[ 4];	m[ 5] = f[ 5];	m[ 9] = f[ 6];	m[13] = f[ 7];
	m[ 2] = f[ 8];	m[ 6] = f[ 9];	m[10] = f[10];	m[14] = f[11];
	m[ 3] = f[12];	m[ 7] = f[13];	m[11] = f[14];	m[15] = f[15];
}


void Matrix::Translation(Matrix &m,
                         const float fx,
                         const float fy,
                         const float fz) {
	m[ 0] = 1;	m[ 4] = 0;	m[ 8] = 0;	m[12] = fx;
	m[ 1] = 0;	m[ 5] = 1;	m[ 9] = 0;	m[13] = fy;
	m[ 2] = 0;	m[ 6] = 0;	m[10] = 1;	m[14] = fz;
	m[ 3] = 0;	m[ 7] = 0;	m[11] = 0;	m[15] = 1;
}


void Matrix::Translation(Matrix &m, const Vec3 &vec) {
    Matrix::Translation(m, vec[0], vec[1], vec[2]);
}


void Matrix::Scale(Matrix &m,
                     const float fx,
                     const float fy,
                     const float fz) {
	m[ 0] = fx;	m[ 4] = 0;	m[ 8] = 0;	m[12] = 0;
	m[ 1] = 0;	m[ 5] = fy;	m[ 9] = 0;	m[13] = 0;
	m[ 2] = 0;	m[ 6] = 0;	m[10] = fz;	m[14] = 0;
	m[ 3] = 0;	m[ 7] = 0;	m[11] = 0;	m[15] = 1;
}


void Matrix::Scale(Matrix &m, const Vec3 &vec) {
    Matrix::Scale(m, vec[0], vec[1], vec[2]);
}


void Matrix::Transpose(Matrix &dst, const Matrix &src) {
    dst[ 0] = src[ 0];  dst[ 4] = src[ 1];  dst[ 8] = src[ 2];  dst[12] = src[ 3];
    dst[ 1] = src[ 4];  dst[ 5] = src[ 5];  dst[ 9] = src[ 6];  dst[13] = src[ 7];
    dst[ 2] = src[ 8];  dst[ 6] = src[ 9];  dst[10] = src[10];  dst[14] = src[11];
    dst[ 3] = src[12];  dst[ 7] = src[13];  dst[11] = src[14];  dst[15] = src[15];
}


// via http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/,
// converted to column major order.
void Matrix::FromQuaternion(Matrix &m, const Quaternion &q) {
    float x = q[0];
    float y = q[1];
    float z = q[2];
    float w = q[3];
    
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float xw = x * w;
    
    float yy = y * y;
    float yz = y * z;
    float yw = y * w;
    
    float zz = z * z;
    float zw = z * w;
    
    float ww = w * w;
    
#if 1
    m[0 ] = ww + xx - yy - zz;
    m[1 ] = 2.0f * (xy - zw);
    m[2 ] = 2.0f * (xz + yw);
    m[3 ] = 0;
    
    m[4 ] = 2.0f * (xy + zw);
    m[5 ] = ww - xx + yy - zz;
    m[6 ] = 2.0f * (yz - xw);
    m[7 ] = 0;
    
    m[8 ] = 2.0f * (xz - yw);
    m[9 ] = 2.0f * (yz + xw);
    m[10] = ww - xx - yy + zz;
    m[11] = 0;
    
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
    
#else
    m[0 ] = 1.0f - 2.0f * (yy + zz);
    m[1 ] = 2.0f * (xy + zw);
    m[2 ] = 2.0f * (xz - yw);
    m[3 ] = 0;
    
    m[4 ] = 2.0f * (xy - zw);
    m[5 ] = 1.0f - 2.0f * (xx + zz);
    m[6 ] = 2.0f * (yz + xw);
    m[7 ] = 0;
    
    m[8 ] = 2.0f * (xz - yw);
    m[9 ] = 2.0f * (yz - xw);
    m[10] = 1.0f - 2.0f * (xx + yy);
    m[11] = 0;
    
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
#endif
}

// Retained API placeholder; no implementation is provided.
void Matrix::ToQuaternion(Quaternion &q, const Matrix &matrix) {
}


void Matrix::RotationX(Matrix &m, const float fAngle) {
    float fsin = hd::sin(fAngle);
    float fcos = hd::cos(fAngle);
    
    m[ 0] = 1;	m[ 4] = 0;      m[ 8] = 0;      m[12] = 0;
	m[ 1] = 0;	m[ 5] = fcos;	m[ 9] = -fsin;	m[13] = 0;
	m[ 2] = 0;	m[ 6] = fsin;   m[10] = fcos;	m[14] = 0;
	m[ 3] = 0;	m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}


void Matrix::RotationY(Matrix &m, const float fAngle) {
    float fsin = hd::sin(fAngle);
    float fcos = hd::cos(fAngle);
    
    m[ 0] = fcos;	m[ 4] = 0;      m[ 8] = fsin;   m[12] = 0;
	m[ 1] = 0;      m[ 5] = 1;      m[ 9] = 0;      m[13] = 0;
	m[ 2] = -fsin;	m[ 6] = 0;      m[10] = fcos;	m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}


void Matrix::RotationZ(Matrix &m, const float fAngle) {
    float fsin = hd::sin(fAngle);
    float fcos = hd::cos(fAngle);
    
    m[ 0] = fcos;	m[ 4] = -fsin;  m[ 8] = 0;      m[12] = 0;
	m[ 1] = fsin;   m[ 5] = fcos;   m[ 9] = 0;      m[13] = 0;
	m[ 2] = 0;      m[ 6] = 0;      m[10] = 1;      m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}


void Matrix::RotationXYZOrigin(Matrix &mOut, const Vec3 &rotation) {
    Matrix rot, rx, ry, rz;
    Matrix::Identity(rot);
    Matrix::Identity(rx);
    Matrix::Identity(ry);
    Matrix::Identity(rz);
    
    if (rotation[0] != 0.0) {
        Matrix::RotationX(rx, rotation[0]);
    }
    if (rotation[1] != 0.0) {
        Matrix::RotationY(ry, rotation[1]);
    }
    if (rotation[2] != 0.0) {
        Matrix::RotationZ(rz, rotation[2]);
    }
    
    mOut = rx * ry * rz;
}


// Matrix to rotate a vector around arbitrary angles around an arbitrary axis
void Matrix::RotationXYZAxis(Matrix &m, const float angle, const Vec3& axis) {
    Matrix::Identity(m);
    
    float fsin = hd::sin(angle);
    float fcos = hd::cos(angle);
    
    m[0] = (axis[0] * axis[0]) * (1 - fcos) + fcos;
    m[1] = (axis[0] * axis[1]) * (1 - fcos) + (axis[2] * fsin);
    m[2] = (axis[0] * axis[2]) * (1 - fcos) - (axis[1] * fsin);
    
    m[3] = (axis[1] * axis[0]) * (1 - fcos) - (axis[2] * fsin);
    m[4] = (axis[1] * axis[1]) * (1 - fcos) + fcos;
    m[5] = (axis[1] * axis[2]) * (1 - fcos) + (axis[0] * fsin);
    
    m[6] = (axis[2] * axis[0]) * (1 - fcos) + (axis[1] * fsin);
    m[7] = (axis[2] * axis[1]) * (1 - fcos) - (axis[0] * fsin);
    m[8] = (axis[2] * axis[2]) * (1 - fcos) + fcos;
}


//
// Line is from origin.
//
void Matrix::RotationArbitraryLine(Matrix &m,
                                   const float delta,
                                   const Vec3 &p) {
    Matrix::Identity(m);
    
    float fsin = hd::sin(delta);
    float fcos = hd::cos(delta);
    
    float u = p[0];
    float v = p[1];
    float w = p[2];
    float L = (u * u) + (v * v) + (w * w);
    
    m[0] = ((u * u) + (v * v + w * w) * fcos) / L;
    m[1] = ((u * v) * (1 - fcos) + (w * sqrtf(L) * fsin)) / L;
    m[2] = ((u * w) * (1 - fcos) + (v * sqrtf(L) * fsin)) / L;
    m[3] = 0;
    
    m[4] = ((u * v) * (1 - fcos) + (w * sqrtf(L) * fsin)) / L;
    m[5] = ((v * v) + (u * u + w * w) * fcos) / L;
    m[6] = ((v * w) * (1 - fcos) + (u * sqrtf(L) * fsin)) / L;
    m[7] = 0;
    
    m[8] = ((u * w) * (1 - fcos) + (v * sqrtf(L) * fsin)) / L;
    m[9] = ((v * w) * (1 - fcos) + (u * sqrtf(L) * fsin)) / L;
    m[10] = ((w * w) + (u * u + v * v) * fcos) / L;
    m[11] = 0;
    
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}


void Matrix::Vec3Multiply(Vec3 &vOut,
                          const Vec3 &vIn,
                          const Matrix &m) {
    Vec3 res;
    
    // row vector * matrix
    res[0] = vIn[0] * m[0] + vIn[1] * m[4] + vIn[2] * m[8]  + m[12];
    res[1] = vIn[0] * m[1] + vIn[1] * m[5] + vIn[2] * m[9]  + m[13];
    res[2] = vIn[0] * m[2] + vIn[1] * m[6] + vIn[2] * m[10] + m[14];
    
    vOut = res;
}


void Matrix::Vec4Multiply(Vec4 &vOut, const Vec4 &v, const Matrix &m) {
    Vec4 res;
    
    res[0] = v[0] * m[ 0] + v[1] * m[ 4] + v[2] * m[ 8] + v[3] * m[12];
    res[1] = v[0] * m[ 1] + v[1] * m[ 5] + v[2] * m[ 9] + v[3] * m[13];
    res[2] = v[0] * m[ 2] + v[1] * m[ 6] + v[2] * m[10] + v[3] * m[14];
    res[3] = v[0] * m[ 3] + v[1] * m[ 7] + v[2] * m[11] + v[3] * m[15];
    
    vOut = res;
}


void Matrix::OrthoRH(Matrix &m, float w, float h, float n, float f) {
    Matrix::Identity(m);
    
    m[ 0] = 2.0f / w;       m[ 4] = 0;              m[ 8] = 0;              m[12] = 0;
	m[ 1] = 0;              m[ 5] = 2.0f / h;       m[ 9] = 0;              m[13] = 0;
	m[ 2] = 0;              m[ 6] = 0;              m[10] = 1.0f/ (n - f);  m[14] = n / (n -f);
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;              m[15] = 1;
}


void Matrix::OrthoOffCenterRH(Matrix &m, float l, float r, float b, float t, float n, float f) {
    Matrix::Identity(m);
    
    m[ 0] = 2.0f / (r - l); m[ 4] = 0;              m[ 8] = 0;              m[12] = (l + r) / (l - r);
	m[ 1] = 0;              m[ 5] = 2.0f / (t - b); m[ 9] = 0;              m[13] = (t + b) / (b - t);
	m[ 2] = 0;              m[ 6] = 0;              m[10] = 1.0f/ (n - f);  m[14] = n / (n - f);
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;              m[15] = 1;
}


void Matrix::Ortho(Matrix &m,
                  float l, float r,
                  float b, float t,
                  float n, float f) {
    Matrix::Identity(m);
    
#if 1
    m[ 0] = 2.0f / (r - l); m[ 4] = 0;              m[ 8] = 0;              m[12] = -(r + l) / (r - l);
	m[ 1] = 0;              m[ 5] = 2.0f / (t - b); m[ 9] = 0;              m[13] = -(t + b) / (t - b);
	m[ 2] = 0;              m[ 6] = 0;              m[10] = -1.0f/ (f - n); m[14] = -n / (f - n);
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;              m[15] = 1;
    
#else
    
#if 1
    m[ 0] = 2.0f / (r - l); m[ 4] = 0;              m[ 8] = 0;              m[12] = -1;
	m[ 1] = 0;              m[ 5] = 2.0f / (t - b); m[ 9] = 0;              m[13] = 1;
	m[ 2] = 0;              m[ 6] = 0;              m[10] = -2.0f/ (f - n); m[14] = (n + f) / (n - f);
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;              m[15] = 1;
    
#else
    m[ 0] = 2.0f / (r - l); m[ 4] = 0;              m[ 8] = 0;              m[12] = -1;
	m[ 1] = 0;              m[ 5] = 2.0f / (t - b); m[ 9] = 0;              m[13] = 1;
	m[ 2] = 0;              m[ 6] = 0;              m[10] = -2.0f/ (f - n); m[14] = 0;
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;              m[15] = 1;
#endif
#endif
}


void Matrix::LookAtRH(Matrix &m,
                      const Vec3 &pos,
                      const Vec3 &target,
                      const Vec3 &up) {
    Vec3 z_axis = hd::normalize(pos - target);
    Vec3 x_axis = hd::normalize(hd::cross(up, z_axis));
    Vec3 y_axis = hd::cross(z_axis, x_axis);

    m[ 0] = x_axis[0];   m[ 4] = x_axis[1];   m[ 8] = x_axis[2];   m[12] = -(x_axis * pos);
	m[ 1] = y_axis[0];   m[ 5] = y_axis[1];   m[ 9] = y_axis[2];   m[13] = -(y_axis * pos);
	m[ 2] = z_axis[0];   m[ 6] = z_axis[1];   m[10] = z_axis[2];   m[14] = -(z_axis * pos);
	m[ 3] = 0;           m[ 7] = 0;           m[11] = 0;           m[15] = 1;
}


void Matrix::PerspectiveFovRH(Matrix &m,
                              const float fov_y,
                              const float aspect,
                              const float _near,
                              const float _far) {
#if 1
    float f = 1.0f / tanf(fov_y * 0.5f);

    m[ 0] = f / aspect;     m[ 4] = 0;          m[ 8] = 0;                   m[12] = 0;
	m[ 1] = 0;              m[ 5] = f;          m[ 9] = 0;                   m[13] = 0;
	m[ 2] = 0;              m[ 6] = 0;          m[10] = (_far + _near) / (_near - _far);  m[14] = 2 * (_far * _near) / (_near - _far);
	m[ 3] = 0;              m[ 7] = 0;         m[11] = -1;                  m[15] = 0;
#else
    
	float f = tan(fov_y * 0.5f);
#if 0
    m[ 0] = 1.0f / (aspect * f); m[ 4] = 0;         m[ 8] = 0;                   m[12] = 0;
	m[ 1] = 0;                   m[ 5] = 1.0f / f;  m[ 9] = 0;                   m[13] = 0;
	m[ 2] = 0;                   m[ 6] = 0;         m[10] = -(_far + _near) / (_far - _near);  m[14] = -2 * (_far * _near) / (_far -_ near);
	m[ 3] = 0;                   m[ 7] = 0;         m[11] = -1;                  m[15] = 0;
#else
    m[ 0] = 1.0f / (aspect * f); m[ 4] = 0;         m[ 8] = 0;                   m[12] = 0;
	m[ 1] = 0;                   m[ 5] = 1.0f / f;  m[ 9] = 0;                   m[13] = 0;
	m[ 2] = 0;                   m[ 6] = 0;         m[10] = _far / (_near - _far);  m[14] = (_far * _near) / (_near - _far);
	m[ 3] = 0;                   m[ 7] = 0;         m[11] = -1;                  m[15] = 0;
#endif
#endif
}


//
// Implements 4x4 determinant, e.g.:
// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
//
float Matrix::Determinant(const Matrix &m) {
    return  m[ 0] * m[ 5] * m[10] * m[15]  +  m[ 0] * m[ 9] * m[14] * m[ 7]  +  m[ 0] * m[13] * m[ 6] * m[11] +
            m[ 4] * m[ 1] * m[14] * m[11]  +  m[ 4] * m[ 9] * m[ 2] * m[15]  +  m[ 4] * m[13] * m[10] * m[ 3] +
            m[ 8] * m[ 1] * m[ 6] * m[15]  +  m[ 8] * m[ 5] * m[14] * m[ 3]  +  m[ 8] * m[13] * m[ 3] * m[ 7] +
            m[12] * m[ 1] * m[10] * m[ 7]  +  m[12] * m[ 5] * m[ 2] * m[11]  +  m[12] * m[ 9] * m[ 6] * m[ 3] -
    
            m[ 0] * m[ 5] * m[14] * m[11]  -  m[ 0] * m[ 9] * m[ 6] * m[15]  -  m[ 0] * m[13] * m[10] * m[ 7] -
            m[ 4] * m[ 1] * m[10] * m[15]  -  m[ 4] * m[ 9] * m[14] * m[ 3]  -  m[ 4] * m[13] * m[ 2] * m[11] -
            m[ 8] * m[ 1] * m[14] * m[ 7]  -  m[ 8] * m[ 5] * m[ 2] * m[15]  -  m[ 8] * m[13] * m[ 6] * m[ 3] -
            m[12] * m[ 1] * m[ 6] * m[11]  -  m[12] * m[ 5] * m[10] * m[ 3]  -  m[12] * m[ 9] * m[ 2] * m[ 7];
}


//
// Implements standard 4x4 inverse, e.g.:
// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
//
bool Matrix::InverseLong(Matrix &m, const Matrix &a) {
    const unsigned _11 = 0;
    const unsigned _21 = 1;
    const unsigned _31 = 2;
    const unsigned _41 = 3;
    
    const unsigned _12 = 4;
    const unsigned _22 = 5;
    const unsigned _32 = 6;
    const unsigned _42 = 7;
    
    const unsigned _13 = 8;
    const unsigned _23 = 9;
    const unsigned _33 = 10;
    const unsigned _43 = 11;
    
    const unsigned _14 = 12;
    const unsigned _24 = 13;
    const unsigned _34 = 14;
    const unsigned _44 = 15;
    
    float determinant = Matrix::Determinant(a);
    if (determinant == 0) {
        hd::log::e("Singular matrix.\n");
        return false;
    }
    
    m[_11] = (a[_22]*a[_33]*a[_44] + a[_23]*a[_34]*a[_42] + a[_24]*a[_32]*a[_43] - a[_22]*a[_34]*a[_43] - a[_23]*a[_32]*a[_44] - a[_24]*a[_33]*a[_42]) / determinant;
    m[_12] = (a[_12]*a[_34]*a[_43] + a[_13]*a[_32]*a[_44] + a[_14]*a[_33]*a[_42] - a[_12]*a[_33]*a[_44] - a[_13]*a[_34]*a[_42] - a[_14]*a[_32]*a[_43]) / determinant;
    m[_13] = (a[_12]*a[_23]*a[_44] + a[_13]*a[_24]*a[_42] + a[_14]*a[_22]*a[_43] - a[_12]*a[_24]*a[_43] - a[_13]*a[_22]*a[_44] - a[_14]*a[_23]*a[_42]) / determinant;
    m[_14] = (a[_12]*a[_24]*a[_33] + a[_13]*a[_22]*a[_34] + a[_14]*a[_23]*a[_32] - a[_12]*a[_23]*a[_34] - a[_13]*a[_24]*a[_32] - a[_14]*a[_22]*a[_33]) / determinant;
    
    m[_21] = (a[_21]*a[_34]*a[_43] + a[_23]*a[_31]*a[_44] + a[_24]*a[_33]*a[_41] - a[_21]*a[_33]*a[_44] - a[_23]*a[_34]*a[_41] - a[_24]*a[_31]*a[_43]) / determinant;
    m[_22] = (a[_11]*a[_33]*a[_44] + a[_13]*a[_34]*a[_41] + a[_14]*a[_31]*a[_43] - a[_11]*a[_34]*a[_43] - a[_13]*a[_31]*a[_44] - a[_14]*a[_33]*a[_41]) / determinant;
    m[_23] = (a[_11]*a[_24]*a[_43] + a[_13]*a[_21]*a[_44] + a[_14]*a[_23]*a[_41] - a[_11]*a[_23]*a[_44] - a[_13]*a[_24]*a[_41] - a[_14]*a[_21]*a[_43]) / determinant;
    m[_24] = (a[_11]*a[_23]*a[_34] + a[_13]*a[_24]*a[_31] + a[_14]*a[_21]*a[_33] - a[_11]*a[_24]*a[_33] - a[_13]*a[_21]*a[_34] - a[_14]*a[_23]*a[_31]) / determinant;
    
    m[_31] = (a[_21]*a[_32]*a[_44] + a[_22]*a[_34]*a[_41] + a[_24]*a[_31]*a[_42] - a[_21]*a[_34]*a[_42] - a[_22]*a[_31]*a[_44] - a[_24]*a[_32]*a[_41]) / determinant;
    m[_32] = (a[_11]*a[_34]*a[_42] + a[_12]*a[_31]*a[_44] + a[_14]*a[_32]*a[_41] - a[_11]*a[_32]*a[_44] - a[_12]*a[_34]*a[_41] - a[_14]*a[_31]*a[_42]) / determinant;
    m[_33] = (a[_11]*a[_22]*a[_44] + a[_12]*a[_24]*a[_41] + a[_14]*a[_21]*a[_42] - a[_11]*a[_24]*a[_42] - a[_12]*a[_21]*a[_44] - a[_14]*a[_22]*a[_41]) / determinant;
    m[_34] = (a[_11]*a[_24]*a[_32] + a[_12]*a[_21]*a[_34] + a[_14]*a[_22]*a[_31] - a[_11]*a[_22]*a[_34] - a[_12]*a[_24]*a[_31] - a[_14]*a[_21]*a[_32]) / determinant;
    
    m[_41] = (a[_21]*a[_33]*a[_42] + a[_22]*a[_31]*a[_43] + a[_23]*a[_32]*a[_41] - a[_21]*a[_32]*a[_43] - a[_22]*a[_33]*a[_41] - a[_23]*a[_31]*a[_42]) / determinant;
    m[_42] = (a[_11]*a[_32]*a[_43] + a[_12]*a[_33]*a[_41] + a[_13]*a[_31]*a[_42] - a[_11]*a[_33]*a[_42] - a[_12]*a[_31]*a[_43] - a[_13]*a[_32]*a[_41]) / determinant;
    m[_43] = (a[_11]*a[_23]*a[_42] + a[_12]*a[_21]*a[_43] + a[_13]*a[_22]*a[_41] - a[_11]*a[_22]*a[_43] - a[_12]*a[_23]*a[_41] - a[_13]*a[_21]*a[_42]) / determinant;
    m[_44] = (a[_11]*a[_22]*a[_33] + a[_12]*a[_23]*a[_31] + a[_13]*a[_21]*a[_32] - a[_11]*a[_23]*a[_32] - a[_12]*a[_21]*a[_33] - a[_13]*a[_22]*a[_31]) / determinant;
    
    return true;
}


void Matrix::Inverse(Matrix   &mOut,
                     const Matrix &mIn) {
    Matrix mDummyMatrix;
    double  det_1;
    double  pos, neg, temp;
    
    /* Calculate the determinant of submatrix A and determine if the
     the matrix is singular as limited by the double precision
     floating-point data representation. */
    pos = neg = 0.0;
    temp =  mIn.f[ 0] * mIn.f[ 5] * mIn.f[10];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp =  mIn.f[ 4] * mIn.f[ 9] * mIn.f[ 2];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp =  mIn.f[ 8] * mIn.f[ 1] * mIn.f[ 6];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -mIn.f[ 8] * mIn.f[ 5] * mIn.f[ 2];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -mIn.f[ 4] * mIn.f[ 1] * mIn.f[10];
    if (temp >= 0.0) pos += temp; else neg += temp;
    temp = -mIn.f[ 0] * mIn.f[ 9] * mIn.f[ 6];
    if (temp >= 0.0) pos += temp; else neg += temp;
    det_1 = pos + neg;
    
    /* Is the submatrix A singular? */
    if ((det_1 == 0.0) || (hd::abs(det_1 / (pos - neg)) < 1.0e-15)) {
        /* Matrix M has no inverse */
        hd::log::e("Matrix has no inverse : singular matrix\n");
        return;
    } else {
        /* Calculate inverse(A) = adj(A) / det(A) */
        det_1 = 1.0 / det_1;
        mDummyMatrix.f[ 0] =   ( mIn.f[ 5] * mIn.f[10] - mIn.f[ 9] * mIn.f[ 6] ) * (float)det_1;
        mDummyMatrix.f[ 1] = - ( mIn.f[ 1] * mIn.f[10] - mIn.f[ 9] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 2] =   ( mIn.f[ 1] * mIn.f[ 6] - mIn.f[ 5] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 4] = - ( mIn.f[ 4] * mIn.f[10] - mIn.f[ 8] * mIn.f[ 6] ) * (float)det_1;
        mDummyMatrix.f[ 5] =   ( mIn.f[ 0] * mIn.f[10] - mIn.f[ 8] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 6] = - ( mIn.f[ 0] * mIn.f[ 6] - mIn.f[ 4] * mIn.f[ 2] ) * (float)det_1;
        mDummyMatrix.f[ 8] =   ( mIn.f[ 4] * mIn.f[ 9] - mIn.f[ 8] * mIn.f[ 5] ) * (float)det_1;
        mDummyMatrix.f[ 9] = - ( mIn.f[ 0] * mIn.f[ 9] - mIn.f[ 8] * mIn.f[ 1] ) * (float)det_1;
        mDummyMatrix.f[10] =   ( mIn.f[ 0] * mIn.f[ 5] - mIn.f[ 4] * mIn.f[ 1] ) * (float)det_1;
        
        /* Calculate -C * inverse(A) */
        mDummyMatrix.f[12] = - ( mIn.f[12] * mDummyMatrix.f[ 0] + mIn.f[13] * mDummyMatrix.f[ 4] + mIn.f[14] * mDummyMatrix.f[ 8] );
        mDummyMatrix.f[13] = - ( mIn.f[12] * mDummyMatrix.f[ 1] + mIn.f[13] * mDummyMatrix.f[ 5] + mIn.f[14] * mDummyMatrix.f[ 9] );
        mDummyMatrix.f[14] = - ( mIn.f[12] * mDummyMatrix.f[ 2] + mIn.f[13] * mDummyMatrix.f[ 6] + mIn.f[14] * mDummyMatrix.f[10] );
        
        /* Fill in last row */
        mDummyMatrix.f[ 3] = 0.0f;
        mDummyMatrix.f[ 7] = 0.0f;
        mDummyMatrix.f[11] = 0.0f;
        mDummyMatrix.f[15] = 1.0f;
    }
    
    /* Copy contents of dummy matrix in pfMatrix */
    mOut = mDummyMatrix;
}


void Matrix::Bias(Matrix &m, float bias) {
    m[ 0] = bias;   m[ 4] = 0;      m[ 8] = 0;      m[12] = bias;
	m[ 1] = 0;      m[ 5] = bias;   m[ 9] = 0;      m[13] = bias;
	m[ 2] = 0;      m[ 6] = 0;      m[10] = bias;	m[14] = bias;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}
