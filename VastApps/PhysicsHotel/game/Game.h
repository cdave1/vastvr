#ifndef _HD_PRODUCTS_GAME_H_
#define _HD_PRODUCTS_GAME_H_

#include "hdk.h"
#include "BlockMover.h"
#include "BlockRenderer.h"
#include "BlockInputSystem.h"
#include "CameraInputSystem.h"
#include "BackgroundRenderer.h"
#include "Cameras.h"
#include "SkyboxRenderer.h"
#include "HudRenderer.h"
#include "UiInputSystem.h"
#include "LightSystem.h"
#include "StereoCameras.h"


using namespace hd;
using namespace hd::base;
using namespace hd::input;
using namespace hd::graphics;

namespace hd {
    namespace application {
        class Client;
    }
}

namespace CollisionCategory {
    typedef enum {
        StaticGeometry = 0x0001,
        Player = 0x0002,
        HarvestItem = 0x0004
    } Category;
}

class hdGame {
public:
    hdGame(hd::application::Client *client);
        
    ~hdGame();
    
    void AddBackground(const float width, const float height, const Vec3 &rotation, const Vec3 &center);
    
    void SpawnBlock(Vec3 pos);
    
    void SpawnPlayerCharacter(Vec3 pos);
    
    void SpawnHarvestItem(AABB aabb);
    
    void OnBlockSelect(Entity *block, const Vec3 &position);
    
    void OnBlockDrag(Entity *block, const Vec3 &position, const Vec3 &delta);
    
    void OnBlockRelease(Entity *block, const Vec3 &position);
    
    
    void OnUiSelect(Entity *element, const Vec3 &position);
    
    void OnUiRelease(Entity *element, const Vec3 &position);
    
    
    void Update();
    
    void Draw();
    
    
private:
    
    void CreateCamera();
    
    void CreateStereoCamera();
    
    void ConstructSkybox();
    
    void CreateLights();
    
    void CreateHud();
    
    void CreateButton(float width, float height, Vec3 center);
    
    void InitPhysicsObjects();
    
    void AddStaticCube(float size, Vec3 center);
    
    void AddDynamicBlock();
    
    void RemoveDynamicBlock();
    
    hd::EntityPool m_entityPool;
    
    hd::EntityPool m_uiEntityPool;
    
    Entity *m_physicsWorld;
    
    Entity *m_pointerJointAnchor;
    
    BlockMover m_blockMover;
    
    BlockRenderer m_blockRenderer;
    
    BlockInputSystem m_blockInputSystem;
    
    CameraInputSystem m_cameraInputSystem;
    
    BackgroundRenderer m_backgroundRenderer;
    
    Cameras m_cameras;
    
    StereoCameras m_stereoCameras;
    
    HudRenderer m_hudRenderer;
    
    UiInputSystem m_uiInput;
    
    SkyboxRenderer m_skyboxRenderer;
    
    LightSystem m_lightSystem;
    
    hd::application::Client *m_client;
    
    hd::ComponentPool<CameraData> *m_cameraDataComponents;
    hd::ComponentPool<PerspectiveData> *m_perspectiveDataComponents;
    hd::ComponentPool<CameraRenderer> *m_cameraRendererComponents;
    hd::ComponentPool<ScreenData> *m_cameraScreenData;
    hd::ComponentPool<Viewport> *m_viewportComponents;
    hd::ComponentPool<RenderTarget> *m_renderTargets;
    hd::ComponentPool<RayTracer> *m_rayTracerComponents;
    
    hd::ComponentPool<physics2d::Body> *m_physicsBodyComponents;
    hd::ComponentPool<physics2d::World> *m_physicsWorldComponents;
    hd::ComponentPool<physics2d::PointerJoint> *m_pointerJointComponents;
    
    hd::ComponentPool<Polygon> *m_polygonComponents;
    hd::ComponentPool<WorldData> *m_worldDataComponents;
    hd::ComponentPool<AABBData> *m_aabbDataComponents;
    hd::ComponentPool<PolygonRenderer> *m_polygonRendererComponents;
    hd::ComponentPool<Material> *m_materials;
    hd::ComponentPool<ExtrusionData> *m_extrusions;
    hd::ComponentPool<ExtrusionRenderer> *m_extrusionRenderers;
    hd::ComponentPool<Shader> *m_shaders;
    hd::ComponentPool<VertexBuffer> *m_vertexBuffers;
    hd::ComponentPool<Light> *m_lightComponents;
    hd::ComponentPool<LightRenderer> *m_lightRenderers;

    hd::ComponentPool<Curve> *m_curves;
    hd::ComponentPool<CurveRenderer> *m_curveRenderers;
    
    hd::ComponentPool<pointer::ResponderState> *m_pointerResponderStates;
    hd::ComponentPool<pointer::SelectResponder> *m_pointerSelectResponders;
    hd::ComponentPool<pointer::DragResponder> *m_pointerDragResponders;
    hd::ComponentPool<pointer::ReleaseResponder> *m_pointerReleaseResponders;
    
    // UI Components
    hd::ComponentPool<graphics::ScreenData> *m_uiScreenData;
};

#endif