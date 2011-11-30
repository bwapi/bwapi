#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "../../svnrev.h"
#include "GameImpl.h"

#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/ForceImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/BulletImpl.h>
#include <TemplatesImpl.h>
#include <BWAPI/Command.h>
#include <BWAPI/Map.h>
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
#include "Config.h"
#include "Detours.h"
#include "../NewHackUtil.h"
#include "BWtoBWAPI.h"

#include "../../Debug.h"

namespace BWAPI
{
  //----------------------------------------------------------------------------------------------------------
  Force* GameImpl::getForce(int forceID)
  {
    return server.getForce(forceID);
  }
  //----------------------------------------------------------------------------------------------------------
  Region *GameImpl::getRegion(int regionID)
  {
    if ( !BW::BWDATA_SAIPathing || regionID < 0 || regionID >= (int)BW::BWDATA_SAIPathing->regionCount )
      return NULL;
    return (Region*)BW::BWDATA_SAIPathing->regions[regionID].unk_28;
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
  //--------------------------------------------- GET REPLAY FRAME COUNT -------------------------------------
  int GameImpl::getReplayFrameCount()
  {
    return (int)*BW::BWDATA_ReplayFrames;
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
  bool GameImpl::getMouseState(int button)
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return false;

    SHORT ButtonDown = 0;
    switch ( button )
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
        return false;
    }
    bool pressed = (ButtonDown & 0x80) > 0;
    return pressed;
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(int key)
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return false;

    if ( key < 0 || key >= K_MAX )
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
    if ( !data->hasGUI ) return;
    rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    /* Sets the screen's position relative to the map */
    if (x < 0)
      x = 0;
    if (y < 0)
      y = 0;
    if (x > Map::getWidth()  * 32 - scrLimit.right)
      x = Map::getWidth() * 32 - scrLimit.right;
    if (y > Map::getHeight() * 32 - (scrLimit.bottom + 80))
      y = Map::getHeight() * 32 - (scrLimit.bottom + 80);

    this->setLastError(Errors::None);
    x &= 0xFFFFFFF8;
    y &= 0xFFFFFFF8;
    *BW::BWDATA_MoveToX = x;
    BW::BWDATA_MoveToTile->x = (u16)(x >> 5); //  /32
    *BW::BWDATA_MoveToY = y;
    BW::BWDATA_MoveToTile->y = (u16)(y >> 5); //  /32
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
    QUEUE_COMMAND(BW::Orders::MinimapPing, x, y);
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(BWAPI::Position p)
  {
    this->setLastError(Errors::None);
    QUEUE_COMMAND(BW::Orders::MinimapPing, p);
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
    if ( this->frameCount > 0 )
    {
      this->setLastError(Errors::Access_Denied);
      return;
    }

    if ( flag >= BWAPI::Flag::Max )
    {
      this->setLastError(Errors::Invalid_Parameter);
      return;
    }
    
    if ( !this->tournamentCheck(Tournament::EnableFlag, &flag) )
      return;

    /* Modify flag state */
    this->flags[flag] = true;
    if ( !this->hTournamentModule )
    {
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
  }
  //--------------------------------------------- GET UNITS ON TILE ------------------------------------------
  std::vector<Unit*>& GameImpl::getUnitsOnTile(int x, int y)
  {
    /* Retrieves a set of units that are on the specified tile */
    if (x < 0 || y < 0 || x >= this->mapWidth() || y >= this->mapHeight())
      return this->emptyVector;
    
    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !isVisible(x,y))
      return this->emptyVector;
    return unitsOnTileData[x][y];
  }
  //--------------------------------------------- GET UNITS IN RECTANGLE -------------------------------------
  Unit *GameImpl::_unitFromIndex(int index)
  {
    return this->unitArray[index-1];
  }
  bool __fastcall BWAPI_squareIterator_callback(Unit *uIterator)
  {
    return ((UnitImpl*)uIterator)->canAccess();
  }
  std::set<Unit*> &GameImpl::getUnitsInRectangle(int left, int top, int right, int bottom) const
  {
    // Initialize static variables
    static std::set<Unit*> unitFinderResults;
    static DWORD g_dwFinderFlags[1701] = { 0 };
    static int lastLeft   = -1;
    static int lastRight  = -1;
    static int lastTop    = -1;
    static int lastBottom = -1;
    static int lastFrame  = -1;
    // Return this reference if the input was exactly the same as the last input
    if ( lastFrame == this->frameCount && lastLeft == left && lastTop == top && lastRight == right && lastBottom == bottom )
      return unitFinderResults;
    
    // Save the input values for this frame
    lastLeft    = left;
    lastTop     = top;
    lastRight   = right;
    lastBottom  = bottom;
    lastFrame   = this->frameCount;

    // Have the unit finder do its stuff
    Templates::manageUnitFinder<BW::unitFinder>(BW::BWDATA_UnitOrderingX, 
                                                BW::BWDATA_UnitOrderingY, 
                                                g_dwFinderFlags, 
                                                left, 
                                                top, 
                                                right, 
                                                bottom,
                                                &BWAPI_squareIterator_callback,
                                                unitFinderResults);
    // Return results
    return unitFinderResults;
  }
  //--------------------------------------------- GET UNITS IN RECTANGLE -------------------------------------
  std::set<Unit*> &GameImpl::getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight) const
  {
    return getUnitsInRectangle(topLeft.x(),topLeft.y(),bottomRight.x(),bottomRight.y());
  }
  //--------------------------------------------- GET UNITS IN RADIUS ----------------------------------------
  BWAPI::Position unitsInRadius_compare;
  int             unitsInRadius_radius;
  bool __fastcall BWAPI_radiusIterator_callback(Unit *uIterator)
  {
    return ((UnitImpl*)uIterator)->canAccess() && uIterator->getDistance(unitsInRadius_compare) <= unitsInRadius_radius;
  }
  std::set<Unit*> &GameImpl::getUnitsInRadius(Position center, int radius) const
  {
    // Initialize static variables
    static std::set<Unit*> unitFinderResults;
    static DWORD g_dwFinderFlags[1701] = { 0 };
    static Position lastPosition = Positions::Invalid;
    static int lastRadius        = -1;
    static int lastFrame         = -1;
    // Return this reference if the input was exactly the same as the last input
    if ( lastFrame == this->frameCount && lastRadius == radius && lastPosition == center )
      return unitFinderResults;

    // save input values for this frame
    lastPosition  = center;
    lastRadius    = radius;
    lastFrame     = this->frameCount;

    // Set rectangular values
    int left    = center.x() - radius;
    int top     = center.y() - radius;
    int right   = center.x() + radius;
    int bottom  = center.y() + radius;

    // Store the data we are comparing found units to
    unitsInRadius_compare = center;
    unitsInRadius_radius  = radius;

    // Have the unit finder do its stuff
    Templates::manageUnitFinder<BW::unitFinder>(BW::BWDATA_UnitOrderingX, 
                                                BW::BWDATA_UnitOrderingY, 
                                                g_dwFinderFlags, 
                                                left, 
                                                top, 
                                                right, 
                                                bottom,
                                                &BWAPI_radiusIterator_callback,
                                                unitFinderResults);
    // Return results
    return unitFinderResults;
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
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool GameImpl::isVisible(int x, int y)
  {
    return map.visible(x, y);
  }
  //--------------------------------------------- IS EXPLORED ------------------------------------------------
  bool GameImpl::isExplored(int x, int y)
  {
    return map.isExplored(x, y);
  }
  //--------------------------------------------- HAS CREEP --------------------------------------------------
  bool GameImpl::hasCreep(int x, int y)
  {
    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !this->isVisible(x, y))
      return false;
    return map.hasCreep(x, y);
  }
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPowerPrecise(int x, int y, UnitType unitType) const
  {
    return Templates::hasPower<UnitImpl>(x, y, unitType, pylons);
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void GameImpl::printf(const char *format, ...)
  {
    if ( !data->hasGUI ) return;
    if ( !format ) return;
    char *buffer;
    vstretchyprintf(buffer, format);

    if ( !this->tournamentCheck(Tournament::Printf, buffer) )
    {
      free(buffer);
      return;
    }

    S_EVT evt = { 4, -1, buffer, strlen(buffer) + 1 };
    SEvtDispatch('SNET', 1, 4, &evt);
    free(buffer);
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::sendText(const char *format, ...)
  {
    if ( !format ) return;
    char *buffer;
    vstretchyprintf(buffer, format);
    sendTextEx(false, "%s", buffer);
    free(buffer);
  }
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    if ( !format ) return;
    char *buffer;
    vstretchyprintf(buffer, format);

    if ( !this->tournamentCheck(Tournament::SendText, buffer) )
    {
      free(buffer);
      return;
    }

    if ( buffer[0] == '/' )
    {
      SNetSendServerChatCommand(buffer);
      free(buffer);
      return;
    }

    if (_isReplay())
    {
      printf("%s", buffer);
      free(buffer);
      return;
    }

    if (_isInGame() && _isSinglePlayer())
    {
      BW::CheatFlags::Enum cheatID = BW::getCheatFlag(buffer);
      if ( cheatID != BW::CheatFlags::None )
      {
        this->cheatFlags ^= cheatID;
        QUEUE_COMMAND(BW::Orders::UseCheat, this->cheatFlags);
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
      free(buffer);
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
          if ( this->BWAPIPlayer->isAlly(players[i]) && BW::BWDATA_Players[i].dwStormId != -1 )
            SNetSendMessage(BW::BWDATA_Players[i].dwStormId, szMessage, msgLen + 3);
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
    free(buffer);
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void GameImpl::changeRace(BWAPI::Race race)
  {
    this->setLastError(Errors::None);
    if ( !this->tournamentCheck(Tournament::ChangeRace, &race) )
      return;
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
    return *BW::BWDATA_isGamePaused != 0;
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
    if ( !this->tournamentCheck(Tournament::StartGame) )
      return;
    QUEUE_COMMAND(BW::Orders::StartGame);
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    /* Pauses the game */
    this->setLastError(Errors::None);
    if ( !this->tournamentCheck(Tournament::PauseGame) )
      return;
    QUEUE_COMMAND(BW::Orders::PauseGame);
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    /* Resumes the game */
    this->setLastError(Errors::None);
    if ( !this->tournamentCheck(Tournament::ResumeGame) )
      return;
    QUEUE_COMMAND(BW::Orders::ResumeGame);
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    /* Leaves the current game. Moves directly to the post-game score screen */
    this->setLastError(Errors::None);
    if ( !this->tournamentCheck(Tournament::LeaveGame) )
      return;
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
    if ( !this->tournamentCheck(Tournament::RestartGame) )
      return;
    QUEUE_COMMAND(BW::Orders::RestartGame);
  }
  //--------------------------------------------- SET ALLIANCE -----------------------------------------------
  bool GameImpl::setAlliance(BWAPI::Player *player, bool allied, bool alliedVictory)
  {
    /* Set the current player's alliance status */
    if ( !BWAPIPlayer || isReplay() || !player || player == BWAPIPlayer )
      return this->setLastError(Errors::Invalid_Parameter);

    u32 alliance = 0;
    for ( int i = 0; i < PLAYER_COUNT; ++i )
      alliance |= (BW::BWDATA_Alliance[BWAPIPlayer->getIndex()].player[i] & 3) << (i*2);
    
    u8 newAlliance = allied ? (alliedVictory ? 2 : 1) : 0;
    if ( allied )
      alliance |= newAlliance << ( ((PlayerImpl*)player)->getIndex()*2);
    else
      alliance &= ~(3 << ( ((PlayerImpl*)player)->getIndex()*2) );
    QUEUE_COMMAND(BW::Orders::SetAllies, alliance);
    return this->setLastError(Errors::None);
  }
  //----------------------------------------------- SET VISION -----------------------------------------------
  bool GameImpl::setVision(BWAPI::Player *player, bool enabled)
  {
    /* Set the current player's vision status */
    if ( !BWAPIPlayer || isReplay() || !player || player == BWAPIPlayer )
      return this->setLastError(Errors::Invalid_Parameter);

    u16 vision = (u16)BW::BWDATA_PlayerVision[BWAPIPlayer->getIndex()];    
    if ( enabled )
      vision |= 1 << ((PlayerImpl*)player)->getIndex();
    else
      vision &= ~(1 <<  ((PlayerImpl*)player)->getIndex() );
    QUEUE_COMMAND(BW::Orders::SetVision, vision);
    return this->setLastError(Errors::None);
  }
  //--------------------------------------------------- GAME SPEED -------------------------------------------
  void  GameImpl::setLocalSpeed(int speed)
  {
    /* Sets the frame rate of the client */
    if ( !this->tournamentCheck(Tournament::SetLocalSpeed, &speed) )
      return;
    if (speed < 0)
    {
      /* Reset the speed if it is negative */
      for ( int i = 0; i < 7; ++i )
      {
        BW::BWDATA_GameSpeedModifiers[i]    = BW::OriginalSpeedModifiers[i];
        BW::BWDATA_GameSpeedModifiers[i+7]  = BW::OriginalSpeedModifiers[i] * 3;
      }
    }
    else
    {
      /* Set all speeds if it is positive */
      for ( int i = 0; i < 7; ++i )
      {
        BW::BWDATA_GameSpeedModifiers[i]    = speed;
        BW::BWDATA_GameSpeedModifiers[i+7]  = speed * 3;
      }
    }
  }
  void GameImpl::setFrameSkip(int frameSkip)
  {
    setLastError(Errors::None);
    if ( !this->tournamentCheck(Tournament::SetFrameSkip, &frameSkip) )
      return;

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
    foreach(Unit* u, units)
    {
      if (u && u->exists() && u->canIssueCommand(command))
      {
        if (command.type == UnitCommandTypes::Train ||
            command.type == UnitCommandTypes::Morph)
          if (u->getType().producesLarva() && command.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva )
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
    for(std::list< std::set<UnitImpl*> >::iterator i = groupsOf12.begin(); i != groupsOf12.end(); ++i)
    {
      int k = 0;
      for(std::set<UnitImpl*>::iterator j = i->begin(); j != i->end(); ++j )
        selected[k++] = *j;

      command.unit  = selected[0];
      selected[k]   = NULL;
      if ( command.type != BWAPI::UnitCommandTypes::Unload )
      {
        BW::Orders::Select sel = BW::Orders::Select((u8)(*i).size(), selected);
        botAPM_select++;
        QueueGameCommand(&sel, sel.size);
      }

      BroodwarImpl.executeCommand( command, false );
      foreach(UnitImpl* j, *i)
      {
        j->lastCommandFrame = BroodwarImpl.frameCount;
        j->lastCommand      = command;
        command.unit        = (Unit*)j;
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
  //---------------------------------------------------- OBSERVERS -------------------------------------------
  std::set<Player*>& GameImpl::observers()
  {
    /* Returns a set of all the enemy players that have not left or been defeated. */
    this->setLastError(Errors::None);
    return _observers;
  }
  //--------------------------------------------------- LATENCY ----------------------------------------------
  int GameImpl::getLatencyFrames()
  {
    DWORD dwCallDelay = 1;
    if ( *BW::BWDATA_NetMode )
    {
      CAPS caps;
      caps.dwSize = sizeof(CAPS);
      SNetGetProviderCaps(&caps);

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
    return data->hasLatCom;
  }
  void GameImpl::setLatCom(bool isEnabled)
  {
    if ( !this->tournamentCheck(Tournament::SetLatCom, &isEnabled) )
      return;
    data->hasLatCom = isEnabled;
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

    if ( !this->tournamentCheck(Tournament::SetMap, (void*)mapFileName) )
      return setLastError(Errors::None);

    strcpy(BW::BWDATA_CurrentMapFileName, mapFileName);
    return setLastError(Errors::None);
  }
  //--------------------------------------------------- HAS PATH ---------------------------------------------
  bool GameImpl::hasPath(Position source, Position destination) const
  {
    Broodwar->setLastError(Errors::None);
    if ( !source.isValid() || !destination.isValid() )
      return Broodwar->setLastError(Errors::Unreachable_Location);

    if ( BW::BWDATA_SAIPathing )
    {
      BW::region *srcRgn = BW::Position((u16)source.x(), (u16)source.y()).getRegion();
      BW::region *dstRgn = BW::Position((u16)destination.x(), (u16)destination.y()).getRegion();

      // Return true if the locations are valid and connected
      if ( srcRgn && dstRgn && srcRgn->groupIndex == dstRgn->groupIndex )
        return true;
    }
    return Broodwar->setLastError(Errors::Unreachable_Location);
  } // hasPath end
  //------------------------------------------------- ELAPSED TIME -------------------------------------------
  int GameImpl::elapsedTime() const
  {
    return (int)*BW::BWDATA_ElapsedTime;
  }
  //-------------------------------------- SET COMMAND OPTIMIZATION LEVEL ------------------------------------
  void GameImpl::setCommandOptimizationLevel(int level)
  {
    if ( level < 0 )
      level = 0;
    if ( level > 4 )
      level = 4;
    if ( !this->tournamentCheck(Tournament::SetCommandOptimizationLevel, &level) )
      return;
    this->commandOptimizerLevel = level;
  }
  //----------------------------------------------- COUNTDOWN TIMER ------------------------------------------
  int GameImpl::countdownTimer() const
  {
    return (int)*BW::BWDATA_CountdownTimer;
  }
  //------------------------------------------------- GET REGION AT ------------------------------------------
  BWAPI::Region *GameImpl::getRegionAt(int x, int y) const
  {
    if ( x < 0 || y < 0 || x >= Broodwar->mapWidth()*32 || y >= Broodwar->mapHeight()*32 )
    {
      Broodwar->setLastError(BWAPI::Errors::Invalid_Parameter);
      return NULL;
    }
    BW::region *rgn = BW::Position((u16)x, (u16)y).getRegion();
    if ( !rgn )
    {
      Broodwar->setLastError(BWAPI::Errors::Invalid_Parameter);
      return NULL;
    }
    return (Region*)rgn->unk_28;
  }
  int GameImpl::getLastEventTime() const
  {
    return this->lastEventTime;
  }
  bool GameImpl::setReplayVision(Player *player, bool enabled)
  {
    // Sets the replay vision status
    if ( !isReplay() || !player )
      return this->setLastError(Errors::Invalid_Parameter);

    u32 vision = *BW::BWDATA_ReplayVision;
    if ( enabled )
      vision |= 1 << ((PlayerImpl*)player)->getIndex();
    else
      vision &= ~(1 <<  ((PlayerImpl*)player)->getIndex() );
    *BW::BWDATA_ReplayVision = vision;
    return this->setLastError(Errors::None);
  }
  bool GameImpl::setRevealAll(bool reveal)
  {
    if ( !isReplay() )
      return this->setLastError(Errors::Invalid_Parameter);
    *BW::BWDATA_ReplayRevealAll = reveal ? 1 : 0;
    return this->setLastError(Errors::None);
  }
};

