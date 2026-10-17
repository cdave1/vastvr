#include "LookAtData.h"

using namespace hd::graphics;

LookAtData::LookAtData(unsigned int ID) : Component(ID),
    m_eye(0, 0, 0),
    m_target(0, 0, 1),
    m_upAxis(0, 1, 0),
    m_location(0, 0, 0),
    m_offset(0, 0, 0),
    m_rotation(),
    m_rotationOffset() {
    Reset();
}


void LookAtData::Reset() {
    Matrix::Identity(m_matrix);
    
    m_eye = Vec3(0, 0, 0);
    m_target = Vec3(0, 0, 1);
    m_upAxis = Vec3(0, 1, 0);
    m_location = Vec3(0, 0, 0);
    m_offset = Vec3(0, 0, 0);
    m_rotation.SetIdentity();
    m_rotationOffset.SetIdentity();

    Update();
}


void LookAtData::Update() {
    Quaternion rotation = m_rotation * m_rotationOffset;

    Vec3 eye = (rotation * (m_eye + m_offset));
    Vec3 target = (rotation * (m_target + m_offset));
    Vec3 up = rotation * m_upAxis;

    Matrix::LookAtRH(m_matrix, m_location + eye, m_location + target, up);
}


void LookAtData::SetEye(const Vec3 &eye) {
    m_eye = eye;
    Update();
}


const Vec3 & LookAtData::GetEye() const {
    return m_eye;
}


void LookAtData::SetTarget(const Vec3 &target) {
    m_target = target;
    Update();
}


Vec3 LookAtData::GetTarget() const {
    Quaternion rotation = m_rotation * m_rotationOffset;
    Vec3 target = (rotation * (m_target + m_offset));
    return target;
}


Vec3 LookAtData::GetRealTarget() const {
    Quaternion rotation = m_rotation * m_rotationOffset;
    Vec3 target = (rotation * (m_target + m_offset));
    return target;
}


void LookAtData::SetUpAxis(const Vec3 &upAxis) {
    m_upAxis = upAxis;
    Update();
}


const Vec3 & LookAtData::GetUpAxis() const {
    return m_upAxis;
}


const Vec3 & LookAtData::GetRawLocation() const {
    return m_location;
}


void LookAtData::SetLocation(const Vec3 &location) {
    m_location = location;
    Update();
}


Vec3 LookAtData::GetLocation() const {
    Vec3 eye = m_rotation * (m_eye + m_offset);
    return m_location + eye;
}


void LookAtData::Rotate(const Quaternion &delta) {
    m_rotation = m_rotation * delta;
    Update();
}


void LookAtData::RotateOffset(const Quaternion &quat) {
    m_rotationOffset = m_rotationOffset * quat;
    Update();
}


void LookAtData::SetRotationOffset(const Quaternion &quat) {
    m_rotationOffset = quat;
    Update();
}


const Quaternion & LookAtData::GetRotationOffset() const {
    return m_rotationOffset;
}


Vec3 LookAtData::GetOffset() const {
    return m_offset;
}


void LookAtData::SetOffset(const Vec3 &offset) {
    m_offset = offset;
}


void LookAtData::FollowTarget(const Vec3 &target) {
    Vec3 eye = m_rotation * m_eye;
    Vec3 up = m_rotation * m_upAxis;

    Matrix::LookAtRH(m_matrix, m_location, target, up);
}


void LookAtData::SetRotation(const Quaternion &quat) {
    m_rotation = quat;
    Update();
}


const Quaternion & LookAtData::GetRotation() const {
    return m_rotation;
}


void LookAtData::SetMatrix(const Matrix &m) {
    m_matrix = m;
}


const Matrix & LookAtData::GetMatrix() const {
    return m_matrix;
}
