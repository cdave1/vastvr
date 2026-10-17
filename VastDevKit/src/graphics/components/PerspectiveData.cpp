#include "PerspectiveData.h"

const float kDefaultFovY = 90.0f*(hd_pi/180.0f);
const float kDefaultAspectY = 1.0f;
const float kDefaultNear = 1.0f;
const float kDefaultFar = 60000.0f;

using namespace hd::graphics;

PerspectiveData::PerspectiveData(unsigned int ID) : Component(ID) {
    Reset();
}


void PerspectiveData::Reset() {
    SetPerspective(kDefaultFovY,
                   kDefaultAspectY,
                   kDefaultNear,
                   kDefaultFar);
    Matrix::Identity(m_perspectiveMat);
}


void PerspectiveData::Update() {
    Matrix::PerspectiveFovRH(m_perspectiveMat, m_fovY, m_aspectY, m_near, m_far);
}


void PerspectiveData::SetFovY(float fovY) {
    m_fovY = fovY;    
}


void PerspectiveData::SetAspectY(float aspectY) {
    m_aspectY = aspectY;
}


void PerspectiveData::SetPerspective(float fovY,
                                     float aspectY,
                                     float _near,
                                     float _far) {
    m_fovY = fovY;
    m_aspectY = aspectY;
    m_near = _near;
    m_far = _far;
}


void PerspectiveData::SetMatrix(const Matrix &m) {
    m_perspectiveMat = m;
}


const Matrix & PerspectiveData::GetMatrix() const {
    return m_perspectiveMat;
}


const Matrix PerspectiveData::GetMatrixInverse() const {
    Matrix inverse;
    Matrix::InverseLong(inverse, m_perspectiveMat);
    return inverse;
}
