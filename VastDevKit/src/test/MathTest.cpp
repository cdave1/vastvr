#include "MathTest.h"

#include <Vast/VastMath.h>
#include <assert.h>



void hd::test::math::TestEquality() {
    Matrix i1, i2;
    Matrix::Identity(i1);
    Matrix::Identity(i2);
    assert(Matrix::Equals(i1, i2));
}


//
// Determinant formula:
// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
//
float hd::test::math::GetTestDeterminant(const Matrix &a) {
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
    
    float result = 0;
    result =a[_11] * a[_22] * a[_33] * a[_44]  +  a[_11] * a[_23] * a[_34] * a[_42]  +  a[_11] * a[_24] * a[_32] * a[_43] +
            a[_12] * a[_21] * a[_34] * a[_43]  +  a[_12] * a[_23] * a[_31] * a[_44]  +  a[_11] * a[_24] * a[_33] * a[_41] +
            a[_13] * a[_21] * a[_32] * a[_44]  +  a[_13] * a[_22] * a[_34] * a[_41]  +  a[_13] * a[_24] * a[_31] * a[_42] +
            a[_14] * a[_21] * a[_33] * a[_42]  +  a[_14] * a[_22] * a[_31] * a[_43]  +  a[_14] * a[_23] * a[_32] * a[_41] -
    
            a[_11] * a[_22] * a[_34] * a[_43]  -  a[_11] * a[_23] * a[_32] * a[_44]  -  a[_11] * a[_24] * a[_33] * a[_42] -
            a[_12] * a[_21] * a[_33] * a[_44]  -  a[_12] * a[_23] * a[_34] * a[_41]  -  a[_12] * a[_24] * a[_31] * a[_43] -
            a[_13] * a[_21] * a[_34] * a[_42]  -  a[_13] * a[_22] * a[_31] * a[_44]  -  a[_13] * a[_24] * a[_32] * a[_41] -
            a[_14] * a[_21] * a[_32] * a[_43]  -  a[_14] * a[_22] * a[_33] * a[_41]  -  a[_14] * a[_23] * a[_31] * a[_42];
    return result;
}


void hd::test::math::TestMatrixDeterminant() {
    Matrix a, b, c, d;
    Matrix::Identity(a);
    float d1 = hd::test::math::GetTestDeterminant(a);
    float d2 = Matrix::Determinant(a);
    assert(d1 == d2);
    
    Matrix::Identity(d);
    d[ 0] =  0.671;      d[ 4] = 0.001;      d[ 8] = 0.000;      d[12] = -343.289;
    d[ 1] = -0.002;      d[ 5] = 0.985;      d[ 9] = -0.175;     d[13] = -27.509;
    d[ 2] =  0.000;      d[ 6] = -0.175;     d[10] = -0.985;     d[14] = 409.445;
    d[ 3] =  0.000;      d[ 7] = -0.175;     d[11] = -0.985;     d[15] = 411.444;
    
    d1 = hd::test::math::GetTestDeterminant(d);
    d2 = Matrix::Determinant(d);
    assert(d1 == d2);
}


void hd::test::math::TestInverse() {
    Matrix identity, result;
    Matrix::Identity(identity);
    Matrix::InverseLong(result, identity);
    assert(Matrix::Equals(result, identity));
    
    Matrix d;
    Matrix::Identity(d);
    d[ 0] =  0.671;      d[ 4] = 0.001;      d[ 8] = 0.000;      d[12] = -343.289;
    d[ 1] = -0.002;      d[ 5] = 0.985;      d[ 9] = -0.175;     d[13] = -27.509;
    d[ 2] =  0.000;      d[ 6] = -0.175;     d[10] = -0.985;     d[14] = 409.445;
    d[ 3] =  0.000;      d[ 7] = -0.175;     d[11] = -0.985;     d[15] = 411.444;
    Matrix::InverseLong(result, d);
    
    // Expected result
    Matrix e;
    e[ 0] =  1.490;      e[ 4] = -0.001;     e[ 8] = -255.856;  e[12] = 255.856;
    e[ 1] =  0.003;      e[ 5] = 0.984;      e[ 9] = -50.036;   e[13] = 49.861;
    e[ 2] = -0.001;      e[ 6] = -0.175;     e[10] = -200.069;  e[14] = 199.084;
    e[ 3] = -0.000;      e[ 7] = -0.000;     e[11] = -0.500;    e[15] = 0.500;
    assert(Matrix::Equals(result, e, 0.001f));
}


