#include "Position.h"
#include "core/entity/Entity.h"
#include "core/device/Device.h"
#include "graphics/components/ScreenData.h"

using namespace hd::input;

pointer::Position::Position(unsigned int ID) : Component(ID),
    m_position(),
    m_delta(),
    m_coordinateSpace() {}


void pointer::Position::SetCoordinateSpace(const AABB &aabb) {
    m_coordinateSpace = aabb;
}


void pointer::Position::SetPosition(const Vec3 &position) {
    m_position = position;
}


void pointer::Position::SetDelta(const Vec3 &delta) {
    m_delta = delta;
}


const Vec3 & pointer::Position::GetPosition() const {
    return m_position;
}


const Vec3 & pointer::Position::GetDelta() const {
    return m_delta;
}


const AABB & pointer::Position::GetCoordinateSpace() const {
    return m_coordinateSpace;
}


void pointer::Position::PrintDebugInfo() const {
    hd::log::i("\tpointer::Position: Position: %3.1f %3.1f \n", m_position[0], m_position[1]);
}


Vec3 pointer::Position::GetDelta(hd::Entity *camera) const {
    auto screenData = camera->GetComponent<hd::graphics::ScreenData>();
    if (!screenData) return m_delta;
    
    Vec3 cameraBox = screenData->GetAABB().upper - screenData->GetAABB().lower;
    
    float unitsPerPixelX = cameraBox[0] / hd::Device::GetDisplayScreenWidth();
	float unitsPerPixelY = cameraBox[1] / hd::Device::GetDisplayScreenHeight();
    
    return Vec3((m_delta[0] * unitsPerPixelX),
                (m_delta[1] * unitsPerPixelY),
                m_delta[2]);
}


Vec3 pointer::Position::GetPosition(hd::Entity *camera) const {
    auto screenData = camera->GetComponent<hd::graphics::ScreenData>();
    if (!screenData) return m_position;
    
    Vec3 cameraBox = screenData->GetAABB().upper - screenData->GetAABB().lower;
    
    float unitsPerPixelX = cameraBox[0] / hd::Device::GetDisplayScreenWidth();
	float unitsPerPixelY = cameraBox[1] / hd::Device::GetDisplayScreenHeight();
    
	return Vec3((m_position[0] * unitsPerPixelX) + screenData->GetAABB().lower[0],
                (m_position[1] * unitsPerPixelY) + screenData->GetAABB().lower[1],
                m_position[2]);
}
