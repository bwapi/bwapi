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
/*      static int getMapSize(lua_State *l);
      static int getMapName(lua_State *l);
      static int getTerrainInfo(lua_State *l);
      static int getStartLocation(lua_State *l);
      static int getCurrrentPlayer(lua_State *l);
      static int getMapHash(lua_State *l);
      static int getPlayerName(lua_State *l);
      static int getPlayerRace(lua_State *l);
      static int getMinerals(lua_State *l);
      static int getGas(lua_State *l);
      static int getSupplyTotal(lua_State *l);
      static int getSupplyUsed(lua_State *l);
      static int getAllUnitCount(lua_State *l);
      static int getCompletedUnitCount(lua_State *l);
      static int getIncompletedUnitCount(lua_State *l);
      static int getDeaths(lua_State *l);
      static int getKills(lua_State *l);
      static int getForceName(lua_State *l);
      static int getPlayerOwner(lua_State *l);
      static int getUnitCountOnTile(lua_State *l);
      static int isFlagEnabled(lua_State *l);
      static int enableFlag(lua_State *l);
*/


  };  // class LUA
} // namespace BWAI

