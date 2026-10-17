#include "CollisionData.h"
//#include "Geometry.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/MeshData.h"

using namespace hd::graphics;

hd::graphics::CollisionData::CollisionData(unsigned int ID) : Component(ID),
    m_meshes(),
    m_currentFrame(0),
    m_distanceToCamera(FLT_MAX),
    m_collision() {
    Reset();
}


void hd::graphics::CollisionData::Reset() {
    m_meshes = NULL;
    m_currentFrame = 0;
    m_distanceToCamera = FLT_MAX;
    m_collision = Collision3();
}


const hd::graphics::MeshData * hd::graphics::CollisionData::GetMeshData() const {
    return m_meshes;
}


void hd::graphics::CollisionData::SetMeshData(hd::graphics::MeshData *meshes) {
    m_meshes = meshes;
}


AABB hd::graphics::CollisionData::GetAABB() const {
    AABB aabb(Vec3(FLT_MAX, FLT_MAX, FLT_MAX), Vec3(FLT_MIN, FLT_MIN, FLT_MIN));

    auto meshes = m_meshes->GetMeshes();

    for (uint32_t i = 0; i < meshes->Size(); ++i) {
        auto mesh = meshes->At(i);

        AABB meshAABB = mesh->GetAABB();
        aabb.upper = hd::max(aabb.upper, meshAABB.upper);
        aabb.lower = hd::min(aabb.lower, meshAABB.lower);
    }

    return aabb;
}


void hd::graphics::CollisionData::SetCurrentFrame(const uint32_t &frame) {
    m_currentFrame = frame;
}


uint32_t hd::graphics::CollisionData::GetCurrentFrame() const {
    return m_currentFrame;
}


void hd::graphics::CollisionData::SetDistanceToCamera(const float distance) {
    m_distanceToCamera = distance;
}


float hd::graphics::CollisionData::GetDistanceToCamera() const {
    return m_distanceToCamera;
}
