#ifndef _VAST_APP_PROTOTYPE_H_
#define _VAST_APP_PROTOTYPE_H_

#include <Vast/Vast.h>
#include "entitysystems/TransformSystem.h"
#include "entitysystems/BlockMover.h"
#include "entitysystems/VertexBufferRenderer.h"
#include "entitysystems/InputSystem.h"
#include "entitysystems/BackgroundRenderer.h"
#include "entitysystems/LabelRenderer.h"
#include "entitysystems/SkyboxRenderer.h"
#include "entitysystems/HudRenderer.h"
#include "entitysystems/GeometryRenderer.h"
#include "entitysystems/DeferredLightSystem.h"
#include "entitysystems/AnimationSystem.h"
#include "entitysystems/AlarmSystem.h"
#include "entitysystems/CullingSystem.h"
#include "entitysystems/MeshRenderer.h"

class AppPrototype {
private:
    hd::EntityPool m_entityPool;


protected:

    TransformSystem m_transformSystem;
    
    BlockMover m_blockMover;
    
    LabelRenderer m_labelRenderer;
    
    InputSystem m_blockInputSystem;
    
    SkyboxRenderer m_skyboxRenderer;
    
    DeferredLightSystem m_deferredLightSystem;
    
    GeometryRenderer m_geometryRenderer;

    AnimationSystem m_animationSystem;

    AlarmSystem m_alarmSystem;
    
    hd::sound::MusicPlayer m_musicPlayer;

    hd::application::Client *m_client;

    CullingSystem m_sceneCulling;
    MeshRenderer m_modelRenderer;
    VertexBufferRenderer m_sceneRenderer;

public:

    std::string m_name;

    std::string m_iconPath;

    virtual void OnActivate(hd::Entity *camera);
    virtual void OnDeactivate();

    
protected:
    friend class AppController;
    
    AppPrototype(hd::application::Client *client);
    
    hd::EntityPool * GetEntityPool() { return &m_entityPool; }
    
    void OnBlockShaderRenderCallback(hd::graphics::Shader *shader);
    
    void AddBackground(const float width, const float height, const Vec3 &rotation, const Vec3 &center);
    
    void ConstructSkybox(unsigned index = 0);
    
    void CreateLights();
    
    void CreateGlobalLight(const Vec3 &position);
    
    void CreateObjects();
    
    void AddStaticCube(float size, Vec3 center);

    Entity * AddCube(float width, float height, float depth, Vec3 center);
    
    Entity * AddFloorCube(float size, float height, Vec3 center);
    
    Entity * AddMesh(const std::string &meshFile, float size, Vec3 center);
    
    Entity * AddLabel(Vec3 center, Vec3 space, const std::string &text);
    
    Entity * AddMenuItem(Vec3 center, float yrot,
                         const std::string imagePath,
                         const std::string identifierString = "NoIdentifier",
                         float radius = 350.0f);
    
    virtual void Update(Entity *camera);

    virtual void HandleInput(Entity *camera);

    virtual void DrawDepth(hd::Entity *camera, hd::input::EyePosition eyePosition);

    virtual void Draw(hd::Entity *camera, hd::input::EyePosition eyePosition);
    
};

#endif
