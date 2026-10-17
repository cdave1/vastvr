#include "TransformStack.h"
#include "math/Geometry.h"
#include "graphics/gl/GraphicsInterface.h"

hd::graphics::TransformStack::TransformStack(unsigned int ID) : Component(ID), m_origin() {
    Reset();
}


void hd::graphics::TransformStack::Reset() {
    m_origin.Set(0, 0, 0);
    m_scale.Set(1, 1, 1);
    m_rotation.SetIdentity();
    Matrix::Identity(m_matrix);
    UpdateMatrix();
}


void hd::graphics::TransformStack::SetOrigin(const Vec3 &origin) {
    m_origin = origin;
    UpdateMatrix();
}


const Vec3& hd::graphics::TransformStack::GetOrigin() const {
    return m_origin;
}


void hd::graphics::TransformStack::SetScale(const Vec3 &scale) {
    m_scale = scale;
    UpdateMatrix();
}


const Vec3& hd::graphics::TransformStack::GetScale() const {
    return m_scale;
}


void hd::graphics::TransformStack::SetScale(const float scale) {
    m_scale.Set(scale, scale, scale);
    UpdateMatrix();
}


void hd::graphics::TransformStack::SetRotation(const Vec3 &rotation, float theta) {
    m_rotation = Quaternion(rotation, theta);
    UpdateMatrix();
}


void hd::graphics::TransformStack::SetRotation(const Quaternion &quat) {
    m_rotation = quat;
    UpdateMatrix();
}


const Quaternion & hd::graphics::TransformStack::GetRotation() const {
    return m_rotation;
}


void hd::graphics::TransformStack::UpdateMatrix() {
    Matrix trans, rot, scale;
    Matrix::Translation(trans, m_origin[0], m_origin[1], m_origin[2]);
    Matrix::FromQuaternion(rot, m_rotation);
    Matrix::Scale(scale, m_scale);
    m_matrix = trans * rot * scale;
}


const Matrix & hd::graphics::TransformStack::GetMatrix() const {
    return m_matrix;
}
