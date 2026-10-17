#include "Projection.h"

using namespace hd::graphics;

Projection::Projection(unsigned int ID) : Component(ID) {
    Reset();
}


void Projection::Reset() {
    m_projectionMatrix = Matrix::Identity();
    m_inverseProjectionMatrix = Matrix::Identity();
}


void Projection::SetMatrix(const Matrix &m) {
    m_projectionMatrix = m;
}


void Projection::SetInverseMatrix(const Matrix &m) {
    m_inverseProjectionMatrix = m;
}


const Matrix & Projection::GetMatrix() const {
    return m_projectionMatrix;
}


const Matrix & Projection::GetInverseMatrix() const {
    return m_inverseProjectionMatrix;
}
