#include "Game.h"
#include "core/application/Client.h"

static float m_time = 0.0f;
static float RADIUS = 15.0f;
static AABB worldSize(0, 0, 1024, 512);
static const unsigned int MAX_TESTBLOCKS = 512;
static const unsigned int MAX_UI_ELEMENTS = 1024;
static const int BLOCK_INPUT_GROUP = 1;

using namespace hd;

hdGame::hdGame(hd::application::Client *client) :
    m_entityPool(MAX_TESTBLOCKS),
    m_uiEntityPool(MAX_UI_ELEMENTS),
    m_pointerJointAnchor(NULL),
    m_blockMover(&m_entityPool),
    m_blockRenderer(&m_entityPool),
    m_backgroundRenderer(&m_entityPool),
    m_blockInputSystem(&m_entityPool, client),
    m_cameraInputSystem(&m_entityPool, client),
    m_cameras(&m_entityPool),
    m_stereoCameras(&m_entityPool, client),
    m_skyboxRenderer(&m_entityPool),
    m_hudRenderer(&m_uiEntityPool),
    m_uiInput(&m_uiEntityPool, client),
m_client(client) {
    
#ifdef DEBUG
    hd::Testing::TestAll();
#endif
    
    // Camera stuff
    m_cameraDataComponents      = new hd::ComponentPool<CameraData>(8);
    m_perspectiveDataComponents = new hd::ComponentPool<PerspectiveData>(8);
    m_cameraRendererComponents  = new hd::ComponentPool<CameraRenderer>(8);
    m_cameraScreenData          = new hd::ComponentPool<ScreenData>(8);
    m_viewportComponents        = new hd::ComponentPool<Viewport>(8);
    m_renderTargets             = new hd::ComponentPool<RenderTarget>(8);
    
    // Physics
    m_physicsWorldComponents    = new hd::ComponentPool<physics2d::World>(4);
    m_physicsBodyComponents     = new hd::ComponentPool<physics2d::Body>(MAX_TESTBLOCKS);
    m_pointerJointComponents    = new hd::ComponentPool<physics2d::PointerJoint>(MAX_TESTBLOCKS);
    
    // Game stuff
    m_polygonComponents         = new hd::ComponentPool<Polygon>(MAX_TESTBLOCKS);
    m_worldDataComponents       = new hd::ComponentPool<WorldData>(MAX_TESTBLOCKS);
    m_aabbDataComponents        = new hd::ComponentPool<AABBData>(MAX_TESTBLOCKS);
    m_polygonRendererComponents = new hd::ComponentPool<PolygonRenderer>(MAX_TESTBLOCKS);
    m_materials                 = new hd::ComponentPool<Material>(MAX_TESTBLOCKS);
    m_extrusions                = new hd::ComponentPool<ExtrusionData>(MAX_TESTBLOCKS);
    m_extrusionRenderers        = new hd::ComponentPool<ExtrusionRenderer>(MAX_TESTBLOCKS);
    m_shaders = new hd::ComponentPool<Shader>(MAX_TESTBLOCKS);
    m_vertexBuffers = new hd::ComponentPool<VertexBuffer>(MAX_TESTBLOCKS);
    
    // Curves
    m_curves                    = new hd::ComponentPool<Curve>(MAX_TESTBLOCKS);
    m_curveRenderers            = new hd::ComponentPool<CurveRenderer>(MAX_TESTBLOCKS);
    
    // Responders
    m_pointerResponderStates    = new hd::ComponentPool<pointer::ResponderState>(MAX_TESTBLOCKS);
    m_pointerSelectResponders   = new hd::ComponentPool<pointer::SelectResponder>(MAX_TESTBLOCKS);
    m_pointerDragResponders     = new hd::ComponentPool<pointer::DragResponder>(MAX_TESTBLOCKS);
    m_pointerReleaseResponders  = new hd::ComponentPool<pointer::ReleaseResponder>(MAX_TESTBLOCKS);
    
    // UI
    m_uiScreenData           = new hd::ComponentPool<graphics::ScreenData>(MAX_UI_ELEMENTS);
    
    
    /*
     m_componentContext->CreatePool<pointer::DragResponder>(MAX_TESTBLOCKS);
     m_componentContext->Get<pointer::DragResponder>();
     */
    
    m_blockInputSystem.m_game = this;
    
    CreateCamera();
    CreateStereoCamera();
    InitPhysicsObjects();
    CreateHud();
    
    
    
}


hdGame::~hdGame() {}


