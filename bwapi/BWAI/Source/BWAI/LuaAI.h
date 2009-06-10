#pragma once

#include <Util/Types.h>

#include <BWAPI.h>
#include <LUA/lua.hpp>

namespace BWAI
{
  class LUA
  {
    public:
      LUA(); // construct
      ~LUA(); // destruct
      bool executeFile(const char *fn); // execute
    private:
      lua_State *lua;
      lua_Debug luaDebug;
    // interfaces to BWAI and BWAPI go here
      static int sendText(lua_State *l);
      static int getMapSize(lua_State *l);
      static int getMapName(lua_State *l);
      static int getTerrainInfo(lua_State *l);
//      static int getCurrentPlayer(lua_State *l);
      static int getPlayerName(lua_State *l);
/*
      static int getStartLocation(lua_State *l);
      static int getMapHash(lua_State *l);
      static int getPlayerRace(lua_State *l);
      static int getCounts(lua_State *l);
      static int getScore(lua_State *l);
      static int getForceName(lua_State *l);
      static int getPlayerOwner(lua_State *l);
      static int isFlagEnabled(lua_State *l);
      static int enableFlag(lua_State *l);
*/


  };  // class LUA
} // namespace BWAI

