#ifndef _APPS_SSAO_TEST_H_
#define _APPS_SSAO_TEST_H_

#include "AppPrototype.h"

class SSAOTest : public AppPrototype {
public:
    SSAOTest(hd::application::Client *client);
    
    ~SSAOTest();
    
    void CreateSSAOTest();
    
    void OnBlockSelect(hd::Entity *block, const Collision3 &collision);
    
    void OnBlockDrag(hd::Entity *block, const Collision3 &collision, const Vec3 &delta);
    
    void OnBlockRelease(hd::Entity *block, const Collision3 &collision);
    
};

#endif
