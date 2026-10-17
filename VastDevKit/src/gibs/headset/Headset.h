#ifndef _VAST_GIBS_HEADSET_H_
#define _VAST_GIBS_HEADSET_H_

#include <Vast/VastMath.h>
#include "core/device/Device.h"
#include "core/component/Component.h"
#include "gibs/Constants.h"
#include "gibs/Headset/HMD.h"
#include "graphics/gl/PixelBufferObject.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class Headset : public Component {
        private:

            bool m_isStereo;

            Vec3 m_position;

            Vec3 m_trackingOffset;

            Quaternion m_orientation;

            Quaternion m_orientationOffset;

			Quaternion m_leftOrientation;

			Quaternion m_rightOrientation;

            Vec3 m_leftEyeOffset;

            Vec3 m_rightEyeOffset;

            AABB m_screenViewport;

            AABB m_leftViewport;

            AABB m_rightViewport;

            Matrix m_screenProjection;

            Matrix m_leftProjection;

            Matrix m_rightProjection;

            hd::graphics::PixelBufferObject m_screenPixelBuffer;

            hd::graphics::PixelBufferObject m_leftPixelBuffer;

            hd::graphics::PixelBufferObject m_rightPixelBuffer;


        protected:

            friend class ComponentPool<Headset>;

            Headset(unsigned int ID);


        public:

            void Reset();

            void SetIsStereo(bool isStereo);

            bool IsStereo() const;

            const Quaternion & GetOrientation() const;

            const Quaternion GetWorldOrientation() const;

            const Vec3 GetWorldPosition() const;

            const Vec3 & GetPosition() const;

            void SetEyeOffset(const Vec3 &offset,
								const hd::input::EyePosition eyePosition = hd::input::EyePosition::Center);

			void SetEyeOrientation(const Quaternion &leftOrientation, const Quaternion &rightOrientation);

            ///
            /// For the given eye position, return the sceenspace offset from the eye to the given collision,
            /// which is assumed to be a ray emitted from between the two hd::input::EyePosition::Center.
            ///
            Vec2 GetScreenspaceOffset(const hd::input::EyePosition eyePosition, const Collision3 &collision) const;

            void SetPosition(const Vec3 &position);

            void SetTrackingOffset(const Vec3 &trackingOffset);

            void SetOrientation(const Quaternion &orientation);

            void SetOrientationOffset(const Quaternion &orientationOffset);

            void RotateBy(const Quaternion &delta);

            void SetViewport(const hd::input::EyePosition eyePosition, const AABB &aabb);

            void SetPixelBufferSize(const hd::input::EyePosition eyePosition, const Size2 &size);

            const AABB & GetViewport(const hd::input::EyePosition eyePosition = hd::input::EyePosition::Center) const;

            void SetProjection(const hd::input::EyePosition eyePosition, const Matrix &matrix);

            const Matrix & GetProjection(const hd::input::EyePosition eyePosition = hd::input::EyePosition::Center) const;

            const Vec3 GetEyeWorldPosition(const hd::input::EyePosition eyePosition = hd::input::EyePosition::Center) const;

            const Matrix GetLookAtMatrix(const hd::input::EyePosition eyePosition = hd::input::EyePosition::Center) const;

            void UpdatePixelBuffer(const hd::input::EyePosition eyePosition);

            Color4f GetColor(const hd::input::EyePosition eyePosition, const Vec2 &offset) const;

            Color4f GetCollisionColor(const Collision3 &collision) const;

        };
    }
}

#endif
