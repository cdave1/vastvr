#ifndef _VAST_COMMON_APP_H_
#define _VAST_COMMON_APP_H_

#include <vector>

#include <Vast/Vast.h>
#include "entitysystems/Cameras.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

namespace hd {
    namespace application {
        class Client;
    }
}

class AppPrototype;

class AppController {
public:
    AppController(hd::application::Client *client);
        
    ~AppController();

    void AddApp(AppPrototype *app);

    bool SelectApp(const std::string &name);

    const AppPrototype * GetCurrentApp() const;

    void SetHomeApp(const std::string &name) { m_homeApp = name; }
    
    void HandleInput();
    
    void Update();
    
    void Draw();
    
    
private:
    
    void CreateCamera();

    void DrawDebugTexture(const Texture *texture);
    
    hd::EntityPool m_entityPool;
    
    Cameras m_cameras;
    
    std::vector<AppPrototype *> m_apps;
    
    uint32_t m_currentAppIndex;

    std::string m_homeApp;
    
    hd::application::Client *m_client;

};

#endif