void hd::test::math::TestViewProjectionInverse() {
    Matrix view;
    Matrix projection;
    Matrix viewProjectionMatrix;
    
    Matrix modelViewMatrix;
    
    Vec3 eye(0, 1, 0);
    Vec3 at(1, 0, 0);
    Vec3 up(0, 1, 0);
    
    Matrix::LookAtRH(view, eye, at, up);
    Matrix::PerspectiveFovRH(projection, hd_half_pi, 1.0f, 1.0f, 100000.0f);
    
    viewProjectionMatrix = projection * view;
    
    Vec3 modelPos(0, 0, 0);
    
    Matrix worldSpaceMatrix;
    Matrix::Translation(worldSpaceMatrix, 5.0f, 0.0f, 1.0f);
    
    Vec3 positionWS;
    Matrix::Vec3Multiply(positionWS, modelPos, worldSpaceMatrix);
    
    float distance = positionWS.Length();
    
    //hd::log::i("The length is: %3.3f\n", distance);
    
    Matrix inverseViewProjectionMatrix;
    Matrix::Inverse(inverseViewProjectionMatrix, viewProjectionMatrix);
    
    Vec3 _positionWS;
    Matrix::Vec3Multiply(_positionWS, Vec3(0, 0, 0), inverseViewProjectionMatrix);
    
    Vec3 iViewRay = _positionWS - eye;
    Vec3 viewRay = normalize(iViewRay);
    
    float viewDistance = distance;
    
    Vec3 rebuiltPositionWS = eye + (viewDistance * viewRay);
    
    
    //hd::log::i("The length is: %3.3f, %3.3f, %3.3f\n", rebuiltPositionWS[0], rebuiltPositionWS[1], rebuiltPositionWS[2]);
    
    
}


//
// Test matrix multiplication.  Our matrices are in COLUMN-MAJOR order.  The index layout is
// as follows
//
//  0   4   8   12
//  1   5   9   13
//  2   6  10   14
//  3   7  11   15
//
//
// Test Identity * Identity == Identity
//
void hd::test::math::TestMatrixMultiplication1() {
    Matrix a, b, r;
    
    Matrix::Identity(a);
    Matrix::Identity(b);
    
    r = a * b;
    
    assert(r[ 0] == 1);
    assert(r[ 1] == 0);
    assert(r[ 2] == 0);
    assert(r[ 3] == 0);
    
    assert(r[ 4] == 0);
    assert(r[ 5] == 1);
    assert(r[ 6] == 0);
    assert(r[ 7] == 0);
    
    assert(r[ 8] == 0);
    assert(r[ 9] == 0);
    assert(r[10] == 1);
    assert(r[11] == 0);
    
    assert(r[12] == 0);
    assert(r[13] == 0);
    assert(r[14] == 0);
    assert(r[15] == 1);
}


