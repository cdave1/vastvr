#ifndef _STEREO_CAMERAS_H_
#define _STEREO_CAMERAS_H_

#include "hdk.h"
#include <list>

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}

class StereoCameras : public hd::EntitySystem {
public:
    
    StereoCameras(hd::EntityPool *entityPool, hd::application::Client *client);
    
    void HandleInput();
    
    void Update();
    
    void RenderLeft();
    
    void RenderRight();
    
    hd::Entity *leftEye;
    
    hd::Entity *rightEye;
    
    hd::Entity *camera;
    
private:
    
    void PointerSelectQuery(hd::Entity *event);
    
    void PointerDragQuery(hd::Entity *event);
    
    void PointerReleaseQuery(hd::Entity *event);
    
    void ButtonDownQuery(hd::Entity *event);
    
    void ButtonUpQuery(hd::Entity *event);
    
    hd::application::Client *m_client;
    
    hd::Entity * cameras[3];
    
};

#endif