#ifndef _CAMERA_INPUT_SYSTEM_H_
#define _CAMERA_INPUT_SYSTEM_H_

#include "hdk.h"

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}

class hdGame;

class CameraInputSystem : public hd::EntitySystem {
public:
    
    CameraInputSystem(hd::EntityPool *entityPool, hd::application::Client *client);
    
    void Update(hd::Entity *currentCamera);
    
    
private:
    
    void PointerSelectQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void PointerDragQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void PointerReleaseQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void ButtonDownQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void ButtonUpQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    hd::application::Client *m_client;
    
};

#endif