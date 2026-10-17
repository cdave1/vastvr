#ifndef _VAST_GEOMETRY_H_
#define _VAST_GEOMETRY_H_

#include "math/Vertex.h"
#include "math/Functions.h"
#include "math/Matrix.h"

namespace hd {
    inline void ScaleVertices(Vertex *vertices, unsigned int vertexCount, const Vec3 &scale) {
        if (vertexCount == 0) return;
        for (unsigned i = 0; i < vertexCount; ++i) {
            vertices[i].origin[0] *= scale[0];
            vertices[i].origin[1] *= scale[1];
            vertices[i].origin[2] *= scale[2];
        }
    }
    
    
    // Rotates vertices around origin.
    inline void RotateVertices(Vertex *vertices, unsigned int vertexCount, const Vec3& rotation) {
        Matrix rot;
        unsigned i;
        
        if (rotation[0] == 0 && rotation[1] == 0 && rotation[2] == 0) return;
        
        Matrix::RotationXYZOrigin(rot, -rotation);
        for (i = 0; i < vertexCount; ++i) {
            Matrix::Vec3Multiply(vertices[i].origin, vertices[i].origin, rot);
        }
    }
    
    
    inline void TranslateVertices(Vertex *vertices, unsigned int vertexCount, const Vec3 &translation) {
        for (unsigned i = 0; i < vertexCount; ++i) {
            vertices[i].origin += translation;
        }
    }
    
    
    /* via SuperCollider help files */
    inline float FastHypotenuse(const float x, const float y) {
        const float sqrt2 = 1.41421f;
        
        return fabs(x) + fabs(y) - ((sqrt2 - 1) * hd::min(fabs(x), fabs(y)));
    }
    
    
    inline float Get2DNormal(const Vec3& a, const Vec3& b) {
        Vec3 diff;
        float normal;
        
        diff = b - a;
        if (diff[0] == 0) {
            normal = 0.0f;
        } else if (diff[1] == 0) {
            normal = (diff[0]) > 0.0f ? 1.0f : -1.0f;
        } else {
            normal = diff[0] / FastHypotenuse(diff[0], diff[1]);
        }
        return normal;
    }
    
    
    // Vector product at point b, from a to b to c
    inline float VectorProduct(const Vec2& a, const Vec2& b, const Vec2& c) {
        return (a[0] * (b[1] - c[1])) + (b[0] * (c[1] - a[1])) + (c[0] * (a[1] - b[1]));
    }
    
    
    inline float VectorProduct(const Vec3& a, const Vec3& b, const Vec3& c) {
        return (a[0] * (b[1] - c[1])) + (b[0] * (c[1] - a[1])) + (c[0] * (a[1] - b[1]));
    }
    
    
    // Returns 1 if c is CCW of vector a-b, -1 if CW, and 0 if parallel
    inline signed int VectorProductSign(const Vec2& a, const Vec2& b, const Vec2& c) {
        float vectorProduct = VectorProduct(a,b,c);
        if (vectorProduct > 0.0f) return 1;
        return (vectorProduct < 0.0f) ? -1 : 0;
    }
    
    
    inline signed int CrossProductSign(const Vec3& a, const Vec3& b, const Vec3&c) {
        // a to b, b to c
        Vec3 ab = b - a;
        Vec3 bc = c - b;
        Vec3 res = hd::cross(ab, bc);
        return (res[2] > 0.0) ? 1 : (res[2] < 0.0f) ? -1 : 0;
    }
    
    
    inline float AABBArea(const AABB& aabb) {
        Vec3 box = aabb.upper - aabb.lower;
        return box[0] * box[1];
    }
    
    
    inline float AABBArea(const Vec3& v1, const Vec3& v2) {
        AABB aabb;
        aabb.lower = hd::min(v1, v2);
        aabb.upper = hd::max(v1, v2);
        return AABBArea(aabb);
    }
    
    
    inline AABB FindAABB(const Vertex *vertices, int vertexCount) {
        AABB aabb;
        if (vertexCount >= 2) {
            aabb.lower = vertices[0].origin;
            aabb.upper = vertices[0].origin;
            
            for (int i = 0; i < vertexCount; ++i) {
                aabb.lower = hd::min(aabb.lower, vertices[i].origin);
                aabb.upper = hd::max(aabb.upper, vertices[i].origin);
            }
        }
        return aabb;
    }
    
    
    // Another method (maybe faster?):
    // - Subtract point from lower, upper, and check signs of result
    // - signs should all be negative.
    inline bool PointInsideAABB(const Vec3& point, const AABB& aabb) {
        return  ((point[0] < aabb.lower[0] || point[0] > aabb.upper[0]) == false) &&
        ((point[1] < aabb.lower[1] || point[1] > aabb.upper[1]) == false) &&
        ((point[2] < aabb.lower[2] || point[2] > aabb.upper[2]) == false);
    }
    
    
    inline bool AABBIntersection(const AABB& aabb1, const AABB& aabb2) {
        return (aabb1.lower[0] < aabb2.upper[0]) &&
        (aabb1.upper[0] > aabb2.lower[0]) &&
        (aabb1.lower[1] < aabb2.upper[1]) &&
        (aabb1.upper[1] > aabb2.lower[1]) &&
        (aabb1.lower[2] < aabb2.upper[2]) &&
        (aabb1.upper[2] > aabb2.lower[2]);
    }


