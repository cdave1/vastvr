#include "AppController.h"
#include "core/application/Client.h"

#include "Console.h"

static const unsigned int MAX_ENTITIES = 512;

using namespace hd;

Console *console = NULL;


#define STEREO_RENDER_STYLE 0
#define MONO_RENDER_STYLE 1

static int render_style = MONO_RENDER_STYLE;

AppController::AppController(hd::application::Client *client) :
m_entityPool(MAX_ENTITIES),
m_cameras(&m_entityPool, client),
m_apps(),
m_currentAppIndex(0),
m_client(client) {

#ifdef DEBUG
    //hd::Testing::TestAll();
#endif

    hd::Components::CreateSmallPool<LookAtData>();
    hd::Components::CreateMediumPool<PerspectiveData>();
    hd::Components::CreateMediumPool<Projection>();
    hd::Components::CreateSmallPool<ScreenData>();
    hd::Components::CreateLargePool<Viewport>();
    hd::Components::CreateSmallPool<RayTracer>();
    hd::Components::CreateSmallPool<PointKernel>();
    hd::Components::CreateSmallPool<Headset>();
    hd::Components::CreateSmallPool<PixelBufferSample>();

    hd::Components::CreateMediumPool<RenderTarget>();
    hd::Components::CreateMediumPool<TextureTarget>();
    hd::Components::CreateMediumPool<GeometryRenderTarget>();

    // Data
    hd::Components::CreateLargePool<hd::data::Parent>();

    // Game stuff
    hd::Components::CreateLargePool<hd::graphics::Polygon>();
    hd::Components::CreateLargePool<CollisionData>();
    hd::Components::CreateLargePool<Transform>();
    hd::Components::CreateLargePool<PolygonRenderer>();
    hd::Components::CreateLargePool<Material>();
    hd::Components::CreateLargePool<OpacityData>();
    hd::Components::CreateLargePool<Shader>();
    hd::Components::CreateLargePool<MeshReference>();
    hd::Components::CreateLargePool<VertexBuffer>();

    // Lights
    hd::Components::CreateSmallPool<Light>();
    hd::Components::CreateSmallPool<LightModel>();
    hd::Components::CreateSmallPool<OrthographicData>();

    // Animation
    hd::Components::CreateLargePool<hd::animation::AnimationTimer>();
    hd::Components::CreateLargePool<hd::animation::OpacityBlock>();
    hd::Components::CreateLargePool<hd::animation::PositionBlock>();
    hd::Components::CreateLargePool<hd::animation::ScaleBlock>();

    // Deferred lighting
    hd::Components::CreateSmallPool<DepthRenderTarget>();

    // Responders
    hd::Components::CreateLargePool<pointer::ResponderState>();
    hd::Components::CreateLargePool<pointer::MoveResponder>();
    hd::Components::CreateLargePool<pointer::SelectResponder>();
    hd::Components::CreateLargePool<pointer::DragResponder>();
    hd::Components::CreateLargePool<pointer::ReleaseResponder>();

    // Other responders
    hd::Components::CreateLargePool<input::LookAtResponder>();
    hd::Components::CreateLargePool<input::RayCollisionResponder>();

    // Events
    hd::Components::CreateLargePool<hd::input::Alarm>();

    // UI
    hd::Components::CreateLargePool<ui::Identifier>();
    hd::Components::CreateLargePool<ui::Font>();
    hd::Components::CreateLargePool<ui::FontRenderer>();
    hd::Components::CreateLargePool<ui::FontLayout>();
    hd::Components::CreateLargePool<ui::FontMesh>();
    hd::Components::CreateLargePool<ui::Text>();
    hd::Components::CreateLargePool<ui::BooleanOption>();

    HMD *hmd = m_client->GetEventQueue()->GetCurrentHMD();
	if (render_style == STEREO_RENDER_STYLE) {
		hmd->InitDevice();
		hdAssert(hmd);
		hmd->InitForRendering();
	}
    
    CreateCamera();


    console = new Console(&m_entityPool);
}


AppController::~AppController() {}


void AppController::AddApp(AppPrototype *app) {
    m_apps.push_back(app);
    m_currentAppIndex = uint32_t(m_apps.size() - 1);
}

