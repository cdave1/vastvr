#ifndef _VAST_UI_SPHERE_LAYOUT_H_
#define _VAST_UI_SPHERE_LAYOUT_H_

#include <Vast/VastMath.h>

///
/// Layout function for positioning items on the surface of a sphere.  Useful for placing components with a
/// transform on a sphere that is positioned around a central point.
///
/// Defines a sphere surface devided into a grid of square rows and columns.
///
namespace hd {
    class Entity;
    
    namespace graphics {
        class Transform;
    }
    namespace ui {
        class SphereLayout {
        private:

            uint32_t m_rows;

            uint32_t m_columns;

            float m_sphereRadius;

            float m_arcWidth;

        public:

            SphereLayout(const uint32_t rows, const uint32_t columns,
                         float sphereRadius, const float arcWidth);

            ///
            /// Determine the world position of an item on the sphere at the given grid location.  The offset is in
            /// grid units.
            ///
            void PositionOnSphere(const float row, const float col,
                                  Vec3 *_pos, Quaternion *_rotation, Vec2 offset = Vec2()) const;

            void PositionOnSphere(const float row, const float col,
                                  hd::graphics::Transform *transform, Vec2 offset = Vec2()) const;

            void PositionOnSphere(const float row, const float col,
                                  const hd::Entity *entity, Vec2 offset = Vec2()) const;

            void NearestPosition(Vec3 *_pos, Quaternion *_rotation, const Vec3 &worldPos);


            void SetRows(const uint32_t rows) {
                m_rows = rows;
            }


            void SetColumns(const uint32_t columns) {
                m_columns = columns;
            }


            void SetSphereRadius(const float radius) {
                m_sphereRadius = radius;
            }


            void SetArcWidth(const float arcWidth) {
                m_arcWidth = arcWidth;
            }


            ///
            /// Calculate the approximate size, in world coordinates, of a single grid node.
            ///
            Vec2 GetGridNodeSize(const uint32_t row = 0, const uint32_t col = 0) const;

            float GetSphereRadius() const {
                return m_sphereRadius;
            }

        };
    }
}



#endif