#include "AppPrototype.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

const std::string BLOCK_ENTITY_NAME = "Block";

AppPrototype::AppPrototype(hd::application::Client *client) : m_entityPool(4096),
    m_transformSystem(&m_entityPool),
    m_blockMover(&m_entityPool),
    m_labelRenderer(&m_entityPool),
    m_blockInputSystem(&m_entityPool, client, BLOCK_ENTITY_NAME),
    m_skyboxRenderer(&m_entityPool),
    m_deferredLightSystem(&m_entityPool, client),
    m_geometryRenderer(&m_entityPool),
    m_animationSystem(&m_entityPool),
    m_alarmSystem(&m_entityPool),
    m_musicPlayer(),
    m_client(client),
    m_sceneCulling(&m_entityPool, client),
    m_modelRenderer(&m_entityPool, client, "Model"),
    m_sceneRenderer(&m_entityPool, client, BLOCK_ENTITY_NAME) {
        m_musicPlayer.Init();
        CreateGlobalLight(Vec3(0.0f, 4000.0f, 0.0f));
}

void AppPrototype::OnActivate(hd::Entity *camera) {
    auto headset = camera->Edit<Headset>();
    headset->SetPosition(Vec3(0, 450, 1400));
    headset->SetOrientationOffset(Quaternion::FromTwoVectors(Vec3(0, 0, -1), Vec3(0, -0.25f, -1)));
}

void AppPrototype::OnDeactivate() {
    m_musicPlayer.Stop();
}

void AppPrototype::OnBlockShaderRenderCallback(Shader *shader) {
}


void AppPrototype::ConstructSkybox(unsigned index) {
    Entity *sphere = m_entityPool.Create();
    sphere->SetName("sphere");
    
    auto transform = sphere->Add<Transform>();
    auto shader = sphere->Add<Shader>();
    auto meshRef = sphere->Add<MeshReference>();
    auto material = sphere->Add<Material>();

    if (index == 0) {
        material->SetTexture(TextureManager::Instance()->FindTexture("resources/noise/noise.png", hd::TextureType::Wall));
        shader->LoadShader("tex0.vert", "galaxy.frag");
    } else if (index == 1) {
        material->SetTexture(TextureManager::Instance()->FindTexture("resources/images/gradient5.png", hd::TextureType::Wall));
        shader->LoadShader("tex0.vert", "tex0.frag");
    } else if (index == 2) {
        material->SetTexture(TextureManager::Instance()->FindTexture("resources/images/gradient3.png", hd::TextureType::Wall));
        shader->LoadShader("tex0.vert", "tex0.frag");
    } else if (index == 3) {
        material->SetTexture(TextureManager::Instance()->FindTexture("resources/images/gradient5.png", hd::TextureType::Wall));
        shader->LoadShader("tex0.vert", "tex0.frag");
    }
    auto meshData = hd::graphics::MeshLoader::Instance()->GetInverseSphere(50000.0f, 32);
    meshRef->SetMeshReference(meshData);
    meshRef->GetMeshData()->SetDefaultAttributeLocations(shader);
}


void AppPrototype::CreateLights() {
    /*
    auto lightEntity = m_entityPool.Create();
    
    auto transform = hd::Components::Get<Transform>();
    auto light = hd::Components::Get<Light>();
    auto lightModel = hd::Components::Get<LightModel>();
    auto lightRenderer = hd::Components::Get<LightRenderer>();
    
    transform->SetOrigin(Vec3(100, 600, 0));
    
    light->SetSpotExponent(hd_pi);
    light->SetSpotCutoff(180);
    light->SetConstantAttenuation(1.0f);
    light->SetLinearAttenuation(0.5f);
    light->SetQuadraticAttenuation(0.5f);
    light->SetIsDirectional(false);
    
    light->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    light->SetDiffuse(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
    light->SetSpecular(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    
    lightModel->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    
    lightEntity->AddComponent(transform);
    lightEntity->AddComponent(light);
    lightEntity->AddComponent(lightModel);
    lightEntity->AddComponent(lightRenderer);*/
}


