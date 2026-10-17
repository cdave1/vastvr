#include "MeshData.h"
#include "Mesh.h"
#include "MeshVertexBuffer.h"

hd::graphics::MeshData::MeshData(uint32_t id) : m_id(id),
    m_meshes(NULL),
    m_prototype() {
    m_meshes = new hd::Vector<hd::graphics::Mesh *>(128);
}


hd::graphics::MeshData::~MeshData() {
    DeleteSafely(m_meshes);
}


hd::Vector<hd::graphics::Mesh *> * hd::graphics::MeshData::GetMeshes() const {
    return m_meshes;
}


const AABB hd::graphics::MeshData::GetAABB() const {
    return m_aabb;
}


void hd::graphics::MeshData::UploadMeshData() {
    for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *mesh = m_meshes->At(i);
        mesh->UploadMeshVertexBuffer();
    }
}


void hd::graphics::MeshData::SetDefaultAttributeLocations(const Shader *shader) const {
	for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *mesh = m_meshes->At(i);
        mesh->GetMeshVertexBuffer()->SetDefaultAttributeLocations(shader);
    }
}


void hd::graphics::MeshData::SetInstancedAttributeLocations(const Shader *shader) const {
	for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *mesh = m_meshes->At(i);
        mesh->GetMeshVertexBuffer()->SetInstancedAttributeLocations(shader);
    }
}


void hd::graphics::MeshData::RenderDepthInstanced(const Matrix *cameraData, const Matrix *instanceData, const uint32_t count) const {
	for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *mesh = m_meshes->At(i);
        mesh->GetMeshVertexBuffer()->RenderDepthInstanced(cameraData, instanceData, count);
    }
}


void hd::graphics::MeshData::RenderInstanced(const MeshInstanceData *renderData, const uint32_t count) const {
	for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *mesh = m_meshes->At(i);
        mesh->GetMeshVertexBuffer()->RenderInstanced(renderData, count);
    }
}


void hd::graphics::MeshData::RenderLines() const {
	for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *mesh = m_meshes->At(i);
        mesh->GetMeshVertexBuffer()->RenderLines();
    }
}


void hd::graphics::MeshData::Render() const {
	for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *mesh = m_meshes->At(i);
        mesh->GetMeshVertexBuffer()->Render();
    }
}


void hd::graphics::MeshData::AddMesh(hd::graphics::Mesh *mesh) {
    m_meshes->Add(mesh);
}


bool hd::graphics::MeshData::ContainsMeshPath(const std::string &path) const {
    return false;
}


void hd::graphics::MeshData::GenerateExtents() {
    if (m_meshes->Size() == 0) {
        m_aabb.lower.Set(0.0f, 0.0f, 0.0f);
        m_aabb.upper.Set(0.0f, 0.0f, 0.0f);
        return;
    }

    hd::graphics::Mesh *mesh = m_meshes->At(0);
    m_aabb.lower = mesh->GetAABB().lower;
    m_aabb.upper = mesh->GetAABB().upper;

    for (uint32_t i = 0; i < m_meshes->Size(); ++i) {
        hd::graphics::Mesh *_mesh = m_meshes->At(i);
        _mesh->GenerateExtents();

        m_aabb.lower = hd::min(m_aabb.lower, _mesh->GetAABB().lower);
        m_aabb.upper = hd::max(m_aabb.upper, _mesh->GetAABB().upper);
    }
}
