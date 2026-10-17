/**
 * This component stores vertex data.
 *
 * TODO: This is an obsolete class.
 */

#ifndef _VAST_GRAPHICS_COMPONENT_POLYGON_H_
#define _VAST_GRAPHICS_COMPONENT_POLYGON_H_

#include <Vast/VastMath.h>
#include "math/Vertex.h"
#include "core/Collections/Collections.h"
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Polygon : public Component {
        private:
            
            void ResetAABB();
            
            AABB m_aabb;
            
            Vector<Vertex> *m_vertices;
            
            float m_texRepeatX;
            
            float m_texRepeatY;
            
            
        protected:
            
            friend class ComponentPool<Polygon>;
            
            Polygon(unsigned int ID);
            
            
        public:
            
            void Reset();
                        
            void AddVertex(Vertex vertex);
            
            void AddVertex(Vec3 vertex);
            
            void AddVertex(const float x, const float y);
            
            void AddVertex(const float x, const float y, const float z);

            void AddVertex(const float x, const float y, const float z, const Vec3 &offset);

            void AddLine(const Vec3 &from, const Vec3 &to);
            
            void RemoveVertex(const int index);
            
            void ClearVertices();
            
            void Rotate(const Vec3 &rotationVector, const Vec3 &rotationAxis);
            
            void Scale(const Vec3 &scaleVector, const Vec3 &scaleAxis);
            
            void MoveTo(const Vec3 &center);
            
            void MoveBy(const Vec3 &delta);
            
            void Create2DBox(const AABB &aabb);
            
            void Create2DCylinder(const Vec3 &center,
                                 const float radius,
                                 const int segments);
            
            /*
             * 5 point star with tips sitting on circle with radius
             */
            void Create2DStar(const AABB &aabb,
                              const float radius,
                              const float innerRadius,
                              const unsigned int circleSegments);

            void CreateCube(const float size, const Vec3 &center);
            
            void CreateCube(const float w, const float h, const float d, const Vec3 &center);
            
            void CreateSphere(const float radius, const float segments);
            
            float GetTextureRepeatX() const;
            
            float GetTextureRepeatY() const;
            
            void SetTextureRepeatX(const float repeatX);
            
            void SetTextureRepeatY(const float repeatY);
            
            Vec2 GetTextureCoordForPoint(const AABB &aabb, const Vec3 &point);
            
            void ResetNormals();
            
            void ResetTextureCoords();
            
            void ResetTextureCoords(const Size2 &size);
            
            void InvertNormals();
            
            AABB GetAABB() const;
            
            const Vector<Vertex> * GetVertices() const;
            
        };
    }
}

#endif
