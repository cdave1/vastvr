#include "LuaLoader.h"

hdk::LuaLoader::LuaLoader() : L()
{
    //lua_newstate()
}


hdk::LuaLoader::~LuaLoader()
{
    lua_close(L);
}
