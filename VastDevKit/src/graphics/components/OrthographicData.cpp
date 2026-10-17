#include "OrthographicData.h"
#include "math/Geometry.h"

using namespace hd::graphics;

OrthographicData::OrthographicData(unsigned int ID) : Component(ID),
m_aabb(), m_orthoMat() {
    Reset();
}


void OrthographicData::Reset() {
    m_aabb = AABB(0, 0, 1, 1);
    Update();
}


void OrthographicData::Update() {
    Matrix::Ortho(m_orthoMat,
                             m_aabb.lower[0], m_aabb.upper[0],
                             m_aabb.lower[1], m_aabb.upper[1],
                             -30000.0f, 30000.0f);
}


void OrthographicData::SetAABB(const AABB &aabb) {
    m_aabb = aabb;
    Update();
}


const AABB& OrthographicData::GetAABB() const {
    return m_aabb;
}


void OrthographicData::SetMatrix(const Matrix &m) {
    m_orthoMat = m;
}


const Matrix & OrthographicData::GetMatrix() const {
    return m_orthoMat;
}
