#include "Testing.h"

#include "MathTest.h"
#include "RayTest.h"
#include "VectorTest.h"
#include "ComponentTest.h"
#include "UITest.h"

void hd::Testing::TestAll() {
    hd::test::math::TestEquality();
    hd::test::math::TestMatrixMultiplication1();
    hd::test::math::TestMatrixMultiplication2();
    hd::test::math::TestMatrixDeterminant();
    hd::test::math::TestInverse();
    hd::test::math::TestViewProjectionInverse();
    hd::test::math::TestQuaternions();
    hd::test::math::TestQuaternions2();
    hd::test::math::TestQuaternions3();
    hd::test::math::TestClipping();
    hd::test::math::TestFrustum();

    hd::test::rays::TestRays();
    hd::test::rays::TestSphereTest();
    hd::test::rays::TestCollisionVolume();

    hd::test::vectors::TestVector();

    hd::test::ui::TestSphereLayout();

    hd::test::components::TestComponentSpeed();
}