    inline bool RayIntersectsSphere(const Vec3 &rayOrigin,
                                    const Vec3 &rayDirection,
                                    const Vec3 &sphereCenter,
                                    const float radius,
                                    Vec3 *location) {
/*
        hd::log::i("Check RayIntersectsSphere: origin: (%.3f, %.3f, %.3f), direction: (%.3f, %.3f, %.3f) sphere: (%.3f, %.3f, %.3f) radius: %3.2f\n",
                   rayOrigin[0], rayOrigin[1], rayOrigin[2],
                   rayDirection[0], rayDirection[1], rayDirection[2],
                   sphereCenter[0], sphereCenter[1], sphereCenter[2],
                   radius);
*/
#if 1
        float a = rayDirection * rayDirection;
        float b = 2 * (rayDirection * (rayOrigin - sphereCenter));
        float c = ((rayOrigin - sphereCenter) * (rayOrigin - sphereCenter)) - (radius * radius);

        float disc = b * b - 4 * a * c;

        if (disc < 0) {
            return false;
        }

        float det = sqrtf(disc);
        float q;
        if (b < 0.0f) {
            q = (-b - det) / 2.0f;
        } else {
            q = (-b + det) / 2.0f;
        }

        float t0 = q / a;
        float t1 = c / q;

        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }

        if (t1 < 0) {
            return false;
        }

        if (t0 < 0) {
            *location = rayOrigin + (t1 * rayDirection);
            //t = t1;
            return true;
        } else {
            *location = rayOrigin + (t0 * rayDirection);
            return true;
        }
#else
#if 1
        Vec3 p = rayOrigin - sphereCenter;
        float b = - (p * rayDirection);
        float det = (b * b) - (p * p) + (radius * radius);
        if (det < 0) {
            return false;
        }
        det = sqrtf(det);
        float i1 = b - det;
        float i2 = b + det;
        if (i2 < 0) {
            return false;
        }
        return true;

#else
        Vec3 L = sphereCenter - rayOrigin;

        float tca = L * rayDirection;
        if (tca < 0) {
            return false;
        }

        float d2 = L * L - tca * tca;
        if (d2 > radius) {
            return false;
        }

        float thc = sqrtf(radius - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        const float T_MAX = 10000000.0f;

        if (t0 > T_MAX) {
            return false;
        }

        *location = rayOrigin + (t0 * rayDirection);

        return true;
#endif
#endif
    }


    // Calculate this for a sphere at 0,0,0 and a ray emitted from the same position.
    inline bool RayIntersectsSphereOrigin(const Vec3 &rayDirection,
                                          const float radius,
                                          Vec3 *location) {
        return RayIntersectsSphere(Vec3(), rayDirection, Vec3(), radius, location);
    }