void hdGame::CreateCamera() {
    m_cameras.mainCamera = m_entityPool.Create();
    
    auto *cameraData = m_cameraDataComponents->Get();
    auto *perspectiveData = m_perspectiveDataComponents->Get();
    auto *renderer = m_cameraRendererComponents->Get();
    auto *screenData = m_cameraScreenData->Get();
    auto *polygon = m_polygonComponents->Get();
    auto *polygonRenderer = m_polygonComponents->Get();
    
    cameraData->SetLocation(Vec3(worldSize.Center()[0], 100.0f, 400));
    cameraData->SetTarget(worldSize.Center() - Vec3(-10,100,600));
    cameraData->SetUpAxis(Vec3(0,1,0));
    
    screenData->SetAABB(AABB(0, 0,
                             Device::GetDisplayScreenWidth(),
                             Device::GetDisplayScreenHeight()));
    
    m_cameras.mainCamera->AddComponent(perspectiveData);
    m_cameras.mainCamera->AddComponent(cameraData);
    m_cameras.mainCamera->AddComponent(renderer);
    m_cameras.mainCamera->AddComponent(screenData);
    m_cameras.mainCamera->AddComponent(polygon);
    m_cameras.mainCamera->AddComponent(polygonRenderer);
    
    auto *responder = m_pointerResponderStates->Get();
    auto *psr = m_pointerSelectResponders->Get();
    auto *pdr = m_pointerDragResponders->Get();
    auto *prr = m_pointerReleaseResponders->Get();
    
    psr->SetPointerId(0);
    pdr->SetPointerId(0);
    prr->SetPointerId(0);
    
    m_cameras.mainCamera->AddComponent(responder);
    m_cameras.mainCamera->AddComponent(psr);
    m_cameras.mainCamera->AddComponent(pdr);
    m_cameras.mainCamera->AddComponent(prr);
    
    m_cameras.mainCamera->SetName("mainCamera");
}


void hdGame::CreateStereoCamera() {
    DeviceDetector *detector = m_client->GetEventQueue()->GetDeviceDetector();
    
    {
        m_stereoCameras.camera = m_entityPool.Create();
        
        auto cameraData = m_cameraDataComponents->Get();
        auto perspectiveData = m_perspectiveDataComponents->Get();
        auto renderer = m_cameraRendererComponents->Get();
        auto viewport = m_viewportComponents->Get();
        auto polygon = m_polygonComponents->Get();
        auto polygonRenderer = m_polygonComponents->Get();
        
        AABB viewportAABB = detector->GetViewport(hd::input::StereoEyeCenter);
        viewport->SetAABB(viewportAABB);
        
        cameraData->SetLocation(Vec3(worldSize.Center()[0], 200.0f, 400));
        cameraData->SetTarget(worldSize.Center() - Vec3(-10,100,600));
        cameraData->SetUpAxis(Vec3(0,1,0));
        
        auto *responder = m_pointerResponderStates->Get();
        auto *psr = m_pointerSelectResponders->Get();
        auto *pdr = m_pointerDragResponders->Get();
        auto *prr = m_pointerReleaseResponders->Get();
        
        psr->SetPointerId(0);
        pdr->SetPointerId(0);
        prr->SetPointerId(0);
        
        m_stereoCameras.camera->AddComponent(perspectiveData);
        m_stereoCameras.camera->AddComponent(cameraData);
        m_stereoCameras.camera->AddComponent(renderer);
        m_stereoCameras.camera->AddComponent(viewport);
        m_stereoCameras.camera->AddComponent(polygon);
        m_stereoCameras.camera->AddComponent(polygonRenderer);
        
        m_stereoCameras.camera->AddComponent(responder);
        m_stereoCameras.camera->AddComponent(psr);
        m_stereoCameras.camera->AddComponent(pdr);
        m_stereoCameras.camera->AddComponent(prr);
    }
    
    
    {
        m_stereoCameras.leftEye = m_entityPool.Create();
        
        auto perspectiveData = m_perspectiveDataComponents->Get();
        auto viewport = m_viewportComponents->Get();
        auto renderTarget = m_renderTargets->Get();
        auto shader = m_shaders->Get();
        auto polygon = m_polygonComponents->Get();
        auto vertexBuffer = m_vertexBuffers->Get();
        
        AABB viewportAABB = detector->GetViewport(hd::input::StereoEyeLeft);
        viewport->SetAABB(viewportAABB);
        renderTarget->Resize(viewportAABB.GetSize2());
        perspectiveData->SetMatrix(detector->GetStereoProjectionMatrix(hd::input::StereoEyeLeft));
        
        shader->Reset();
        // Lens-distortion samples were removed; retain a plain render-target preview.
        shader->LoadFragmentShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/RenderTarget.fsh");
        shader->LoadVertexShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/RenderTarget.vsh");
        shader->Link();
        
        polygon->Create2DBox(viewport->GetAABB());
        polygon->ResetTextureCoords(renderTarget->GetTextureSize());
        vertexBuffer->SetFromPolygon(polygon);
        polygon->Free();
        
        uint32_t originLocation = shader->GetAttributeLocation("position");
        uint32_t colorLocation = shader->GetAttributeLocation("color");
        uint32_t texCoordLocation = shader->GetAttributeLocation("texCoord");
        
        vertexBuffer->SetOriginAttributeLocation(originLocation);
        vertexBuffer->SetColorAttributeLocation(colorLocation);
        vertexBuffer->SetTexCoordAttributeLocation(texCoordLocation);
        
        m_stereoCameras.leftEye->AddComponent(perspectiveData);
        m_stereoCameras.leftEye->AddComponent(viewport);
        m_stereoCameras.leftEye->AddComponent(renderTarget);
        m_stereoCameras.leftEye->AddComponent(shader);
        m_stereoCameras.leftEye->AddComponent(vertexBuffer);
    }
    
    {
        m_stereoCameras.rightEye = m_entityPool.Create();
        
        auto perspectiveData = m_perspectiveDataComponents->Get();
        auto viewport = m_viewportComponents->Get();
        auto renderTarget = m_renderTargets->Get();
        auto shader = m_shaders->Get();
        auto polygon = m_polygonComponents->Get();
        auto vertexBuffer = m_vertexBuffers->Get();
        
        AABB viewportAABB = detector->GetViewport(hd::input::StereoEyeRight);
        viewport->SetAABB(AABB(0, 0, viewportAABB.Width(), viewportAABB.Height()));
        renderTarget->Resize(viewportAABB.GetSize2());
        perspectiveData->SetMatrix(detector->GetStereoProjectionMatrix(hd::input::StereoEyeRight));
        
        shader->Reset();
        // Lens-distortion samples were removed; retain a plain render-target preview.
        shader->LoadFragmentShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/RenderTarget.fsh");
        shader->LoadVertexShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/RenderTarget.vsh");
        shader->Link();
        
        polygon->Create2DBox(viewportAABB);
        polygon->ResetTextureCoords(renderTarget->GetTextureSize());
        vertexBuffer->SetFromPolygon(polygon);
        polygon->Free();
        
        uint32_t originLocation = shader->GetAttributeLocation("position");
        uint32_t colorLocation = shader->GetAttributeLocation("color");
        uint32_t texCoordLocation = shader->GetAttributeLocation("texCoord");
        
        vertexBuffer->SetOriginAttributeLocation(originLocation);
        vertexBuffer->SetColorAttributeLocation(colorLocation);
        vertexBuffer->SetTexCoordAttributeLocation(texCoordLocation);
        
        m_stereoCameras.rightEye->AddComponent(perspectiveData);
        m_stereoCameras.rightEye->AddComponent(viewport);
        m_stereoCameras.rightEye->AddComponent(renderTarget);
        m_stereoCameras.rightEye->AddComponent(shader);
        m_stereoCameras.rightEye->AddComponent(vertexBuffer);
    }
}


