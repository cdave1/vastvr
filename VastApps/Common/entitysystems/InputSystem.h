#ifndef _APP_INPUT_SYSTEM_H_
#define _APP_INPUT_SYSTEM_H_

#include <Vast/Vast.h>

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}

class hdGame;

class InputSystem : public hd::EntitySystem {
public:
    
    InputSystem(hd::EntityPool *entityPool, hd::application::Client *client, const std::string entityName);
    
    void Update(hd::Entity *currentCamera);

    std::string m_entityName;


private:

    void PointerMoveQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void PointerSelectQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void PointerDragQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void PointerReleaseQuery(hd::Entity *event, hd::Entity *currentCamera);

    void ButtonSelectQuery(hd::Entity *event, hd::Entity *currentCamera);

    void ButtonReleaseQuery(hd::Entity *event, hd::Entity *currentCamera);

	void GamepadQuery(hd::Entity * event);

    void ScrollWheelQuery(hd::Entity *event, hd::Entity *currentCamera);

    void LookAtQuery(hd::Entity *event, hd::Entity *currentCamera);

    hd::Entity * RayTestQuery(hd::Entity *event, hd::Entity *currentCamera, Vec3 pointerPosition,
                              Collision3 *collision);
    
    hd::application::Client *m_client;
    
};

#endif