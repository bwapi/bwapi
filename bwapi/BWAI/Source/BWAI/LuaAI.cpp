#include "LuaAI.h"
#include "AI.h"

#include <BWAPI.h>

#include "Globals.h"
#include "TaskUpgrade.h"
#include "TaskInvent.h"
#include "Player.h"
#include "BuildingPositionSet.h"

namespace BWAI
{
  LUA::LUA(void)
  {
    lua = lua_open();
    luaL_openlibs(lua);
    lua_register(lua, "sendText", sendText);
    lua_register(lua, "mapSize", getMapSize);
    lua_register(lua, "mapName", getMapName);
    lua_register(lua, "terrainInfo", getTerrainInfo);
    lua_register(lua, "currentPlayer", getCurrentPlayer);
    lua_register(lua, "playerName", getPlayerName);
    lua_register(lua, "mapHash", getMapHash);
    lua_register(lua, "race", getRace);
    lua_register(lua, "force", getForceName);
    lua_register(lua, "upgrading", isUpgrading);
    lua_register(lua, "upgraded", getUpgrade);
    lua_register(lua, "researched", isResearched);
    lua_register(lua, "researching", isResearching);
    lua_register(lua, "kills", getKills);
    lua_register(lua, "deaths", getDeaths);
    lua_register(lua, "units", getAllUnits);
    lua_register(lua, "complete", getCompleteUnits);
    lua_register(lua, "incomplete", getIncompleteUnits);
    lua_register(lua, "build", build);
    lua_register(lua, "upgrade", upgrade);
    lua_register(lua, "research", upgrade);

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
  
// ======================================== Get Map Size
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
      lua_pushstring(l, BWAPI::Broodwar->mapName().c_str());
      break;
    case 1:
      lua_pushstring(l, BWAPI::Broodwar->mapFilename().c_str());
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
      lua_pushnumber(l, BWAPI::Broodwar->visible(x, y));
      break;
    case 3: // ground height
      lua_pushnumber(l, BWAPI::Broodwar->groundHeight(x, y));
      break;
    }
    return 1;
  }
// ======================================== Get Current Player
  int LUA::getCurrentPlayer(lua_State *l)
  {
    if (lua_gettop(l) != 0)
    {
      // error msg
      return 0;
    }
    lua_pushnumber(l, BWAPI::Broodwar->self()->getID());
    return 1;
  }
// ======================================== Get Player Name
  int LUA::getPlayerName(lua_State *l)
  {
    if (lua_gettop(l) != 0)
    {
      // error msg
      return 0;
    }
    lua_pushstring(l, BWAPI::Broodwar->self()->getName().c_str());
    return 1;
  }
// ======================================== Get Map Hash
  int LUA::getMapHash(lua_State *l)
  {
    if (lua_gettop(l) != 0)
    {
      // error msg
      return 0;
    }
    lua_pushnumber(l, BWAPI::Broodwar->getMapHash());
    return 1;
  }
// ======================================== Get Race
  int LUA::getRace(lua_State *l)
  {
    if (lua_gettop(l) != 0)
    {
      // error msg
      return 0;
    }
    lua_pushnumber(l, BWAPI::Broodwar->self()->getRace().getID());
    return 1;
  }
// ======================================== Get Force Name
  int LUA::getForceName(lua_State *l)
  {
    if (lua_gettop(l) != 0)
    {
      // error msg
      return 0;
    }
    lua_pushstring(l, BWAPI::Broodwar->self()->getForce()->getName().c_str());
    return 1;
  }
// ======================================== Is Upgrading
  int LUA::isUpgrading(lua_State *l)
  {
    u8 type = (u8)lua_tonumber(l, 1);
    if (lua_gettop(l) != 1 || type > 60)
    {
      // error msg
      return 0;
    }
    lua_pushnumber(l, BWAPI::Broodwar->self()->upgrading(type));
    return 1;
  }
// ======================================== Get Upgrade Level
  int LUA::getUpgrade(lua_State *l)
  {
    u8 type = (u8)lua_tonumber(l, 1);
    if (lua_gettop(l) != 1 || type > 60)
    {
      // error msg
      return 0;
    }
    lua_pushnumber(l, BWAPI::Broodwar->self()->upgradeLevel(type));
    return 1;
  }
// ======================================== Is Researched
  int LUA::isResearched(lua_State *l)
  {
    u8 type = (u8)lua_tonumber(l, 1);
    if (lua_gettop(l) != 1 || type > 45)
    {
      // error msg
      return 0;
    }
    lua_pushnumber(l, BWAPI::Broodwar->self()->researched(type));
    return 1;
  }
// ======================================== Is Researching
  int LUA::isResearching(lua_State *l)
  {
    u8 type = (u8)lua_tonumber(l, 1);
    if (lua_gettop(l) != 1 || type > 45)
    {
      // error msg
      return 0;
    }
    lua_pushnumber(l, BWAPI::Broodwar->self()->researching(type));
    return 1;
  }
// ======================================== Kills
  int LUA::getKills(lua_State *l)
  {
    u16 type;
    switch(lua_gettop(l))
    {
    case 0:
      lua_pushnumber(l, BWAI::ai->player->getKills(BWAI::UnitTypeGroup::All));
      break;
    case 1:
      type = (u16)lua_tonumber(l, 1);
      if (type > 336)
      {
        // error msg
        return 0;
      }
      lua_pushnumber(l, BWAPI::Broodwar->self()->getKills(BWAPI::UnitType(type)));
      break;
    default:
      // error msg
      return 0;
    }
    
    return 1;
  }