static const Color4f BUTTON_COLOR(0.9f, 0.6f, 0.9f, 0.5f);
void hdGame::CreateHud() {
    CreateButton(100, 50, Vec3(100, 100, 0));
    CreateButton(100, 50, Vec3(250, 100, 0));
    CreateButton(100, 50, Vec3(400, 100, 0));
    CreateButton(100, 50, Vec3(400, 200, 0));
    
    Entity *curveEntity = m_uiEntityPool.Create();
    
    Curve *curve = m_curves->Get();
    curve->AddCurve(Vec3(), Vec3(1.0f, 1.0f, 0), Vec3(0, 1.0f, 0), Vec3(1.0f, 0));
    curveEntity->AddComponent(curve);

    CurveRenderer *curveRenderer = m_curveRenderers->Get();
    curveEntity->AddComponent(curveRenderer);
    
    WorldData *worldData = m_worldDataComponents->Get();
    worldData->Reset();
    worldData->SetScale(1000.0f);
    curveEntity->AddComponent(worldData);
}


void hdGame::CreateButton(float width, float height, Vec3 center) {
    Entity *button = m_uiEntityPool.Create();
    
    AABB aabb(width, height);
    
    WorldData *worldData = m_worldDataComponents->Get();
    worldData->SetOrigin(center);
    button->AddComponent(worldData);
    
    Polygon *polygon = m_polygonComponents->Get();
    polygon->Create2DBox(aabb);
    button->AddComponent(polygon);
    
    PolygonRenderer * renderer = m_polygonRendererComponents->Get();
    button->AddComponent(renderer);
    
    Material * mat = m_materials->Get();
    //mat->SetTexture(TextureManager::Instance()->FindTexture("/Users/david1/Projects/dmg/PhysicsHotel/Resources/pc.png", hd::TextureType::Pic16));
    button->AddComponent(mat);
    mat->SetColor(BUTTON_COLOR);
    
    ScreenData *screen = m_uiScreenData->Get();
    button->AddComponent(screen);
    
    auto *responder = m_pointerResponderStates->Get();
    button->AddComponent(responder);
    
    auto *psr = m_pointerSelectResponders->Get();
    psr->SetPointerId(0);
    button->AddComponent(psr);
    
    auto *pdr = m_pointerDragResponders->Get();
    pdr->SetPointerId(0);
    button->AddComponent(pdr);
    
    auto *prr = m_pointerReleaseResponders->Get();
    prr->SetPointerId(0);
    button->AddComponent(prr);
}


