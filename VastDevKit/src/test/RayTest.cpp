#include "RayTest.h"

#include <Vast/Vast.h>
#include <assert.h>
#include <float.h>

void hd::test::rays::TestRays() {
    Vec3 miss[3];
    Vec3 hit[3];

    miss[0] = Vec3(0, 1, 0);
    miss[1] = Vec3(0, 1, 2);
    miss[2] = Vec3(0, 2, 0);

    hit[0] = Vec3(2, -1, 0);
    hit[1] = Vec3(2, -1, 2);
    hit[2] = Vec3(2, 2, 0);

    Vec3 origin(0, 0, 0);
    Vec3 direction(1.5, 0.0, 1);
    direction = hd::normalize(direction);

    float t = FLT_MAX;

    bool missResult;
    bool hitResult;

    missResult = hd::graphics::RayHitTester::RayHitsTriangle(miss, origin, direction, &t);
    hdAssert(missResult == false);

    hitResult  = hd::graphics::RayHitTester::RayHitsTriangle(hit, origin, direction, &t);
    direction = t * direction;
    hdAssert(hitResult == true);
}


void hd::test::rays::TestSphereTest() {
    Vec3 missOrigin(0, 0, 0);
    Vec3 missDirection(-1, -1, -1);

    Vec3 hitOrigin(10, 10, 10);
    Vec3 hitDirection(1, 1, 1);

    Vec3 origin(50, 50, 50);
    float radius = 4;

    Vec3 result;

    bool missResult = hd::RayIntersectsSphere(missOrigin, missDirection, origin, radius, &result);
    hdAssert(missResult == false);

    bool hitResult = hd::RayIntersectsSphere(hitOrigin, hitDirection, origin, radius, &result);
    hdAssert(hitResult == true);

/*
 Checking pointer ray at: 58976.570, -348.014, 3647.772
 TestRayMeshData: (-3190.166, 440.000, -1314.952) -> (0.997, -0.013, 0.080)
 Ray hit test for sphere: (0.000, 400.000, -1024.000), radius: 350.00
 Ray hit test for sphere: (0.000, 400.000, -1024.000), radius: 350.00
 Checking pointer ray at: 58976.570, -348.014, 3647.772
 */

    {
        Vec3 o(-3190.166, 440.000, -1314.952);
        Vec3 d(0.997, -0.013, 0.080);

        Vec3 sp(0.000, 400.000, -1024.000);
        radius = 350.0f;

        bool intersectionResult = hd::RayIntersectsSphere(o, d, sp, radius, &result);
        hdAssert(intersectionResult == true);
    }

    {
        /*
        Checking pointer ray at: -31619.879, 239.116, 56847.516
    TestRayMeshData: (880.000, 400.000, -2960.000) -> (-0.477, -0.002, 0.879)
        Ray hit test for sphere: (0.000, 400.000, -1024.000), radius: 350.00
            Ray hit test for sphere: (0.000, 400.000, -1024.000), radius: 350.00
                Checking pointer ray at: -31619.879, 239.116, 56847.516
         */

        Vec3 o(880.000f, 400.000f, -2960.000f);
        Vec3 d = Vec3(-31619.879f, 239.116f, 56847.516f) - o;

        Vec3 sp(0.000f, 400.000f, -1024.000f);
        radius = 350.0f;

        bool intersectionResult = hd::RayIntersectsSphere(o, d, sp, radius, &result);
        hdAssert(intersectionResult == true);
    }
}


void hd::test::rays::TestCollisionVolume() {
    auto components = new ComponentPool<hd::graphics::CollisionVolume>(2);
    auto collisionVolume = components->Get();

    AABB aabb(Vec3(0, 0, 0), Vec3(500, 0, 500));
    Vec3 nodeSize(50, 0, 50);

    Collision3 bad, edge1, mid1, mid2, outer1, outer2, y1, y2;
    bad.origin = Vec3(-1.0f, 0.0f, 0.0f);
    edge1.origin = Vec3(0.0f, 0.0f, 0.0f);
    mid1.origin = Vec3(150.0f, 0.0f, 150.0f);
    mid2.origin = Vec3(499.0f, 0.0f, 499.0f);
    outer1.origin = Vec3(501.0f, 0.0f, 0.0f);
    outer2.origin = Vec3(0.0f, 0.0f, 501.0f);
    y1.origin = Vec3(0.0f, 10.0f, 0.0f);
    y2.origin = Vec3(0.0f, -10.0f, 0.0f);

    collisionVolume->SetAABB(aabb);
    collisionVolume->SetNodeSize(nodeSize);

    Size2 index;
    hdAssert(collisionVolume->IndexOf(bad, &index) == false);
    hdAssert(collisionVolume->IndexOf(edge1, &index) == true);
    hdAssert(index[0] == 0);
    hdAssert(index[1] == 0);

    hdAssert(collisionVolume->IndexOf(mid1, &index) == true);
    hdAssert(index[0] == 3);
    hdAssert(index[1] == 3);

    hdAssert(collisionVolume->IndexOf(mid2, &index) == true);
    hdAssert(index[0] == 9);
    hdAssert(index[1] == 9);

    hdAssert(collisionVolume->IndexOf(outer1, &index) == false);
    hdAssert(collisionVolume->IndexOf(outer2, &index) == false);
    //hdAssert(collisionVolume->IndexOf(y1, &index) == false);
    //hdAssert(collisionVolume->IndexOf(y2, &index) == false);
}
