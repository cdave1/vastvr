#include "GeometryRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

static const uint32_t MAX_MESH_INSTANCE_SIZE = 4096;
static Matrix meshInstanceData[MAX_MESH_INSTANCE_SIZE];

GeometryRenderer::GeometryRenderer(EntityPool *entityPool) :
    EntitySystem(entityPool),
    m_geometryShaderEntity(NULL),
    m_queryList(entityPool->GetCapacity()),
    m_meshList(entityPool->GetCapacity()) {}


void GeometryRenderer::UpdateGeometry(const hd::Entity *cameraEntity) {
#if 1
    const std::string BLOCK_NAME = "Block";
    EntityQuery *blocks = m_entityPool->Query<CollisionData, Transform, MeshReference>();
    
    auto shader = m_geometryShaderEntity->Edit<graphics::Shader>();

    auto orthographicData = cameraEntity->Get<hd::graphics::OrthographicData>();
    auto lookAtData = cameraEntity->Get<hd::graphics::LookAtData>();

    Matrix mvp = orthographicData->GetMatrix() * lookAtData->GetMatrix();
    Frustum frustum(mvp);

    m_queryList.Clear();
    m_meshList.Clear();

    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);
        if (entity->GetName() != "Block" && entity->GetName() != "Model") continue;

        auto opacityData = entity->GetComponent<hd::graphics::OpacityData>();
        if (opacityData && opacityData->GetOpacity() < 1.0) {
            continue;
        }

        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto mesh = entity->Edit<hd::graphics::MeshReference>();
        if (frustum.AABBInside(transform->GetOrigin(), transform->GetScale(), mesh->GetMeshData()->GetAABB())) {
            m_queryList.Add(entity);
            bool paired = false;
            for (int j = 0; j < m_meshList.Size(); ++j) {
                auto meshIndexRef = m_meshList.At(j);
                if (mesh->GetMeshData() == meshIndexRef->GetMeshData()) {
                    paired = true;
                }
            }

            if (!paired) {
                m_meshList.Add(mesh);
            }
        }
    }

    Matrix cameraMatrices[2];
    cameraMatrices[0] = orthographicData->GetMatrix();
    cameraMatrices[1] = lookAtData->GetMatrix();

    uint32_t meshInstanceCount = 0;
    for (int i = 0; i < m_meshList.Size(); ++i) {
        auto mesh = m_meshList.At(i);

        meshInstanceCount = 0;
        for (int j = 0; j < m_queryList.Size(); ++j) {
            auto entity = m_queryList.At(j);

            if (entity->Get<hd::graphics::MeshReference>()->GetMeshData() == mesh->GetMeshData()) {
                auto transform = entity->Get<hd::graphics::Transform>();
                meshInstanceData[meshInstanceCount] = transform->GetMatrix();
                meshInstanceCount++;
            }
        }



        shader->Begin();
        //mesh->GetMeshData()->SetInstancedAttributeLocations(shader);
        //mesh->GetMeshData()->SetDe(shader);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, lookAtData->GetMatrix());
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, orthographicData->GetMatrix());
        mesh->RenderDepthInstanced(cameraMatrices, meshInstanceData, meshInstanceCount);
        shader->End();
    }
#else
    #warning "Geometry renderer not rendering..."
#endif
}