void hdGame::InitPhysicsObjects() {
    physics2d::BodyInfo bodyInfo;
    
    m_physicsWorld = m_entityPool.Create();
    physics2d::World * worldComponent = m_physicsWorldComponents->Get();
    m_physicsWorld->AddComponent(worldComponent);
    
    physics2d::WorldInfo worldInfo;
    worldComponent->CreateWorld(worldInfo);
    
    const float BOUNDARY_THICKNESS = 80.0f;
    
    // top
    AddStaticBlock(worldSize.Width(), BOUNDARY_THICKNESS, Vec3(0.5f * worldSize.Width(), worldSize.Height()));
        
    // left
    AddStaticBlock(BOUNDARY_THICKNESS, worldSize.Height(), Vec3(0.0f, 0.5f * worldSize.Height()));
        
    // right
    AddStaticBlock(BOUNDARY_THICKNESS, worldSize.Height(), Vec3(worldSize.Width(), 0.5f * worldSize.Height()));
                       
    // bottom
    AddStaticBlock(worldSize.Width(), BOUNDARY_THICKNESS, Vec3(0.5f * worldSize.Width(), 0.0f - BOUNDARY_THICKNESS * 0.5f));
    
    // Player character
    SpawnPlayerCharacter(worldSize.lower + Vec3(worldSize.Width() * 0.5f, worldSize.Height() * 0.5f, 0.0f));
    
    //SpawnHarvestItem(AABB(worldSize.lower.x + 80.0f, worldSize.lower.y, 80.0f, 80.0f));
    
    AddBackground(worldSize.Width(), worldSize.Height(), Vec3(), worldSize.Center() - Vec3(0.0f, 0.0f, 5.0f));
    AddBackground(worldSize.Width(), worldSize.Height(), Vec3(0, hd_half_pi, 0), worldSize.Center() + Vec3(-0.5f * worldSize.Width(), 0.0f, 0.5f * worldSize.Width()));
    
    ConstructSkybox();
}


void  hdGame::ConstructSkybox() {
    Entity *sphere = m_entityPool.Create();
    WorldData * worldData = m_worldDataComponents->Get();
    worldData->SetOrigin(worldSize.Center() + Vec3(0.0f, 0.0f, 4000.0f));
    sphere->AddComponent(worldData);
    
    Polygon *polygon = m_polygonComponents->Get();
    polygon->CreateSphere(50000.0f, 32);
    sphere->AddComponent(polygon);
    
    PolygonRenderer *renderer = m_polygonRendererComponents->Get();
    sphere->AddComponent(renderer);
    
    auto *buffer = m_vertexBuffers->Get();
    buffer->SetFromSphere(polygon);
    sphere->AddComponent(buffer);
    
    Material * mat = m_materials->Get();
    mat->SetTexture(TextureManager::Instance()->FindTexture("/Users/david1/Projects/dmg/PhysicsHotel/resources/desert.sphere_skybox.png", hd::TextureType::Wall));
    sphere->AddComponent(mat);
    
    auto *shader = m_shaders->Get();
    shader->Reset();
    shader->LoadFragmentShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/galaxy.fsh");
    shader->LoadVertexShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/Shader.vsh");
    shader->Link();
    sphere->AddComponent(shader);
    
    uint32_t originLocation = shader->GetAttributeLocation("position");
    uint32_t colorLocation = shader->GetAttributeLocation("color");
    uint32_t texCoordLocation = shader->GetAttributeLocation("texCoord");
    
    buffer->SetOriginAttributeLocation(originLocation);
    buffer->SetColorAttributeLocation(colorLocation);
    buffer->SetTexCoordAttributeLocation(texCoordLocation);
    
    sphere->SetName("sphere");
}


void hdGame::AddBackground(const float width, const float height, const Vec3 &rotation, const Vec3 &center) {
    if (m_entityPool.Empty()) {
        return;
    }
    
    Entity *panel = m_entityPool.Create();
    
    WorldData * worldData = m_worldDataComponents->Get();
    worldData->SetOrigin(center);
    worldData->SetRotation(rotation);
    panel->AddComponent(worldData);
    
    Polygon * polygon = m_polygonComponents->Get();
    polygon->Create2DBox(AABB(width, height));
    panel->AddComponent(polygon);
    
    PolygonRenderer *renderer = m_polygonRendererComponents->Get();
    panel->AddComponent(renderer);
    
    Material * mat = m_materials->Get();
    mat->SetTexture(TextureManager::Instance()->FindTexture("/Users/david1/Projects/dmg/PhysicsHotel/resources/noise.png", hd::TextureType::Wall));
    panel->AddComponent(mat);
    
    auto *shader = m_shaders->Get();
    shader->Reset();
    shader->LoadFragmentShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/solidBackground.fsh");
    shader->LoadVertexShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/Shader.vsh");
    shader->Link();
    panel->AddComponent(shader);
    
    uint32_t originLocation = shader->GetAttributeLocation("position");
    uint32_t colorLocation = shader->GetAttributeLocation("color");
    uint32_t texCoordLocation = shader->GetAttributeLocation("texCoord");
    
    auto *buffer = m_vertexBuffers->Get();
    buffer->SetFromPolygon(polygon);
    buffer->SetOriginAttributeLocation(originLocation);
    buffer->SetColorAttributeLocation(colorLocation);
    buffer->SetTexCoordAttributeLocation(texCoordLocation);
    panel->AddComponent(buffer);

    panel->SetName("block");
}


