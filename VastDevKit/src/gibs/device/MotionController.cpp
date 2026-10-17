#include "MotionController.h"

using namespace hd::input;

device::MotionController::MotionController(unsigned int ID) : Component(ID),
    m_deviceId(hd::input::UnspecifiedDeviceId),
    m_position(),
    m_trackingOffset(),
    m_orientation(),
    m_orientationOffset()
{}


int device::MotionController::GetDeviceId() const {
    return m_deviceId;
}


void device::MotionController::Reset() {
    m_position = Vec3();
    m_trackingOffset = Vec3();

    m_orientation = Quaternion();
    m_orientationOffset = Quaternion();
}


const Vec3 device::MotionController::GetPosition() const {
    return m_position;
}


const Vec3 device::MotionController::GetTrackingOffset() const {
    return m_trackingOffset;
}


const Vec3 device::MotionController::GetWorldPosition() const {
    return m_position + (m_orientationOffset * m_orientation * m_trackingOffset);
}


const Quaternion device::MotionController::GetOrientation() const {
    return m_orientation;
}


const Quaternion device::MotionController::GetOrientationOffset() const {
    return m_orientationOffset;
}


const Quaternion device::MotionController::GetWorldOrientation() const {
    return m_orientationOffset * m_orientation;
}


void device::MotionController::Update() {

}


void device::MotionController::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


void device::MotionController::SetPosition(const Vec3 &position) {
    m_position = position;
}


void device::MotionController::SetOrientation(const Quaternion &orientation) {
    m_orientation = orientation;
}


void device::MotionController::SetOrientationOffset(const Quaternion &orientationOffset) {
    m_orientationOffset = orientationOffset;
}


void device::MotionController::SetTrackingOffset(const Vec3 &trackingOffset) {
    m_trackingOffset = trackingOffset;
}


void device::MotionController::RotateBy(const Quaternion &delta) {
    m_orientationOffset = m_orientationOffset * delta;
}


const Matrix device::MotionController::GetLookAtMatrix() const {
    Vec3 eye(0, 0, 0);
    Vec3 target(0, 0, 1);
    Vec3 upAxis(0, 1, 0);

    Vec3 eyeOffset;

    eye = m_orientationOffset * m_orientation * (eye + eyeOffset);
    target = m_orientationOffset * m_orientation * (target + eyeOffset);
    upAxis = m_orientationOffset * m_orientation * upAxis;

    Vec3 worldPosition = GetWorldPosition();

    Matrix m;
    Matrix::LookAtRH(m, worldPosition + eye, worldPosition + target, upAxis);
    return m;
}


void device::MotionController::CopyMotionController(const MotionController *motionController) {
    this->SetDeviceId(motionController->GetDeviceId());
    this->SetPosition(motionController->GetPosition());
    this->SetTrackingOffset(motionController->GetTrackingOffset());
    this->SetOrientation(motionController->GetOrientation());
    this->SetOrientationOffset(motionController->GetOrientationOffset());
}
