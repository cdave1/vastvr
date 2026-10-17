#ifndef _APPS_BUILDER_SERIALIZATION_H_
#define _APPS_BUILDER_SERIALIZATION_H_

#include <Vast/Vast.h>

class Builder;

class BuilderSerialization {
private:
    friend class Game;

    static void SaveToJSON(hd::EntityPool *entityPool, const std::string &name = "");

    static void LoadFromJSON(hd::EntityPool *entityPool, std::function<void(hd::Entity *entity)> onRestoreCallback);
    
};

#endif
