#include "Cameras.h"

using namespace hd;
using namespace hd::base;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

Cameras::Cameras(EntityPool *entityPool) : EntitySystem(entityPool) {}


void Cameras::Update() {
    auto perspectiveData = mainCamera->GetComponent<PerspectiveData>();
    auto cameraData = mainCamera->GetComponent<CameraData>();
    auto screenData = mainCamera->GetComponent<ScreenData>();
    
    const float displayWidth = Device::GetDisplayScreenWidth();
    const float displayHeight = Device::GetDisplayScreenHeight();
    
    screenData->SetAABB(AABB(0, 0,
                             displayWidth,
                             displayHeight));
    
    perspectiveData->SetFovY(hd_half_pi);
    perspectiveData->SetAspectY(displayWidth /displayHeight);
    perspectiveData->Update();
    
    cameraData->Update();
}


void Cameras::Render() {
    auto cameraData = mainCamera->GetComponent<CameraData>();
    PerspectiveData *perspectiveData =  mainCamera->GetComponent<PerspectiveData>();
    CameraRenderer *cameraRenderer =    mainCamera->GetComponent<CameraRenderer>();
    
    cameraRenderer->RenderPerspectiveLookAt(perspectiveData, cameraData);
}
