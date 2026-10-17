#include "RayHitTester.h"

#include <float.h>
#include <cmath>

#include "graphics/components/Polygon.h"
#include "graphics/components/Transform.h"
#include "graphics/mesh/MeshData.h"
#include "graphics/mesh/Mesh.h"

using namespace hd::graphics;

typedef struct RayIntersectionMesh {
    MeshData *meshData;
    Mesh *mesh;
    Vec3 hitLocation;
} RayIntersectionMesh;

#define MAX_COLLISION_ITEMS 2


bool RayHitTester::TestFastRayMeshData(const Vec3 &rayOrigin,
                                       const Vec3 &rayDirection,
                                       const MeshData *meshData,
                                       Collision3 *collision,
                                       const Matrix &transform) {
    Vec3 origin;
    Matrix::Vec3Multiply(origin, origin, transform);

    Vec3 size = meshData->GetAABB().Size();
    Matrix::Vec3Multiply(size, size, transform);
    size = size - origin;
    float radius = size.Length();

    Vec3 sphereHitLocation;

    if (hd::RayIntersectsSphere(rayOrigin, rayDirection, origin, radius, &sphereHitLocation)) {
        collision->origin = sphereHitLocation;
        return true;
    }

    return false;
}


bool RayHitTester::TestFastRayPolygon(const Vec3 &rayOrigin,
                                      const Vec3 &rayDirection,
                                      const hd::graphics::Polygon *polygon,
                                      Collision3 *collision,
                                      const Matrix &transform) {
    Vec3 origin;
    Matrix::Vec3Multiply(origin, origin, transform);

    Vec3 size = polygon->GetAABB().Size();
    Matrix::Vec3Multiply(size, size, transform);
    size = size - origin;
    float radius = size.Length();

    Vec3 intersectionLocation;

    if (hd::RayIntersectsSphere(rayOrigin, rayDirection, origin, radius, &intersectionLocation)) {
        collision->origin = intersectionLocation;
        return true;
    }

    return false;
}


bool RayHitTester::TestRayMeshData(const Vec3 &rayOrigin,
                                   const Vec3 &rayDirection,
                                   const MeshData *meshData,
                                   Collision3 *collision,
                                   const Matrix &transform) {
    auto meshes = meshData->GetMeshes();

    Vec3 origin;
    Matrix::Vec3Multiply(origin, origin, transform);

    Vec3 size = meshData->GetAABB().Size();
    Matrix::Vec3Multiply(size, size, transform);
    size = size - origin;
    float radius = size.Length();

    Vec3 intersectionLocation, intersectionNormal;
    float winningDistance = FLT_MAX;
    bool collisionFound = false;

    Vec3 sphereHitLocation;

    if (hd::RayIntersectsSphere(rayOrigin, rayDirection, origin, radius, &sphereHitLocation)) {
        for (uint32_t i = 0; i < meshes->Size(); ++i) {
            auto mesh = meshes->At(i);

            Vec3 triangle[3];

            const hd::Vector<Vertex> *vertices = mesh->GetVertices();
            const hd::Vector<Size3> *triangles = mesh->GetTriangles();
            const hd::Vector<Size4> *quads = mesh->GetQuads();

			for (uint32_t idx = 0; idx < triangles->Size(); ++idx) {
                Size3 triangleIndex = triangles->At(idx);

                triangle[0] = vertices->At(triangleIndex[0]).origin;
                triangle[1] = vertices->At(triangleIndex[1]).origin;
                triangle[2] = vertices->At(triangleIndex[2]).origin;

                Matrix::Vec3Multiply(triangle[0], triangle[0], transform);
                Matrix::Vec3Multiply(triangle[1], triangle[1], transform);
                Matrix::Vec3Multiply(triangle[2], triangle[2], transform);

                if (RayHitTester::RayIntersectsTriangle(triangle, rayOrigin, rayOrigin + rayDirection, &intersectionLocation, &intersectionNormal)) {
                    collisionFound = true;

                    float distance = (rayOrigin - intersectionLocation).Length();

                    if (distance < winningDistance) {
                        winningDistance = distance;
                        collision->origin = intersectionLocation;
                        collision->normal = intersectionNormal;
                        collision->incidentRay = hd::normalize(rayDirection);
                    }
                }
            }

			for (uint32_t idx = 0; idx < quads->Size(); ++idx) {
                Size4 quadIndex = quads->At(idx);

                triangle[0] = vertices->At(quadIndex[0]).origin;
                triangle[1] = vertices->At(quadIndex[1]).origin;
                triangle[2] = vertices->At(quadIndex[2]).origin;

                Matrix::Vec3Multiply(triangle[0], triangle[0], transform);
                Matrix::Vec3Multiply(triangle[1], triangle[1], transform);
                Matrix::Vec3Multiply(triangle[2], triangle[2], transform);

                if (RayHitTester::RayIntersectsTriangle(triangle, rayOrigin, rayOrigin + rayDirection, &intersectionLocation, &intersectionNormal)) {
                    collisionFound = true;

                    float distance = (rayOrigin - intersectionLocation).Length();

                    if (distance < winningDistance) {
                        winningDistance = distance;
                        collision->origin = intersectionLocation;
                        collision->normal = intersectionNormal;
                        collision->incidentRay = hd::normalize(rayDirection);
                    }
                }

                triangle[0] = vertices->At(quadIndex[2]).origin;
                triangle[1] = vertices->At(quadIndex[3]).origin;
                triangle[2] = vertices->At(quadIndex[0]).origin;

                Matrix::Vec3Multiply(triangle[0], triangle[0], transform);
                Matrix::Vec3Multiply(triangle[1], triangle[1], transform);
                Matrix::Vec3Multiply(triangle[2], triangle[2], transform);

                if (RayHitTester::RayIntersectsTriangle(triangle, rayOrigin, rayOrigin + rayDirection, &intersectionLocation, &intersectionNormal)) {
                    collisionFound = true;

                    float distance = (rayOrigin - intersectionLocation).Length();

                    if (distance < winningDistance) {
                        winningDistance = distance;
                        collision->origin = intersectionLocation;
                        collision->normal = intersectionNormal;
                        collision->incidentRay = hd::normalize(rayDirection);
                    }
                }
            }
        }
    }
    return collisionFound;
}


