#include "Curve.h"
#include "math/Geometry.h"
#include "graphics/gl/GraphicsInterface.h"

using namespace hd::graphics;

Curve::Curve(unsigned int ID) : Component(ID) {
    Reset();
}


void Curve::Reset() {
    m_p1.Set(0, 0, 0);
    m_p2.Set(1, 1, 0);
    m_c1.Set(0, 0, 0);
    m_c2.Set(1, 1, 0);
}


void Curve::AddCurve(const Vec3 &p1, const Vec3 &p2, const Vec3 &c1, const Vec3 &c2) {
    m_p1.Set(p1);
    m_p2.Set(p2);
    m_c1.Set(c1);
    m_c2.Set(c2);
}


Vec3 Curve::GetPoint(unsigned int index) const {
    if (index == 0) return m_p1;
    else return m_p2;
}


Vec3 Curve::GetControlPoint(unsigned int index) const {
    if (index == 0) return m_c1;
    else return m_c2;
}