void hdGame::AddStaticBlock(float width, float height, Vec3 center) {
    if (m_entityPool.Empty()) {
        return;
    }
    
    WorldData *worldData = m_worldDataComponents->Get();
    physics2d::Body *body = m_physicsBodyComponents->Get();
    Polygon *polygon = m_polygonComponents->Get();
    PolygonRenderer *renderer = m_polygonRendererComponents->Get();
    ExtrusionData *extrusion = m_extrusions->Get();
    ExtrusionRenderer *extrusionRenderer = m_extrusionRenderers->Get();
    Material *mat = m_materials->Get();
    Shader *shader = m_shaders->Get();
    VertexBuffer *buffer = m_vertexBuffers->Get();
    
    Entity *block = m_entityPool.Create();
    block->AddComponent(worldData);
    block->AddComponent(body);
    block->AddComponent(polygon);
    block->AddComponent(renderer);
    block->AddComponent(extrusion);
    block->AddComponent(extrusionRenderer);
    block->AddComponent(mat);
    block->AddComponent(shader);
    block->AddComponent(buffer);
    block->SetName("block");
    
    physics2d::BodyInfo bodyInfo;
    bodyInfo.configuration = physics2d::Rectangle;
    bodyInfo.aabb = AABB(width, height, center);
    bodyInfo.isStatic = true;
    bodyInfo.friction = 0.2f;
    bodyInfo.density = 0.5f;
    bodyInfo.restitution = 0.5f;
    bodyInfo.position = center;
    bodyInfo.categoryBits = CollisionCategory::StaticGeometry;
    bodyInfo.maskBits = CollisionCategory::Player | CollisionCategory::HarvestItem;
    
    physics2d::World * worldComponent = m_physicsWorld->GetComponent<physics2d::World>();
    
    worldData->SetOrigin(center);
    body->CreatePhysicsBody(worldComponent, bodyInfo);
    polygon->Create2DBox(AABB(width, height));
    
    extrusion->SetDepth(10.0f);
    extrusion->GenerateTextureCoords(polygon);
    
    //Vec3 position = body->GetPosition();
    //polygon->MoveBy(position - aabb.Center());
    //worldData->SetOrigin(position);
	
    Vec3 rotation(0.0f, 0.0f, body->GetAngle());
    worldData->SetRotation(rotation);
    
    mat->SetColor(Color4f(0.3f, 0.6f, 0.9f, 1.0f));
    
    if (!m_pointerJointAnchor) {
        m_pointerJointAnchor = block;
    }
    
    shader->Reset();
    shader->LoadFragmentShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/tex0.fsh");
    shader->LoadVertexShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/Shader.vsh");
    shader->Link();
    
    uint32_t originLocation = shader->GetAttributeLocation("position");
    uint32_t colorLocation = shader->GetAttributeLocation("color");
    uint32_t texCoordLocation = shader->GetAttributeLocation("texCoord");
    
    buffer->SetFromPolygon(polygon);
    buffer->SetOriginAttributeLocation(originLocation);
    buffer->SetColorAttributeLocation(colorLocation);
    buffer->SetTexCoordAttributeLocation(texCoordLocation);
}


