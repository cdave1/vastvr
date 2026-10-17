#ifndef _APP_GAMEPAD_SYSTEM_H_
#define _APP_GAMEPAD_SYSTEM_H_

#include <Vast/Vast.h>

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}

class hdGame;

class GamepadSystem : public hd::EntitySystem {
public:
    
	GamepadSystem(hd::EntityPool *entityPool, hd::application::Client *client, const std::string entityName);
    
    void Update(hd::Entity *currentCamera);

    std::string m_entityName;


private:

	void GamepadQuery(hd::Entity * event);

    hd::Entity * RayTestQuery(hd::Entity *event, hd::Entity *currentCamera, Vec3 pointerPosition,
                              Collision3 *collision);
    
    hd::application::Client *m_client;
    
};

#endif