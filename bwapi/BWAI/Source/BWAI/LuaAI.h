/*#pragma once

#include <Util/Types.h>

#include <BWAPI/Globals.h>
#include <BWAPI/Game.h>

#include <LUA/lua.hpp>

namespace BWAI
{
  class LUA
  {
    public:
      LUA(); // construct
      ~LUA(); // destruct
      static bool executeFile(const char *fn); // execute
    private:
      lua_State *lua;
      lua_Debug luaDebug;
    // interfaces to BWAI and BWAPI go here
      static int sendText(lua_State *l);

  };  // class LUA
} // namespace BWAI

*/