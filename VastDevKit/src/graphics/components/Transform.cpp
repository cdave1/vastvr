#include "Transform.h"
#include "math/Geometry.h"
#include "graphics/gl/GraphicsInterface.h"

using namespace hd::graphics;

Transform::Transform(unsigned int ID) : Component(ID),
    m_origin(),
    m_scale(1.0f),
    m_rotation(),
    m_matrix(),
    m_contextMatrix(),
    m_isDirty(false) {
    Reset();
}


void Transform::Reset() {
    m_origin.Set(0, 0, 0);
    m_scale.Set(1, 1, 1);
    m_rotation.SetIdentity();
    Matrix::Identity(m_matrix);
    Matrix::Identity(m_contextMatrix);
    m_isDirty = false;
}


void Transform::SetOrigin(const Vec3 &origin) {
    m_origin = origin;
    m_isDirty = true;
}


const Vec3& Transform::GetOrigin() const {
    return m_origin;
}


void Transform::SetScale(const Vec3 &scale) {
    m_scale = scale;
    m_isDirty = true;
}


const Vec3& Transform::GetScale() const {
    return m_scale;
}


void Transform::SetScale(const float scale) {
    m_scale.Set(scale, scale, scale);
    m_isDirty = true;
}


void Transform::SetRotation(const Vec3 &rotation, float theta) {
    m_rotation = Quaternion(rotation, theta);
    m_isDirty = true;
}


void Transform::SetRotation(const Quaternion &quat) {
    m_rotation = quat;
    m_isDirty = true;
}


const Quaternion & Transform::GetRotation() const {
    return m_rotation;
}


void Transform::Update() {
    if (m_isDirty) {
        Matrix trans, rot, scale;
        Matrix::Translation(trans, m_origin[0], m_origin[1], m_origin[2]);
        Matrix::FromQuaternion(rot, m_rotation);
        Matrix::Scale(scale, m_scale);
        m_matrix = m_contextMatrix * (trans * rot * scale);
        m_isDirty = false;
    }
}


void Transform::SetContext(const Matrix &context) {
    m_isDirty = true;
    m_contextMatrix = context;
}


const Matrix & Transform::GetMatrix() const {
    return m_matrix;
}


const Matrix & Transform::GetContextMatrix() const {
    return m_contextMatrix;
}
