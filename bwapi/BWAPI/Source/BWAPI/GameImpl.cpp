#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "../../svnrev.h"
#include "GameImpl.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

#include <Util/FileLogger.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/ForceImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/BulletImpl.h>
#include <BWAPI/Command.h>
#include <BWAPI/Map.h>
#include <BWAPI/ScreenLogger.h>
#include <BWAPI/Flag.h>
#include <BWAPI.h>

#include <BW/Unit.h>
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include <BW/UnitTarget.h>
#include <BW/OrderTypes.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/GameType.h>
#include <BW/WeaponType.h>
#include <BW/CheatType.h>
#include <BW/Dialog.h>

#include "BWAPI/AIModule.h"
#include "DLLMain.h"
#include "../NewHackUtil.h"

#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"
#include "BWtoBWAPI.h"

namespace BWAPI
{
  //----------------------------------------------- GET FORCES -----------------------------------------------
  std::set< Force* >& GameImpl::getForces()
  {
    return forces;
  }
  //----------------------------------------------- GET PLAYERS ----------------------------------------------
  std::set< Player* >& GameImpl::getPlayers()
  {
    return playerSet;
  }
  //------------------------------------------------- GET UNITS ----------------------------------------------
  std::set< Unit* >& GameImpl::getAllUnits()
  {
    return accessibleUnits;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  std::set< Unit* >& GameImpl::getMinerals()
  {
    return minerals;
  }
  //---------------------------------------------- GET GEYSERS -----------------------------------------------
  std::set< Unit* >& GameImpl::getGeysers()
  {
    return geysers;
  }
  //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
  std::set< Unit* >& GameImpl::getNeutralUnits()
  {
    return neutralUnits;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  std::set< Unit* >& GameImpl::getStaticMinerals()
  {
    return staticMinerals;
  }
  //---------------------------------------------- GET GEYSERS -----------------------------------------------
  std::set< Unit* >& GameImpl::getStaticGeysers()
  {
    return staticGeysers;
  }
  //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
  std::set< Unit* >& GameImpl::getStaticNeutralUnits()
  {
    return staticNeutralUnits;
  }
  //---------------------------------------------- GET BULLETS -----------------------------------------------
  std::set< Bullet* >& GameImpl::getBullets()
  {
    return bullets;
  }
  //---------------------------------------------- GET EVENTS ------------------------------------------------
  std::list< Event >& GameImpl::getEvents()
  {
    return events;
  }
  //----------------------------------------------------------------------------------------------------------
  Force* GameImpl::getForce(int forceID)
  {
    return server.getForce(forceID);
  }
  //----------------------------------------------------------------------------------------------------------
  Player* GameImpl::getPlayer(int playerID)
  {
    return server.getPlayer(playerID);
  }
  //----------------------------------------------------------------------------------------------------------
  Unit* GameImpl::getUnit(int unitID)
  {
    return server.getUnit(unitID);
  }
  //----------------------------------------------------------------------------------------------------------
  Unit* GameImpl::indexToUnit(int unitIndex)
  {
    if ( !this->isFlagEnabled(Flag::CompleteMapInformation) )
      return NULL;
    int i = (unitIndex & 0x7FF);
    if ( i < 1700 && this->unitArray[i]->canAccess() )
      return this->unitArray[i];
    return NULL;
  }
  //--------------------------------------------- GET GAME TYPE ----------------------------------------------
  GameType GameImpl::getGameType()
  {
    if (isReplay())
      return GameTypes::None;
    return GameType(*BW::BWDATA_gameType);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  int GameImpl::getLatency()
  {
    /* Returns the real latency values */
    if (_isSinglePlayer())
      return BWAPI::Latency::SinglePlayer;

    if (_isBattleNet())
    {
      switch(*BW::BWDATA_Latency)
      {
        case 0:
          return BWAPI::Latency::BattlenetLow;
        case 1:
          return BWAPI::Latency::BattlenetMedium;
        case 2:
          return BWAPI::Latency::BattlenetHigh;
        default:
          return BWAPI::Latency::BattlenetLow;
      }
    }
    else
    {
      switch(*BW::BWDATA_Latency)
      {
        case 0:
          return BWAPI::Latency::LanLow;
        case 1:
          return BWAPI::Latency::LanMedium;
        case 2:
          return BWAPI::Latency::LanHigh;
        default:
          return BWAPI::Latency::LanLow;
      }
    }
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int GameImpl::getFrameCount()
  {
    return this->frameCount;
  }
  //------------------------------------------------ GET FPS -------------------------------------------------
  int GameImpl::getFPS()
  {
    return fps;
  }
  //-------------------------------------------- GET Average FPS ---------------------------------------------
  double GameImpl::getAverageFPS()
  {
    return averageFPS;
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int GameImpl::getMouseX()
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return 0;
    return BW::BWDATA_Mouse->x;
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int GameImpl::getMouseY()
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return 0;
    return BW::BWDATA_Mouse->y;
  }
  //------------------------------------------- GET MOUSE POSITION -------------------------------------------
  BWAPI::Position GameImpl::getMousePosition()
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return BWAPI::Positions::Unknown;
    return BWAPI::Position(BW::BWDATA_Mouse->x, BW::BWDATA_Mouse->y);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(MouseButton button)
  {
    return getMouseState((int)button);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(int button)
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return false;

    if (button < 0 || button >= 3) 
      return false;

    SHORT ButtonDown = 0;
    switch (button)
    {
      case BWAPI::M_LEFT:
        ButtonDown = GetKeyState(VK_LBUTTON);
        break;
      case BWAPI::M_MIDDLE:
        ButtonDown = GetKeyState(VK_MBUTTON);
        break;
      case BWAPI::M_RIGHT:
        ButtonDown = GetKeyState(VK_RBUTTON);
        break;
      default:
        break;
    }
    bool pressed = (ButtonDown & 128) > 0;
    return pressed;
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(Key key)
  {
    return getKeyState((int)key);
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(int key)
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return false;

    if (key < 0 || key >= 255)
      return false;

    return (GetKeyState(key) & 128) > 0;
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int GameImpl::getScreenX()
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return 0;
    return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int GameImpl::getScreenY()
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return 0;
    return *(BW::BWDATA_ScreenY);
  }
  //------------------------------------------- GET SCREEN POSITION ------------------------------------------
  BWAPI::Position GameImpl::getScreenPosition()
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return BWAPI::Positions::Unknown;
    return BWAPI::Position(*(BW::BWDATA_ScreenX),*(BW::BWDATA_ScreenY));
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    BW::rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    /* Sets the screen's position relative to the map */
    if (x < 0)
      x = 0;
    if (y < 0)
      y = 0;
    if (x > Map::getWidth()  * 32 - scrLimit.Xmax)
      x = Map::getWidth() * 32 - scrLimit.Xmax;
    if (y > Map::getHeight() * 32 - (scrLimit.Ymax + 80))
      y = Map::getHeight() * 32 - (scrLimit.Ymax + 80);

    this->setLastError(Errors::None);
    x &= 0xFFFFFFF8;
    y &= 0xFFFFFFF8;
    *BW::BWDATA_MoveToX = x;
    BW::BWDATA_MoveToTile->x = (u16)(x >> 5);
    *BW::BWDATA_MoveToY = y;
    BW::BWDATA_MoveToTile->y = (u16)(y >> 5);
    BW::BWFXN_UpdateScreenPosition();
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(BWAPI::Position p)
  {
    setScreenPosition(p.x(),p.y());
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(int x, int y)
  {
    this->setLastError(Errors::None);
    QueueGameCommand((PBYTE)&BW::Orders::MinimapPing(BW::Position((u16)x,(u16)y)), sizeof(BW::Orders::MinimapPing));
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(BWAPI::Position p)
  {
    this->setLastError(Errors::None);
    QueueGameCommand((PBYTE)&BW::Orders::MinimapPing(BW::Position((u16)p.x(),(u16)p.y())), sizeof(BW::Orders::MinimapPing));
  }
  //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
  bool  GameImpl::isFlagEnabled(int flag)
  {
    return this->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void  GameImpl::enableFlag(int flag)
  {
    /* Enable the specified flag */

    /* Error checking */
    this->setLastError(Errors::None);
    if (this->frameCount > 0)
    {
      this->sendText("Flags can only be enabled at the start of a game.");
      return;
    }

    if (flag >= BWAPI::FLAG_COUNT)
    {
      this->sendText("Invalid flag (%d).", flag);
      return;
    }

    /* Modify flag state */
    this->flags[flag] = true;
    switch(flag)
    {
    case BWAPI::Flag::CompleteMapInformation:
      this->sendText("Enabled Flag CompleteMapInformation");
      break;
    case BWAPI::Flag::UserInput:
      this->sendText("Enabled Flag UserInput");
      break;
    }
  }
  //--------------------------------------------- UNITS ON TILE ----------------------------------------------
  std::set<Unit*>& GameImpl::unitsOnTile(int x, int y)
  {
    /* Retrieves a set of units that are on the specified tile */
    if (x < 0 || y < 0 || x >= this->mapWidth() || y >= this->mapHeight())
      return this->emptySet;

    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !isVisible(x,y))
      return this->emptySet;
    return unitsOnTileData[x][y];
  }
  //--------------------------------------------- GET LAST ERROR ---------------------------------------------
  Error GameImpl::getLastError() const
  {
    /* returns the last error encountered in BWAPI */
    return this->lastError;
  }
  //----------------------------------------------- MAP WIDTH ------------------------------------------------
  int GameImpl::mapWidth()
  {
    return Map::getWidth();
  }
  //----------------------------------------------- MAP HEIGHT -----------------------------------------------
  int GameImpl::mapHeight()
  {
    return Map::getHeight();
  }
  //---------------------------------------------- MAP FILE NAME ---------------------------------------------
  std::string GameImpl::mapFileName()
  {
    return Map::getFileName();
  }
  //---------------------------------------------- MAP PATH NAME ---------------------------------------------
  std::string GameImpl::mapPathName()
  {
    return Map::getPathName();
  }
  //------------------------------------------------ MAP NAME ------------------------------------------------
  std::string GameImpl::mapName()
  {
    return Map::getName();
  }
  //----------------------------------------------- GET MAP HASH ---------------------------------------------
  std::string GameImpl::mapHash()
  {
    return savedMapHash;
  }
  //--------------------------------------------- IS WALKABLE ------------------------------------------------
  bool GameImpl::isWalkable(int x, int y)
  {
    return map.walkable(x, y);
  }
  //--------------------------------------------- GET GROUND HEIGHT ------------------------------------------
  int GameImpl::getGroundHeight(int x, int y)
  {
    return map.groundHeight(x, y);
  }
  //--------------------------------------------- GET GROUND HEIGHT ------------------------------------------
  int GameImpl::getGroundHeight(TilePosition position)
  {
    return map.groundHeight(position.x(), position.y());
  }
  //--------------------------------------------- IS BUILDABLE -----------------------------------------------
  bool GameImpl::isBuildable(int x, int y)
  {
    return map.buildable(x, y);
  }
  //--------------------------------------------- IS BUILDABLE -----------------------------------------------
  bool GameImpl::isBuildable(TilePosition position)
  {
    return map.buildable(position.x(), position.y());
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool GameImpl::isVisible(int x, int y)
  {
    return map.visible(x, y);
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool GameImpl::isVisible(TilePosition position)
  {
    return map.visible(position.x(), position.y());
  }
  //--------------------------------------------- IS EXPLORED ------------------------------------------------
  bool GameImpl::isExplored(int x, int y)
  {
    return map.isExplored(x, y);
  }
  //--------------------------------------------- IS EXPLORED ------------------------------------------------
  bool GameImpl::isExplored(TilePosition position)
  {
    return map.isExplored(position.x(), position.y());
  }
  //--------------------------------------------- HAS CREEP --------------------------------------------------
  bool GameImpl::hasCreep(int x, int y)
  {
    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !this->isVisible(x, y))
      return false;
    return map.hasCreep(x, y);
  }
  //--------------------------------------------- HAS CREEP --------------------------------------------------
  bool GameImpl::hasCreep(TilePosition position)
  {
    return hasCreep(position.x(), position.y());
  }
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPower(int x, int y, int tileWidth, int tileHeight)
  {
    if (!(tileWidth == 2 && tileHeight == 2) &&
        !(tileWidth == 3 && tileHeight == 2) &&
        !(tileWidth == 4 && tileHeight == 3))
      return false;

    if (tileWidth == 4)
      x++;
    /* Loop through all pylons for the current player */
    foreach (UnitImpl* i, pylons)
    {
      if ( !i->isCompleted() )
        continue;
      int px = i->getTilePosition().x();
      int py = i->getTilePosition().y();
      int bx = x - px + 7;
      int by = y - py + 5;
      /* Deal with special cases, pylon offset cutoff */
      if (bx >= 0 && by >= 0 && bx <= 14 && by <= 9)
      {
        switch(by)
        {
        case 0:
          if ( tileHeight == 3 && bx >= 4 && bx <= 9 )
            return true;
          break;
        case 1:
          if (bx >= 1 && bx <= 12) 
            return true;
          break;
        case 2:
          if (bx <= 13) 
            return true;
          break;
        case 3:
        case 4:
        case 5:
        case 6:
          return true;
        case 7:
          if (bx <= 13) 
            return true;
          break;
        case 8:
          if (bx >= 1 && bx <= 12) 
            return true;
          break;
        case 9:
          if (bx >= 4 && bx <= 9) 
            return true;
          break;
        }
      }
    }
    return false;
  }
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPower(TilePosition position, int tileWidth, int tileHeight)
  {
    return hasPower(position.x(),position.y(),tileWidth,tileHeight);
  }
  //--------------------------------------------- CAN BUILD HERE ---------------------------------------------
  bool GameImpl::canBuildHere(Unit* builder, TilePosition position, UnitType type)
  {
    this->setLastError(Errors::Unbuildable_Location);
    int width  = type.tileWidth();
    int height = type.tileHeight();

    int left   = position.x();
    int top    = position.y();
    int right  = left + width;
    int bottom = top + height;

    /* Map limit Check */
    if (left < 0) // left
      return false;
    if (top < 0) // top
      return false;
    if (right > this->mapWidth()) // right
      return false;
    if (bottom >= this->mapHeight()) // bottom
      return false;

    //if the unit is a refinery, we just need to check the set of geysers to see if the position
    //matches one of them (and the type is still vespene geyser)
    if (type.isRefinery())
    {
      foreach (Unit* g, geysers)
      {
        if (g->getTilePosition() == position)
        {
          if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
            return false;
          this->setLastError(Errors::None);
          return true;
        }
      }
      return false;
    }

    // Retrieve the region of the builder
    BW::region *startRgn = NULL;
    if ( builder && BW::BWDATA_SAIPathing )
    {
      TilePosition builderPos = builder->getTilePosition();
      startRgn = getRegion( BW::BWDATA_SAIPathing->mapTileRegionId[ builderPos.y() ][ builderPos.x() ] );
    }

    /* Tile buildability check */
    for(int ix = left; ix < right; ++ix)
    {
      for(int iy = top; iy < bottom; ++iy)
      {
        // Check if tile is buildable and explored
        if ( !this->isBuildable(ix, iy) || !this->map.isExplored(ix, iy) )
          return false; // @TODO: Error code for !isExplored ??

        // Check if builder is capable of reaching the building site
        if ( builder && BW::BWDATA_SAIPathing && !startRgn->isConnectedTo(BW::BWDATA_SAIPathing->mapTileRegionId[iy][ix]) )
          return false;
      }
    }

    /* Ground unit dimension check */
    int targetX = left * 32 + type.tileWidth() * 32 / 2;
    int targetY = top * 32 + type.tileHeight() * 32 / 2;
    for(int ix = left; ix < right; ++ix)
    {
      for(int iy = top; iy < bottom; ++iy)
      {
        foreach(Unit *u, unitsOnTile(ix,iy))
        {
          if ( u != builder &&
               u->getPosition().x() + u->getType().dimensionRight() >= targetX - type.dimensionLeft()  &&
               u->getPosition().y() + u->getType().dimensionDown()  >= targetY - type.dimensionUp()    &&
               u->getPosition().x() - u->getType().dimensionLeft()  <= targetX + type.dimensionRight() &&
               u->getPosition().y() - u->getType().dimensionUp()    <= targetY + type.dimensionDown() )
            return false;
        }
      }
    }

    /* Creep Check */
    if (type.getRace() == BWAPI::Races::Zerg)
    {
      //Most Zerg buildings can only be built on creep
      if (!type.isResourceDepot())
        for(int ix = left; ix < right; ++ix)
          for(int iy = top; iy < bottom; ++iy)
            if (!BWAPI::Broodwar->hasCreep(ix, iy))
              return false;
    }
    else
    {
      //Non-zerg buildings cannot be built on creep
      for(int ix = left; ix < right; ++ix)
        for(int iy = top; iy < bottom; ++iy)
          if (BWAPI::Broodwar->hasCreep(ix, iy))
            return false;
    }

    /* Power Check */
    if ( type.requiresPsi() && !this->hasPower(left, top, width, height) )
      return false;

    /* Resource Check (CC, Nex, Hatch) */
    if (type.isResourceDepot())
    {
      foreach (BWAPI::Unit* m, getStaticMinerals())
      {
        if (isVisible(m->getInitialTilePosition()) ||
            isVisible(m->getInitialTilePosition().x() + 1, m->getInitialTilePosition().y()))
          if (!m->isVisible())
            continue; // tile position is visible, but mineral is not => mineral does not exist
        if (m->getInitialTilePosition().x() > left - 5 &&
            m->getInitialTilePosition().y() > top  - 4 &&
            m->getInitialTilePosition().x() < left + 7 &&
            m->getInitialTilePosition().y() < top  + 6)
          return false;
      }
      foreach (BWAPI::Unit* g, getStaticGeysers())
      {
        if (g->getInitialTilePosition().x() > left - 7 &&
            g->getInitialTilePosition().y() > top  - 5 &&
            g->getInitialTilePosition().x() < left + 7 &&
            g->getInitialTilePosition().y() < top  + 6)
          return false;
      }
    }
    //if the build site passes all these tests, return true.
    this->setLastError(Errors::None);
    return true;
  }
  //--------------------------------------------- CAN MAKE ---------------------------------------------------
  bool  GameImpl::canMake(Unit* builder, UnitType type)
  {
    /* Error checking */
    this->setLastError(Errors::None);
    if ( !self() )
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if ( builder )
    {
      /* Check if the owner of the unit is you */
      if (builder->getPlayer() != self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }

      /* Check if this unit can actually build the unit type */
      if (builder->getType() != type.whatBuilds().first)
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }

      /* Carrier space */
      if ( builder->getType() == UnitTypes::Protoss_Carrier )
      {
        int max_amt = 4;
        if (self()->getUpgradeLevel(UpgradeTypes::Carrier_Capacity) > 0)
          max_amt += 4;
        if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt)
        {
          this->setLastError(Errors::Insufficient_Space);
          return false;
        }
      }

      /* Reaver Space */
      if ( builder->getType() == UnitTypes::Protoss_Reaver )
      {
        int max_amt = 5;
        if (self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0)
          max_amt += 5;
        if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= max_amt)
        {
          this->setLastError(Errors::Insufficient_Space);
          return false;
        }
      }
    }

    /* Check if player has enough minerals */
    if (self()->minerals() < type.mineralPrice())
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }

    /* Check if player has enough gas */
    if (self()->gas() < type.gasPrice())
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    
    /* Check if player has enough supplies */
    if (type.supplyRequired() > 0 && self()->supplyTotal() < self()->supplyUsed() + type.supplyRequired() - type.whatBuilds().first.supplyRequired())
    {
      this->setLastError(Errors::Insufficient_Supply);
      return false;
    }

    UnitType addon = UnitTypes::None;
    std::map<UnitType, int>::const_iterator requiredEnd = type.requiredUnits().end();
    for(std::map<UnitType, int>::const_iterator i = type.requiredUnits().begin(); i != requiredEnd; ++i)
    {
      if (i->first.isAddon())
        addon = i->first;

      bool pass = false;
      if (self()->completedUnitCount(i->first) >= i->second)
        pass = true;
      if ( i->first == UnitTypes::Zerg_Hatchery &&
           self()->completedUnitCount(UnitTypes::Zerg_Hatchery) +
           self()->completedUnitCount(UnitTypes::Zerg_Lair)     +
           self()->completedUnitCount(UnitTypes::Zerg_Hive)     >= i->second )
        pass = true;
      if ( i->first == UnitTypes::Zerg_Lair && 
           self()->completedUnitCount(UnitTypes::Zerg_Lair) + 
           self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
        pass = true;
      if ( i->first == UnitTypes::Zerg_Spire && 
           self()->completedUnitCount(UnitTypes::Zerg_Spire) +
           self()->completedUnitCount(UnitTypes::Zerg_Greater_Spire) >= i->second )
        pass = true;
      if ( !pass )
      {
        this->setLastError(Errors::Insufficient_Tech);
        return false;
      }
    }

    if (type.requiredTech() != TechTypes::None && !self()->hasResearched(type.requiredTech()))
    {
      this->setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if ( builder && 
         addon != UnitTypes::None &&
         addon.whatBuilds().first == type.whatBuilds().first &&
         (!builder->getAddon() || builder->getAddon()->getType() != addon) )
    {
      this->setLastError(Errors::Insufficient_Tech);
      return false;
    }
    return true;
  }
  //--------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool  GameImpl::canResearch(Unit* unit, TechType type)
  {
    /* Error checking */
    this->setLastError(Errors::None);
    if ( !self() )
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if ( unit )
    {
      if (unit->getPlayer() != self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }
      if (unit->getType() != type.whatResearches())
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->isResearching(type))
    {
      this->setLastError(Errors::Currently_Researching);
      return false;
    }
    if (self()->hasResearched(type))
    {
      this->setLastError(Errors::Already_Researched);
      return false;
    }
    if (self()->minerals() < type.mineralPrice())
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }
    if (self()->gas() < type.gasPrice())
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    return true;
  }
  //----------------------------------------------- CAN UPGRADE ----------------------------------------------
  bool  GameImpl::canUpgrade(Unit* unit, UpgradeType type)
  {
    this->setLastError(Errors::None);
    if ( !self() )
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if ( unit )
    {
      if (unit->getPlayer() != self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }
      if (unit->getType() != type.whatUpgrades())
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->isUpgrading(type))
    {
      this->setLastError(Errors::Currently_Upgrading);
      return false;
    }
    if (self()->getUpgradeLevel(type)>=type.maxRepeats())
    {
      this->setLastError(Errors::Fully_Upgraded);
      return false;
    }
    if (self()->minerals() < type.mineralPriceBase()+type.mineralPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }
    if (self()->gas() < type.gasPriceBase()+type.gasPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    return true;
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  std::set< TilePosition >& GameImpl::getStartLocations()
  {
    return startLocations;
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void GameImpl::printf(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);

    if (_isReplay() || _isInGame())
    {
      printEx(8, "%s", buffer);
      return;
    }

    if (!_isInGame())
      printEx(8, "%s", buffer);
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::sendText(const char *format, ...)
  {
    va_list ap;
    va_start(ap, format);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, format, ap);
    va_end(ap);
    sendTextEx(false, "%s", buffer);
  }
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    va_list ap;
    va_start(ap, format);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, format, ap);
    va_end(ap);
    char* txtout = buffer;

    if (_isReplay())
    {
      printEx(8, "%s", buffer);
      return;
    }

    if (_isInGame() && _isSinglePlayer())
    {
      BW::CheatFlags::Enum cheatID = BW::getCheatFlag(buffer);
      if (cheatID != BW::CheatFlags::None)
      {
        this->cheatFlags ^= cheatID;
        QueueGameCommand((PBYTE)&BW::Orders::UseCheat(this->cheatFlags), sizeof(BW::Orders::UseCheat));
        if (cheatID == BW::CheatFlags::ShowMeTheMoney ||
            cheatID == BW::CheatFlags::BreatheDeep ||
            cheatID == BW::CheatFlags::WhatsMineIsMine ||
            cheatID == BW::CheatFlags::SomethingForNothing)
          this->cheatFlags ^= cheatID;
      }
      else
      {
        printEx(this->BWAPIPlayer->getIndex(), "%s", buffer);
      }
      return;
    }

    if (_isInGame())
    {
      *BW::BWDATA_SendTextFilter = 0xFFFF;
      if ( toAllies )
      {
        *BW::BWDATA_SendTextFilter = 0;
        for ( u8 p = 0; p < PLAYER_COUNT; ++p )
        {
          if ( BW::BWDATA_Alliance[BWAPIPlayer->getID()].player[p] != 0 )
            *BW::BWDATA_SendTextFilter |= 1 << p;
        }
      }
      __asm
      {
        pushad
        mov esi, txtout
        call [BW::BWFXN_SendPublicCallTarget]
        popad
      }
    }
    else
      __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
      }
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void GameImpl::changeRace(BWAPI::Race race)
  {
    this->setLastError(Errors::None);
    this->_changeRace(this->BWAPIPlayer->getIndex(),race);
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::isInGame()
  {
    this->setLastError(Errors::None);
    return inGame;
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::isMultiplayer()
  {
    this->setLastError(Errors::None);
    return *BW::BWDATA_GameModule != 0 && *BW::BWDATA_GameModule != -1;
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::isBattleNet()
  {
    this->setLastError(Errors::None);
    return *BW::BWDATA_GameModule == 'TENB';
  }
  //----------------------------------------------- IS PAUSED ------------------------------------------------
  bool GameImpl::isPaused()
  {
    this->setLastError(Errors::None);
    return *BW::BWDATA_IsRunning == 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::isReplay()
  {
    this->setLastError(Errors::None);
    return *BW::BWDATA_InReplay != 0;
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void GameImpl::startGame()
  {
    /* Starts the game as a lobby host */
    this->setLastError(Errors::None);
    QueueGameCommand((PBYTE)&BW::Orders::StartGame(), sizeof(BW::Orders::StartGame));
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    /* Pauses the game */
    this->setLastError(Errors::None);
    QueueGameCommand((PBYTE)&BW::Orders::PauseGame(), sizeof(BW::Orders::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    /* Resumes the game */
    this->setLastError(Errors::None);
    QueueGameCommand((PBYTE)&BW::Orders::ResumeGame(), sizeof(BW::Orders::ResumeGame));
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    /* Leaves the current game. Moves directly to the post-game score screen */
    this->setLastError(Errors::None);
    *BW::BWDATA_GameState    = 0;
    *BW::BWDATA_GamePosition = 6;
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    /* Restarts the current match 
       Does not work on Battle.net */
    this->setLastError(Errors::None);
    *BW::BWDATA_GameState    = 0;
    *BW::BWDATA_GamePosition = 5;
  }
  //--------------------------------------------------- GAME SPEED -------------------------------------------
  void  GameImpl::setLocalSpeed(int speed)
  {
    /* Sets the frame rate of the client */
    if (speed < 0)
    {
      /* Reset the speed if it is negative */
      for ( int i = 0; i < 7; i++ )
      {
        BW::BWDATA_GameSpeedModifiers[i]    = BW::OriginalSpeedModifiers[i];
        BW::BWDATA_GameSpeedModifiers[i+7]  = BW::OriginalSpeedModifiers[i] * 3;
      }
      /*BW::BWDATA_ScreenLayers[0].buffers = 1;
      BW::BWDATA_ScreenLayers[1].buffers = 1;
      BW::BWDATA_ScreenLayers[2].buffers = 1;
      BW::BWDATA_ScreenLayers[5].buffers = 1;

      HackUtil::PatchImport("storm.dll", 313, &SBltROP3);
      HackUtil::PatchImport("storm.dll", 431, &STransBlt);
      HackUtil::PatchImport("storm.dll", 432, &STransBltUsingMask);
      */
    }
    else
    {
      /* Set all speeds if it is positive */
      for (int i = 0; i < 7; i++)
      {
        BW::BWDATA_GameSpeedModifiers[i]    = speed;
        BW::BWDATA_GameSpeedModifiers[i+7]  = speed * 3;
      }
      if ( speed == 0 )
      {
        /*BW::BWDATA_ScreenLayers[0].buffers = 0;
        BW::BWDATA_ScreenLayers[1].buffers = 0;
        BW::BWDATA_ScreenLayers[2].buffers = 0;
        BW::BWDATA_ScreenLayers[5].buffers = 0;

        HackUtil::PatchImport("storm.dll", 313, &BW::FakeROP3);
        HackUtil::PatchImport("storm.dll", 431, &BW::FakeBlt);
        HackUtil::PatchImport("storm.dll", 432, &BW::FakeBltUsingMask);
        */
      }
    }
  }
  //------------------------------------------ GET SELECTED UNITS --------------------------------------------
  std::set<BWAPI::Unit*>& GameImpl::getSelectedUnits()
  {
    this->setLastError(Errors::None);
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
    {
      this->setLastError(Errors::Access_Denied);
      return emptySet;
    }
    return selectedUnitSet;
  }
  //----------------------------------------------------- SELF -----------------------------------------------
  Player*  GameImpl::self()
  {
    /* Retrieves the class for the current player */
    this->setLastError(Errors::None);
    return (Player*)this->BWAPIPlayer;
  }
  //----------------------------------------------------- ENEMY ----------------------------------------------
  Player *GameImpl::enemy()
  {
    /* Retrieves the class for the first opponent player */
    this->setLastError(Errors::None);
    for (int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i)
    {
      if ( (this->players[i]->getType() == BW::PlayerType::Computer ||
            this->players[i]->getType() == BW::PlayerType::EitherPreferComputer ||
            this->players[i]->getType() == BW::PlayerType::Player) &&
           this->players[i]->isParticipating() &&
           this->BWAPIPlayer->isEnemy(this->players[i]) )
      return this->players[i];
    }
    return NULL;
  }
  //-------------------------------------------------- DRAW TEXT ---------------------------------------------
  void  GameImpl::setTextSize(int size)
  {
    if ( size < 1 || size > 3 )
      size = 1;
    textSize = size;
  }
  void  GameImpl::drawText(int ctype, int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);
    this->shapes.push_back(new ShapeText(ctype,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextMap(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Map,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextMouse(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Mouse,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextScreen(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Screen,x,y,std::string(buffer),(char)textSize));
  }
  //--------------------------------------------------- DRAW BOX ---------------------------------------------
  void  GameImpl::drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box */
    if (!inScreen(ctype,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the map */
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the mouse */
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the screen */
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom)) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color.getID(), isSolid));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void  GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Map, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy)) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Screen, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void  GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(ctype, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Map, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Mouse, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius)) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Screen, x, y, radius, color.getID(), isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void  GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(ctype, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Map, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Mouse, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Screen, x, y, xrad, yrad, color.getID(), isSolid));
  }
  //--------------------------------------------------- DRAW DOT ---------------------------------------------
  void  GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y)) return;
    this->shapes.push_back(new ShapeDot(ctype, x, y, color.getID()));
  }
  void  GameImpl::drawDotMap(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x,y)) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Map, x, y, color.getID()));
  }
  void  GameImpl::drawDotMouse(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x,y)) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Mouse, x, y, color.getID()));
  }
  void  GameImpl::drawDotScreen(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x,y)) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Screen, x, y, color.getID()));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void  GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2)) return;
    this->shapes.push_back(new ShapeLine(ctype, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2)) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Map, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2)) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Mouse, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2)) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Screen, x1, y1, x2, y2, color.getID()));
  }
  //------------------------------------------------ SCREEN BUFFER -------------------------------------------
  void *GameImpl::getScreenBuffer()
  {
    return BW::BWDATA_GameScreenBuffer->data;
  }
  //--------------------------------------------------- LATENCY ----------------------------------------------
  int GameImpl::getLatencyFrames()
  {
    DWORD caps[9];
    caps[0] = 36;
    SNetGetProviderCaps(caps);

    int turns;
    SNetGetTurnsInTransit(&turns);

    return BW::BWDATA_LatencyFrames[*BW::BWDATA_GameSpeed] * (caps[8] + turns);
  }
  int GameImpl::getLatencyTime()
  {
    return getLatencyFrames() * BW::BWDATA_GameSpeedModifiers[*BW::BWDATA_GameSpeed];
  }
  int GameImpl::getRemainingLatencyFrames()
  {
    DWORD caps[9];
    caps[0] = 36;
    SNetGetProviderCaps(caps);

    int turns;
    SNetGetTurnsInTransit(&turns);

    DWORD latFrames = BW::BWDATA_LatencyFrames[*BW::BWDATA_GameSpeed];
    int rval = latFrames * (caps[8] + turns - 1);
    return rval + latFrames - (this->getFrameCount() - lastTurnFrame);
  }
  int GameImpl::getRemainingLatencyTime()
  {
    DWORD caps[9];
    caps[0] = 36;
    SNetGetProviderCaps(caps);

    int turns;
    SNetGetTurnsInTransit(&turns);

    DWORD speed     = *BW::BWDATA_GameSpeed;
    DWORD latFrames = BW::BWDATA_LatencyFrames[speed];
    DWORD speedMod  = BW::BWDATA_GameSpeedModifiers[speed];

    int rval = latFrames * (caps[8] + turns - 1) * speedMod;
    return rval + (speedMod * latFrames) - (GetTickCount() - lastTurnTime);
  }
  //--------------------------------------------------- VERSION ----------------------------------------------
  int GameImpl::getRevision()
  {
    return SVN_REV;
  }
  bool GameImpl::isDebug()
  {
#ifdef BWAPI_DEBUG
    return true;
#else
    return false;
#endif
  }
};