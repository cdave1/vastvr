#include "DeferredLightSystem.h"

#define GLOBAL_LIGHT_ENTITY_KEY "GlobalLight"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

DeferredLightSystem::DeferredLightSystem(EntityPool *entityPool, hd::application::Client *client) :
    EntitySystem(entityPool), m_client(client), m_rotationDelta(0.0f) {}

void DeferredLightSystem::HandleInput() {
    hdAssert(m_client);
    hdAssert(m_client->GetEventQueue());

    const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();

    for (int i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);

        auto baseEvent = event->Get<hd::input::BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }

        if (event->ContainsComponent<pointer::Position, pointer::Move>()) {
            auto move = event->Edit<pointer::Move>();
            auto pointer = event->Edit<pointer::Position>();

            if (move->GetControl() == pointer::Control::PRIMARY_SCROLL_WHEEL) {
                m_rotationDelta += pointer->GetDelta()[1];
            }
        }
    }
}


void DeferredLightSystem::Update(hd::Entity *camera) {
    EntityQuery *lights = m_entityPool->QueryByName(GLOBAL_LIGHT_ENTITY_KEY);

    Vec3 playerLocation = camera->GetComponent<hd::input::Headset>()->GetPosition();

    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
    //glCullFace(GL_FRONT);

	for (int i = 0; i < lights->Size(); ++i) {
        Entity *lightEntity = lights->At(i);

        auto perspectiveData = lightEntity->GetComponent<PerspectiveData>();
        auto viewport = lightEntity->GetComponent<Viewport>();
        auto orthographicData = lightEntity->GetComponent<OrthographicData>();

        auto depthRenderTarget = lightEntity->Edit<DepthRenderTarget>();
        auto lookAtData = lightEntity->Edit<LookAtData>();
        auto light = lightEntity->Edit<Light>();

        lookAtData->Rotate(Quaternion(Vec3(-1, 0, 0), 0.0f));
        
        Vec3 location(5714, 4700, 7000);
        location = Quaternion(Vec3(1, 0, 0), m_rotationDelta * 0.002f) * location;
        lookAtData->SetLocation(location + playerLocation);
        lookAtData->FollowTarget(playerLocation);

        light->SetPosition(lookAtData->GetLocation());
        light->SetSpotDirection(hd::normalize(lookAtData->GetLocation() - lookAtData->GetTarget()));
/*
        AABB space = orthographicData->GetAABB();
        space.SetCenter(playerLocation);
        orthographicData->SetAABB(space);
        orthographicData->Update();
*/
        depthRenderTarget->Start();
        depthRenderTarget->Clear();
        //cameraRenderer->RenderPerspectiveLookAt(perspectiveData, lookAtData);
        //cameraRenderer->RenderOrthographicLookAt(orthographicData, lookAtData);
        depthRenderTarget->RenderGeometry(lightEntity);
        depthRenderTarget->End();
    }

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
}


Matrix DeferredLightSystem::GetLightProjectionMatrix() {
    EntityQuery *lights = m_entityPool->QueryByName(GLOBAL_LIGHT_ENTITY_KEY);
    if (lights->Size() > 0) {
        Entity *lightEntity = lights->At(0);

        auto orthographicData = lightEntity->GetComponent<OrthographicData>();

        return orthographicData->GetMatrix();
    } else {
        return Matrix::Identity();
    }
}


Matrix DeferredLightSystem::GetLightModelViewMatrix() {
    EntityQuery *lights = m_entityPool->QueryByName(GLOBAL_LIGHT_ENTITY_KEY);
    if (lights->Size() > 0) {
        Entity *lightEntity = lights->At(0);

        auto lookAtData = lightEntity->GetComponent<LookAtData>();

        return lookAtData->GetMatrix();
    } else {
        return Matrix::Identity();
    }
}


Vec4 DeferredLightSystem::GetLightPositionWorldSpace() {
    EntityQuery *lights = m_entityPool->QueryByName(GLOBAL_LIGHT_ENTITY_KEY);
    if (lights->Size() > 0) {
        Entity *lightEntity = lights->At(0);
        auto light = lightEntity->Get<Light>();
        return Vec4(light->GetPosition()[0],
                    light->GetPosition()[1],
                    light->GetPosition()[2],
                    light->IsDirectional() ? 1.0f : 0.0f);
    } else {
        return Vec4();
    }
}


const hd::Texture * DeferredLightSystem::GetShadowTexture() {
    EntityQuery *lights = m_entityPool->QueryByName(GLOBAL_LIGHT_ENTITY_KEY);
    if (lights->Size() > 0) {
        Entity *lightEntity = lights->At(0);
        auto depthRenderTarget = lightEntity->GetComponent<DepthRenderTarget>();
        return depthRenderTarget->GetDepthTexture();
    } else {
        return NULL;
    }
}
