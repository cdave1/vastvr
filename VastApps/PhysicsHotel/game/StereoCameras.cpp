#include "StereoCameras.h"

using namespace hd;
using namespace hd::base;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

StereoCameras::StereoCameras(EntityPool *entityPool,
                             Client *client) :
    EntitySystem(entityPool),
    m_client(client) {
}


void StereoCameras::HandleInput() {
    DeviceDetector *detector = m_client->GetEventQueue()->GetDeviceDetector();
    
    Matrix orientation = detector->GetDeviceOrientationMatrix();
    
    Vec3 target(0, 0, -1);
    Matrix::Vec3Multiply(target, target, orientation);
    Vec3 up(0,1,0);
    Matrix::Vec3Multiply(up, up, orientation);
    
    auto cameraData = camera->GetComponent<CameraData>();
    cameraData->SetUpAxis(up);
    cameraData->SetTarget(target + cameraData->GetLocation());
    
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
            PointerSelectQuery(event);
        } else if (event->ContainsComponent<pointer::Position, pointer::Drag>()) {
            PointerDragQuery(event);
        } else if (event->ContainsComponent<pointer::Position, pointer::Release>()) {
            PointerReleaseQuery(event);
        } else if (event->ContainsComponent<button::Down>()) {
            ButtonDownQuery(event);
        } else if (event->ContainsComponent<button::Up>()) {
            ButtonUpQuery(event);
        }
    }
}


void StereoCameras::Update() {
    DeviceDetector *detector = m_client->GetEventQueue()->GetDeviceDetector();
    auto cameraData = camera->GetComponent<CameraData>();
    cameraData->Update();
    
    {
        auto viewport = camera->GetComponent<Viewport>();
        auto perspectiveData = camera->GetComponent<PerspectiveData>();
        
        const float displayWidth = Device::GetDisplayScreenWidth();
        const float displayHeight = Device::GetDisplayScreenHeight();
        
        viewport->SetAABB(AABB(0, 0, displayWidth, displayHeight));
        
        perspectiveData->SetFovY(hd_half_pi);
        perspectiveData->SetAspectY(displayWidth /displayHeight);
        perspectiveData->Update();
        
        //viewport->SetAABB(detector->GetViewport(hd::input::StereoEyeCenter));
        //perspectiveData->SetMatrix(detector->GetStereoProjectionMatrix(hd::input::StereoEyeCenter));
    }
    
    {
        auto viewport = leftEye->GetComponent<Viewport>();
        auto perspectiveData = leftEye->GetComponent<PerspectiveData>();
        
        viewport->SetAABB(detector->GetViewport(hd::input::StereoEyeLeft));
        perspectiveData->SetMatrix(detector->GetStereoProjectionMatrix(hd::input::StereoEyeLeft));
    }
    
    {
        auto viewport = rightEye->GetComponent<Viewport>();
        auto perspectiveData = rightEye->GetComponent<PerspectiveData>();
        
        AABB viewportAABB = detector->GetViewport(hd::input::StereoEyeRight);
        viewport->SetAABB(AABB(0, 0, viewportAABB.Width(), viewportAABB.Height()));
        perspectiveData->SetMatrix(detector->GetStereoProjectionMatrix(hd::input::StereoEyeRight));
    }
}


void StereoCameras::RenderLeft() {
    auto cameraRenderer = camera->GetComponent<CameraRenderer>();
    auto cameraData = camera->GetComponent<CameraData>();
    
    auto viewport = leftEye->GetComponent<Viewport>();
    auto perspectiveData = leftEye->GetComponent<PerspectiveData>();
    
    viewport->Render();
    cameraRenderer->RenderPerspectiveLookAt(perspectiveData, cameraData);
}


void StereoCameras::RenderRight() {
    auto cameraRenderer = camera->GetComponent<CameraRenderer>();
    auto cameraData = camera->GetComponent<CameraData>();
    
    auto viewport = rightEye->GetComponent<Viewport>();
    auto perspectiveData = rightEye->GetComponent<PerspectiveData>();
    
    viewport->Render();
    cameraRenderer->RenderPerspectiveLookAt(perspectiveData, cameraData);
}


void StereoCameras::PointerSelectQuery(hd::Entity *event) {
    auto responderState = camera->GetComponent<pointer::ResponderState>();
    
    if (responderState->GetCurrentState() == pointer::FREE) {
        responderState->Select();
        
        if (camera->ContainsComponent<pointer::SelectResponder>()) {
            auto responder = camera->GetComponent<pointer::SelectResponder>();
            auto baseEvent = event->GetComponent<BaseEvent>();
            baseEvent->ConsumeEvent(camera->GetID());
        }
    }
}


void StereoCameras::PointerDragQuery(hd::Entity *event) {
    auto responderState = camera->GetComponent<pointer::ResponderState>();
    
    if (responderState->GetCurrentState() == pointer::SELECTED ||
        responderState->GetCurrentState() == pointer::DRAGGED) {
        responderState->Drag();
        
        if (camera->ContainsComponent<pointer::DragResponder>()) {
            auto baseEvent = event->GetComponent<BaseEvent>();
            auto pointer = event->GetComponent<pointer::Position>();
            
            auto responder = camera->GetComponent<pointer::DragResponder>();
            auto cameraData = camera->GetComponent<CameraData>();
            auto perspective = camera->GetComponent<PerspectiveData>();
            auto viewport = camera->GetComponent<Viewport>();
            auto polygon = camera->GetComponent<Polygon>();
            
            baseEvent->ConsumeEvent(camera->GetID());
            
            Vec3 delta = pointer->GetDelta(camera);
            Vec3 diff = cameraData->GetTarget() - cameraData->GetLocation();
            Vec3 n = hd::normalize(diff);
            
            delta *= 0.0025f; // assume this is the angle in radians
            
            // We want to rotate the current target around the y axis by the x
            // distance of this delta
            Matrix xyz;
            Vec3 target;
            Matrix::RotationXYZOrigin(xyz, Vec3(5.0f * delta[1] * hd::abs(n[0]), delta[0], delta[1] * hd::abs(n[2])));
            Matrix::Vec3Multiply(target, diff, xyz);
            
            GLfloat winX, winY, winZ;
            
            winX = (float)pointer->GetPosition()[0];
            winY = (float)pointer->GetPosition()[1];
            glReadPixels(int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
            
            Vec3 posn(pointer->GetPosition()[0], pointer->GetPosition()[1], winZ);
            Matrix projMatrix = perspective->GetMatrix();
            Matrix modelMatrix = cameraData->GetMatrix();
            Vec3 output = hd::unproject(posn, modelMatrix, projMatrix, viewport->GetAABB());
            
            polygon->AddLine(cameraData->GetLocation(), output);
        }
    }
}


void StereoCameras::PointerReleaseQuery(hd::Entity *event) {
    auto *responderState = camera->GetComponent<pointer::ResponderState>();
    
    if (responderState->GetCurrentState() == pointer::DRAGGED ||
        responderState->GetCurrentState() == pointer::SELECTED) {
        responderState->Release();
        
        if (camera->GetComponent<pointer::SelectResponder>()) {
            auto *baseEvent = event->GetComponent<BaseEvent>();
            baseEvent->ConsumeEvent(camera->GetID());
        }
    }
}


void StereoCameras::ButtonDownQuery(hd::Entity *event) {
    auto baseEvent = event->GetComponent<BaseEvent>();
    auto down = event->GetComponent<button::Down>();
    auto cameraData = camera->GetComponent<CameraData>();
    
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


void StereoCameras::ButtonUpQuery(hd::Entity *event) {
    
}
