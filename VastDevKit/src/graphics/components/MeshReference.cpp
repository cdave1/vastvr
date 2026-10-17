#include "MeshReference.h"
#include "graphics/mesh/MeshData.h"

using namespace hd;
using namespace hd::graphics;

MeshReference::MeshReference(unsigned int ID) : Component(ID),
    m_mesh(nullptr) {}


void MeshReference::Reset() {
    m_mesh = nullptr;
}


void MeshReference::RenderDepthInstanced(const Matrix *cameraData, const Matrix *instanceData, const uint32_t count) const {
    if (m_mesh) {
        m_mesh->RenderDepthInstanced(cameraData, instanceData, count);
    }
}


void MeshReference::RenderInstanced(const MeshInstanceData *instanceData, const uint32_t count) const {
    if (m_mesh) {
        m_mesh->RenderInstanced(instanceData, count);
    }
}


void MeshReference::RenderLines() const {
    if (m_mesh) {
        m_mesh->RenderLines();
    }
}


void MeshReference::Render() const {
    if (m_mesh) {
        m_mesh->Render();
    }
}