/**
 * Where the "world" represents some kind of coordinate space, this component
 * stores basic information about an item in that space.
 *
 * The data here can be applied to anything that has area or volume in the 
 * coordinate space.
 */

#ifndef _VAST_GRAPHICS_TRANSFORM_COMPONENT_H_
#define _VAST_GRAPHICS_TRANSFORM_COMPONENT_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Transform : public Component {
        private:
            
            Vec3 m_origin;
            
            Vec3 m_scale;
            
            Quaternion m_rotation;
            
            Matrix m_matrix;

            Matrix m_contextMatrix;

            bool m_isDirty;
            
            
        protected:
            
            friend class ComponentPool<Transform>;
            
            Transform(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void SetOrigin(const Vec3 &origin);
            
            const Vec3& GetOrigin() const;

            void SetScale(const float scale);
            
            void SetScale(const Vec3 &scale);
            
            const Vec3& GetScale() const;
            
            void SetRotation(const Vec3 &rotation, float theta);
            
            void SetRotation(const Quaternion &quat);
            
            const Quaternion& GetRotation() const;

            const Matrix & GetMatrix() const;

            const Matrix & GetContextMatrix() const;

            void SetContext(const Matrix &context);

            void Update();

            
        };
    }
}

#endif