const AppPrototype * AppController::GetCurrentApp() const {
    return m_apps.empty() ? nullptr : m_apps[m_currentAppIndex];
}

bool AppController::SelectApp(const std::string &name) {
    for (size_t i = 0; i < m_apps.size(); ++i) {
        if (m_apps[i]->m_name != name) continue;
        m_apps[m_currentAppIndex]->OnDeactivate();
        m_currentAppIndex = uint32_t(i);
        m_apps[i]->OnActivate(Cameras::mainCamera);
        return true;
    }
    return false;
}

void AppController::CreateCamera() {
    HMD *hmd = m_client->GetEventQueue()->GetCurrentHMD();

    m_cameras.cameraCount = 1;

    hd::input::EyePosition eyePositions[2];
    eyePositions[0] = hd::input::EyePosition::Center;

    if (render_style == STEREO_RENDER_STYLE) {
        eyePositions[0] = hd::input::EyePosition::Left;
        eyePositions[1] = hd::input::EyePosition::Right;

        m_cameras.cameraCount = 2;
    }

    {
        Cameras::mainCamera = m_entityPool.Create();

        auto headset = Cameras::mainCamera->Add<Headset>();
        auto orthographicData = Cameras::mainCamera->Add<OrthographicData>();
        auto viewport = Cameras::mainCamera->Add<Viewport>();

        auto vertexBuffer = Cameras::mainCamera->Add<VertexBuffer>();
        auto renderTarget = Cameras::mainCamera->Add<RenderTarget>();
        auto geometryRenderTarget = Cameras::mainCamera->Add<GeometryRenderTarget>();
        auto rayTracer = Cameras::mainCamera->Add<RayTracer>();

        AABB screenAABB(0, 0,
                        Device::GetDisplayScreenWidth(),
                        Device::GetDisplayScreenHeight());

        headset->SetViewport(hd::input::EyePosition::Center,  hmd->GetViewport(hd::input::EyePosition::Center));
        headset->SetProjection(hd::input::EyePosition::Center, hmd->GetProjectionMatrix(hd::input::EyePosition::Center));

        headset->SetOrientation(hmd->GetDeviceOrientation());
        headset->SetTrackingOffset(hmd->GetDeviceTrackingOffset());
        if (m_cameras.cameraCount == 2) {
            headset->SetIsStereo(true);
            headset->SetViewport(eyePositions[0], hmd->GetViewport(eyePositions[0]));
            headset->SetViewport(eyePositions[1], hmd->GetViewport(eyePositions[1]));

            headset->SetProjection(eyePositions[0], hmd->GetProjectionMatrix(eyePositions[0]));
            headset->SetProjection(eyePositions[1], hmd->GetProjectionMatrix(eyePositions[1]));

            headset->SetEyeOffset(hmd->GetEyeOffset(eyePositions[0]), eyePositions[0]);
            headset->SetEyeOffset(hmd->GetEyeOffset(eyePositions[1]), eyePositions[1]);

            headset->SetPixelBufferSize(eyePositions[0], hmd->GetViewport(eyePositions[0]).GetSize2());
            headset->SetPixelBufferSize(eyePositions[1], hmd->GetViewport(eyePositions[1]).GetSize2());
        } else {
            headset->SetIsStereo(false);
            headset->SetPixelBufferSize(eyePositions[0], screenAABB.GetSize2());
        }
        
        headset->SetPosition(Vec3(0.0f, 400.0f, 400.0f));
        //headset->SetOrientationOffset(Quaternion(Vec3(1, 0, 0), 3.14f * 0.5f));
        
        float deltaX = 3.14f * 0.25f;
        headset->SetOrientationOffset(Quaternion(Axis_X, deltaX, Rotate_CW));
        
        orthographicData->SetAABB(screenAABB);

        viewport->SetAABB(screenAABB);
        renderTarget->Resize(screenAABB.GetSize2());
        geometryRenderTarget->Resize(screenAABB.GetSize2());

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(screenAABB);
        hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
        mesh->ResetTextureCoords(screenAABB.GetSize2());
        vertexBuffer->AddMesh(mesh);

        auto responder = Cameras::mainCamera->Add<pointer::ResponderState>();
        auto moveResponder = Cameras::mainCamera->Add<pointer::MoveResponder>();
        auto selectResponder = Cameras::mainCamera->Add<pointer::SelectResponder>();
        auto dragResponder = Cameras::mainCamera->Add<pointer::DragResponder>();
        auto releaseResponder = Cameras::mainCamera->Add<pointer::ReleaseResponder>();

        Cameras::mainCamera->SetName("mainCamera");
    }

    // Eyes -- geometry rendered with this camera
    for (int i = 0; i < m_cameras.cameraCount; ++i) {
        m_cameras.eyes[i] = m_entityPool.Create();

        AABB viewportAABB = hmd->GetViewport(eyePositions[i]);

        auto projection = m_cameras.eyes[i]->Add<Projection>();
        auto viewport = m_cameras.eyes[i]->Add<Viewport>();
        auto geometryRenderTarget = m_cameras.eyes[i]->Add<GeometryRenderTarget>();
        auto vertexBuffer = m_cameras.eyes[i]->Add<VertexBuffer>();

		viewport->SetAABB(viewportAABB);
		geometryRenderTarget->Resize(viewportAABB.GetSize2());
        projection->SetMatrix(hmd->GetProjectionMatrix(eyePositions[i]));
        projection->SetInverseMatrix(hmd->GetInverseProjectionMatrix(eyePositions[i]));

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(viewportAABB);
        hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
        mesh->ResetTextureCoords(geometryRenderTarget->GetTextureSize());
        vertexBuffer->AddMesh(mesh);
    }

    // Shadow target
    for (int i = 0; i < m_cameras.cameraCount; ++i) {
        m_cameras.shadowStep[i] = m_entityPool.Create();

        AABB viewportAABB = hmd->GetViewport(eyePositions[i]);

        auto polygon = m_cameras.shadowStep[i]->Add<hd::graphics::Polygon>();
        auto shader = m_cameras.shadowStep[i]->Add<Shader>();
        auto buffer = m_cameras.shadowStep[i]->Add<VertexBuffer>();
        auto viewport = m_cameras.shadowStep[i]->Add<Viewport>();
        auto orthographicData = m_cameras.shadowStep[i]->Add<OrthographicData>();
        auto textureTarget = m_cameras.shadowStep[i]->Add<TextureTarget>();

        textureTarget->Resize(viewportAABB.GetSize2());
        viewport->SetAABB(viewportAABB);

        orthographicData->SetAABB(viewportAABB);

        polygon->Create2DBox(viewportAABB);
        polygon->ResetTextureCoords(viewportAABB.GetSize2());

        shader->LoadShader("filterShadow.vert", "filterShadow.frag");
        //shader->LoadShader("shadowMapToEye.vert", "shadowMapToEye.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(viewportAABB);
        hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
        mesh->ResetTextureCoords(viewportAABB.GetSize2());
        buffer->AddMesh(mesh);
        buffer->SetDefaultAttributeLocations(shader);
    }

    // Global illumination
    for (int i = 0; i < m_cameras.cameraCount; ++i) {
        m_cameras.illuminationStep[i] = m_entityPool.Create();

        AABB viewportAABB = hmd->GetViewport(eyePositions[i]);

        auto shader = m_cameras.illuminationStep[i]->Add<Shader>();
        auto buffer = m_cameras.illuminationStep[i]->Add<VertexBuffer>();
        auto viewport = m_cameras.illuminationStep[i]->Add<Viewport>();
        auto orthographicData = m_cameras.illuminationStep[i]->Add<OrthographicData>();
        auto material = m_cameras.illuminationStep[i]->Add<Material>();
        auto pointKernel = m_cameras.illuminationStep[i]->Add<PointKernel>();
        auto textureTarget = m_cameras.illuminationStep[i]->Add<TextureTarget>();

        textureTarget->Resize(viewportAABB.GetSize2());
        viewport->SetAABB(viewportAABB);

        orthographicData->SetAABB(viewportAABB);

        shader->LoadShader("ambientObscurance.vert", "ambientObscurance.frag");

        material->SetTexture(TextureManager::Instance()->FindTexture("resources/noise/noise_normal_256a.png", hd::TextureType::Wall));

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(viewportAABB);
        hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
        buffer->AddMesh(mesh);
        buffer->SetDefaultAttributeLocations(shader);

        pointKernel->GenerateSSAO();
    }

    // Post processors
    for (int i = 0; i < m_cameras.cameraCount; ++i) {
        m_cameras.postProcessingStep[i] = m_entityPool.Create();

        AABB viewportAABB = hmd->GetViewport(eyePositions[i]);
        auto shader = m_cameras.postProcessingStep[i]->Add<Shader>();
        auto buffer = m_cameras.postProcessingStep[i]->Add<VertexBuffer>();
        auto viewport = m_cameras.postProcessingStep[i]->Add<Viewport>();
        auto textureTarget = m_cameras.postProcessingStep[i]->Add<TextureTarget>();
		auto orthographicData = m_cameras.postProcessingStep[i]->Add<OrthographicData>();

        textureTarget->Resize(viewportAABB.GetSize2());
        viewport->SetAABB(viewportAABB);
		orthographicData->SetAABB(viewportAABB);

        shader->LoadShader("postBlur.vert", "postBlur.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(viewportAABB);
        hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
        buffer->AddMesh(mesh);
        buffer->SetDefaultAttributeLocations(shader);
    }

    {
        m_cameras.textureRenderCamera = m_entityPool.Create();
        auto shader = m_cameras.textureRenderCamera->Add<Shader>();
        auto buffer = m_cameras.textureRenderCamera->Add<VertexBuffer>();
        auto viewport = m_cameras.textureRenderCamera->Add<Viewport>();
        auto orthographicData = m_cameras.textureRenderCamera->Add<OrthographicData>();
        auto transform = m_cameras.textureRenderCamera->Add<Transform>();

        AABB debugScreen(0, 0, hd::Device::GetDisplayScreenWidth(), hd::Device::GetDisplayScreenHeight());
        viewport->SetAABB(debugScreen);

        orthographicData->SetAABB(debugScreen);

        AABB debugScreenSize(0, 0, debugScreen.Width() * 0.2f, debugScreen.Height() * 0.2f);

        shader->LoadShader("debug/texDebug.vert", "debug/texDebug.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(debugScreenSize);
        hd::graphics::Mesh *mesh = meshData->GetMeshes()->At(0);
        buffer->AddMesh(mesh);
        buffer->SetDefaultAttributeLocations(shader);
    }

    {
        m_cameras.rayDebugEntity = m_entityPool.Create();
        m_cameras.rayDebugEntity->Add<hd::graphics::Polygon>();
        m_cameras.rayDebugEntity->Add<PolygonRenderer>();
    }
}


void AppController::Update() {
    m_entityPool.Update();

    HMD *hmd = m_client->GetEventQueue()->GetCurrentHMD();
    hmd->BeginTiming();

    m_cameras.Update();
    m_cameras.HandleInput();

    m_apps.at(m_currentAppIndex)->Update(Cameras::mainCamera);

    HandleInput();

    hmd->UpdateTiming();
}


static bool showMenu = false;

void AppController::HandleInput() {
    hdAssert(m_client);
    hdAssert(m_client->GetEventQueue());

    const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();
    for (int i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);

        auto baseEvent = event->Get<BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }

        if (event->ContainsComponent<keyboard::KeyRelease>()) {
            auto key = event->Get<keyboard::KeyRelease>();
            if (key->GetKeyCode() == hd::Device::Keycode::KEY_1 && m_apps.size() > 1) {
                SelectApp(m_apps[(m_currentAppIndex + m_apps.size() - 1) % m_apps.size()]->m_name);
            }
            if (key->GetKeyCode() == hd::Device::Keycode::KEY_2 && m_apps.size() > 1) {
                SelectApp(m_apps[(m_currentAppIndex + 1) % m_apps.size()]->m_name);
            }
            if (key->GetKeyCode() == hd::Device::Keycode::TAB && !m_homeApp.empty()) {
                SelectApp(m_homeApp);
            }
            if (key->GetKeyCode() == hd::Device::Keycode::TILDE) {
                console->ToggleConsole(Cameras::mainCamera);
            }
        }

        showMenu = false;
        if (event->ContainsComponent<device::Keyboard>()) {
            auto keyboard = event->Get<device::Keyboard>();
            showMenu = keyboard->IsKeyDown(hd::Device::Keycode::SPACE);
        }
    }

    m_apps.at(m_currentAppIndex)->HandleInput(Cameras::mainCamera);
}