// ======================================== Deaths
  int LUA::getDeaths(lua_State *l)
  {
    u16 type;
    switch(lua_gettop(l))
    {
    case 0:
      lua_pushnumber(l, BWAI::ai->player->getDeaths(BWAI::UnitTypeGroup::All));
      break;
    case 1:
      type = (u16)lua_tonumber(l, 1);
      if (type > 336)
      {
        // error msg
        return 0;
      }
      lua_pushnumber(l, BWAPI::Broodwar->self()->getDeaths(BWAPI::UnitType(type)));
      break;
    default:
      // error msg
      return 0;
    }
    
    return 1;
  }
// ======================================== Get all Units
  int LUA::getAllUnits(lua_State *l)
  {
    u16 type;
    switch(lua_gettop(l))
    {
    case 0:
      lua_pushnumber(l, BWAI::ai->player->getAllUnits(BWAI::UnitTypeGroup::All));
      break;
    case 1:
      type = (u16)lua_tonumber(l, 1);
      if (type > 336)
      {
        // error msg
        return 0;
      }
      lua_pushnumber(l, BWAPI::Broodwar->self()->getAllUnits(BWAPI::UnitType(type)));
      break;
    default:
      // error msg
      return 0;
    }
    
    return 1;
  }
// ======================================== Completed Units
  int LUA::getCompleteUnits(lua_State *l)
  {
    u16 type;
    u8 race;
    switch(lua_gettop(l))
    {
    case 0:
      lua_pushnumber(l, BWAI::ai->player->getCompletedUnits(BWAI::UnitTypeGroup::All));
      break;
    case 1:
      type = (u16)lua_tonumber(l, 1);
      if (type > 336)
      {
        // error msg
        return 0;
      }
      lua_pushnumber(l, BWAPI::Broodwar->self()->getCompletedUnits(BWAPI::UnitType(type)));
      break;
    case 2:
      type = (u16)lua_tonumber(l, 1);
      race = (u8)lua_tonumber(l, 2);
      if (type > 336)
      {
        // error msg
        return 0;
      }
      lua_pushnumber(l, BWAI::ai->player->getCompletedUnits(BWAPI::UnitType(type)));
      break;
    default:
      // error msg
      return 0;
    }
    
    return 1;
  }
// ======================================== Incomplete units
  int LUA::getIncompleteUnits(lua_State *l)
  {
    u16 type;
    switch(lua_gettop(l))
    {
    case 0:
      lua_pushnumber(l, BWAI::ai->player->getIncompleteUnits(BWAI::UnitTypeGroup::All));
      break;
    case 1:
      type = (u16)lua_tonumber(l, 1);
      if (type > 336)
      {
        // error msg
        return 0;
      }
      lua_pushnumber(l, BWAPI::Broodwar->self()->getIncompleteUnits(BWAPI::UnitType(type)));
      break;
    default:
      // error msg
      return 0;
    }
    
    return 1;
  }

// ======================================== Build
  int LUA::build(lua_State *l)
  {
/*    u8 argcount = lua_gettop(l);
    if (argcount > 3 || argcount < 2)
    {
      // error msg
      return 0;
    }

    u16 type = (u16)lua_tonumber(l, 1);
    if (type > 228)
    {
      // error msg
      return 0;
    }
    BWAI::Unit *testunit = NULL;
    BWAI::BuildingPosition spot(AI::getFreeBuildingSpot(lua_tostring(l, 2), (BWAI::Unit *&)testunit));
    BWAI::Unit builder(AI::freeBuilder(spot));
    u16 priority = 50;

    if (argcount == 3)
      priority = lua_tonumber(l, 3);
    BWAI::ai->plannedBuildings.push_back(new BWAI::TaskBuild(type, builder, spot, priority));
    BWAI::ai->buildTaskUnitsPlanned[type]++;
*/
    return 0;
  }
// ======================================== Upgrade
  int LUA::upgrade(lua_State *l)
  {
    u8 argcount = lua_gettop(l);
    if (argcount > 3 || argcount < 1)
    {
      // error msg
      return 0;
    }

    u16 type = (u16)lua_tonumber(l, 1);
    if (type > 60)
    {
      // error msg
      return 0;
    }
    u16 priority = 50;
    if (argcount == 3)
      priority = lua_tonumber(l, 3);

    u16 level = BWAPI::Broodwar->self()->upgradeLevel(type) + 1;
    if (argcount >= 2)
      level = lua_tonumber(l, 2);

    BWAI::ai->plannedUpgrades.push_back(new BWAI::TaskUpgrade(BWAPI::UpgradeType(type), level, priority));

    return 0;
  }
// ======================================== Research
  int LUA::research(lua_State *l)
  {
    u8 argcount = lua_gettop(l);
    if (argcount > 2 || argcount < 1)
    {
      // error msg
      return 0;
    }

    u16 type = (u16)lua_tonumber(l, 1);
    if (type > 60)
    {
      // error msg
      return 0;
    }
    u16 priority = 50;
    if (argcount == 2)
      priority = lua_tonumber(l, 2);

    BWAI::ai->plannedInvents.push_back(new BWAI::TaskInvent(BWAPI::TechType(type), priority));

    return 0;
  }

} // namespace BWAI

