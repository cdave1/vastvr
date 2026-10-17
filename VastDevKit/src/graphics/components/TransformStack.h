/**
 * This component stores information about an item within a local coordinate space.
 */

#ifndef _VAST_TRANSFORM_STACK_COMPONENT_H_
#define _VAST_TRANSFORM_STACK_COMPONENT_H_

#include "core/component/Component.h"
#include <Vast/Vast.h>

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
/*
 class TransformStack
 private:
 Transforms[] as stack

 public:
 // pushes a new transform on to the stack
 Push()

 // pops the top transform from the stack
 Pop()

 // All set operations alter the top most transform on the stack
 SetOrigin(Vec3)

 SetRotation(Quaternion)

 SetScale(Vec3)

 */
 /*
        class Transform {



        };*/


        class TransformStack : public Component {
        private:
            Vec3 m_origin;

            Vec3 m_scale;

            Quaternion m_rotation;

            hd::Vector<hd::graphics::Transform *> m_transformStack;
            
            Matrix m_matrix;
            
            void UpdateMatrix();
            
            
        protected:
            
            friend class ComponentPool<TransformStack>;
            
            TransformStack(unsigned int ID);
            
            
        public:
            
            void Reset();

            void Push();

            void Pop();
            
            void SetOrigin(const Vec3 &origin);
            
            const Vec3& GetOrigin() const;
            
            void SetScale(const float scale);
            
            void SetScale(const Vec3 &scale);
            
            const Vec3& GetScale() const;
            
            void SetRotation(const Vec3 &rotation, float theta);
            
            void SetRotation(const Quaternion &quat);
            
            const Quaternion& GetRotation() const;
            
            const Matrix & GetMatrix() const;
            
        };
    }
}

#endif
