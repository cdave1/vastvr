#ifndef _ALARM_SYSTEM_H_
#define _ALARM_SYSTEM_H_

#include <Vast/Vast.h>

using namespace hd;

class AlarmSystem : public EntitySystem {
public:

    AlarmSystem(EntityPool *entityPool);

    void Update();

};

#endif