//
// Setup some values and then verify that the multiplication function produces the expected
// result matrix.
//
void hd::test::math::TestMatrixMultiplication2() {
    Matrix a, b, r;
    
    float aa[16];
    float bb[16];
    
    Matrix::Identity(a);
    Matrix::Identity(b);
    
    for (int i = 0; i < 16; ++i) {
        a.f[i] = aa[i] = -5 + i;
        b.f[i] = bb[i] = -10 + (i * 2);
    }
    
    r = a * b;
    
    assert(r[ 0] == (aa[ 0] * bb[ 0]) + (aa[ 4] * bb[ 1]) + (aa[ 8] * bb[ 2]) + (aa[12] * bb[ 3]));
    assert(r[ 1] == (aa[ 1] * bb[ 0]) + (aa[ 5] * bb[ 1]) + (aa[ 9] * bb[ 2]) + (aa[13] * bb[ 3]));
    assert(r[ 2] == (aa[ 2] * bb[ 0]) + (aa[ 6] * bb[ 1]) + (aa[10] * bb[ 2]) + (aa[14] * bb[ 3]));
    assert(r[ 3] == (aa[ 3] * bb[ 0]) + (aa[ 7] * bb[ 1]) + (aa[11] * bb[ 2]) + (aa[15] * bb[ 3]));
    
    assert(r[ 4] == (aa[ 0] * bb[ 4]) + (aa[ 4] * bb[ 5]) + (aa[ 8] * bb[ 6]) + (aa[12] * bb[ 7]));
    assert(r[ 5] == (aa[ 1] * bb[ 4]) + (aa[ 5] * bb[ 5]) + (aa[ 9] * bb[ 6]) + (aa[13] * bb[ 7]));
    assert(r[ 6] == (aa[ 2] * bb[ 4]) + (aa[ 6] * bb[ 5]) + (aa[10] * bb[ 6]) + (aa[14] * bb[ 7]));
    assert(r[ 7] == (aa[ 3] * bb[ 4]) + (aa[ 7] * bb[ 5]) + (aa[11] * bb[ 6]) + (aa[15] * bb[ 7]));
    
    assert(r[ 8] == (aa[ 0] * bb[ 8]) + (aa[ 4] * bb[ 9]) + (aa[ 8] * bb[10]) + (aa[12] * bb[11]));
    assert(r[ 9] == (aa[ 1] * bb[ 8]) + (aa[ 5] * bb[ 9]) + (aa[ 9] * bb[10]) + (aa[13] * bb[11]));
    assert(r[10] == (aa[ 2] * bb[ 8]) + (aa[ 6] * bb[ 9]) + (aa[10] * bb[10]) + (aa[14] * bb[11]));
    assert(r[11] == (aa[ 3] * bb[ 8]) + (aa[ 7] * bb[ 9]) + (aa[11] * bb[10]) + (aa[15] * bb[11]));
    
    assert(r[12] == (aa[ 0] * bb[12]) + (aa[ 4] * bb[13]) + (aa[ 8] * bb[14]) + (aa[12] * bb[15]));
    assert(r[13] == (aa[ 1] * bb[12]) + (aa[ 5] * bb[13]) + (aa[ 9] * bb[14]) + (aa[13] * bb[15]));
    assert(r[14] == (aa[ 2] * bb[12]) + (aa[ 6] * bb[13]) + (aa[10] * bb[14]) + (aa[14] * bb[15]));
    assert(r[15] == (aa[ 3] * bb[12]) + (aa[ 7] * bb[13]) + (aa[11] * bb[14]) + (aa[15] * bb[15]));
}


