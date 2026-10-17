#ifndef _VAST_GRAPHICS_COMPONENT_RAY_HIT_TESTER_H_
#define _VAST_GRAPHICS_COMPONENT_RAY_HIT_TESTER_H_

#include <Vast/VastMath.h>

namespace hd {
    namespace graphics {
        class Polygon;
        class Transform;
        class MeshData;
        
        class RayHitTester
        {
        public:

            ///
            /// Only test the sphere
            ///
            static bool TestFastRayMeshData(const Vec3 &rayOrigin,
                                            const Vec3 &rayDirection,
                                            const MeshData *meshData,
                                            Collision3 *collision,
                                            const Matrix &transform);

            static bool TestFastRayPolygon(const Vec3 &rayOrigin,
                                           const Vec3 &rayDirection,
                                           const hd::graphics::Polygon *polygon,
                                           Collision3 *collision,
                                           const Matrix &transform);


            static bool TestRayPolygon(const Vec3 &rayOrigin,
                                       const Vec3 &rayDirection,
									   const hd::graphics::Polygon *polygon,
                                       Collision3 *collision,
                                       const Matrix &transform);

            ///
            /// Assume rayOrigin and rayDirection are within the camera world space.
            ///
            static bool TestRayMeshData(const Vec3 &rayOrigin,
                                        const Vec3 &rayDirection,
                                        const MeshData *meshSet,
                                        Collision3 *collision,
                                        const Matrix &transform);

            static bool TestPoint(Vec3 point,
				const hd::graphics::Polygon *polygon,
                                  const Matrix &transform);

            static bool RayIntersectsTriangle(const Vec3 *triangle,
                                              const Vec3 &ray0,
                                              const Vec3 &ray1,
                                              Vec3 *intersectionLocation,
                                              Vec3 *normal);

            static bool RayHitsTriangle(const Vec3 triangle[3],
                                        const Vec3 &rayOrigin,
                                        const Vec3 &rayDirection,
                                        float *result);

        };
    }
}

#endif