void hdGame::SpawnBlock(Vec3 pos) {
    if (m_entityPool.Empty()) {
        return;
    }
    
    Entity *block = m_entityPool.Create();
    block->SetName("block");
    
    float radius = hd::randomf(RADIUS, RADIUS * 3.0f);
    
    AABB aabb(pos[0], pos[1], radius * 2, radius * 2);
    
    physics2d::BodyInfo bodyInfo;
    bodyInfo.configuration = physics2d::Circle;
    bodyInfo.radius = radius;
    bodyInfo.aabb = aabb;
    bodyInfo.position = pos;
    bodyInfo.friction = 0.2f;
    bodyInfo.density = 0.5f;
    bodyInfo.restitution = 0.5f;
    
    physics2d::Body * body = m_physicsBodyComponents->Get();
    physics2d::World *worldComponent = m_physicsWorld->GetComponent<physics2d::World>();
    body->CreatePhysicsBody(worldComponent, bodyInfo);
    block->AddComponent(body);
    
    WorldData * worldData = m_worldDataComponents->Get();
    worldData->SetOrigin(bodyInfo.aabb.Center());
    Vec3 rotation(0.0f, 0.0f, body->GetAngle());
    worldData->SetRotation(rotation);
    worldData->SetOrigin(aabb.Center());
    block->AddComponent(worldData);
    
    Polygon * polygon = m_polygonComponents->Get();
    polygon->Create2DStar(bodyInfo.aabb, bodyInfo.radius, bodyInfo.radius * hd::randomf(0.25f, 1.0f), int(hd::randomf(3.0f, 30.0f)));
    //polygon->MoveBy(body->GetPosition() - bodyInfo.position);
    block->AddComponent(polygon);
    
    PolygonRenderer * renderer = m_polygonRendererComponents->Get();
    block->AddComponent(renderer);
    
    ExtrusionData * extrusion = m_extrusions->Get();
    extrusion->SetDepth(8.0f);
    extrusion->GenerateTextureCoords(polygon);
    block->AddComponent(extrusion);
    
    ExtrusionRenderer * extrusionRenderer = m_extrusionRenderers->Get();
    block->AddComponent(extrusionRenderer);
    
    Material * mat = m_materials->Get();
    mat->SetTexture(TextureManager::Instance()->FindTexture("/Users/david1/Projects/dmg/PhysicsHotel/resources/wood.png", hd::TextureType::Wall));
    block->AddComponent(mat);
    
    auto *responder = m_pointerResponderStates->Get();
    block->AddComponent(responder);
    
    auto *pointerJoint = m_pointerJointComponents->Get();
    block->AddComponent(pointerJoint);
    
    auto *psr = m_pointerSelectResponders->Get();
    psr->SetPointerId(0);
    block->AddComponent(psr);
    
    auto *pdr = m_pointerDragResponders->Get();
    pdr->SetPointerId(0);
    block->AddComponent(pdr);
    
    auto *prr = m_pointerReleaseResponders->Get();
    prr->SetPointerId(0);
    block->AddComponent(prr);
    
    auto *shader = m_shaders->Get();
    shader->Reset();
    shader->LoadFragmentShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/tex0.fsh");
    shader->LoadVertexShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/Shader.vsh");
    shader->Link();
    block->AddComponent(shader);
    
    uint32_t originLocation = shader->GetAttributeLocation("position");
    uint32_t colorLocation = shader->GetAttributeLocation("color");
    uint32_t texCoordLocation = shader->GetAttributeLocation("texCoord");
    
    auto *buffer = m_vertexBuffers->Get();
    buffer->SetFromPolygon(polygon);
    buffer->SetOriginAttributeLocation(originLocation);
    buffer->SetColorAttributeLocation(colorLocation);
    buffer->SetTexCoordAttributeLocation(texCoordLocation);
    block->AddComponent(buffer);
}


petrievoid hdGame::SpawnPlayerCharacter(Vec3 pos) {
    if (m_entityPool.Empty()) {
        return;
    }
    
    Entity *block = m_entityPool.Create();
    block->SetName("block");
    
    float radius = hd::randomf(RADIUS, RADIUS * 3.0f);
    
    AABB aabb(-radius, -radius, radius * 2, radius * 2);
    
    physics2d::BodyInfo bodyInfo;
    bodyInfo.configuration = physics2d::Circle;
    bodyInfo.radius = radius;
    bodyInfo.aabb = aabb;
    bodyInfo.position = pos;
    bodyInfo.friction = 0.2f;
    bodyInfo.density = 2.5f;
    bodyInfo.restitution = 0.2f;
    bodyInfo.categoryBits = CollisionCategory::Player;
    bodyInfo.maskBits = CollisionCategory::StaticGeometry;
    
    physics2d::Body * body = m_physicsBodyComponents->Get();
    physics2d::World *worldComponent = m_physicsWorld->GetComponent<physics2d::World>();
    body->CreatePhysicsBody(worldComponent, bodyInfo);
    block->AddComponent(body);
    
    WorldData * worldData = m_worldDataComponents->Get();
    Vec3 rotation(0.0f, 0.0f, body->GetAngle());
    worldData->SetRotation(rotation);
    worldData->SetOrigin(pos);
    block->AddComponent(worldData);
    
    Polygon * polygon = m_polygonComponents->Get();
    polygon->Create2DCylinder(aabb, radius, 32);
    block->AddComponent(polygon);
    
    PolygonRenderer * renderer = m_polygonRendererComponents->Get();
    block->AddComponent(renderer);
    
    ExtrusionData * extrusion = m_extrusions->Get();
    extrusion->SetDepth(8.0f);
    extrusion->GenerateTextureCoords(polygon);
    block->AddComponent(extrusion);
    
    ExtrusionRenderer * extrusionRenderer = m_extrusionRenderers->Get();
    block->AddComponent(extrusionRenderer);
    
    Material * mat = m_materials->Get();
    mat->SetTexture(TextureManager::Instance()->FindTexture("/Users/david1/Projects/dmg/PhysicsHotel/resources/pc.png", hd::TextureType::Wall));
    block->AddComponent(mat);
    
    auto *responder = m_pointerResponderStates->Get();
    block->AddComponent(responder);
    
    auto *pointerJoint = m_pointerJointComponents->Get();
    block->AddComponent(pointerJoint);
    
    auto *psr = m_pointerSelectResponders->Get();
    psr->SetPointerId(0);
    block->AddComponent(psr);
    
    auto *pdr = m_pointerDragResponders->Get();
    pdr->SetPointerId(0);
    block->AddComponent(pdr);
    
    auto *prr = m_pointerReleaseResponders->Get();
    prr->SetPointerId(0);
    block->AddComponent(prr);
    
    auto *shader = m_shaders->Get();
    shader->Reset();
    shader->LoadFragmentShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/tex0.fsh");
    shader->LoadVertexShader("/Users/david1/Projects/dmg/PhysicsHotel/resources/Shader.vsh");
    shader->Link();
    block->AddComponent(shader);
    
    uint32_t originLocation = shader->GetAttributeLocation("position");
    uint32_t colorLocation = shader->GetAttributeLocation("color");
    uint32_t texCoordLocation = shader->GetAttributeLocation("texCoord");
    
    auto *buffer = m_vertexBuffers->Get();
    buffer->SetFromPolygon(polygon);
    buffer->SetOriginAttributeLocation(originLocation);
    buffer->SetColorAttributeLocation(colorLocation);
    buffer->SetTexCoordAttributeLocation(texCoordLocation);
    block->AddComponent(buffer);
}