void hd::test::math::TestQuaternions() {
    Quaternion q1(Vec3(0, 1, 0), hd_half_pi);
    Quaternion q2(Axis_Y, hd_half_pi, Rotate_CCW);
    assert(q1[0] == q2[0]);
    assert(q1[1] == q2[1]);
    assert(q1[2] == q2[2]);
    assert(q1[3] == q2[3]);
    Matrix m;
    Matrix::RotationY(m, -hd_half_pi);
    Matrix qm;
    Matrix::FromQuaternion(qm, q2);
    assert(Matrix::Equals(m, qm, 0.0005));

    // Camera lookat
    Quaternion q3;
    Matrix lookAtMatrix;
    Matrix resultMatrix;

    Matrix::Identity(lookAtMatrix);
    Matrix::Identity(resultMatrix);

    Vec3 eye = Vec3(0, 0, 0);
    Vec3 target = Vec3(0, 0, 1);
    Vec3 upAxis = Vec3(0, 1, 0);
    Vec3 location = Vec3(0, 0, 0);

    Vec3 rotation = Vec3(0, 0, 0);
    Quaternion rotationQuat;
    rotationQuat.SetIdentity();

    Matrix::LookAtRH(lookAtMatrix, location + eye, location + target, upAxis);

    Matrix rotm;
    Matrix::RotationXYZOrigin(rotm, Vec3(hd_half_pi, hd_half_pi, hd_half_pi));

    {
        Matrix rot_1, rot_2;

        Quaternion qx(Axis_X, hd_half_pi, Rotate_CW);
        Quaternion qy(Axis_Y, hd_half_pi, Rotate_CW);
        Quaternion qz(Axis_Z, hd_half_pi, Rotate_CW);

        Quaternion qw = qx * qy * qz;
        Matrix::FromQuaternion(rot_1, qw);

        Quaternion qq1(Vec3(1, 0, 0), -hd_half_pi);
        Quaternion qq2(Vec3(0, 1, 0), -hd_half_pi);
        Quaternion qq3(Vec3(0, 0, 1), -hd_half_pi);

        Quaternion qqr = qq1 * qq2 * qq3;
        Matrix::FromQuaternion(rot_2, qqr);

        hdAssert(Matrix::Equals(rot_1, rot_2, 0.0005));
    }

    Matrix m1, m2, m3, mm;
    Matrix::RotationY(m1, 0.2 * hd_half_pi);
    Matrix::RotationY(m2, 0.2 * hd_half_pi);
    Matrix::RotationY(m3, 0.4 * hd_half_pi);

    mm = m1 * m2;

    hdAssert(Matrix::Equals(mm, m3, 0.0005));

    // Test quaternion rotation operator
    Quaternion qrot(Vec3(1, 0, 0), 0.5 * hd_half_pi);
    Matrix mrot, mqrot;
    Matrix::RotationX(mrot, 0.5 * hd_half_pi);
    Matrix::FromQuaternion(mqrot, qrot);

    Vec3 victim(0, 1, 1);
    Vec3 r1, r2, r3;

    Matrix::Vec3Multiply(r1, victim, mrot);
    Matrix::Vec3Multiply(r2, victim, mqrot);
    r3 = qrot * victim;

    float theta = hd_2_pi / 64.0f;
    Quaternion qtheta(Vec3(0, 1, 0), theta);
    Quaternion curr(0, 0, 0, 1);
    Vec3 start(2, 0, 2);
    Vec3 value = start;
    for (int i = 0; i < 128; ++i) {
        value = qtheta * value;
        curr = curr * qtheta;
        Vec3 next = curr * start;
        const float EPSILON = 0.00001f;
        hdAssert(value[0] - next[0] < EPSILON);
        hdAssert(value[1] - next[1] < EPSILON);
        hdAssert(value[2] - next[2] < EPSILON);
    }

    hd::log::i("Done\n");
}


void hd::test::math::TestQuaternions2() {
    Vec3 locationQuat(0.0f, 0.0f, 1100.0f);
    Vec3 locationMatrix(0.0f, 0.0f, 1100.0f);

    const float ROTATION_START = 0.2f;

    Quaternion rotationAdjustment(Vec3(0, 1, 0), - (hd_half_pi / 2));
    Quaternion rotationQuat(Vec3(0, 1, 0), ROTATION_START);
    rotationQuat = rotationQuat * rotationAdjustment;

    Quaternion stepQuat(Vec3(0, 1, 0), (hd_half_pi / 4));

    locationQuat = rotationQuat * locationQuat;

    const unsigned int MAX_ITERATIONS = 12;
    const float RADIUS = 10.0f;
    const float EPSILON = 0.001f;

    Matrix stepMatrix;
    Matrix::RotationY(stepMatrix, (hd_half_pi / 4));

    Matrix rot;
    float rotationAngle = ROTATION_START - (hd_half_pi / 2);

    Matrix::RotationY(rot, ROTATION_START - (hd_half_pi / 2));
    Matrix::Vec3Multiply(locationMatrix, locationMatrix, rot);

    hdAssert(fabs(locationMatrix[0] - locationQuat[0]) < EPSILON);
    hdAssert(fabs(locationMatrix[1] - locationQuat[1]) < EPSILON);
    hdAssert(fabs(locationMatrix[2] - locationQuat[2]) < EPSILON);

    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        Vec3 adjustment = Vec3(0.0f, ((i % 2) * RADIUS), 0.0f);

        Vec3 originQuat = locationQuat + adjustment;

        Vec3 originMatrix = locationMatrix + adjustment;

        hdAssert(fabs(originMatrix[0] - originQuat[0]) < EPSILON);
        hdAssert(fabs(originMatrix[1] - originQuat[1]) < EPSILON);
        hdAssert(fabs(originMatrix[2] - originQuat[2]) < EPSILON);

        locationQuat = stepQuat * locationQuat;
        rotationQuat = rotationQuat * stepQuat;

        Matrix::Vec3Multiply(locationMatrix, locationMatrix, stepMatrix);
        rotationAngle += (hd_half_pi / 4);
        Quaternion rotQuat(Vec3(0, 1, 0), rotationAngle);

        Matrix rq1, rq2;
        Matrix::FromQuaternion(rq1, rotQuat);
        Matrix::FromQuaternion(rq2, rotationQuat);
        hdAssert(Matrix::Equals(rq1, rq2, 0.001f));
    }
}


