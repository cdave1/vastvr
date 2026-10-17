#ifndef _VAST_LUA_LOADER_H_
#define _VAST_LUA_LOADER_H_

#include <iostream>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

namespace hdk
{
    class LuaLoader
    {
    public:
        
        LuaLoader();
        
        ~LuaLoader();
        
        
        
    private:
        
        lua_State *L;
        
        
        
    };
}


#endif