    inline Vec3 GetPointInsideSphere(const float latitude,
                                     const float longitude) {
        Vec3 loc;

        Vec3 direction(1.0f, 0.0f, 0.0f);
        Vec3 result;

        Matrix rot;
        Matrix::RotationXYZOrigin(rot, Vec3(0.0f, longitude, latitude));
        Matrix::Vec3Multiply(result, direction, rot);
        result = hd::normalize(result);
/*
        Vec3 lat(cosf(latitude), sinf(latitude), sinf(longitude));
        lat = hd::normalize(lat);
*/
        hd::RayIntersectsSphere(Vec3(0.0f, 0.0f, 0.0f),
                                result,
                                Vec3(0.0f, 0.0f, 0.0f),
                                1.0f, &loc);
        loc = hd::normalize(loc);
        return loc;
    }
    
    
    // Assume, for now, that point is on the same plane as points in polygon.
    inline bool PolygonContainsPoint(const Vec3* vertices, const int vertexCount, const Vec3& point) {
        if (vertexCount == 1) return point == vertices[0];
        
        signed int contains = 0;
        for (int i = 0; i < vertexCount; ++i) {
            contains += hd::CrossProductSign(vertices[i], vertices[(i+1)%vertexCount], point);
        }
        
        return contains == vertexCount;
    }
    
    
    // Determines if the polygon contains the point.
    // Assumes that the polygon points are arranged in counter clockwise order.
    inline bool PolygonContainsPoint(const Vec2* polygon, const int polygonCount, const Vec2& point) {
        if (polygonCount == 1) return point == polygon[0];
        
        signed int contains = 0x0001;
        for (int i = 0; i < polygonCount; ++i) {
            contains |= hd::VectorProductSign(polygon[i], polygon[(i+1)%polygonCount], point);
        }
        
        return contains == 0x0001;
    }
    
    
    // Assuming the vertices define the hull of a solid shape, this function
    // returns true as soon as a triangle can be found that contains the given point.
    //
    // Use sparingly.
    inline bool ConcavePolygonContainsPoint(const Vec3* vertices, const int vertexCount, const Vec3& center, const Vec3& point) {
        if (vertexCount == 1) return point == vertices[0];
        if (vertexCount == 2) return false; // lazy
        
        Vec3 triangle[3];
        for (int i = 0; i < vertexCount; ++i) {
            triangle[0] = center;
            triangle[1] = vertices[i];
            triangle[2] = vertices[(i+1) % vertexCount];
            if (hd::PolygonContainsPoint(triangle, 3, point)) return true;
        }
        
        return false;
    }
    
    
    inline bool ConcavePolygonContainsPoint2D(const Vec3* vertices, const int vertexCount, const Vec3& center, const Vec2& point) {
        if (vertexCount <= 2) return false;
        
        Vec3 triangle[3];
        Vec3 point2D(point[0], point[1], 0.0f);
        for (int i = 0; i < vertexCount; ++i) {
            triangle[0].Set(center[0], center[1], 0.0f);
            triangle[1].Set(vertices[i][0], vertices[i][1], 0.0f);
            triangle[2].Set(vertices[(i+1) % vertexCount][0], vertices[(i+1) % vertexCount][1], 0.0f);
            if (hd::PolygonContainsPoint(triangle, 3, point2D)) return true;
        }
        
        return false;
    }
    
    
    // angle between v1 and v2 using ref
    inline float GetAngle(const Vec2& v1, const Vec2& v2, const Vec2& ref) {
        Vec2 _v1 = v1 - ref;
        Vec2 _v2 = v2 - ref;
        
        _v1 = hd::normalize(_v1);
        _v2 = hd::normalize(_v2);
        
        float angle = atan2f(_v2[1], _v2[0]) - atan2f(_v1[1], _v1[0]);
        
        
        
        //	Vec2 c = hdCross(v1, v2);
        //	float angle = atan2(c.Length(), hdDot(v1, v2));
        //return (angle < 0.0f) ? angle + (2.0f * _VAST_pi) : angle;
        return angle;
    }
    
    
    inline float GetAngle(const Vec3& v1, const Vec3& v2, const Vec3& ref) {
        return hd::GetAngle(Vec2(v1[0], v1[1]),
                            Vec2(v2[0], v2[1]),
                            Vec2(ref[0], ref[1]));
    }
    
    
    // Adapted from algorithm 6 from softsurfer.com.
    // Copyright 2001 softSurfer, 2012 Dan Sunday
    // This code may be freely used and modified for any purpose
    // providing that this copyright notice is included with it.
    // SoftSurfer makes no warranty for this code, and cannot be held
    // liable for any real or imagined damage resulting from its use.
    // Users of this code must verify correctness for their application.
    inline bool RayIntersectsTriangle(const Vec3 *triangle, const Vec3 *ray, Vec3 &vOut) {
        Vec3 u, v, n;
        Vec3 dir, w0, w;
        float r, a, b;
        
        u = triangle[2] - triangle[0];
        v = triangle[1] - triangle[0];
        n = hd::cross(u, v);
        
        dir = ray[1] - ray[0];
        w0 = ray[0] - triangle[0];
        
        a = -(n * w0); // dot product
        b = n * dir; // dot product
        
        if (fabs(b) < 0.0000001f) {
            return false;
        }
        
        // Get intersected point of ray with triangles plane
        r = a / b;
        if (r < 0.0f) return false;
        
        dir *= r;
        vOut = ray[0] + dir;
        
        //hd::log::i("%f, %f, %f\n", vOut[0], vOut[1], vOut[2]);
        //return hdPolygonContainsPoint(triangle, 3, vOut);
        float uu, uv, vv, wu, wv, D;
        
        uu = u * u;
        uv = u * v;
        vv = v * v;
        
        w = vOut - triangle[0];
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
    
    
    inline bool RayIntersectsVertices(const Vec3 *vertices, const int vertexCount, const Vec3& center, const Vec3 *ray,  Vec3 &vOut) {
        if (vertexCount < 3) return false;
        
        Vec3 triangle[3];
        for (int i = 0; i < vertexCount; ++i) {
            triangle[0] = center;
            triangle[1] = vertices[i];
            triangle[2] = vertices[(i+1) % vertexCount];
            if (hd::RayIntersectsTriangle(triangle, ray, vOut)) return true;
        }
        
        return false;
    }
    
    
    inline float PolygonArea(const Vec3 *vertices, const int vertexCount) {
        if (vertexCount < 3) return 0;
        
        float accum = 0.0f;
        
        Vec3 tri[3];
        for (int i = 0; i < (vertexCount - 2); ++i) {
            tri[0] = vertices[i];
            tri[1] = vertices[(i+1) % vertexCount];
            tri[2] = vertices[(i+2) % vertexCount];
            
            float area = (tri[0][0] * (tri[1][1] - tri[2][1])) 
            + (tri[1][0] * (tri[2][1] - tri[0][1])) 
            + (tri[2][0] * (tri[0][1] - tri[1][1]));
            accum += (fabs(area) * 0.5f);
        }
        
        return accum;
    }
    
    
    // Return the "most counter clockwise" point of a and b in around pivot.
    // convex if sum of cross product signs is positive and equal to the number of 
    // vertices.
    inline bool IsConvexCCW(const Vec3* vertices, const int vertexCount) {
        if (vertexCount == 1) return false;
        
        signed int contains = 0;
        for (int i = 0; i < vertexCount; ++i) {
            contains += hd::CrossProductSign(vertices[i], vertices[(i+1)%vertexCount], vertices[(i+2)%vertexCount]);
        }
        
        return contains == vertexCount;
    }
    
    
    inline bool IsConvexCW(const Vec3* vertices, const int vertexCount) {
        if (vertexCount == 1) return false;
        
        signed int contains = 0;
        for (int i = 0; i < vertexCount; ++i) {
            contains += hd::CrossProductSign(vertices[i], vertices[(i+1)%vertexCount], vertices[(i+2)%vertexCount]);
        }
        
        return contains == -vertexCount;
    }
}

#endif
