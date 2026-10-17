#include "CameraInputSystem.h"
#include "Game.h"
#include <assert.h>
#include <GLUT/glut.h>

using namespace hd;
using namespace hd::base;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

CameraInputSystem::CameraInputSystem(EntityPool *entityPool,
                                     Client *client) :
    EntitySystem(entityPool),
    m_client(client) {}


void CameraInputSystem::Update(Entity *currentCamera) {
    hdAssert(m_client);
    hdAssert(m_client->GetEventQueue());
    
    const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();
    
    for (int i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);
        
        BaseEvent *baseEvent = event->GetComponent<BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }
        
        if (event->ContainsComponent<pointer::Position, pointer::Select>()) {
            PointerSelectQuery(event, currentCamera);
        } else if (event->ContainsComponent<pointer::Position, pointer::Drag>()) {
            PointerDragQuery(event, currentCamera);
        } else if (event->ContainsComponent<pointer::Position, pointer::Release>()) {
            PointerReleaseQuery(event, currentCamera);
        } else if (event->ContainsComponent<button::Down>()) {
            ButtonDownQuery(event, currentCamera);
        } else if (event->ContainsComponent<button::Up>()) {
            ButtonUpQuery(event, currentCamera);
        }
    }
    
    auto cameraData = currentCamera->GetComponent<CameraData>();
    DeviceDetector *detector = m_client->GetEventQueue()->GetDeviceDetector();
    
    Matrix orientation = detector->GetDeviceOrientationMatrix();
    
    Vec3 target(0, 0, -1);
    Matrix::Vec3Multiply(target, target, orientation);
    Vec3 up(0,1,0);
    Matrix::Vec3Multiply(up, up, orientation);

    cameraData->SetUpAxis(up);
    cameraData->SetTarget(target + cameraData->GetLocation());
    cameraData->Update();
}


void CameraInputSystem::PointerSelectQuery(Entity *event, Entity *currentCamera) {
    auto responderState = currentCamera->GetComponent<pointer::ResponderState>();
        
    if (responderState->GetCurrentState() == pointer::FREE) {
        responderState->Select();
        
        if (currentCamera->ContainsComponent<pointer::SelectResponder>()) {
            auto responder = currentCamera->GetComponent<pointer::SelectResponder>();
            auto baseEvent = event->GetComponent<BaseEvent>();
            
            baseEvent->ConsumeEvent(currentCamera->GetID());
        }
    }
}


void CameraInputSystem::PointerDragQuery(Entity *event, Entity *currentCamera) {
    auto responderState = currentCamera->GetComponent<pointer::ResponderState>();
    
    if (responderState->GetCurrentState() == pointer::SELECTED ||
        responderState->GetCurrentState() == pointer::DRAGGED) {
        responderState->Drag();
        
        if (currentCamera->ContainsComponent<pointer::DragResponder>()) {
            auto baseEvent = event->GetComponent<BaseEvent>();
            auto pointer = event->GetComponent<pointer::Position>();
            
            auto responder = currentCamera->GetComponent<pointer::DragResponder>();
            auto cameraData = currentCamera->GetComponent<CameraData>();
            auto perspective = currentCamera->GetComponent<PerspectiveData>();
            auto screen = currentCamera->GetComponent<ScreenData>();
            auto polygon = currentCamera->GetComponent<Polygon>();
            auto rayTracer = currentCamera->GetComponent<RayTracer>();
            
            baseEvent->ConsumeEvent(currentCamera->GetID());

            Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                                    screen->GetAABB(),
                                                    perspective,
                                                    cameraData);
            
            polygon->AddLine(cameraData->GetLocation(), output);
        }
    }
}


void CameraInputSystem::PointerReleaseQuery(Entity *event, Entity *currentCamera) {
    auto *responderState = currentCamera->GetComponent<pointer::ResponderState>();
    
    if (responderState->GetCurrentState() == pointer::DRAGGED ||
        responderState->GetCurrentState() == pointer::SELECTED) {
        responderState->Release();
        
        if (currentCamera->GetComponent<pointer::SelectResponder>()) {
            auto *baseEvent = event->GetComponent<BaseEvent>();
            baseEvent->ConsumeEvent(currentCamera->GetID());
        }
    }
}


void CameraInputSystem::ButtonDownQuery(hd::Entity *event, hd::Entity *currentCamera) {
    auto baseEvent = event->GetComponent<BaseEvent>();
    auto down = event->GetComponent<button::Down>();
    auto cameraData = currentCamera->GetComponent<CameraData>();
    
    const float MOVEMENT_SPEED = 100.0f;

    Vec3 diff = cameraData->GetTarget() - cameraData->GetLocation();
    Vec3 n = hd::normalize(diff);
    
    Matrix rotn;
    
    if (down->GetButtonValue() == 'w') {
        // Move both vectors along current direction.
        
    } else if (down->GetButtonValue() == 'a') {
        
        Matrix::RotationY(rotn, hd_half_pi);
        Matrix::Vec3Multiply(n, n, rotn);
        
    } else if (down->GetButtonValue() == 'd') {
        
        Matrix::RotationY(rotn, -hd_half_pi);
        Matrix::Vec3Multiply(n, n, rotn);
        
    } else if (down->GetButtonValue() == 's') {
        n = -n;
        
    } else {
        return;
    }
    
    cameraData->SetLocation(cameraData->GetLocation() + (MOVEMENT_SPEED * n));
    cameraData->SetTarget(cameraData->GetTarget() + (MOVEMENT_SPEED * n));
    
    
    hdPrintf("Client received button down: %c\n", down->GetButtonValue());
}


void CameraInputSystem::ButtonUpQuery(hd::Entity *event, hd::Entity *currentCamera) {
    auto baseEvent = event->GetComponent<BaseEvent>();
    auto up = event->GetComponent<button::Up>();
    
    hdPrintf("Client received button up: %c\n", up->GetButtonValue());
}
