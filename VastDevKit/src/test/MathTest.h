#ifndef _VAST_TEST_MATH_H_
#define _VAST_TEST_MATH_H_

#include <Vast/VastMath.h>

namespace hd {
    namespace test {
        class math {
        public:
            
            static void TestEquality();
            
            static void TestMatrixDeterminant();
            
            static void TestInverse();
            
            static void TestViewProjectionInverse();
            
            static void TestMatrixMultiplication1();
            
            static void TestMatrixMultiplication2();
            
            static void TestQuaternions();

            static void TestQuaternions2();

            static void TestQuaternions3();

            static void TestClipping();

            static void TestFrustum();
            
        private:
            
            static float GetTestDeterminant(const Matrix &a);
            
        };
    }
}


#endif