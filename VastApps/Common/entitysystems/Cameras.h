#ifndef _CAMERAS_H_
#define _CAMERAS_H_

#include <Vast/Vast.h>

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}

class Cameras : public hd::EntitySystem {
public:
    
    Cameras(hd::EntityPool *entityPool,
            hd::application::Client *client);
    
    void HandleInput();
    
    void Update();
    
    void Render();
    
    static hd::Entity *mainCamera;

    hd::Entity *eyes[2];

    hd::Entity *shadowStep[2];

    hd::Entity *illuminationStep[2];

    hd::Entity *postProcessingStep[2];
    
    hd::Entity *textureRenderCamera;

    hd::Entity *rayDebugEntity;

    Color4f eyeGazeColor[2];

    uint32_t cameraCount;
    
private:

    void PointerMoveQuery(hd::Entity *event);

    void PointerSelectQuery(hd::Entity *event);
    
    void PointerDragQuery(hd::Entity *event);
    
    void PointerReleaseQuery(hd::Entity *event);
    
    void KeySelectQuery(hd::Entity *event);
    
    void KeyReleaseQuery(hd::Entity *event);
    
    void KeyboardQuery(hd::Entity *event);
    
    void GamepadQuery(hd::Entity *event);

	void GamepadAsTouchController(hd::Entity *event);
    
    hd::application::Client *m_client;
    
};

#endif