void hdGame::SpawnHarvestItem(AABB aabb) {
    if (m_entityPool.Empty()) {
        return;
    }
    
    Entity * block = m_entityPool.Create();
    block->SetName("block");
    
    physics2d::Body *body = m_physicsBodyComponents->Get();
    physics2d::BodyInfo bodyInfo;
    bodyInfo.configuration = physics2d::Rectangle;
    bodyInfo.aabb = aabb;
    bodyInfo.isStatic = true;
    bodyInfo.friction = 0.2f;
    bodyInfo.density = 0.5f;
    bodyInfo.restitution = 1.0f;
    bodyInfo.position = aabb.Center();
    bodyInfo.categoryBits = CollisionCategory::HarvestItem;
    bodyInfo.maskBits = CollisionCategory::HarvestItem;
    physics2d::World * worldComponent = m_physicsWorld->GetComponent<physics2d::World>();
    body->CreatePhysicsBody(worldComponent, bodyInfo);
    block->AddComponent(body);
    
    WorldData *worldData = m_worldDataComponents->Get();
    block->AddComponent(worldData);
    worldData->SetOrigin(aabb.Center());
    Vec3 rotation(0.0f, 0.0f, body->GetAngle());
    worldData->SetRotation(rotation);
    
    
    Polygon * polygon = m_polygonComponents->Get();
    polygon->Create2DBox(aabb);
    //Vec3 position = body->GetPosition();
    //polygon->MoveBy(position - aabb.Center());
    block->AddComponent(polygon);
    
    PolygonRenderer * renderer = m_polygonRendererComponents->Get();
    block->AddComponent(renderer);
    
    
    ExtrusionData * extrusion = m_extrusions->Get();
    extrusion->SetDepth(10.0f);
    extrusion->GenerateTextureCoords(polygon);
    block->AddComponent(extrusion);
    
    ExtrusionRenderer * extrusionRenderer = m_extrusionRenderers->Get();
    block->AddComponent(extrusionRenderer);
    
    Material * mat = m_materials->Get();
    mat->SetColor(Color4f(0.3f, 0.6f, 0.9f, 1.0f));
    block->AddComponent(mat);
}


void hdGame::OnBlockSelect(Entity *block, const Vec3 &position) {
}


void hdGame::OnBlockDrag(Entity *block, const Vec3 &position, const Vec3 &delta) {
    if (!m_pointerJointAnchor) return;
    if (!block->ContainsComponent<physics2d::PointerJoint>()) return;
    if (!block->ContainsComponent<physics2d::Body>()) return;

    auto *pointerJoint = block->GetComponent<physics2d::PointerJoint>();
    
    if (pointerJoint->GetJoint()) {
        pointerJoint->UpdateJoint(position + delta);
    } else {
        auto *anchorBody = m_pointerJointAnchor->GetComponent<physics2d::Body>();
        auto *physicsBody = block->GetComponent<physics2d::Body>();
        
        block->AddComponent(pointerJoint);
        
        physics2d::PointerJointInfo pointerJointInfo;
        pointerJointInfo.target = Vec3(position + delta);
     
        auto *worldComponent = m_physicsWorld->GetComponent<physics2d::World>();
        
        pointerJoint->CreateJoint(worldComponent, pointerJointInfo,
                                  anchorBody, physicsBody);
    }
}


void hdGame::OnBlockRelease(Entity *block, const Vec3 &position) {
    block->PrintDebugInfo();
    
    if (block->ContainsComponent<physics2d::PointerJoint>()) {
        auto *worldComponent = m_physicsWorld->GetComponent<physics2d::World>();
        auto *pointerJoint = block->GetComponent<physics2d::PointerJoint>();
        pointerJoint->DestroyJoint(worldComponent);
        
        hd::sound::SoundController::PlayEffect("/Users/david1/Projects/dmg/PhysicsHotel/Resources/blockSelect01.caf");
    }
}


void hdGame::OnUiSelect(Entity *element, const Vec3 &position) {
    hdPrintf("Select %lu\n", element->GetID());
}