bool RayHitTester::TestRayPolygon(const Vec3 &rayOrigin,
                                  const Vec3 &rayDirection,
                                  const hd::graphics::Polygon *polygon,
                                  Collision3 *collision,
                                  const Matrix &transform) {
    hdAssert(collision);

    Vec3 origin;
    Matrix::Vec3Multiply(origin, origin, transform);

    Vec3 size = polygon->GetAABB().Size();
    Matrix::Vec3Multiply(size, size, transform);
    size = size - origin;
    float radius = size.Length();

    Vec3 intersectionLocation, intersectionNormal;
    float winningDistance = FLT_MAX;
    bool collisionFound = false;

    if (hd::RayIntersectsSphere(rayOrigin, rayDirection, origin, radius, &intersectionLocation)) {
        Vec3 triangle[3];
        const Vector<Vertex> *vertices = polygon->GetVertices();

        for (uint32_t i = 0; i < vertices->Size(); ++i) {
            triangle[0] = polygon->GetAABB().Center();
            triangle[1] = vertices->At(i).origin;
            triangle[2] = vertices->At((i+1) % vertices->Size()).origin;

            Matrix::Vec3Multiply(triangle[0], triangle[0], transform);
            Matrix::Vec3Multiply(triangle[1], triangle[1], transform);
            Matrix::Vec3Multiply(triangle[2], triangle[2], transform);

            if (RayHitTester::RayIntersectsTriangle(triangle, rayOrigin, rayOrigin + rayDirection, &intersectionLocation, &intersectionNormal)) {
                collisionFound = true;

                float distance = (rayOrigin - intersectionLocation).Length();

                if (distance < winningDistance) {
                    winningDistance = distance;
                    collision->origin = intersectionLocation;
                    collision->normal = intersectionNormal;
                    collision->incidentRay = hd::normalize(rayDirection);
                }
            }
        }
    }

    return collisionFound;
}


bool RayHitTester::TestPoint(Vec3 point,
                             const hd::graphics::Polygon *polygon,
                             const Matrix &transform) {
    return hd::PointInsideAABB(point, polygon->GetAABB());
}


// Original plane/edge-half-space ray test; no copied Wikipedia implementation.
// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
bool RayHitTester::RayHitsTriangle(const Vec3 triangle[3],
                                   const Vec3 &rayOrigin,
                                   const Vec3 &rayDirection,
                                   float *result) {
    const Vec3 face = hd::cross(triangle[1] - triangle[0], triangle[2] - triangle[0]);
    const float denominator = face * rayDirection;
    if (std::fabs(denominator) < FLT_EPSILON) return false;
    const float parameter = (face * (triangle[0] - rayOrigin)) / denominator;
    if (!(parameter > FLT_EPSILON) || !std::isfinite(parameter)) return false;
    const Vec3 point = rayOrigin + parameter * rayDirection;
    for (unsigned int edge = 0; edge < 3; ++edge) {
        const Vec3 tangent = triangle[(edge + 1) % 3] - triangle[edge];
        if ((hd::cross(tangent, point - triangle[edge]) * face) < 0.0f) return false;
    }
    *result = parameter;
    return true;
}


// Adapted from algorithm 6 from softsurfer.com.
// Copyright 2001 softSurfer, 2012 Dan Sunday
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.
bool RayHitTester::RayIntersectsTriangle(const Vec3 *triangle,
                                                const Vec3 &ray0,
                                                const Vec3 &ray1,
                                                Vec3 *intersectionLocation,
                                                Vec3 *normal) {
    Vec3 u, v, n;
    Vec3 dir, w0, w;
    float r, a, b;

    u = triangle[1] - triangle[0];
    v = triangle[2] - triangle[0];
    n = hd::cross(u, v);

    if (n[0] == 0 && n[1] == 0 && n[2] == 0) {
        return false;
    }

    dir = ray1 - ray0;
    w0 = ray0 - triangle[0];

    if (dir * n > 0) {
        *normal = n;
    } else {
        *normal = -n;
    }

    a = -(n * w0); // dot product
    b = n * dir; // dot product
    
    if (fabs(b) < 0.0000001f) {
        return false;
    }
    
    // Get intersected point of ray with triangles plane
    r = a / b;
    if (r < 0.0f) return false;
    
    dir *= r;
    *intersectionLocation = ray0 + dir;
    
    float uu, uv, vv, wu, wv, D;
    
    uu = u * u;
    uv = u * v;
    vv = v * v;
    
    w = *intersectionLocation - triangle[0];
    wu = w * u;
    wv = w * v;
    
    D = (uv * uv) - (uu * vv);
    
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0) return false;
    
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0f) return false;
    
    return true;
}
