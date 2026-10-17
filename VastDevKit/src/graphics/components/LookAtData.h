#ifndef _VAST_GRAPHICS_COMPONENT_LOOKAT_DATA_H_
#define _VAST_GRAPHICS_COMPONENT_LOOKAT_DATA_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class LookAtData : public Component {
        private:
            
            Vec3 m_eye;
            
            Vec3 m_target;
            
            Vec3 m_upAxis;
            
            Vec3 m_location;

            Vec3 m_offset;

            Quaternion m_rotation;

            Quaternion m_rotationOffset;
            
            Matrix m_matrix;
            
            
        protected:
            
            friend class ComponentPool<LookAtData>;
            
            LookAtData(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void Update();
            
            void SetEye(const Vec3 &eye);
            
            const Vec3 & GetEye() const;
            
            void SetTarget(const Vec3 &target);
            
            Vec3 GetTarget() const;

            Vec3 GetRealTarget() const;
            
            void SetUpAxis(const Vec3 &upAxis);
            
            const Vec3 & GetUpAxis() const;

            const Vec3 & GetRawLocation() const;
            
            void SetLocation(const Vec3 &location);
            
            Vec3 GetLocation() const;
            
            void SetRotation(const Quaternion &quat);

            void Rotate(const Quaternion &quat);

            void RotateOffset(const Quaternion &quat);

            void SetRotationOffset(const Quaternion &quat);

            const Quaternion & GetRotationOffset() const;

            Vec3 GetOffset() const;

            void SetOffset(const Vec3 &offset);

            void FollowTarget(const Vec3 &target);
            
            const Quaternion & GetRotation() const;
            
            void SetMatrix(const Matrix &m);
            
            const Matrix & GetMatrix() const;
            
        };
    }
}

#endif