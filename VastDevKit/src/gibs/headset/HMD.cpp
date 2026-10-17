#include "HMD.h"
#include <Vast/VastGraphics.h>

hd::input::HMD::HMD() {}


int hd::input::HMD::InitDevice() {
	return 0;
}


void hd::input::HMD::InitForRendering() {}


Quaternion hd::input::HMD::GetDeviceOrientation() {
    return Quaternion();
}


void hd::input::HMD::RecenterDevice() {}


Vec3 hd::input::HMD::GetDeviceTrackingOffset() {
    return Vec3();
}


AABB hd::input::HMD::GetViewport(const hd::input::EyePosition eye) {
        float width =  hd::Device::GetDisplayScreenWidth();
        float height = hd::Device::GetDisplayScreenHeight();

        return AABB(0.0f, 0.0f, width, height);
}


static const float DEFAULT_PERSPECTIVE_FOV = 100.0f*(hd_pi/180.0f);

Matrix hd::input::HMD::GetProjectionMatrix(hd::input::EyePosition eye) {
    const float displayWidth = Device::GetDisplayScreenWidth();
    const float displayHeight = Device::GetDisplayScreenHeight();

    Matrix tmp, projection;
    Matrix::PerspectiveFovRH(projection, 0.5f * DEFAULT_PERSPECTIVE_FOV, displayWidth / displayHeight, 1.0f, 60000.0f);

    return projection;
}


static const float IPD_ADJUSTMENT_FACTOR = 300.0f;

Vec3 hd::input::HMD::GetEyeOffset(hd::input::EyePosition eye) {
    return Vec3();
}


Quaternion hd::input::HMD::GetEyeOrientation(hd::input::EyePosition eye) {
	return Quaternion();
}


float hd::input::HMD::GetIPDRadius() const {
    return 0.0f;
}


Matrix hd::input::HMD::GetInverseProjectionMatrix(hd::input::EyePosition eye) {
    const float displayWidth = Device::GetDisplayScreenWidth();
    const float displayHeight = Device::GetDisplayScreenHeight();

    Matrix projection;
    Matrix inverse;

    Matrix::PerspectiveFovRH(projection, 0.5f * DEFAULT_PERSPECTIVE_FOV, displayWidth / displayHeight, 1.0f, 60000.0f);
    Matrix::InverseLong(inverse, projection);
    return inverse;
}


void hd::input::HMD::BeginTiming() {
}


void hd::input::HMD::UpdateTiming() {
}


void hd::input::HMD::EndTiming() {
}


void hd::input::HMD::PreRender(hd::input::EyePosition eye) {
}


void hd::input::HMD::Render() {
}


void hd::input::HMD::PostRender(hd::input::EyePosition eye) {
}


void hd::input::HMD::RenderMirror() {
}
