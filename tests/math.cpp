// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vast/VastMath.h>
#include "graphics/components/RayHitTester.h"

static void require(bool value, const char *message) {
    if (!value) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}

static void near(const Vec3 &actual, const Vec3 &expected, const char *message) {
    require((actual - expected).Length() < 0.0002f, message);
}

int main() {
    const Vec3 x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);
    near(Quaternion::FromTwoVectors(x, y) * x, y, "quarter turn maps source to target");
    near(Quaternion::FromTwoVectors(x * 2, z * 7) * x, z, "non-unit direction inputs yield a rotation");
    near(Quaternion::FromTwoVectors(x, -x) * x, -x, "opposite X directions");
    near(Quaternion::FromTwoVectors(y, -y) * y, -y, "opposite Y directions");
    near(Quaternion::FromTwoVectors(z, -z) * z, -z, "opposite Z directions");
    near(Quaternion::FromTwoVectors(x, x) * y, y, "parallel directions yield identity");
    near(Quaternion::FromTwoVectors(Vec3(), x) * y, y, "zero source yields identity");
    near(Quaternion::FromTwoVectors(x, Vec3()) * y, y, "zero target yields identity");
    near(Quaternion::FromTwoVectors(Vec3(std::numeric_limits<float>::infinity(), 0, 0), y) * x,
         x, "nonfinite input yields identity");
    near(Quaternion::FromTwoVectors(x * 1e30f, y * 1e30f) * x, y, "large finite input avoids float overflow");
    near(Quaternion(z, hd_pi * 0.5f) * x, y, "licensed vector routine rotates around Z");

    for (int i = 1; i <= 100; ++i) {
        const Vec3 source(std::sin(float(i)), std::cos(float(i * 3)), 0.25f);
        const Vec3 target(std::cos(float(i * 2)), 0.5f, std::sin(float(i * 5)));
        const Quaternion rotation = Quaternion::FromTwoVectors(source, target);
        require(std::fabs(rotation.Length() - 1.0f) < 0.0002f, "constructed quaternion has unit length");
        near(rotation * hd::normalize(source), hd::normalize(target), "arbitrary directions align");
    }

    const Vec3 triangle[] = {Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(0, 1, 0)};
    const Vec3 reversed[] = {triangle[0], triangle[2], triangle[1]};
    using hd::graphics::RayHitTester;
    float distance = -1;
    require(RayHitTester::RayHitsTriangle(triangle, Vec3(0.2f, 0.2f, 2), Vec3(0, 0, -2), &distance), "direction ray hits triangle");
    require(std::fabs(distance - 1) < 0.0001f, "parameter uses the supplied direction magnitude");
    require(RayHitTester::RayHitsTriangle(reversed, Vec3(0.2f, 0.2f, 2), Vec3(0, 0, -1), &distance), "both triangle windings accepted");
    require(RayHitTester::RayHitsTriangle(triangle, Vec3(0, 0, 1), Vec3(0, 0, -1), &distance), "vertex hit included");
    require(RayHitTester::RayHitsTriangle(triangle, Vec3(0.5f, 0.5f, 1), Vec3(0, 0, -1), &distance), "edge hit included");
    require(!RayHitTester::RayHitsTriangle(triangle, Vec3(2, 2, 1), Vec3(0, 0, -1), &distance), "outside triangle rejected");
    require(!RayHitTester::RayHitsTriangle(triangle, Vec3(0.2f, 0.2f, 1), z, &distance), "backward hit rejected");
    require(!RayHitTester::RayHitsTriangle(triangle, Vec3(0.2f, 0.2f, 1), x, &distance), "parallel ray rejected");
    const Vec3 degenerate[] = {x, x, x};
    require(!RayHitTester::RayHitsTriangle(degenerate, z, -z, &distance), "degenerate triangle rejected");

    Vec3 ray[] = {Vec3(0.2f, 0.2f, 1), Vec3(0.2f, 0.2f, 0)};
    Vec3 point, normal;
    require(hd::RayIntersectsTriangle(triangle, ray, point), "retained SoftSurfer endpoint API hits");
    near(point, Vec3(0.2f, 0.2f, 0), "endpoint API computes the intersection");
    require(RayHitTester::RayIntersectsTriangle(triangle, ray[0], ray[1], &point, &normal), "component endpoint API hits");
    require(normal * (ray[1] - ray[0]) > 0, "legacy normal orientation retained");
    std::puts("Quaternion alignment, finite-input handling, ray edges/windings, and licensed endpoint APIs passed.");
}
