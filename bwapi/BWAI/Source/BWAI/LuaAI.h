#pragma once

#include <Util/Types.h>

#include <BWAPI.h>

extern "C"
{
  #include "../Lua/Source/lua.h"
  #include "../Lua/Source/lualib.h"
  #include "../Lua/Source/lauxlib.h"
  #include "../Lua/Source/luaconf.h"
}


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
      static int getCurrentPlayer(lua_State *l);
      static int getPlayerName(lua_State *l);
      static int getMapHash(lua_State *l);
      static int getRace(lua_State *l);
      static int getForceName(lua_State *l);
      static int isUpgrading(lua_State *l);
      static int getUpgrade(lua_State *l);
      static int isResearched(lua_State *l);
      static int isResearching(lua_State *l);
      static int getKills(lua_State *l);
      static int getDeaths(lua_State *l);
      static int getAllUnits(lua_State *l);
      static int getCompleteUnits(lua_State *l);
      static int getIncompleteUnits(lua_State *l);


/*
      static int getStartLocation(lua_State *l);
      static int getCounts(lua_State *l);
      static int getScore(lua_State *l);
      static int getForceName(lua_State *l);
      static int isFlagEnabled(lua_State *l);
      static int enableFlag(lua_State *l);
*/


  };  // class LUA
} // namespace BWAI