void hd::test::math::TestQuaternions3() {
    hd::log::i("Testing quaternion length and normalization functions\n");
    Quaternion q1(0.0f, 0.0f, 0.0f, 0.0f);
    hdAssert(q1.Length() == 0.0f);

    Quaternion q2(1.0f, 0.0f, 0.0f, 1.0f);
    float q2Length = q2.Length();
    float q2LengthExpected = sqrt(2.0f);
    hdAssert(q2Length == q2LengthExpected);

    q2.Set(3.0f, 4.0f, 5.0f, 6.0f);
    q2Length = q2.Length();
    q2LengthExpected = sqrt(9.0f + 16.0f + 25.0f + 36.0f);
    hdAssert(q2LengthExpected == q2Length);
/*
    Quaternion q3;
    q3[3] = 0.0f;
    hdAssert(q3.Length() == 0);
    q3.Normalize();
    hdAssert(q3 == Quaternion());

    Quaternion q4(1.0f, 1.0f, 1.0f, 1.0f);
    q4.Normalize();
    hdAssert(q4 == Quaternion(0.5f, 0.5f, 0.5f, 0.5f));
*/
    hd::log::i("Done %s\n", __FUNCTION__);
}


void hd::test::math::TestClipping() {
    Vec3 inside(0, 0, 50);
    Vec3 outside(0, 0, -100);

    Vec3 eye(0, 0, 0);
    Vec3 at(0, 0, 100);
    Vec3 up(0, 1, 0);

    Matrix lookat;
    Matrix perspective;
    Matrix projection;
    Matrix::LookAtRH(lookat, eye, at, up);
    Matrix::PerspectiveFovRH(perspective, 90.0f*(hd_pi/180.0f), 1.0f, 1.0f, 1000.0f);
    projection = perspective * lookat;

    Vec3 insideResult, outsideResult;
    Matrix::Vec3Multiply(insideResult, inside, projection);
    Matrix::Vec3Multiply(outsideResult, outside, projection);

    hd::log::i("Finished\n");
}


void hd::test::math::TestFrustum() {
    Vec3 inside(0, 0, 50);
    Vec3 outside(0, 0, -100);

    Vec3 null(0, 0, 0);
    Vec3 one(0, 0, 1);
    Vec3 front(5, 5, 999);
    Vec3 b(10000, 10000, 999);

    Vec3 eye(0, 0, 0);
    Vec3 at(0, 0, 100);
    Vec3 up(0, 1, 0);

    Matrix lookat;
    Matrix perspective;
    Matrix projection;
    Matrix::LookAtRH(lookat, eye, at, up);
    Matrix::PerspectiveFovRH(perspective, 90.0f*(hd_pi/180.0f), 1.0f, 1.0f, 1000.0f);
    projection = perspective * lookat;

    Frustum frustum(projection);

    hdAssert(frustum.Contains(inside, 0.0f));
    hdAssert(!frustum.Contains(null, 0.0f));
    hdAssert(frustum.Contains(one, 0.0f));
    hdAssert(frustum.Contains(front, 0.0f));
    hdAssert(!frustum.Contains(b, 0.0f));
    hdAssert(!frustum.Contains(outside, 0.0f));

    hd::log::i("TestFrustum\n");
}