void AppPrototype::CreateGlobalLight(const Vec3 &position) {
    if (m_geometryRenderer.m_geometryShaderEntity == NULL) {
        m_geometryRenderer.m_geometryShaderEntity = m_entityPool.Create();
        auto shader = hd::Components::Get<Shader>();

        shader->LoadShader("geometry.vert", "geometry.frag");
        
        m_geometryRenderer.m_geometryShaderEntity->AddComponent(shader);
    }
    
    auto globalLightEntity = m_entityPool.Create();
    globalLightEntity->SetName("GlobalLight");
    
    auto shader = globalLightEntity->AddComponent<Shader>();
    auto perspectiveData = globalLightEntity->AddComponent<PerspectiveData>();
    auto lookAtData = globalLightEntity->AddComponent<LookAtData>();
    auto viewport = globalLightEntity->AddComponent<Viewport>();
    auto orthographicData = globalLightEntity->AddComponent<OrthographicData>();
    auto depthRenderTarget = globalLightEntity->AddComponent<DepthRenderTarget>();
    
    const float displayWidth = Device::GetDisplayScreenWidth();
    const float displayHeight = Device::GetDisplayScreenHeight();
    
    perspectiveData->SetFovY(hd_half_pi);
    perspectiveData->SetAspectY(displayWidth / displayHeight);
    perspectiveData->Update();

    lookAtData->SetLocation(Vec3(0, 0, 0));
    lookAtData->SetEye(Vec3(0, 1, 0));
    lookAtData->SetTarget(Vec3(0, 0, 0));
    lookAtData->SetUpAxis(Vec3(0, 1, 0));
    lookAtData->Update();

    lookAtData->SetLocation(position);
    
    AABB targetSize(0, 0, displayWidth, displayHeight);
    viewport->SetAABB(targetSize);
    
    depthRenderTarget->Resize(targetSize.GetSize2());
    depthRenderTarget->OnRenderGeometry(&m_geometryRenderer, &GeometryRenderer::UpdateGeometry);
    
    const float ORTHOGRAPHIC_SCALE = 0.25f;
    orthographicData->SetAABB(AABB(-displayWidth * ORTHOGRAPHIC_SCALE,
                                   -displayHeight * ORTHOGRAPHIC_SCALE,
                                    displayWidth * ORTHOGRAPHIC_SCALE * 2.0f,
                                    displayHeight * ORTHOGRAPHIC_SCALE * 2.0f));
    orthographicData->Update();

    shader->LoadShader("geometry.vert", "geometry.frag");

    auto light = globalLightEntity->AddComponent<Light>();
    auto lightModel = globalLightEntity->AddComponent<LightModel>();

    light->SetSpotExponent(hd_pi);
    light->SetSpotCutoff(180);
    light->SetConstantAttenuation(1.0f);
    light->SetLinearAttenuation(0.5f);
    light->SetQuadraticAttenuation(0.5f);
    light->SetIsDirectional(false);
    light->SetPosition(position);

    light->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    light->SetDiffuse(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
    light->SetSpecular(Color4f(1.0f, 1.0f, 1.0f, 1.0f));

    lightModel->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
}


void AppPrototype::AddBackground(const float width, const float height, const Vec3 &rotation, const Vec3 &center) {
    /*
    if (m_entityPool.Empty()) {
        return;
    }
    
    Entity *panel = m_entityPool.Create();
    panel->SetName("background");
    
    auto transform = panel->AddComponent<Transform>();
    auto polygon = panel->AddComponent<Polygon>();
    auto renderer = panel->AddComponent<PolygonRenderer>();
    auto shader = panel->AddComponent<Shader>();
    auto buffer = panel->AddComponent<VertexBuffer>();
    auto mat = panel->AddComponent<Material>();
    
    transform->SetOrigin(center);
    //transform->SetRotation(rotation);
    
    polygon->Create2DBox(AABB(width, height));
    
    mat->SetTexture(TextureManager::Instance()->FindTexture("resources/noise/noise.png", hd::TextureType::Wall));
    
    shader->Reset();
    shader->LoadShader("Shader.vert", "solidBackground.frag");
    
    buffer->SetFromPolygon(polygon);
    buffer->SetDefaultAttributeLocations(shader);
     */
}


Entity * AppPrototype::AddMenuItem(Vec3 center,
                                   float yrot,
                                   const std::string imagePath,
                                   const std::string identifierString,
                                   float radius) {
    if (m_entityPool.Empty()) {
        hd::log::e("Ran out of entities.\n");
        return NULL;
    }
    
    Entity *menuItem = m_entityPool.Create();
    menuItem->SetName(BLOCK_ENTITY_NAME);

    auto transform = menuItem->AddComponent<Transform>();
    auto renderer = menuItem->AddComponent<PolygonRenderer>();
    auto shader = menuItem->AddComponent<Shader>();
    auto buffer = menuItem->AddComponent<VertexBuffer>();
    auto material = menuItem->AddComponent<Material>();
    auto identifier = menuItem->AddComponent<ui::Identifier>();
    auto collisionMesh = menuItem->AddComponent<CollisionData>();
    auto opacityData = menuItem->AddComponent<OpacityData>();
    
    auto responder = menuItem->AddComponent<pointer::ResponderState>();
    auto selectResponder = menuItem->AddComponent<pointer::SelectResponder>();
    auto dragResponder = menuItem->AddComponent<pointer::DragResponder>();
    auto releaseResponder = menuItem->AddComponent<pointer::ReleaseResponder>();
    
    transform->SetOrigin(center);
    transform->SetRotation(Quaternion(Vec3(0, -1, 0), yrot - hd_half_pi));

    shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongTexture.frag");

    auto meshData = hd::graphics::MeshLoader::Instance()->GetCylinder(radius, 20.0f, 6);
    hd::Vector<hd::graphics::Mesh *> *meshes = meshData->GetMeshes();
    for (int i = 0; i < meshes->GetSize(); ++i) {
        hd::graphics::Mesh *mesh = meshes->At(i);
        buffer->AddMesh(mesh);
    }
    buffer->SetDefaultAttributeLocations(shader);
    collisionMesh->SetMeshData(meshData);
    collisionMesh->OnGetTransform([transform]() { return transform->GetMatrix(); });
    menuItem->Add<LookAtResponder>();
    opacityData->SetOpacity(1.0f);

    identifier->SetId(identifierString);
    
    material->SetAmbient(Color4f(0.2f, 0.2f, 0.2f, 1.0f));
    material->SetDiffuse(Color4f(0.4f, 0.4f, 0.5f, 1.0f));
    material->SetSpecular(Color4f(0.8f, 0.8f, 0.8f, 1.0f));
    material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
    material->SetShininess(20.0f);
    material->SetTexture(TextureManager::Instance()->FindTexture(imagePath.c_str(), hd::TextureType::Wall));

    auto timer = menuItem->AddComponent<hd::animation::AnimationTimer>();
    auto positionBlock = menuItem->AddComponent<hd::animation::PositionBlock>();
    auto opacityBlock = menuItem->AddComponent<hd::animation::OpacityBlock>();

    timer->SetDuration(1.0f);

    positionBlock->SetStartPosition(center);

    opacityBlock->SetStartOpacity(0.0f);
    opacityBlock->SetEndOpacity(1.0f);

    return menuItem;
}


void AppPrototype::AddStaticCube(float size, Vec3 center) {
    if (m_entityPool.Empty()) {
        hd::log::e("Ran out of entities.\n");
        return;
    }
    
    Entity *block = m_entityPool.Create();
    block->SetName(BLOCK_ENTITY_NAME);
    
    auto transform = block->AddComponent<Transform>();
    auto renderer = block->AddComponent<PolygonRenderer>();
    auto shader = block->AddComponent<Shader>();
    auto buffer = block->AddComponent<VertexBuffer>();
    auto material = block->AddComponent<Material>();
    auto collisionMesh = block->Add<CollisionData>();
    
    auto responder = block->AddComponent<pointer::ResponderState>();
    auto selectResponder = block->AddComponent<pointer::SelectResponder>();
    auto dragResponder = block->AddComponent<pointer::DragResponder>();
    auto releaseResponder = block->AddComponent<pointer::ReleaseResponder>();
    
    transform->SetOrigin(center);

    shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");
    shader->SetPreRenderCallback<AppPrototype>(this, &AppPrototype::OnBlockShaderRenderCallback);

    auto meshData = hd::graphics::MeshLoader::Instance()->GetCube(Vec3(size));
    hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
    buffer->AddMesh(mesh);
    block->Add<MeshReference>()->SetMeshReference(meshData);
    buffer->SetDefaultAttributeLocations(shader);
    collisionMesh->SetMeshData(meshData);
    collisionMesh->OnGetTransform([transform]() { return transform->GetMatrix(); });
    block->Add<LookAtResponder>();

    material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    material->SetDiffuse(Color4f(0.4f, 0.4f, 0.5f, 1.0f));
    material->SetSpecular(Color4f(0.8f, 0.8f, 0.7f, 1.0f));
    material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
    material->SetShininess(0.5f);
}


Entity * AppPrototype::AddCube(float width, float height, float depth, Vec3 center) {
    if (m_entityPool.Empty()) {
        hd::log::e("Ran out of entities.\n");
        return nullptr;
    }

    Entity *block = m_entityPool.Create();
    block->SetName(BLOCK_ENTITY_NAME);

    auto transform = block->AddComponent<Transform>();
    auto renderer = block->AddComponent<PolygonRenderer>();
    auto shader = block->AddComponent<Shader>();
    auto buffer = block->AddComponent<VertexBuffer>();
    auto material = block->AddComponent<Material>();
    auto collisionMesh = block->Add<CollisionData>();

    transform->SetOrigin(center);

    shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

    auto meshData = hd::graphics::MeshLoader::Instance()->GetCube(Vec3(width, height, depth));
    hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
    buffer->AddMesh(mesh);
    block->Add<MeshReference>()->SetMeshReference(meshData);
    buffer->SetDefaultAttributeLocations(shader);
    collisionMesh->SetMeshData(meshData);
    collisionMesh->OnGetTransform([transform]() { return transform->GetMatrix(); });
    block->Add<LookAtResponder>();

    material->SetAmbient(Color4f(0.1f, 0.4f, 0.8f, 1.0f));
    material->SetDiffuse(Color4f(0.4f, 0.4f, 0.4f, 1.0f));
    material->SetSpecular(Color4f(0.9f, 0.9f, 0.9f, 1.0f));
    material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
    material->SetShininess(0.6f);

    block->AddComponent<pointer::ResponderState>();
    block->AddComponent<pointer::SelectResponder>();
    block->AddComponent<pointer::DragResponder>();
    block->AddComponent<pointer::ReleaseResponder>();

    return block;
}


Entity * AppPrototype::AddFloorCube(float size, float height, Vec3 center) {
    return AddCube(size, height, size, center);
}


Entity * AppPrototype::AddMesh(const std::string &meshFile, float size, Vec3 center) {
    hd::graphics::MeshLoader *loader = hd::graphics::MeshLoader::Instance();
    hd::graphics::MeshData *meshData = loader->GetMeshFromFile(meshFile);

    if (!meshData) {
        hd::log::e("[%s] Mesh data could not be found for: %s\n", __FUNCTION__, meshFile.c_str());
        return nullptr;
    }

    auto entity = m_entityPool.Create();
    entity->SetName("Model");

    auto transform = entity->Add<Transform>();
    auto shader = entity->Add<Shader>();
    auto mesh = entity->Add<hd::graphics::MeshReference>();
    auto material = entity->Add<Material>();
    auto collisionMesh = entity->Add<CollisionData>();

    auto responder = entity->Add<pointer::ResponderState>();
    auto selectResponder = entity->Add<pointer::SelectResponder>();
    auto dragResponder = entity->Add<pointer::DragResponder>();
    auto releaseResponder = entity->Add<pointer::ReleaseResponder>();

    transform->SetOrigin(center);
    transform->SetScale(size);
    transform->SetRotation(Quaternion(Vec3(0, 1, 0), hd::randomf(-hd_2_pi, hd_2_pi)));

    shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

    collisionMesh->SetMeshData(meshData);
    collisionMesh->OnGetTransform([transform]() { return transform->GetMatrix(); });
    entity->Add<LookAtResponder>();
    mesh->SetMeshReference(meshData);
    mesh->GetMeshData()->SetDefaultAttributeLocations(shader);

    const bool USE_MESH_MATERIAL_DATA = false;
    if (USE_MESH_MATERIAL_DATA) {
        const hd::graphics::MeshMaterial *meshMaterial = nullptr;//meshData->GetMeshMaterial();
        if (meshMaterial && meshMaterial->GetColorTexture()) {
            material->SetAmbient(meshMaterial->GetAmbient());
            material->SetDiffuse(meshMaterial->GetDiffuse());
            material->SetSpecular(meshMaterial->GetSpecular());
            material->SetEmission(meshMaterial->GetEmission());
            material->SetShininess(meshMaterial->GetShininess());
            ///material->SetTexture(meshMaterial->GetColorTexture());
            //material->SetNormalTexture(meshMaterial->GetNormalTexture());
        } else {
            material->SetAmbient(Color4f(0.7f, 0.2f, 0.2f, 1.0f));
            material->SetDiffuse(Color4f(0.5f, 0.5f, 0.5f, 1.0f));
            material->SetSpecular(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
            material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
            material->SetShininess(1.0f);
            //material->SetTexture(TextureManager::Instance()->GetNullTexture());
        }
    } else {
        material->SetAmbient(Color4f(0.7f, 0.2f, 0.2f, 1.0f));
        material->SetDiffuse(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetSpecular(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(10.0f);
    }

    return entity;
}


hd::Entity * AppPrototype::AddLabel(Vec3 center, Vec3 space, const std::string &labelText) {
    auto label = m_entityPool.Create();
    label->SetName("label");

    auto transform = label->AddComponent<Transform>();
    auto shader = label->AddComponent<Shader>();
    auto material = label->AddComponent<Material>();
    auto font = label->AddComponent<ui::Font>();
    auto vertexBuffer = label->AddComponent<VertexBuffer>();
    auto text = label->AddComponent<ui::Text>();
    auto fontMesh = label->AddComponent<ui::FontMesh>();
    
    shader->LoadShader("standardGeometry.vert", "standardGeometry.frag");
    shader->SetPreRenderCallback<AppPrototype>(this, &AppPrototype::OnBlockShaderRenderCallback);

    material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    material->SetDiffuse(Color4f(0.4f, 0.4f, 0.5f, 1.0f));
    material->SetSpecular(Color4f(0.8f, 0.8f, 0.9f, 1.0f));
    material->SetEmission(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    material->SetShininess(1.0f);
    
    difont::Font *ffont = hd::ui::FontLoader::Instance()->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 32);
    font->SetFont(ffont);

    text->SetText(labelText);

    fontMesh->RenderMesh(font, labelText);
    vertexBuffer->AddMeshes(fontMesh->GetMeshData());
    vertexBuffer->SetDefaultAttributeLocations(shader);

    AABB fontSize = font->GetSize(labelText);

    transform->SetOrigin(center);

    transform->SetOrigin(center - fontSize.Half());

    return label;
}


void AppPrototype::Update(Entity *camera) {
    m_entityPool.Update();
    m_blockMover.Update();
    m_animationSystem.Update();
    m_alarmSystem.Update();
    m_transformSystem.Update();
    m_sceneCulling.Update(camera);
    m_blockInputSystem.Update(camera);

    m_skyboxRenderer.Update(camera);
    m_deferredLightSystem.Update(camera);
    m_transformSystem.Update();
    m_modelRenderer.Update(camera);
}


void AppPrototype::HandleInput(Entity *camera) {
}


void AppPrototype::DrawDepth(hd::Entity *camera, hd::input::EyePosition eyePosition) {
}


void AppPrototype::Draw(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    m_sceneRenderer.RenderOpaque(camera, eyePosition);
    m_modelRenderer.RenderOpaque(camera, eyePosition);
    m_skyboxRenderer.Draw(camera, eyePosition);
    m_sceneRenderer.RenderTransparent(camera, eyePosition);
    m_modelRenderer.RenderTransparent(camera, eyePosition);
    m_labelRenderer.Update(camera, eyePosition);
}
