/*
 Partially borrowed from the Oolong header file, Matrix.h
 */


#ifndef _HD_MATRIX_H_
#define _HD_MATRIX_H_

struct Vec2;
struct Vec3;


// Column major order
//
// A little bit concerned that the oolong one is 
// in row major order.
#define __11 0
#define __21 1
#define __31 2
#define __41 3
#define __12 4
#define __22 5
#define __32 6
#define __42 7
#define __13 8
#define __23 9
#define __33 10
#define __43 11
#define __14 12
#define __24 13
#define __34 14
#define __44 15

// Matrix contains floats
class Matrix
{
public:
	float* operator [] ( const int Row )
	{
		return &f[Row<<2];
	}
	float f[16];
};


//
// Reset matrix to identity
// outputs the identity matrix
//
void MatrixIdentity(Matrix &mOut);

//
// Multiply mA by mB and assign the result to mOut
// (mOut = p1 * p2). A copy of the result matrix is done in
// the function because mOut can be a parameter mA or mB.
//
void MatrixMultiply(Matrix		&mOut,
                      const Matrix	&mA,
                      const Matrix	&mB);


void PrintMatrix(Matrix &matrix);


//
// Build a translation matrix mOut using fX, fY and fZ.
//
void MatrixTranslation(Matrix	&mOut,
					   const float	fX,
					   const float	fY,
					   const float	fZ);


//
// Build a scale matrix mOut using fX, fY and fZ.
//
void MatrixScaling(Matrix	&mOut,
				   const float fX,
				   const float fY,
				   const float fZ);



//
// Create an X rotation matrix mOut.
//
void MatrixRotationX(Matrix	&mOut,
					 const float fAngle);


//
// Create an Y rotation matrix mOut.
//
void MatrixRotationY(Matrix	&mOut,
					 const float fAngle);

//
// Create an Z rotation matrix mOut.
//
void MatrixRotationZ(Matrix	&mOut,
					 const float fAngle);


void MatrixRotationXYZOrigin(Matrix &mOut, 
                             const Vec3 &rotation);

// Matrix to rotate a vector an angle around an arbitrary axis
void MatrixRotationXYZAxis(Matrix &mOut, 
                           const float fAngle, 
                           const Vec3& axis);


void MatrixVec3Multiply(Vec3		&vOut,
						const Vec3	&vIn,
						const Matrix	&mIn);


void MatrixVec4Multiply(Vec3 &vOut, 
                        const Vec3 &vIn, 
                        const Matrix &mIn);


void MatrixLookAtRH(Matrix &matrix,
                    const Vec3 &eye,
                    const Vec3 &at,
                    const Vec3 &up);


void MatrixGluLookAt(Matrix &matrix,
                     const Vec3 &eye,
                     const Vec3 &at,
                     const Vec3 &up);



void MatrixPerspectiveFovRH(Matrix	&mOut,
							const float	fFOVy,
							const float	fAspect,
							const float	fNear,
							const float	fFar,
							const bool  bRotate = false);


void MatrixInverse(Matrix			&mOut,
				   const Matrix	&mIn);



void aglMatrixPerspectiveFovRH(
                               float *mOut,
                               const float fFOVy,
                               const float fAspect,
                               const float fNear,
                               const float fFar);


#endif