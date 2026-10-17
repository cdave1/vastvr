#include "RayTracer.h"
#include "math/Geometry.h"
#include "core/device/Device.h"

using namespace hd::graphics;

RayTracer::RayTracer(unsigned int ID) : Component(ID) {
    Reset();
}


void RayTracer::Reset() {
}


Vec3 RayTracer::PointerToWorld(const Vec3 &pointer,
                               const AABB &viewport,
                               const Matrix &projection,
                               const Matrix &modelViewMatrix) const {
    Vec3 posn(pointer[0], pointer[1], 1.0f);
    Vec3 output = hd::unproject(posn, modelViewMatrix, projection, viewport);
    return output;
}