void AppController::Draw() {
    // Offscreen passes change GL_VIEWPORT to their texture size. Preserve the
    // drawable viewport supplied by AppView, which uses Retina backing pixels.
    GLint drawableViewport[4];
    glGetIntegerv(GL_VIEWPORT, drawableViewport);
    if (drawableViewport[2] <= 0 || drawableViewport[3] <= 0) return;

    if (render_style == MONO_RENDER_STYLE) {
        // Keep projections and input in view points, but render every scene
        // pass at the drawable's pixel size. Reallocate only when it changes.
        const Size2 pixelSize(drawableViewport[2], drawableViewport[3]);
        auto geometry = m_cameras.eyes[0]->Edit<GeometryRenderTarget>();
        const Size2 currentSize = geometry->GetTextureSize();
        if (currentSize[0] != pixelSize[0] || currentSize[1] != pixelSize[1]) {
            geometry->Resize(pixelSize);
            m_cameras.shadowStep[0]->Edit<TextureTarget>()->Resize(pixelSize);
            m_cameras.illuminationStep[0]->Edit<TextureTarget>()->Resize(pixelSize);
            m_cameras.postProcessingStep[0]->Edit<TextureTarget>()->Resize(pixelSize);
        }
    }
    hd::input::EyePosition eyePositions[2];
    eyePositions[0] = hd::input::EyePosition::Center;

    if (render_style == STEREO_RENDER_STYLE) {
        eyePositions[0] = hd::input::EyePosition::Left;
        eyePositions[1] = hd::input::EyePosition::Right;
    }

    HMD *hmd = m_client->GetEventQueue()->GetCurrentHMD();
    AppPrototype *currentApp = m_apps.at(m_currentAppIndex);

    auto headset = Cameras::mainCamera->Get<Headset>();
    auto screenOrthographicData = Cameras::mainCamera->Get<OrthographicData>();

    const static bool renderWithEarlyZ = false;

    if (renderWithEarlyZ) {
        hd::gl::EnableFaceCulling();
        for (int i = 0; i < m_cameras.cameraCount; ++i) {
            auto geometryRenderTarget = m_cameras.eyes[i]->Edit<GeometryRenderTarget>();

            geometryRenderTarget->StartDepthOnly();
            geometryRenderTarget->SetDepthMask();
            geometryRenderTarget->ClearDepth();
            m_apps.at(m_currentAppIndex)->DrawDepth(Cameras::mainCamera, eyePositions[i]);
            geometryRenderTarget->End();
        }

        for (int i = 0; i < m_cameras.cameraCount; ++i) {
            auto geometryRenderTarget = m_cameras.eyes[i]->Edit<GeometryRenderTarget>();
            geometryRenderTarget->Start();
            geometryRenderTarget->SetColorMask();
            geometryRenderTarget->ClearColor();
            m_apps.at(m_currentAppIndex)->Draw(Cameras::mainCamera, eyePositions[i]);
            geometryRenderTarget->End();
        }
        hd::gl::DisableFaceCulling();
    } else {
        hd::gl::EnableFaceCulling();
        for (int i = 0; i < m_cameras.cameraCount; ++i) {
            auto geometryRenderTarget = m_cameras.eyes[i]->Edit<GeometryRenderTarget>();
            geometryRenderTarget->Start();
            geometryRenderTarget->Clear();
            m_apps.at(m_currentAppIndex)->Draw(Cameras::mainCamera, eyePositions[i]);
            geometryRenderTarget->End();
        }
        hd::gl::DisableFaceCulling();
    }

    // Shadow step
    const static bool doShadowStep = true;
    if (doShadowStep) {
        for (int i = 0; i < m_cameras.cameraCount; ++i) {
            auto geometryRenderTarget = m_cameras.eyes[i]->Get<GeometryRenderTarget>();
            auto viewport = m_cameras.eyes[i]->Get<Viewport>();

            auto shadowRenderTarget = m_cameras.shadowStep[i]->Edit<TextureTarget>();
            auto shadowRenderVertexBuffer = m_cameras.shadowStep[i]->Get<VertexBuffer>();
            auto shadowShader = m_cameras.shadowStep[i]->Edit<Shader>();

			auto ortho = m_cameras.postProcessingStep[i]->Get<OrthographicData>();
            auto projection = m_cameras.eyes[i]->Get<Projection>();

            Matrix projectionMatrix = projection->GetMatrix();
            Matrix viewMatrix = headset->GetLookAtMatrix(eyePositions[i]);

            Matrix viewProjectionMatrix = projectionMatrix * viewMatrix;

            Matrix inverseViewMatrix;
            Matrix inverseProjectionMatrix;// = projection->GetInverseMatrix();
            Matrix::InverseLong(inverseViewMatrix, viewMatrix);
            Matrix::InverseLong(inverseProjectionMatrix, projectionMatrix);

            Matrix lightProjectionMatrix = currentApp->m_deferredLightSystem.GetLightProjectionMatrix();
            Matrix lightModelViewMatrix = currentApp->m_deferredLightSystem.GetLightModelViewMatrix();
            Vec4 lightPositionWorldSpace = currentApp->m_deferredLightSystem.GetLightPositionWorldSpace();

            const hd::Texture *shadowMap = currentApp->m_deferredLightSystem.GetShadowTexture();

            //Vec4 projectionParams(projectionMatrix.f[0], projectionMatrix.f[5], projectionMatrix.f[10], projectionMatrix.f[14]);

            hd::gl::DisableFaceCulling();
            shadowRenderTarget->Start();
            shadowRenderTarget->Clear();
            shadowShader->Begin();
            shadowShader->SetUniform1f("iGlobalTime", m_client->GetTimer()->GetElapsedTime());

            shadowShader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, Matrix::Identity());
			shadowShader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, ortho->GetMatrix());

            shadowShader->SetUniform2f("shadowMapSize", shadowMap->width, shadowMap->height);
            shadowShader->SetUniformMatrix("lightProjectionMatrix", lightProjectionMatrix);
            shadowShader->SetUniformMatrix("lightModelViewMatrix", lightModelViewMatrix);
            shadowShader->SetUniform4f("lightPositionWorldSpace", lightPositionWorldSpace);
            shadowShader->SetUniformMatrix("inverseProjectionMatrix", inverseProjectionMatrix);
            shadowShader->SetUniformMatrix("inverseViewMatrix", inverseViewMatrix);
            //shadowShader->SetUniform4f("projectionParams", projectionParams);

            shadowShader->SetUniform3f("cameraPositionWorldSpace", headset->GetEyeWorldPosition(eyePositions[i]));
            shadowShader->BindTexture("iColorTexture", geometryRenderTarget->GetColorTexture());
            shadowShader->BindTexture("iLightingTexture", geometryRenderTarget->GetLightingTexture());
            shadowShader->BindTexture("iNormalTexture", geometryRenderTarget->GetNormalsTexture());
            shadowShader->BindTexture("iDepthTexture", geometryRenderTarget->GetDepthTexture());
            shadowShader->BindTexture("iShadowMap", shadowMap);
            shadowRenderVertexBuffer->Render();
            shadowShader->End();
            shadowRenderTarget->End();
        }
    }

    // Global illumination
    const static bool doAmbientOcclusionStep = false;
    if (doAmbientOcclusionStep) {
        for (int i = 0; i < m_cameras.cameraCount; ++i) {
            auto geometryRenderTarget = m_cameras.eyes[i]->Get<GeometryRenderTarget>();
            auto viewport = m_cameras.eyes[i]->Get<Viewport>();

            auto aoTarget = m_cameras.illuminationStep[i]->Edit<TextureTarget>();
            auto aoShader = m_cameras.illuminationStep[i]->Edit<Shader>();

            auto aoBuffer = m_cameras.illuminationStep[i]->Get<VertexBuffer>();
            auto aoNoise = m_cameras.illuminationStep[i]->Get<Material>();
            auto pointKernel = m_cameras.illuminationStep[i]->Get<PointKernel>();
			auto ortho = m_cameras.postProcessingStep[i]->Get<OrthographicData>();
            auto projection = m_cameras.eyes[i]->Get<Projection>();

            Matrix projectionMatrix = projection->GetMatrix();
            Matrix viewMatrix = headset->GetLookAtMatrix(eyePositions[i]);

            Matrix inverseViewMatrix;
            Matrix inverseProjectionMatrix;// = projection->GetInverseMatrix();
            Matrix::InverseLong(inverseViewMatrix, viewMatrix);
            Matrix::InverseLong(inverseProjectionMatrix, projectionMatrix);

            Matrix viewProjectionMatrix = projectionMatrix * viewMatrix;
            Matrix viewProjectionInverseMatrix;
            Matrix::Inverse(viewProjectionInverseMatrix, viewProjectionMatrix);

            hd::gl::DisableFaceCulling();
            aoTarget->Start();
            aoTarget->Clear();
            aoShader->Begin();
            aoShader->SetUniform1f("iGlobalTime", m_client->GetTimer()->GetElapsedTime());
            //aoShader->SetUniform1i("uKernelSize", pointKernel->GetKernelSize());
            //aoShader->SetUniform3fv("uKernelOffsets", pointKernel->GetKernelSize(), pointKernel->GetKernelLocation());
            aoShader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, Matrix::Identity());
			aoShader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, ortho->GetMatrix());
            aoShader->SetUniformMatrix("viewProjectionInverseMatrix", viewProjectionInverseMatrix);
            aoShader->SetUniformMatrix("inverseProjectionMatrix", inverseProjectionMatrix);
            aoShader->SetUniformMatrix("inverseViewMatrix", inverseViewMatrix);

            aoShader->SetUniform3f("cameraPositionWorldSpace", headset->GetEyeWorldPosition(eyePositions[i]));
            aoShader->SetUniform1f("uIntensity", 50.0f);
            aoShader->SetUniform2f("uNoiseScale", 10.0f, 10.0f);
            aoShader->SetUniform1f("uSampleRadiusWS", 50.0f);
            aoShader->SetUniform1f("uBias", 0.1f);
            const Size2 textureSize = geometryRenderTarget->GetTextureSize();
            aoShader->SetUniform2f("uScreenSize", textureSize[0], textureSize[1]);

            aoShader->BindTexture("iNormalTexture", geometryRenderTarget->GetNormalsTexture());
            aoShader->BindTexture("iDepthTexture", geometryRenderTarget->GetDepthTexture());
            aoShader->BindTexture("iNoiseTexture", aoNoise->GetTexture());
            aoBuffer->Render();
            aoShader->End();
            aoTarget->End();
        }
    }
    
    // It's only at this point that we render to the screen.
    if (render_style == MONO_RENDER_STYLE) {
        glViewport(drawableViewport[0], drawableViewport[1], drawableViewport[2], drawableViewport[3]);
        for (int i = 0; i < m_cameras.cameraCount; ++i) {
            auto geometryRenderTarget = m_cameras.eyes[i]->Get<GeometryRenderTarget>();

            auto shadowRenderTarget = m_cameras.shadowStep[i]->Get<TextureTarget>();
            auto aoTarget = m_cameras.illuminationStep[i]->Get<TextureTarget>();

            auto postRenderVertexBuffer = m_cameras.postProcessingStep[i]->Edit<VertexBuffer>();
            auto postShader = m_cameras.postProcessingStep[i]->Edit<Shader>();

            hd::gl::DisableFaceCulling();
            postShader->Begin();
            postShader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, Matrix::Identity());
			postShader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, screenOrthographicData->GetMatrix());
            // Preserve one-pixel detail in the completed scene, including UI.
            postShader->SetUniform1i("uBlurSize", 1);
            const Size2 textureSize = shadowRenderTarget->GetTextureSize();
            postShader->SetUniform2f("uTextureSize", textureSize[0], textureSize[1]);
            postShader->BindTexture("iDepthTexture", geometryRenderTarget->GetDepthTexture());
            postShader->BindTexture("iColorTexture", geometryRenderTarget->GetColorTexture());
            postShader->BindTexture("iShadowTexture", shadowRenderTarget->GetTexture());
            if (doAmbientOcclusionStep) {
                postShader->BindTexture("iAmbientOcclusionTexture", aoTarget->GetTexture());
            }
            postRenderVertexBuffer->Render();
            postShader->End();
        }

        hmd->EndTiming();
    } else {
        for (int i = 0; i < m_cameras.cameraCount; ++i) {
            auto geometryRenderTarget = m_cameras.eyes[i]->Get<GeometryRenderTarget>();

            auto shadowRenderTarget = m_cameras.shadowStep[i]->Get<TextureTarget>();
            auto aoTarget = m_cameras.illuminationStep[i]->Get<TextureTarget>();

            auto postRenderVertexBuffer = m_cameras.postProcessingStep[i]->Get<VertexBuffer>();
            auto postRenderTarget = m_cameras.postProcessingStep[i]->Edit<TextureTarget>();
            auto postShader = m_cameras.postProcessingStep[i]->Edit<Shader>();
            auto viewport = m_cameras.postProcessingStep[i]->Get<Viewport>();
			auto ortho = m_cameras.postProcessingStep[i]->Get<OrthographicData>();

            hd::gl::DisableFaceCulling();
            //postRenderTarget->Start();
            //postRenderTarget->Clear();

			hmd->PreRender(eyePositions[i]);

            postShader->Begin();
            postShader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, Matrix::Identity());
			postShader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, ortho->GetMatrix());
            postShader->SetUniform1i("uBlurSize", 3);
            const Size2 textureSize = shadowRenderTarget->GetTextureSize();
            postShader->SetUniform2f("uTextureSize", textureSize[0], textureSize[1]);
            postShader->BindTexture("iDepthTexture", currentApp->m_deferredLightSystem.GetShadowTexture()); //geometryRenderTarget->GetDepthTexture());
            postShader->BindTexture("iColorTexture", geometryRenderTarget->GetColorTexture());
            postShader->BindTexture("iShadowTexture", shadowRenderTarget->GetTexture());

            if (doAmbientOcclusionStep) {
                postShader->BindTexture("iAmbientOcclusionTexture", aoTarget->GetTexture());
            }
            
            postRenderVertexBuffer->Render();
            postShader->End();
            //postRenderTarget->End();

			hmd->PostRender(eyePositions[i]);
        }

		

        //detector->Render(m_cameras.eyes[0]->Get<GeometryRenderTarget>()->GetColorTexture(),
        //                 m_cameras.eyes[1]->Get<GeometryRenderTarget>()->GetColorTexture());

        //detector->Render(m_cameras.shadowStep[0]->Get<TextureTarget>()->GetTexture(),
         //                m_cameras.shadowStep[1]->Get<TextureTarget>()->GetTexture());
        hd::gl::DisableFaceCulling();
        hmd->Render();
        hmd->EndTiming();

		hmd->RenderMirror();
    }

    //DrawDebugTexture(currentApp->m_deferredLightSystem.GetShadowTexture());

    for (int i = 0; i < m_cameras.cameraCount; ++i) {
        //Cameras::mainCamera->Edit<Headset>()->UpdatePixelBuffer(eyePositions[i]);
    }
}


void AppController::DrawDebugTexture(const Texture *texture) {
    AppPrototype *currentApp = m_apps.at(m_currentAppIndex);
    auto buffer = m_cameras.textureRenderCamera->Get<VertexBuffer>();
    auto transform = m_cameras.textureRenderCamera->Get<Transform>();

    auto shader = m_cameras.textureRenderCamera->Edit<Shader>();
    auto orthographicData = m_cameras.textureRenderCamera->Edit<OrthographicData>();

    orthographicData->Update();
    shader->Begin();
    shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, Matrix::Identity());
    shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
    shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, orthographicData->GetMatrix());
    shader->BindTexture("iTexture0", texture);
    buffer->Render();
    shader->End();
}
