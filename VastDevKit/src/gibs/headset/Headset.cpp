#include "Headset.h"

using namespace hd::input;

hd::input::Headset::Headset(unsigned int ID) : Component(ID) {}


void hd::input::Headset::Reset() {
    m_position = Vec3();
    m_trackingOffset = Vec3();

    m_orientation = Quaternion();
    m_orientationOffset = Quaternion();

    m_leftEyeOffset = Vec3();
    m_rightEyeOffset = Vec3();

    m_screenViewport = AABB();
    m_leftViewport = AABB();
    m_rightViewport = AABB();

    m_screenProjection = Matrix::Identity();
    m_leftProjection = Matrix::Identity();
    m_rightProjection = Matrix::Identity();

    m_screenPixelBuffer.Reset();
    m_leftPixelBuffer.Reset();
    m_rightPixelBuffer.Reset();

    m_isStereo = false;
}


void hd::input::Headset::SetIsStereo(bool isStereo) {
    m_isStereo = isStereo;
}


bool hd::input::Headset::IsStereo() const {
    return m_isStereo;
}


const Quaternion & hd::input::Headset::GetOrientation() const {
    return m_orientation;
}


const Quaternion hd::input::Headset::GetWorldOrientation() const {
    return m_orientationOffset * m_orientation;
}


const Vec3 hd::input::Headset::GetWorldPosition() const {
    return m_position + (m_orientationOffset * m_orientation * m_trackingOffset);
}


const Vec3 & hd::input::Headset::GetPosition() const {
    return m_position;
}


void hd::input::Headset::SetEyeOffset(const Vec3 &offset,
                                      const hd::input::EyePosition eyePosition) {
    if (eyePosition == hd::input::EyePosition::Left) {
        m_leftEyeOffset = offset;
    }

    if (eyePosition == hd::input::EyePosition::Right) {
        m_rightEyeOffset = offset;
    }
}


void hd::input::Headset::SetEyeOrientation(const Quaternion &leftOrientation, const Quaternion &rightOrientation) {
	m_leftOrientation = leftOrientation;
	m_rightOrientation = rightOrientation;
}


Vec2 hd::input::Headset::GetScreenspaceOffset(const hd::input::EyePosition eyePosition, const Collision3 &collision) const {
    Vec3 eyeWorldPosition = GetEyeWorldPosition(eyePosition);
    eyeWorldPosition = eyeWorldPosition + (GetWorldOrientation() * Vec3(0, 0, 1000));

    Matrix viewProjectionMatrix = GetProjection(eyePosition) * GetLookAtMatrix(eyePosition);
    const AABB &viewport = GetViewport(eyePosition);

    Vec4 eyePos, collisionPos;
    Matrix::Vec4Multiply(eyePos, Vec4(eyeWorldPosition, 1.0f), viewProjectionMatrix);
    Matrix::Vec4Multiply(collisionPos, Vec4(collision.origin, 1.0f), viewProjectionMatrix);

    Vec3 ndcEyePos = Vec3(eyePos[0], eyePos[1], eyePos[2]) / eyePos[3];
    Vec3 ndcCollisionPos = Vec3(collisionPos[0], collisionPos[1], collisionPos[2]) / collisionPos[3];

    Vec3 diff = ndcEyePos - ndcCollisionPos;

    Vec2 offset(0.5f * viewport.Width(), 0.5f * viewport.Height());
    return Vec2(((diff[0] + 1.0) / 2.0) * viewport.Width() - offset[0],
                ((1.0f - diff[1]) / 2.0) * viewport.Height() - offset[1]);
}


void hd::input::Headset::SetPosition(const Vec3 &position) {
    m_position = position;
}


void hd::input::Headset::SetOrientation(const Quaternion &orientation) {
    m_orientation = orientation;
}


void hd::input::Headset::SetOrientationOffset(const Quaternion &orientationOffset) {
    m_orientationOffset = orientationOffset;
}


void hd::input::Headset::SetTrackingOffset(const Vec3 &trackingOffset) {
    m_trackingOffset = trackingOffset;
}


void hd::input::Headset::RotateBy(const Quaternion &delta) {
    m_orientationOffset = m_orientationOffset * delta;
}


void hd::input::Headset::SetViewport(const hd::input::EyePosition eyePosition, const AABB &aabb) {
    if (eyePosition == hd::input::EyePosition::Left) {
        m_leftViewport = aabb;
    } else if (eyePosition == hd::input::EyePosition::Right) {
        m_rightViewport = aabb;
    } else {
        m_screenViewport = aabb;
    }
}


