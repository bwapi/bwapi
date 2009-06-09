#include "LuaAI.h"

namespace BWAI
{
  LUA::LUA(void)
  {
    lua = lua_open();
    luaL_openlibs(lua);
    lua_register(lua, "sendText", sendText);
  
  } // constructor

  LUA::~LUA(void)
  {
    lua_close(lua);
  } // destructor
  
  bool LUA::executeFile(const char *fn)
  {
    if(luaL_dofile(lua, fn) != 0)
      return false;
    return true;
  } // executeFile

  int LUA::sendText(lua_State *l)
  {
    u32 argcount = lua_gettop(l);
    //const char *test = lua_tostring(l, 1);
    
    BWAPI::Broodwar->printPublic(lua_tostring(l, 1));
    
    lua_pushnumber(l, 1);
    return 1;
  }
  
} // namespace BWAI

