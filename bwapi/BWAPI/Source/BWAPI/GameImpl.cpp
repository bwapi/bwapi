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
#include <BWAPI/TemplatesImpl.h>
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
#include <BW/GameType.h>
#include <BW/CheatType.h>
#include <BW/Dialog.h>

#include "BWAPI/AIModule.h"
#include "DLLMain.h"
#include "Detours.h"
#include "../NewHackUtil.h"
#include "BWtoBWAPI.h"

#include "../../Debug.h"

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
  //---------------------------------------------- GET NUKE DOTS ---------------------------------------------
  std::set< Position >& GameImpl::getNukeDots()
  {
    return nukeDots;
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
    if ( i < UNIT_ARRAY_MAX_LENGTH && this->unitArray[i]->canAccess() )
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
    if ( noGUI ) return;
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
      this->printf("\x06" "ERROR: Flags can only be enabled at the start of a game.");
      return;
    }

    if (flag >= BWAPI::Flag::Max)
    {
      this->printf("\x06" "ERROR: Invalid flag (%d).", flag);
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
  //--------------------------------------------- GET UNITS ON TILE ------------------------------------------
  std::set<Unit*>& GameImpl::getUnitsOnTile(int x, int y)
  {
    /* Retrieves a set of units that are on the specified tile */
    if (x < 0 || y < 0 || x >= this->mapWidth() || y >= this->mapHeight())
      return this->emptySet;

    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !isVisible(x,y))
      return this->emptySet;
    return unitsOnTileData[x][y];
  }

  //--------------------------------------------- GET UNITS IN RECTANGLE -------------------------------------
  std::set<Unit*> GameImpl::getUnitsInRectangle(int left, int top, int right, int bottom)
  {
    // localize the variables
    std::set<Unit*> unitFinderResults;
    BW::unitFinder *xFinder = BW::BWDATA_UnitOrderingX;
    BW::unitFinder *yFinder = BW::BWDATA_UnitOrderingY;

    // Grab the minimum x value without performing any additional computation
    unsigned int xMin = 0;
    while ( xFinder[xMin].searchValue < left && xFinder[xMin].unitIndex && xMin < MAX_SEARCH )
      ++xMin;

    if ( !xFinder[xMin].unitIndex ) // no units were found on the horizontal plane
      return unitFinderResults; // no results

    // Grab the minimum y value without performing any additional computation
    unsigned int yMin = 0;
    while ( yFinder[yMin].searchValue < top && yFinder[yMin].unitIndex && yMin < MAX_SEARCH )
      ++yMin;

    if ( !yFinder[yMin].unitIndex ) // no units were found on the vertical plane
      return unitFinderResults; // no results

    // Retrieve the unit indexes for the horizontal plane
    std::vector<int> xList;
    for ( unsigned int x = xMin; xFinder[x].searchValue <= right && xFinder[x].unitIndex && x < MAX_SEARCH; ++x )
      xList.push_back(xFinder[x].unitIndex);

    if ( xList.empty() )
      return unitFinderResults; // no results

    // Retrieve the unit indexes for the vertical plane
    std::vector<int> yList;
    for ( unsigned int y = yMin; yFinder[y].searchValue <= bottom && yFinder[y].unitIndex && y < MAX_SEARCH; ++y )
      yList.push_back(yFinder[y].unitIndex);

    if ( yList.empty() )
      return unitFinderResults; // no results

    // Save the intersection of the values found in both the horizontal and vertical planes
    for each ( int xUnit in xList )
    {
      for each ( int yUnit in yList )
      {
        // Note: we use <= because we subtract 1
        if ( xUnit == yUnit && xUnit > 0 && xUnit <= UNIT_ARRAY_MAX_LENGTH ) // intersection
        {
          UnitImpl *u = unitArray[xUnit-1];
          if ( u && u->exists() )
            unitFinderResults.insert(u);
        }
      }
    }
    return unitFinderResults;
  }
  //--------------------------------------------- GET UNITS IN RECTANGLE -------------------------------------
  std::set<Unit*> GameImpl::getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight)
  {
    return getUnitsInRectangle(topLeft.x(),topLeft.y(),bottomRight.x(),bottomRight.y());
  }
  //--------------------------------------------- GET UNITS IN RADIUS ----------------------------------------
  std::set<Unit*> GameImpl::getUnitsInRadius(Position center, int radius)
  {
    center.makeValid();
    std::set<Unit*> tempResults = getUnitsInRectangle(center.x() - radius, center.y() - radius, center.x() + radius, center.y() + radius);
    std::set<Unit*> unitFinderResults;
    for each ( Unit *u in tempResults )
    {
      if ( center.getApproxDistance(u->getPosition()) <= radius )
        unitFinderResults.insert(u);
    }
    return unitFinderResults;
  }
  //--------------------------------------------- GET LAST ERROR ---------------------------------------------
  Error GameImpl::getLastError() const
  {
    /* returns the last error encountered in BWAPI */
    return lastError;
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  bool GameImpl::setLastError(BWAPI::Error e)
  {
    /* implies that an error has occured */
    lastError = e;
    return e == Errors::None;
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
  bool GameImpl::isBuildable(int x, int y, bool includeBuildings)
  {
    return map.buildable(x, y) && (includeBuildings ? !map.isOccupied(x, y) : true);
  }
  //--------------------------------------------- IS BUILDABLE -----------------------------------------------
  bool GameImpl::isBuildable(TilePosition position, bool includeBuildings)
  {
    int x = position.x(), y = position.y();
    return map.buildable(x, y) && (includeBuildings ? !map.isOccupied(x, y) : true);
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
  bool GameImpl::canBuildHere(const Unit* builder, TilePosition position, UnitType type, bool checkExplored)
  {
    return Templates::canBuildHere<class GameImpl, class PlayerImpl, class UnitImpl>(builder,position,type,checkExplored);
  }
  //--------------------------------------------- CAN MAKE ---------------------------------------------------
  bool GameImpl::canMake(const Unit* builder, UnitType type)
  {
    return Templates::canMake<class GameImpl, class PlayerImpl, class UnitImpl>(builder,type);
  }
  //--------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool GameImpl::canResearch(const Unit* unit, TechType type)
  {
    return Templates::canResearch<class GameImpl, class PlayerImpl, class UnitImpl>(unit,type);
  }
  //--------------------------------------------- CAN UPGRADE ------------------------------------------------
  bool GameImpl::canUpgrade(const Unit* unit, UpgradeType type)
  {
    return Templates::canUpgrade<class GameImpl, class PlayerImpl, class UnitImpl>(unit,type);
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  std::set< TilePosition >& GameImpl::getStartLocations()
  {
    return startLocations;
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void GameImpl::printf(const char *format, ...)
  {
    if ( noGUI ) return;
    if ( !format ) return;
    char *buffer;
    vstretchyprintf(buffer, format);

    s_evt evt;
    evt.dwFlags    = 4;
    evt.dwPlayerId = -1;
    evt.pData      = (BYTE*)buffer;
    evt.dwSize     = strlen(buffer) + 1;

    SEvtDispatch('SNET', 1, 4, &evt);
    delete buffer;
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::sendText(const char *format, ...)
  {
    if ( !format ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    sendTextEx(false, "%s", buffer);
    delete buffer;
  }
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    if ( !format ) return;
    char *buffer;
    vstretchyprintf(buffer, format);

    if ( buffer[0] == '/' )
    {
      SNetSendServerChatCommand(buffer);
      delete buffer;
      return;
    }

    if (_isReplay())
    {
      printf("%s", buffer);
      delete buffer;
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
        printf("%c%s: %c%s", this->BWAPIPlayer->getTextColor(), this->BWAPIPlayer->getName().c_str(), 0x07, buffer);
      }
      delete buffer;
      return;
    }

    char szMessage[82];
    szMessage[0] = 0;
    szMessage[1] = 1;
    int msgLen = SStrCopy(&szMessage[2], buffer, 80);

    if (_isInGame())
    {
      if ( toAllies )
      {
        for ( int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
        {
          if ( this->BWAPIPlayer->isAlly(players[i]) && BW::BWDATA_Players[i].dwStormId != -1 )
            SNetSendMessage(BW::BWDATA_Players[i].dwStormId, szMessage, msgLen + 3);
        }
      }
      else
      {
        SNetSendMessage(-1, szMessage, msgLen + 3);
      } // toAllies
    }
    else
    {
      szMessage[1] = 0x4C;
      SNetSendMessage(-1, &szMessage[1], msgLen + 2);
    } // isInGame
    delete buffer;
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
    return *BW::BWDATA_NetMode != 0 && *BW::BWDATA_NetMode != -1;
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::isBattleNet()
  {
    this->setLastError(Errors::None);
    return *BW::BWDATA_NetMode == 'BNET';
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
    *BW::BWDATA_GameState      = 0;
    *BW::BWDATA_gwNextGameMode = 6;
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    /* Restarts the current match 
       Does not work on Battle.net */
    if ( this->isMultiplayer() )
    {
      this->setLastError(Errors::Invalid_Parameter);
      return;
    }

    this->setLastError(Errors::None);
    *BW::BWDATA_GameState      = 0;
    *BW::BWDATA_gwGameMode     = 1;
    *BW::BWDATA_gwNextGameMode = 1;
    *BW::BWDATA_OpheliaEnabled = 1;
    if ( strchr(BW::BWDATA_CurrentMapFileName, '.') )
    {
      *BW::BWDATA_CampaignIndex  = 0;
    }
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
    }
    else
    {
      /* Set all speeds if it is positive */
      for (int i = 0; i < 7; i++)
      {
        BW::BWDATA_GameSpeedModifiers[i]    = speed;
        BW::BWDATA_GameSpeedModifiers[i+7]  = speed * 3;
      }
    }
  }
  void GameImpl::setFrameSkip(int frameSkip)
  {
    setLastError(Errors::None);
    if ( frameSkip > 0 )
    {
      *BW::BWDATA_FrameSkip = frameSkip;
      return;
    }
    setLastError(Errors::Invalid_Parameter);
  }
  //------------------------------------------ ISSUE COMMAND -------------------------------------------------
  bool GameImpl::issueCommand(const std::set<BWAPI::Unit*>& units, UnitCommand command)
  {
    std::list< std::set<UnitImpl*> > groupsOf12;
    std::set<UnitImpl* > nextGroup;
    for each(Unit* u in units)
    {
      if (u && u->exists() && u->canIssueCommand(command))
      {
        if (command.type == UnitCommandTypes::Train ||
            command.type == UnitCommandTypes::Morph)
          if (u->getType().producesLarva() && UnitType(command.extra).whatBuilds().first == UnitTypes::Zerg_Larva )
            u = *u->getLarva().begin();
        nextGroup.insert((UnitImpl*)u);
        if (nextGroup.size()>=12)
        {
          groupsOf12.push_back(nextGroup);
          nextGroup.clear();
        }
      }
    }
    if (nextGroup.empty()==false)
    {
      groupsOf12.push_back(nextGroup);
      nextGroup.clear();
    }
    if (groupsOf12.empty())
      return false;
    UnitImpl* selected[13];
    for(std::list< std::set<UnitImpl*> >::iterator i=groupsOf12.begin();i!=groupsOf12.end();i++)
    {
      int k=0;
      for each(UnitImpl* j in *i)
      {
        selected[k]=j;
        k++;
      }
      command.unit = selected[0];
      selected[k]=NULL;
      BW::Orders::Select sel = BW::Orders::Select((u8)(*i).size(), selected);
      botAPM_select++;
      QueueGameCommand((PBYTE)&sel, sel.size);

      BroodwarImpl.executeCommand( command, false );
      for each(UnitImpl* j in *i)
      {
        j->lastCommandFrame = BroodwarImpl.frameCount;
        command.unit = (Unit*)j;
        j->lastCommand = command;
        BroodwarImpl.addToCommandBuffer(new Command(command));
      }
    }
    return true;
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
    /* Retrieves the object for the current player */
    this->setLastError(Errors::None);
    return (Player*)this->BWAPIPlayer;
  }
  //----------------------------------------------------- ENEMY ----------------------------------------------
  Player *GameImpl::enemy()
  {
    /* Retrieves the object for the first opponent player */
    this->setLastError(Errors::None);
    return (Player*)this->enemyPlayer;
  }
  //----------------------------------------------------- NEUTRAL --------------------------------------------
  Player *GameImpl::neutral()
  {
    /* Retrieves the object for the neutral player */
    this->setLastError(Errors::None);
    return (Player*)players[11];
  }
  //----------------------------------------------------- ALLIES ---------------------------------------------
  std::set<Player*>& GameImpl::allies()
  {
    /* Returns a set of all the ally players that have not left or been defeated. Does not include self. */
    this->setLastError(Errors::None);
    return _allies;
  }
  //----------------------------------------------------- ENEMIES --------------------------------------------
  std::set<Player*>& GameImpl::enemies()
  {
    /* Returns a set of all the enemy players that have not left or been defeated. */
    this->setLastError(Errors::None);
    return _enemies;
  }
  //--------------------------------------------------- LATENCY ----------------------------------------------
  int GameImpl::getLatencyFrames()
  {
    CAPS caps;
    caps.dwSize = sizeof(CAPS);
    SNetGetProviderCaps(&caps);

    DWORD dwCallDelay = 1;
    if ( *BW::BWDATA_NetMode )
    {
      dwCallDelay = caps.dwCallDelay;
      if ( dwCallDelay > 8 )
        dwCallDelay = 8;
      else if ( dwCallDelay < 2 )
        dwCallDelay = 2;
    }
    return (BW::BWDATA_LatencyFrames[*BW::BWDATA_GameSpeed]) * (*BW::BWDATA_Latency + dwCallDelay + 1);
  }
  int GameImpl::getLatencyTime()
  {
    return getLatencyFrames() * BW::BWDATA_GameSpeedModifiers[*BW::BWDATA_GameSpeed];
  }
  int GameImpl::getRemainingLatencyFrames()
  {
    return getLatencyFrames() - (this->getFrameCount() - lastTurnFrame);
  }
  int GameImpl::getRemainingLatencyTime()
  {
    return (getLatencyFrames() * BW::BWDATA_GameSpeedModifiers[*BW::BWDATA_GameSpeed]) - (GetTickCount() - lastTurnTime);
  }
  //--------------------------------------------------- VERSION ----------------------------------------------
  int GameImpl::getRevision()
  {
    return SVN_REV;
  }
  bool GameImpl::isDebug()
  {
    return BUILD_DEBUG == 1;
  }
  //----------------------------------------------- LAT COMPENSATION -----------------------------------------
  bool GameImpl::isLatComEnabled()
  {
    return hasLatCom;
  }
  void GameImpl::setLatCom(bool isEnabled)
  {
    hasLatCom = isEnabled;
  }
  //--------------------------------------------------- REPLAY -----------------------------------------------
  int GameImpl::getReplayFrameCount()
  {
    return (int)*BW::BWDATA_ReplayFrames;
  }
  //----------------------------------------------- GET INSTANCE ID ------------------------------------------
  int GameImpl::getInstanceNumber()
  {
    return (int)gdwProcNum;
  }
  //---------------------------------------------------- GET APM ---------------------------------------------
  int GameImpl::getAPM(bool includeSelects)
  {
    if ( includeSelects )
      return botAPM_selects;
    return botAPM_noselects;
  }
  //---------------------------------------------------- SET MAP ---------------------------------------------
  bool GameImpl::setMap(const char *mapFileName)
  {
    if ( !mapFileName || strlen(mapFileName) >= MAX_PATH || !mapFileName[0] )
      return setLastError(Errors::Invalid_Parameter);

    if ( GetFileAttributes(mapFileName) == INVALID_FILE_ATTRIBUTES )
      return setLastError(Errors::File_Not_Found);

    strcpy(BW::BWDATA_CurrentMapFileName, mapFileName);
    return setLastError(Errors::None);
  }
};