void hdGame::OnUiRelease(Entity *element, const Vec3 &position) {
    element->PrintDebugInfo();
    if (element->ContainsComponent<graphics::WorldData, graphics::Material>()) {
        auto *mat  = element->GetComponent<graphics::Material>();
        auto *data = element->GetComponent<graphics::WorldData>();
        
        Vec3 scale = data->GetScale();
        if (scale[0] >= 1.5f) {
            data->SetScale(1.0f);
            mat->SetColor(BUTTON_COLOR);
        } else {
            data->SetScale(scale[0] + 0.1f);
            mat->SetColor(Color4f(hd::randomf(0.0f, 1.0f),
                                  hd::randomf(0.0f, 1.0f),
                                  hd::randomf(0.0f, 1.0f),
                                  hd::randomf(0.0f, 1.0f)));
        }
        
        
        
    }
}


void hdGame::AddDynamicBlock() {}


void hdGame::RemoveDynamicBlock() {}


static int render_style = 0;
void hdGame::Update() {
    m_entityPool.Update();
    m_uiEntityPool.Update();
    
    m_uiInput.Update();
    
    m_blockInputSystem.Update(m_cameras.mainCamera);
    
    if (render_style == 0) {
        m_stereoCameras.HandleInput();
        m_stereoCameras.Update();
    } else if (render_style == 1) {
        m_cameraInputSystem.Update(m_cameras.mainCamera);
        m_cameras.Update();
    } else {
        m_cameraInputSystem.Update(m_cameras.mainCamera);
        m_cameras.Update();
    }
    
    physics2d::World *worldComponent = m_physicsWorld->GetComponent<physics2d::World>();
    worldComponent->Update();
    m_blockMover.Update();
}


void hdGame::Draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glDisable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_LINE_SMOOTH);
    
    glPointSize(1.0f);
	
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    if (render_style == 0) {
        auto raytraceLines = m_cameras.mainCamera->GetComponent<Polygon>();
        auto raytraceRenderer = m_cameras.mainCamera->GetComponent<PolygonRenderer>();
        
        auto leftRenderTarget = m_stereoCameras.leftEye->GetComponent<RenderTarget>();
        auto leftShader = m_stereoCameras.leftEye->GetComponent<Shader>();
        auto leftVertexBuffer = m_stereoCameras.leftEye->GetComponent<VertexBuffer>();
        
        leftRenderTarget->Start();
        leftRenderTarget->Clear();
        m_stereoCameras.RenderLeft();
        m_backgroundRenderer.Update();
        m_blockRenderer.Update();
        m_skyboxRenderer.Update();
        raytraceRenderer->RenderLines(raytraceLines);
        leftRenderTarget->End();
        
        auto rightRenderTarget = m_stereoCameras.rightEye->GetComponent<RenderTarget>();
        auto rightShader = m_stereoCameras.rightEye->GetComponent<Shader>();
        auto rightVertexBuffer = m_stereoCameras.rightEye->GetComponent<VertexBuffer>();
        
        rightRenderTarget->Start();
        rightRenderTarget->Clear();
        m_stereoCameras.RenderRight();
        m_backgroundRenderer.Update();
        m_blockRenderer.Update();
        m_skyboxRenderer.Update();
        raytraceRenderer->RenderLines(raytraceLines);
        rightRenderTarget->End();
        
        glViewport(0, 0, hd::Device::GetDisplayScreenWidth(), hd::Device::GetDisplayScreenHeight());
        glDisable(GL_SCISSOR_TEST);
        glScissor(0, 0, hd::Device::GetDisplayScreenWidth(), hd::Device::GetDisplayScreenHeight());
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        hd::gl::Orthof(0, hd::Device::GetDisplayScreenWidth(),
                       0, hd::Device::GetDisplayScreenHeight(),
                       -10000, 10000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        DeviceDetector *detector = m_client->GetEventQueue()->GetDeviceDetector();
        leftShader->Begin();
        detector->SetShaderParams(leftShader, StereoEyeLeft);
        leftRenderTarget->BindTexture();
        leftVertexBuffer->Render();
        leftShader->End();
        
        rightShader->Begin();
        detector->SetShaderParams(rightShader, StereoEyeRight);
        rightRenderTarget->BindTexture();
        rightVertexBuffer->Render();
        rightShader->End();
        
    } else if (render_style == 1) {
        m_cameras.Render();
        m_backgroundRenderer.Update();
        m_blockRenderer.Update();
        m_skyboxRenderer.Update();
        
        auto raytraceLines = m_cameras.mainCamera->GetComponent<Polygon>();
        auto raytraceRenderer = m_cameras.mainCamera->GetComponent<PolygonRenderer>();
        raytraceRenderer->RenderLines(raytraceLines);

    } else {
        m_cameras.RenderLeftEye();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_backgroundRenderer.Update();
        m_blockRenderer.Update();
        m_skyboxRenderer.Update();
        
        m_cameras.RenderRightEye();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_backgroundRenderer.Update();
        m_blockRenderer.Update();
        m_skyboxRenderer.Update();
        
    }
}
