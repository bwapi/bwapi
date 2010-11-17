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
    char buffer[256];
    va_list ap;
    va_start(ap, format);
    vsnprintf_s(buffer, 256, 256, format, ap);
    va_end(ap);

    s_evt evt;
    evt.dwFlags    = 4;
    evt.dwPlayerId = -1;
    evt.pData      = (BYTE*)buffer;
    evt.dwSize     = strlen(buffer) + 1;

    SEvtDispatch('SNET', 1, 4, &evt);
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::sendText(const char *format, ...)
  {
    char buffer[80];
    va_list ap;
    va_start(ap, format);
    vsnprintf_s(buffer, 80, 80, format, ap);
    va_end(ap);
    sendTextEx(false, "%s", buffer);
  }
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    char buffer[80];
    va_list ap;
    va_start(ap, format);
    vsnprintf_s(buffer, 80, 80, format, ap);
    va_end(ap);

    if (_isReplay())
    {
      printf("%s", buffer);
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
    *BW::BWDATA_GameState      = 0;
    *BW::BWDATA_gwNextGameMode = 6;
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    /* Restarts the current match 
       Does not work on Battle.net */
    this->setLastError(Errors::None);
    *BW::BWDATA_GameState      = 0;
    *BW::BWDATA_gwNextGameMode = 5;
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
  //------------------------------------------ ISSUE COMMAND -------------------------------------------------
  bool GameImpl::issueCommand(const std::set<BWAPI::Unit*>& units, UnitCommand command)
  {
    std::list< std::set<UnitImpl*> > groupsOf12;
    std::set<UnitImpl* > nextGroup;
    for each(Unit* u in units)
    {
      if (u->canIssueCommand(command))
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
    if (nextGroup.size()>=0)
    {
      groupsOf12.push_back(nextGroup);
      nextGroup.clear();
    }
    UnitImpl* selected[13];
    for(std::list< std::set<UnitImpl*> >::iterator i=groupsOf12.begin();i!=groupsOf12.end();i++)
    {
      int k=0;
      for each(UnitImpl* j in *i)
      {
        selected[k]=j;
        k++;
      }
      command.unit = selected[9];
      selected[k]=NULL;
      BW::Orders::Select sel = BW::Orders::Select((u8)(*i).size(), selected);
      QueueGameCommand((PBYTE)&sel, sel.size);

      BroodwarImpl.executeCommand( command, false );
      for each(UnitImpl* j in *i)
      {
        j->lastOrderFrame = BroodwarImpl.frameCount;
        command.unit = (Unit*)j;
        BroodwarImpl.addToCommandBuffer(new Command(command));
      }
    }
    return false;
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
    if ( noGUI ) return;
    char buffer[MAX_BUFFER];
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);
    this->shapes.push_back(new ShapeText(ctype,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextMap(int x, int y, const char* text, ...)
  {
    if ( noGUI ) return;
    char buffer[MAX_BUFFER];
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Map,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextMouse(int x, int y, const char* text, ...)
  {
    if ( noGUI ) return;
    char buffer[MAX_BUFFER];
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, MAX_BUFFER, MAX_BUFFER, text, ap);
    va_end(ap);
    this->shapes.push_back(new ShapeText(BWAPI::CoordinateType::Mouse,x,y,std::string(buffer),(char)textSize));
  }
  void  GameImpl::drawTextScreen(int x, int y, const char* text, ...)
  {
    if ( noGUI ) return;
    char buffer[MAX_BUFFER];
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
    if (!inScreen(ctype,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the map */
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the mouse */
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the screen */
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom) || noGUI) return;
    this->shapes.push_back(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color.getID(), isSolid));
  }
  //------------------------------------------------ DRAW TRIANGLE -------------------------------------------
  void  GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Map, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy) || noGUI) return;
    this->shapes.push_back(new ShapeTriangle(BWAPI::CoordinateType::Screen, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  //------------------------------------------------- DRAW CIRCLE --------------------------------------------
  void  GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(ctype, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Map, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Mouse, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius) || noGUI) return;
    this->shapes.push_back(new ShapeCircle(BWAPI::CoordinateType::Screen, x, y, radius, color.getID(), isSolid));
  }
  //------------------------------------------------- DRAW ELIPSE --------------------------------------------
  void  GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(ctype, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Map, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Mouse, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad) || noGUI) return;
    this->shapes.push_back(new ShapeEllipse(BWAPI::CoordinateType::Screen, x, y, xrad, yrad, color.getID(), isSolid));
  }
  //--------------------------------------------------- DRAW DOT ---------------------------------------------
  void  GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(ctype, x, y, color.getID()));
  }
  void  GameImpl::drawDotMap(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Map, x, y, color.getID()));
  }
  void  GameImpl::drawDotMouse(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Mouse, x, y, color.getID()));
  }
  void  GameImpl::drawDotScreen(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x,y) || noGUI) return;
    this->shapes.push_back(new ShapeDot(BWAPI::CoordinateType::Screen, x, y, color.getID()));
  }
  //-------------------------------------------------- DRAW LINE ---------------------------------------------
  void  GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2) || noGUI) return;
    this->shapes.push_back(new ShapeLine(ctype, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2) || noGUI) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Map, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2) || noGUI) return;
    this->shapes.push_back(new ShapeLine(BWAPI::CoordinateType::Mouse, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2) || noGUI) return;
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
    /*
    DWORD caps[9];
    caps[0] = 36;
    SNetGetProviderCaps(caps);

    int turns;
    SNetGetTurnsInTransit(&turns);
    */
    return BW::BWDATA_LatencyFrames[*BW::BWDATA_GameSpeed];// * (caps[8] + turns);
  }
  int GameImpl::getLatencyTime()
  {
    return (BW::BWDATA_LatencyFrames[*BW::BWDATA_GameSpeed] * BW::BWDATA_GameSpeedModifiers[*BW::BWDATA_GameSpeed]) >> 1;
  }
  int GameImpl::getRemainingLatencyFrames()
  {
    /*DWORD caps[9];
    caps[0] = 36;
    SNetGetProviderCaps(caps);

    int turns;
    SNetGetTurnsInTransit(&turns);
    */
    DWORD latFrames = BW::BWDATA_LatencyFrames[*BW::BWDATA_GameSpeed];
    //int rval = latFrames * (caps[8] + turns - 1);
    return /*rval + */latFrames - (this->getFrameCount() - lastTurnFrame);
  }
  int GameImpl::getRemainingLatencyTime()
  {
    /*DWORD caps[9];
    caps[0] = 36;
    SNetGetProviderCaps(caps);

    int turns;
    SNetGetTurnsInTransit(&turns);
    */
    DWORD speed     = *BW::BWDATA_GameSpeed;
    DWORD latFrames = BW::BWDATA_LatencyFrames[speed];
    DWORD speedMod  = BW::BWDATA_GameSpeedModifiers[speed];

    int rval = latFrames /* * (caps[8] + turns - 1)*/ * speedMod;
    return rval + (speedMod * latFrames) - (GetTickCount() - lastTurnTime);
  }
  //--------------------------------------------------- VERSION ----------------------------------------------
  int GameImpl::getRevision()
  {
    return SVN_REV;
  }
  bool GameImpl::isDebug()
  {
#ifdef _DEBUG
    return true;
#else
    return false;
#endif
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
  //--------------------------------------------------- SET GUI ----------------------------------------------
  void GameImpl::setGUI(bool enabled)
  {
    noGUI = !enabled;
    if ( enabled )
    {
      BW::BWDATA_ScreenLayers[0].buffers = 1;
      BW::BWDATA_ScreenLayers[1].buffers = 1;
      BW::BWDATA_ScreenLayers[2].buffers = 1;
      BW::BWDATA_ScreenLayers[5].buffers = 1;
    }
    else
    {
      BW::BWDATA_ScreenLayers[0].buffers = 0;
      BW::BWDATA_ScreenLayers[1].buffers = 0;
      BW::BWDATA_ScreenLayers[2].buffers = 0;
      BW::BWDATA_ScreenLayers[5].buffers = 0;
    }
  }
  int GameImpl::getInstanceNumber()
  {
    return (int)gdwProcNum;
  }
};