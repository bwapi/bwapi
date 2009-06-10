#include "LuaAI.h"
#include "AI.h"

namespace BWAI
{
  LUA::LUA(void)
  {
    lua = lua_open();
    luaL_openlibs(lua);
    lua_register(lua, "sendText", sendText);
//    lua_register(lua, "mapSize", getMapSize);
//    lua_register(lua, "mapName", getMapName);
//    lua_register(lua, "mapFile", getMapFileName);

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
// ======================================== Send text
  int LUA::sendText(lua_State *l)
  {
    if(lua_gettop(l) != 1)
    {
      // error msg
      return 0;
    }

    if(!lua_isstring(l, 1))
    {
      // error msg
      return 0;
    }

    BWAPI::Broodwar->printPublic(lua_tostring(l, 1));
    
    lua_pushnumber(l, 1);
    return 1;
  }
/*// ======================================== Get Map Size
  int LUA::getMapSize(lua_State *l)
  {
    if(lua_gettop(l) != 0)
    {
      // error msg
      return 0;
    }

    lua_pushnumber(l, BWAPI::Broodwar->mapWidth());
    lua_pushnumber(l, BWAPI::Broodwar->mapHeight());
    return 2;
  }
// ======================================== Get Map Name
  int LUA::getMapName(lua_State *l)
  {
    if (lua_gettop(l) != 1)
    {
      // error msg
      return 0;
    }

    switch((u32)lua_tonumber(l, 1))
    {
    case 0:
      lua_pushstring(l, BWAPI::Broodwar->mapFilename().c_str());
      break;
    case 1:
      lua_pushstring(l, BWAPI::Broodwar->mapName().c_str());
      break;
    }
    return 1;
  }
// ======================================== Get Terrain Info
  int LUA::getTerrainInfo(lua_State *l)
  {
    if (lua_gettop(l) != 3)
    {
      // error msg
      return 0;
    }

    u32 x = lua_tonumber(l,2);
    u32 y = lua_tonumber(l,3);

    switch((u32)lua_tonumber(l, 1))
    {
    case 0: // buildable
      lua_pushnumber(l, BWAPI::Broodwar->buildable(x, y));
      break;
    case 1: // walkable
      lua_pushnumber(l, BWAPI::Broodwar->walkable(x, y));
      break;
    case 2: // visible
      lua_pushnumber(l, BWAPI::Broodwar->walkable(x, y));
      break;
    case 3: // ground height
      lua_pushnumber(l, BWAPI::Broodwar->groundHeight(x, y));
      break;
    }
    return 1;
  }
  */
  
} // namespace BWAI