void hd::input::Headset::SetPixelBufferSize(const hd::input::EyePosition eyePosition, const Size2 &size) {
    if (eyePosition == hd::input::EyePosition::Left) {
        m_leftPixelBuffer.SetSize(size);
    } else if (eyePosition == hd::input::EyePosition::Right) {
        m_rightPixelBuffer.SetSize(size);
    } else {
        m_screenPixelBuffer.SetSize(size);
    }
}


const AABB & hd::input::Headset::GetViewport(const hd::input::EyePosition eyePosition) const {
    if (eyePosition == hd::input::EyePosition::Left) {
        return m_leftViewport;
    } else if (eyePosition == hd::input::EyePosition::Right) {
        return m_rightViewport;
    } else {
        return m_screenViewport;
    }
}


void hd::input::Headset::SetProjection(const hd::input::EyePosition eyePosition, const Matrix &matrix) {
    if (eyePosition == hd::input::EyePosition::Left) {
        m_leftProjection = matrix;
    } else if (eyePosition == hd::input::EyePosition::Right) {
        m_rightProjection = matrix;
    } else {
        m_screenProjection = matrix;
    }
}


const Matrix& hd::input::Headset::GetProjection(const hd::input::EyePosition eyePosition) const {
    if (eyePosition == hd::input::EyePosition::Left) {
        return m_leftProjection;
    } else if (eyePosition == hd::input::EyePosition::Right) {
        return m_rightProjection;
    } else {
        return m_screenProjection;
    }
}


const Vec3 hd::input::Headset::GetEyeWorldPosition(const hd::input::EyePosition eyePosition) const {
    Vec3 eye(0, 0, 0);
	Vec3 eyeOffset;
	Quaternion orientation;

	if (eyePosition == hd::input::EyePosition::Left) {
		eyeOffset = m_leftEyeOffset;
		orientation = m_orientationOffset * m_leftOrientation;
	}
	else if (eyePosition == hd::input::EyePosition::Right) {
		eyeOffset = m_rightEyeOffset;
		orientation = m_orientationOffset * m_rightOrientation;
	}
	eye = m_position + (m_orientationOffset * eyeOffset);
    return eye;
}


const Matrix hd::input::Headset::GetLookAtMatrix(const hd::input::EyePosition eyePosition) const {
    Vec3 eye(0, 0, 0);
    Vec3 target(0, 0, -1);
    Vec3 upAxis(0, 1, 0);

    Vec3 eyeOffset;
    Quaternion orientation = m_orientationOffset; //GetWorldOrientation();

    if (eyePosition == hd::input::EyePosition::Left) {
        eyeOffset = m_leftEyeOffset;
		orientation = m_orientationOffset * m_leftOrientation;
    } else if (eyePosition == hd::input::EyePosition::Right) {
        eyeOffset = m_rightEyeOffset;
		orientation = m_orientationOffset * m_rightOrientation;
    }

	eye = m_position + (m_orientationOffset * eyeOffset);
	target = orientation * target; 
	upAxis = orientation * upAxis;

    Matrix m;
    Matrix::LookAtRH(m, eye, eye + target, upAxis);
    return m;
}


void hd::input::Headset::UpdatePixelBuffer(const hd::input::EyePosition eyePosition) {
    if (eyePosition == hd::input::EyePosition::Left) {
        m_leftPixelBuffer.Update();
    } else if (eyePosition == hd::input::EyePosition::Right) {
        m_rightPixelBuffer.Update();
    } else {
        m_screenPixelBuffer.Update();
    }
}


Color4f hd::input::Headset::GetColor(const hd::input::EyePosition eyePosition, const Vec2 &offset) const {
    if (eyePosition == hd::input::EyePosition::Left) {
        return m_leftPixelBuffer.GetColor(offset);
    } else if (eyePosition == hd::input::EyePosition::Right) {
        return m_rightPixelBuffer.GetColor(offset);
    } else {
        return m_screenPixelBuffer.GetColor(offset);
    }
}


Color4f hd::input::Headset::GetCollisionColor(const Collision3 &collision) const {
    if (m_isStereo) {
        Vec2 leftOffset = GetScreenspaceOffset(hd::input::EyePosition::Left, collision);
        Vec2 rightOffset = GetScreenspaceOffset(hd::input::EyePosition::Right, collision);

        Color4f leftColor = m_leftPixelBuffer.GetColor(leftOffset);
        Color4f rightColor = m_rightPixelBuffer.GetColor(rightOffset);

        return rightColor; //hd::lerp(leftColor, rightColor, 0.5f);
    } else {
        return m_screenPixelBuffer.GetColor(Vec2());
    }
}
