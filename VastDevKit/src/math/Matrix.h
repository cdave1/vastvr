#ifndef _VAST_MATRIX_H_
#define _VAST_MATRIX_H_

struct Vec2;
struct Vec3;
struct Vec4;
struct Quaternion;

//
// Our matrices are in COLUMN-MAJOR order.  The index layout is as follows
//
//  0   4   8   12
//  1   5   9   13
//  2   6  10   14
//  3   7  11   15
//
class Matrix {
public:
    float f[16];
    
    Matrix();
    
    ~Matrix();
    
    float& operator[](unsigned int index);
    
    const float& operator[](unsigned int index) const;
    
    Matrix operator * (const Matrix& b) const {
        Matrix result;
        Multiply(&result, * this, b);
        return result;
    }
    
    //
    // Set identity matrix.
    //
    static void Identity(Matrix &m);

    static Matrix Identity();
    
    //
    // Multiply mA by mB and assign the result to mOut
    // (mOut = p1 * p2). A copy of the result matrix is done in
    // the function because mOut can be a parameter mA or mB.
    //
    static Matrix& Multiply(Matrix *res,
                         const Matrix &a,
                         const Matrix &b);
    
    
    static void Print(Matrix &m);
    
    
    static bool Equals(const Matrix &a, const Matrix &b, const float precision=0);
    
    
    static void Set(Matrix &m, float *f);
    
    
    //
    // Build a translation matrix mOut using fX, fY and fZ.
    //
    static void Translation(Matrix &m,
                            const float fX,
                            const float fY,
                            const float fZ);
    
    
    static void Translation(Matrix &m, const Vec3 &vec);
    
    
    //
    // Build a scale matrix mOut using fX, fY and fZ.
    //
    static void Scale(Matrix &m,
                      const float fX,
                      const float fY,
                      const float fZ);
    
    
    static void Scale(Matrix &m, const Vec3 &vec);
    
    
    static void Transpose(Matrix &m, const Matrix &matrix);
    
    
    static void FromQuaternion(Matrix &m, const Quaternion &q);


    static void ToQuaternion(Quaternion &q, const Matrix &matrix);
    
    
    //
    // Create an X rotation matrix mOut.
    //
    static void RotationX(Matrix &m,
                          const float fAngle);
    
    
    //
    // Create an Y rotation matrix mOut.
    //
    static void RotationY(Matrix &m,
                          const float fAngle);
    
    //
    // Create an Z rotation matrix mOut.
    //
    static void RotationZ(Matrix &m, const float fAngle);
    
    
    static void RotationXYZOrigin(Matrix &m, const Vec3 &rotation);
    
    // Matrix to rotate a vector an angle around an arbitrary axis
    static void RotationXYZAxis(Matrix &m,
                                const float fAngle,
                                const Vec3& axis);
    
    
    static void RotationArbitraryLine(Matrix &m,
                                      const float delta,
                                      const Vec3 &p);
    
    
    static void Vec3Multiply(Vec3  &vOut,
                             const Vec3 &vIn,
                             const Matrix &mIn);
    
    
    static void Vec4Multiply(Vec4 &vOut,
                             const Vec4 &vIn,
                             const Matrix &mIn);
    
    
    static void OrthoRH(Matrix &m, float w, float h, float n, float f);
    
    
    // See: http://msdn.microsoft.com/en-us/library/windows/desktop/bb205348(v=vs.85).aspx
    static void OrthoOffCenterRH(Matrix &m, float l, float r, float b, float t, float n, float f);
    
    
    static void Ortho(Matrix &m, float l, float r, float b, float t, float n, float f);
    
    
    static void LookAtRH(Matrix &m,
                         const Vec3 &eye,
                         const Vec3 &at,
                         const Vec3 &up);
    
    
    static void PerspectiveFovRH(Matrix &m,
                                 const float fFOVy,
                                 const float fAspect,
                                 const float fNear,
                                 const float fFar);
    
    
    static float Determinant(const Matrix &m);
    
    
    static bool InverseLong(Matrix &m, const Matrix &a);
    
    
    static void Inverse(Matrix   &mOut,
                        const Matrix &mIn);
    
    static void Bias(Matrix &mOut, float bias);
    
};

#endif
