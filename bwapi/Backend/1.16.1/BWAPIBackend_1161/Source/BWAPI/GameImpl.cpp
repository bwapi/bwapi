#include "../svnrev.h"
#include "GameImpl.h"

#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <set>

#include "../Convenience.h"

#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "RegionImpl.h"
#include "UnitImpl.h"
#include "BulletImpl.h"
#include "Templates.h"
#include "Command.h"
#include "Map.h"
#include <BWAPI/Flag.h>

#include <BWAPI4/Unitset.h>

#include "../BW/CUnit.h"
#include "../BW/CBullet.h"
#include "../BW/Offsets.h"
#include "../BW/UnitTarget.h"
#include "../BW/OrderTypes.h"
#include <BWAPI4/Latency.h>
#include "../BW/TileType.h"
#include "../BW/TileSet.h"
#include "../BW/CheatType.h"
#include "../BW/Dialog.h"

#include "../DLLMain.h"
#include "../Config.h"
#include "../Detours.h"
#include "../NewHackUtil.h"
#include "BWtoBWAPI.h"

#include "../BW/Pathing.h"

#include <BWAPI/Input.h>

namespace BWAPI4
{
  //----------------------------------------------------------------------------------------------------------
  Force GameImpl::getForce(int forceID) const
  {
    return server.getForce(forceID);
  }
  //----------------------------------------------------------------------------------------------------------
  RegionImpl* GameImpl::getRegion(int regionID) const
  {
    auto it = regionMap.find(regionID);
    return it != regionMap.end() ? it->second : nullptr;
  }
  //----------------------------------------------------------------------------------------------------------
  Player GameImpl::getPlayer(int playerID) const
  {
    return server.getPlayer(playerID);
  }
  //----------------------------------------------------------------------------------------------------------
  Unit GameImpl::getUnit(int unitID) const
  {
    return server.getUnit(unitID);
  }
  //----------------------------------------------------------------------------------------------------------
  Unit GameImpl::indexToUnit(int unitIndex) const
  {
    if ( !this->isFlagEnabled(Flag::CompleteMapInformation) )
      return nullptr;
    int i = (unitIndex & 0x7FF);
    if (i < BW::UNIT_ARRAY_MAX_LENGTH && this->unitArray[i]->canAccess())
      return this->unitArray[i];
    return nullptr;
  }
  //--------------------------------------------- GET GAME TYPE ----------------------------------------------
  GameType GameImpl::getGameType() const
  {
    if (isReplay())
      return GameTypes::None;
    return GameType(BW::BWDATA::gameType);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  int GameImpl::getLatency() const
  {
    // Returns the real latency values
    if ( !this->isMultiplayer() )
      return BWAPI4::Latency::SinglePlayer;

    if ( this->isBattleNet() )
    {
      switch(BW::BWDATA::Latency)
      {
        case 0:
          return BWAPI4::Latency::BattlenetLow;
        case 1:
          return BWAPI4::Latency::BattlenetMedium;
        case 2:
          return BWAPI4::Latency::BattlenetHigh;
        default:
          return BWAPI4::Latency::BattlenetLow;
      }
    }
    else
    {
      switch(BW::BWDATA::Latency)
      {
        case 0:
          return BWAPI4::Latency::LanLow;
        case 1:
          return BWAPI4::Latency::LanMedium;
        case 2:
          return BWAPI4::Latency::LanHigh;
        default:
          return BWAPI4::Latency::LanLow;
      }
    }
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int GameImpl::getFrameCount() const
  {
    return this->frameCount;
  }
  //--------------------------------------------- GET REPLAY FRAME COUNT -------------------------------------
  int GameImpl::getReplayFrameCount() const
  {
    return BW::BWDATA::ReplayHead.frameCount;
  }
  //------------------------------------------------ GET FPS -------------------------------------------------
  int GameImpl::getFPS() const
  {
    return fpsCounter.getFps();
  }
  //-------------------------------------------- GET Average FPS ---------------------------------------------
  double GameImpl::getAverageFPS() const
  {
    return fpsCounter.getAverageFps();
  }
  //------------------------------------------- GET MOUSE POSITION -------------------------------------------
  BWAPI::Position GameImpl::getMousePosition() const
  {
    if ( !this->isFlagEnabled(BWAPI4::Flag::UserInput) )
      return BWAPI4::Positions::Unknown;
    return BWAPI::Position(BW::BWDATA::Mouse.x, BW::BWDATA::Mouse.y);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(MouseButton button) const
  {
    if ( !this->isFlagEnabled(BWAPI4::Flag::UserInput) )
      return false;

    int vkValue;
    switch ( button )
    {
      case BWAPI4::M_LEFT:
        vkValue = K_LBUTTON;
        break;
      case BWAPI4::M_RIGHT:
        vkValue = K_RBUTTON;
        break;
      case BWAPI4::M_MIDDLE:
        vkValue = K_MBUTTON;
        break;
      default:
        return false;
    }
    return (GetKeyState(vkValue) & 0x80) > 0;
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(Key key) const
  {
    if ( !this->isFlagEnabled(BWAPI4::Flag::UserInput) )
      return false;

    if ( key < 0 || key >= K_MAX )
      return false;

    return (GetKeyState(key) & 128) > 0;
  }
  //------------------------------------------- GET SCREEN POSITION ------------------------------------------
  BWAPI::Position GameImpl::getScreenPosition() const
  {
    if ( !this->isFlagEnabled(BWAPI4::Flag::UserInput) )
      return BWAPI4::Positions::Unknown;
    return BWAPI::Position(BW::BWDATA::ScreenX, BW::BWDATA::ScreenY);
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    this->setLastError();
    if ( !data->hasGUI ) return;

    Position scrSize(BW::BWDATA::GameScreenBuffer.width(), BW::BWDATA::GameScreenBuffer.height()-108);
    Position mapSize( TilePosition(Map::getWidth(), Map::getHeight()) );

    // Sets the screen's position relative to the map
    Position movePos(x,y);
    movePos.setMin(0, 0);
    movePos.setMax( mapSize - scrSize);

    movePos &= 0xFFFFFFF8;
    BW::BWDATA::MoveToX = movePos.x;
    BW::BWDATA::MoveToY = movePos.y;
    BW::BWDATA::Game.screenTilePosition = BW::TilePosition(movePos);
    BW::BWFXN_UpdateScreenPosition();
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(int x, int y)
  {
    this->setLastError();
    QUEUE_COMMAND(BW::Orders::MinimapPing, x, y);
  }
  //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
  bool  GameImpl::isFlagEnabled(int flag) const
  {
    // Check if index is valid
    if ( flag < 0 || flag >= BWAPI4::Flag::Max ) 
      return false;

    // Make completeMapInfo appear true if the match has ended
    if ( flag == Flag::CompleteMapInformation && this->calledMatchEnd )
      return true;

    // Return the state of the flag
    return this->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void  GameImpl::enableFlag(int flag)
  {
    // Enable the specified flag
    this->setLastError();

    // Don't allow flag changes after frame 0
    if ( this->frameCount > 0 )
    {
      this->setLastError(Errors::Access_Denied);
      return;
    }

    // check if index is valid
    if ( flag < 0 || flag >= BWAPI4::Flag::Max )
    {
      this->setLastError(Errors::Invalid_Parameter);
      return;
    }
    
    // Modify flag state 
    this->flags[flag] = true;
    switch(flag)
    {
    case BWAPI4::Flag::CompleteMapInformation:
      this->sendText("Enabled Flag CompleteMapInformation");
      break;
    case BWAPI4::Flag::UserInput:
      this->sendText("Enabled Flag UserInput");
      break;
    }
  }
  //--------------------------------------------- GET UNITS IN RECTANGLE -------------------------------------
  Unit GameImpl::_unitFromIndex(int index)
  {
    return this->unitArray[index-1];
  }
  Unitset GameImpl::getUnitsInRectangle(int left, int top, int right, int bottom, std::function<bool(Unit)> pred) const
  {
    Unitset unitFinderResults;

    // Have the unit finder do its stuff
    Templates::iterateUnitFinder<BW::unitFinder>(BW::BWDATA::UnitOrderingX.data(),
                                                 BW::BWDATA::UnitOrderingY.data(),
                                                 BW::BWDATA::UnitOrderingCount,
                                                 left,
                                                 top,
                                                 right,
                                                 bottom,
                                                 [&](Unit u){ if ( !pred || pred(u) )
                                                                 unitFinderResults.insert(u); });
    // Return results
    return unitFinderResults;
  }
  //----------------------------------------------- MAP WIDTH ------------------------------------------------
  int GameImpl::mapWidth() const
  {
    return Map::getWidth();
  }
  //----------------------------------------------- MAP HEIGHT -----------------------------------------------
  int GameImpl::mapHeight() const
  {
    return Map::getHeight();
  }
  //---------------------------------------------- MAP FILE NAME ---------------------------------------------
  std::string GameImpl::mapFileName() const
  {
    return Map::getFileName();
  }
  //---------------------------------------------- MAP PATH NAME ---------------------------------------------
  std::string GameImpl::mapPathName() const
  {
    return Map::getPathName();
  }
  //------------------------------------------------ MAP NAME ------------------------------------------------
  std::string GameImpl::mapName() const
  {
    return Map::getName();
  }
  //----------------------------------------------- GET MAP HASH ---------------------------------------------
  std::string GameImpl::mapHash() const
  {
    return Map::getMapHash();
  }
  //--------------------------------------------- IS WALKABLE ------------------------------------------------
  bool GameImpl::isWalkable(int x, int y) const
  {
    return Map::walkable(x, y);
  }
  //--------------------------------------------- GET GROUND HEIGHT ------------------------------------------
  int GameImpl::getGroundHeight(int x, int y) const
  {
    return Map::groundHeight(x, y);
  }
  //--------------------------------------------- IS BUILDABLE -----------------------------------------------
  bool GameImpl::isBuildable(int x, int y, bool includeBuildings) const
  {
    if ( Map::buildable(x,y) )
    {
      if ( includeBuildings && this->isVisible(x,y) && Map::isOccupied(x,y) )
        return false;
      return true;
    }
    return false;
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool GameImpl::isVisible(int x, int y) const
  {
    return Map::visible(x, y);
  }
  //--------------------------------------------- IS EXPLORED ------------------------------------------------
  bool GameImpl::isExplored(int x, int y) const
  {
    return Map::isExplored(x, y);
  }
  //--------------------------------------------- HAS CREEP --------------------------------------------------
  bool GameImpl::hasCreep(int x, int y) const
  {
    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !this->isVisible(x, y))
      return false;
    return Map::hasCreep(x, y);
  }
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPowerPrecise(int x, int y, UnitType unitType) const
  {
    return Templates::hasPower(x, y, unitType, pylons);
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void GameImpl::vPrintf(const char *format, va_list arg)
  {
    // nogui & safety
    if ( !data->hasGUI ) return;
    if ( !format ) return;
    
    // Expand format into buffer
    char buffer[512];
    VSNPrintf(buffer, format, arg);

    // Dispatch message using existing Storm library function (lobby+game)
    SNETEVENT evt = { SNET_EVENT_SERVERMESSAGE, SNET_BROADCASTPLAYERID, buffer, strlen(buffer) + 1 };
    SEvtDispatch('SNET', 1, SNET_EVENT_SERVERMESSAGE, &evt);
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::vSendTextEx(bool toAllies, const char *format, va_list arg)
  {
    // safety
    if ( !format ) return;
    
    // Expand format and store in buffer
    char buffer[80]; // Use maximum size of 80 since there is a hardcoded limit in Broodwar of 80 characters
    VSNPrintf(buffer, format, arg);

    if ( buffer[0] == '/' )    // If we expect a battle.net command
    {
      SNetSendServerChatCommand(buffer);
      return;
    }

    if ( this->isReplay() )  // Just print the text if in a replay
    {
      BroodwarImpl.printf("%s", buffer);
      return;
    }

    // If we're in single player
    if ( this->isInGame() && !this->isMultiplayer() )
    {
      BW::CheatFlags::Enum cheatID = BW::getCheatFlag(buffer);
      if ( cheatID != BW::CheatFlags::None )  // Apply cheat code if it is one
      {
        this->cheatFlags ^= cheatID;
        QUEUE_COMMAND(BW::Orders::UseCheat, this->cheatFlags);
        if (cheatID == BW::CheatFlags::ShowMeTheMoney ||
            cheatID == BW::CheatFlags::BreatheDeep ||
            cheatID == BW::CheatFlags::WhatsMineIsMine ||
            cheatID == BW::CheatFlags::SomethingForNothing)
          this->cheatFlags ^= cheatID;
      }
      else  // Just print the message otherwise
      {
        BroodwarImpl.printf("%c%s: %c%s", this->BWAPIPlayer->getTextColor(), this->BWAPIPlayer->getName().c_str(), Text::Green, buffer);
      }
    } // single
    else  // multiplayer or lobby
    {
      // Otherwise, send the message using Storm command
      char szMessage[82];
      szMessage[0] = 0;
      szMessage[1] = 1;
      int msgLen = SStrCopy(&szMessage[2], buffer, 80);

      if ( this->isInGame() )    // in game
      {
        if ( toAllies ) // Send to all allies
        {
          for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
            if ( this->BWAPIPlayer->isAlly(players[i]) && BW::BWDATA::Players[i].dwStormId != -1 )
              SNetSendMessage(BW::BWDATA::Players[i].dwStormId, szMessage, msgLen + 3);
        }
        else  // Otherwise send to all
        {
          SNetSendMessage(SNET_BROADCASTPLAYERID, szMessage, msgLen + 3);
        } // toAllies
      }
      else  // assume in lobby, then send lobby message
      {
        szMessage[1] = 0x4C;
        SNetSendMessage(SNET_BROADCASTPLAYERID, &szMessage[1], msgLen + 2);
      } // isInGame
    } // multi
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::isInGame() const
  {
    return inGame;
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::isMultiplayer() const
  {
    return BW::BWDATA::NetMode != 0 && BW::BWDATA::NetMode != -1;
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::isBattleNet() const
  {
    return BW::BWDATA::NetMode == 'BNET';
  }
  //----------------------------------------------- IS PAUSED ------------------------------------------------
  bool GameImpl::isPaused() const
  {
    return BW::BWDATA::isGamePaused != 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::isReplay() const
  {
    return BW::BWDATA::InReplay != 0;
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void GameImpl::_startGame()
  {
    // Starts the game as a lobby host 
    QUEUE_COMMAND(BW::Orders::StartGame);
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    // Pauses the game 
    this->setLastError();
    QUEUE_COMMAND(BW::Orders::PauseGame);
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    // Resumes the game 
    this->setLastError();
    QUEUE_COMMAND(BW::Orders::ResumeGame);
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    // Leaves the current game. Moves directly to the post-game score screen 
    this->setLastError();
    BW::BWDATA::GameState      = 0;
    BW::BWDATA::gwNextGameMode = 6;
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    // Restarts the current match 
    // Does not work on Battle.net
    if ( this->isMultiplayer() )
    {
      this->setLastError(Errors::Invalid_Parameter);
      return;
    }

    this->setLastError();
    QUEUE_COMMAND(BW::Orders::RestartGame);
  }
  //--------------------------------------------- SET ALLIANCE -----------------------------------------------
  bool GameImpl::setAlliance(BWAPI4::Player player, bool allied, bool alliedVictory)
  {
    // Set the current player's alliance status 
    if ( !BWAPIPlayer || isReplay() || !player || player == BWAPIPlayer )
      return this->setLastError(Errors::Invalid_Parameter);

    u32 alliance = 0;
    for (int i = 0; i < BW::PLAYER_COUNT; ++i)
      alliance |= (BW::BWDATA::Game.playerAlliances[BWAPIPlayer->getIndex()][i] & 3) << (i*2);
    
    u8 newAlliance = allied ? (alliedVictory ? 2 : 1) : 0;
    if ( allied )
      alliance |= newAlliance << (player->getIndex()*2);
    else
      alliance &= ~(3 << (player->getIndex()*2));
    QUEUE_COMMAND(BW::Orders::SetAllies, alliance);
    return this->setLastError();
  }
  //----------------------------------------------- SET VISION -----------------------------------------------
  bool GameImpl::setVision(BWAPI4::Player player, bool enabled)
  {
    if ( !player )  // Parameter check
      return this->setLastError(Errors::Invalid_Parameter);

    if ( this->isReplay() )
    {
      u32 vision = BW::BWDATA::ReplayVision;
      if ( enabled )
        vision |= 1 << player->getIndex();
      else
        vision &= ~(1 <<  player->getIndex() );
      BW::BWDATA::ReplayVision = vision;
    }
    else
    {
      if ( !BWAPIPlayer || player == BWAPIPlayer )
        return this->setLastError(Errors::Invalid_Parameter);

      u16 vision = static_cast<u16>(BW::BWDATA::Game.playerVision[BWAPIPlayer->getIndex()]);
      if ( enabled )
        vision |= 1 << player->getIndex();
      else
        vision &= ~(1 <<  player->getIndex() );
      QUEUE_COMMAND(BW::Orders::SetVision, vision);
    }
    return this->setLastError();
  }
  //--------------------------------------------------- GAME SPEED -------------------------------------------
  void GameImpl::setLocalSpeed(int speed)
  {
    // Sets the frame rate of the client 
    if (this->speedOverride != std::numeric_limits<decltype(this->speedOverride)>::min()) return;

    setLocalSpeedDirect(speed);
  }
  void GameImpl::setLocalSpeedDirect(int speed)
  {
    if (speed < 0)
    {
      // Reset the speed if it is negative
      for (int i = 0; i < BW::NUM_SPEEDS; ++i)
      {
        BW::BWDATA::GameSpeedModifiers.gameSpeedModifiers[i] = BW::OriginalSpeedModifiers[i];
        BW::BWDATA::GameSpeedModifiers.altSpeedModifiers[i] = BW::OriginalSpeedModifiers[i] * 3;
      }
    }
    else
    {
      // Set all speeds if it is positive
      for (int i = 0; i < BW::NUM_SPEEDS; ++i)
      {
        BW::BWDATA::GameSpeedModifiers.gameSpeedModifiers[i] = speed;
        BW::BWDATA::GameSpeedModifiers.altSpeedModifiers[i] = speed * 3;
      }
    }
  }
  void GameImpl::setFrameSkip(int frameSkip)
  {
    setLastError(Errors::None);

    if ( frameSkip > 0 )
    {
      BW::BWDATA::FrameSkip = frameSkip;
      return;
    }
    setLastError(Errors::Invalid_Parameter);
  }
  //------------------------------------------ ISSUE COMMAND -------------------------------------------------
  bool GameImpl::issueCommand(const Unitset& units, UnitCommand command)
  {
    std::vector< std::vector<Unit> > groupsOf12;
    std::vector<Unit> nextGroup;

    // Iterate the set of units
    for (Unit u : units)
    {
      // Skip on invalid units that can't issue commands
      if ( !u->exists() )
        continue;

      // If unit can't issue the command while grouped (e.g. if it is a building) then try to issue
      // it individually instead.
      if ( !u->canIssueCommandGrouped(command) )
      {
        u->issueCommand(command);
        continue;
      }

      // If the command optimizer has taken over the command, then don't add it to this group
      static_cast<UnitImpl*>(u)->prepareIssueCommand(command);

      // Insert the unit into the next group
      nextGroup.push_back(u);

      // Create a new group of 12
      if ( nextGroup.size() >= 12 )
      {
        groupsOf12.push_back(nextGroup);
        nextGroup.clear();
      }
    }

    // Insert the last group into the groups of 12, if it is an incomplete group
    if ( !nextGroup.empty() )
      groupsOf12.push_back(nextGroup);

    // Return if no units to command
    if ( groupsOf12.empty() )
      return false;

    // Iterate our groups of 12
    for ( auto &v : groupsOf12 )
    {
      // Get the first unit available
      command.unit  = v.front();

      // Command optimization (no select) for unit unloading, but only if optimizer level >= 2
      //if ( command.type != BWAPI4::UnitCommandTypes::Unload || commandOptimizer.level < 2 )
      {
        // Select the unit group
        BW::Orders::Select sel(v);
        QueueGameCommand(&sel, sel.size());
        apmCounter.addSelect();
      }

      // Execute the command
      BroodwarImpl.executeCommand( command );
    }
    return true;
  }
  //------------------------------------------ GET SELECTED UNITS --------------------------------------------
  const Unitset& GameImpl::getSelectedUnits() const
  {
    this->setLastError();
    if ( !this->isFlagEnabled(BWAPI4::Flag::UserInput) )
    {
      this->setLastError(Errors::Access_Denied);
      return Unitset::none;
    }
    return selectedUnitSet;
  }
  //----------------------------------------------------- SELF -----------------------------------------------
  Player GameImpl::self() const
  {
    return this->BWAPIPlayer;
  }
  //----------------------------------------------------- ENEMY ----------------------------------------------
  Player GameImpl::enemy() const
  {
    return this->enemyPlayer;
  }
  //----------------------------------------------------- NEUTRAL --------------------------------------------
  Player GameImpl::neutral() const
  {
    return players[11];
  }
  //----------------------------------------------------- ALLIES ---------------------------------------------
  std::set<Player>& GameImpl::allies()
  {
    return _allies;
  }
  //----------------------------------------------------- ENEMIES --------------------------------------------
  std::set<Player>& GameImpl::enemies()
  {
    return _enemies;
  }
  //---------------------------------------------------- OBSERVERS -------------------------------------------
  std::set<Player>& GameImpl::observers()
  {
    return _observers;
  }
  //--------------------------------------------------- LATENCY ----------------------------------------------
  int GameImpl::getLatencyFrames() const
  {
    DWORD dwCallDelay = 1;
    if ( BW::BWDATA::NetMode )
    {
      SNETCAPS caps;
      caps.size = sizeof(SNETCAPS);
      SNetGetProviderCaps(&caps);

      dwCallDelay = std::clamp<int>(caps.defaultturnsintransit, 2, 8);
    }
    return (BW::BWDATA::LatencyFrames[BW::BWDATA::GameSpeed]) * (BW::BWDATA::Latency + dwCallDelay + 1);
  }
  int GameImpl::getLatencyTime() const
  {
    return getLatencyFrames() * BW::BWDATA::GameSpeedModifiers.gameSpeedModifiers[BW::BWDATA::GameSpeed];
  }
  int GameImpl::getRemainingLatencyFrames() const
  {
    return getLatencyFrames() - (this->getFrameCount() - lastTurnFrame);
  }
  int GameImpl::getRemainingLatencyTime() const
  {
    return (getLatencyFrames() * BW::BWDATA::GameSpeedModifiers.gameSpeedModifiers[BW::BWDATA::GameSpeed]) - (GetTickCount() - lastTurnTime);
  }
  //--------------------------------------------------- VERSION ----------------------------------------------
  bool GameImpl::isDebug() const
  {
    return BUILD_DEBUG == 1;
  }
  //----------------------------------------------- LAT COMPENSATION -----------------------------------------
  bool GameImpl::isLatComEnabled() const
  {
    return data->hasLatCom;
  }
  void GameImpl::setLatCom(bool isEnabled)
  {
    data->hasLatCom = isEnabled;
  }
  //----------------------------------------------- GET INSTANCE ID ------------------------------------------
  int GameImpl::getInstanceNumber() const
  {
    return (int)gdwProcNum;
  }
  //---------------------------------------------------- GET APM ---------------------------------------------
  int GameImpl::getAPM(bool includeSelects) const
  {
    return apmCounter.apm(includeSelects);
  }
  //---------------------------------------------------- SET MAP ---------------------------------------------
  bool GameImpl::setMap(const std::string &mapFileName)
  {
    constexpr int MAX_LENGTH = std::extent<decltype(BW::BWDATA::Game.mapFileName)>::value;

    if (mapFileName.length() >= MAX_LENGTH || mapFileName.empty())
      return setLastError(Errors::Invalid_Parameter);

    if ( !std::ifstream(mapFileName).is_open() )
      return setLastError(Errors::File_Not_Found);

    char mapFileNameRaw[MAX_LENGTH] = {};
    std::snprintf(mapFileNameRaw, MAX_LENGTH, "%s", mapFileName.c_str());

    strcpy(BW::BWDATA::Game.mapFileName, mapFileNameRaw);
    return setLastError();
  }
  //------------------------------------------------- ELAPSED TIME -------------------------------------------
  int GameImpl::elapsedTime() const
  {
    return BW::BWDATA::Game.elapsedTime;
  }
  //----------------------------------------------- COUNTDOWN TIMER ------------------------------------------
  int GameImpl::countdownTimer() const
  {
    return BW::BWDATA::Game.countdownTimer;
  }
  //------------------------------------------------- GET REGION AT ------------------------------------------
  BWAPI4::RegionImpl* GameImpl::getRegionAt(int x, int y) const
  {
    this->setLastError();
    if ( !isValid(Position(x, y)) )
    {
      this->setLastError(BWAPI4::Errors::Invalid_Parameter);
      return nullptr;
    }
    const BW::region * const rgn = BW::getRegionAt(x,y);
    if ( !rgn )
    {
      this->setLastError(BWAPI4::Errors::Invalid_Parameter);
      return nullptr;
    }
    return getRegion(rgn->getIndex());
  }
  int GameImpl::getLastEventTime() const
  {
    return this->lastEventTime;
  }
  bool GameImpl::setRevealAll(bool reveal)
  {
    if ( !isReplay() )
      return this->setLastError(Errors::Invalid_Parameter);
    BW::BWDATA::ReplayRevealAll = reveal ? 1 : 0;
    return this->setLastError();
  }
  //-------------------------------------------------- GET RANDOM SEED ---------------------------------------
  unsigned GameImpl::getRandomSeed() const
  {
    return BW::BWDATA::ReplayHead.gameSeed.randSeed;
  }
  //------------------------------------------------- CREATE, REMOVE, KILL ------------------------------------
  void GameImpl::killUnit(Unit unit)
  {
    this->unitsToKill.insert(unit);
  }
  void GameImpl::removeUnit(Unit unit)
  {
    this->unitsToRemove.insert(unit);
  }
  void GameImpl::createUnit(UnitType unitType, Player player, int x, int y, int hpPercent, int shieldsPercent, int energyPercent, int resources, int hangarCount, bool cloaked, bool burrowed, bool lifted, bool hallucinated, bool invincible)
  {
    if (!unitType.isValid() || !player) return;

    BW::TrigCreateUnitProperties properties;
    properties.locationSlot = 0;
    properties.propertySlot = 1; // properties are 1-based

    BW::TrigLocation oldLocation = BW::BWDATA::TrigLocations[0];
    BW::TrigUnitPropertySlot oldUnitProperties = BW::BWDATA::TrigUnitProperties[0];

    BW::TrigLocation newLocation = {};
    newLocation.left = newLocation.right = x;
    newLocation.top = newLocation.bottom = y;

    BW::TrigUnitPropertySlot newUnitProperties = {};
    newUnitProperties.validFlags = newUnitProperties.validProperties = 0xFFFF;
    newUnitProperties.hpPercent = static_cast<u8>(hpPercent);
    newUnitProperties.shieldPercent = static_cast<u8>(shieldsPercent);
    newUnitProperties.energyPercent = static_cast<u8>(energyPercent);
    newUnitProperties.resourceAmount = resources;
    newUnitProperties.unitsInHangar = static_cast<u16>(hangarCount);
    if (cloaked) newUnitProperties.flags |= 0x01;
    if (burrowed) newUnitProperties.flags |= 0x02;
    if (lifted) newUnitProperties.flags |= 0x04;
    if (hallucinated) newUnitProperties.flags |= 0x08;
    if (invincible) newUnitProperties.flags |= 0x10;

    // Overwrite data needed by this function
    BW::BWDATA::TrigLocations[0] = newLocation;
    BW::BWDATA::TrigUnitProperties[0] = newUnitProperties;

    BW::BWFXN_CreateUnitWithProperties(player->getID(), static_cast<u16>(unitType.getID()), &properties);
    
    // Restore original data
    BW::BWDATA::TrigLocations[0] = oldLocation;
    BW::BWDATA::TrigUnitProperties[0] = oldUnitProperties;
  }
  //----------------------------------------------- GET FORCES -----------------------------------------------
  const std::set<ForceImpl*>& GameImpl::getForces() const
  {
    return forces;
  }
  //----------------------------------------------- GET PLAYERS ----------------------------------------------
  const std::set<Player>& GameImpl::getPlayers() const
  {
    return playerSet;
  }
  //------------------------------------------------- GET UNITS ----------------------------------------------
  const Unitset& GameImpl::getAllUnits() const
  {
    return accessibleUnits;
  }
  //------------------------------------------------- GET MINERALS -------------------------------------------
  const Unitset& GameImpl::getMinerals() const
  {
    return minerals;
  }
  //------------------------------------------------- GET GEYSERS --------------------------------------------
  const Unitset& GameImpl::getGeysers() const
  {
    return geysers;
  }
  //------------------------------------------------- GET NEUTRAL UNITS --------------------------------------
  const Unitset& GameImpl::getNeutralUnits() const
  {
    return neutralUnits;
  }
  //------------------------------------------------- GET STATIC MINERALS ------------------------------------
  const Unitset& GameImpl::getStaticMinerals() const
  {
    return staticMinerals;
  }
  //------------------------------------------------- GET STATIC GEYSERS -------------------------------------
  const Unitset& GameImpl::getStaticGeysers() const
  {
    return staticGeysers;
  }
  //------------------------------------------------- GET STATIC NEUTRAL UNITS -------------------------------
  const Unitset& GameImpl::getStaticNeutralUnits() const
  {
    return staticNeutralUnits;
  }
  //------------------------------------------------ GET BULLETS ---------------------------------------------
  const std::set<BulletImpl*>& GameImpl::getBullets() const
  {
    return bullets;
  }
  //------------------------------------------------ GET NUKE DOTS -------------------------------------------
  const Position::list& GameImpl::getNukeDots() const
  {
    return nukeDots;
  }
  //------------------------------------------------ GET EVENTS ----------------------------------------------
  const std::list< Event >& GameImpl::getEvents() const
  {
    return events;
  }
  //----------------------------------------------- GET LAST ERROR -------------------------------------------
  Error GameImpl::getLastError() const
  {
    // returns the last error encountered in BWAPI
    return lastError;
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  bool GameImpl::setLastError(BWAPI4::Error e) const
  {
    // implies that an error has occured
    lastError = e;
    return e == Errors::None;
  }
  //--------------------------------------------- CAN BUILD HERE ---------------------------------------------
  bool GameImpl::canBuildHere(TilePosition position, UnitType type, Unit builder, bool checkExplored)
  {
    return Templates::canBuildHere(builder, position, type, checkExplored);
  }
  //--------------------------------------------- CAN MAKE ---------------------------------------------------
  bool GameImpl::canMake(UnitType type, Unit builder) const
  {
    return Templates::canMake(builder, type);
  }
  //--------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool GameImpl::canResearch(TechType type, Unit unit, bool checkCanIssueCommandType)
  {
    return Templates::canResearch(unit, type, checkCanIssueCommandType);
  }
  //--------------------------------------------- CAN UPGRADE ------------------------------------------------
  bool GameImpl::canUpgrade(UpgradeType type, Unit unit, bool checkCanIssueCommandType)
  {
    return Templates::canUpgrade(unit, type, checkCanIssueCommandType);
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  const TilePosition::list& GameImpl::getStartLocations() const
  {
    return startLocations;
  }
  //----------------------------------------------- GET ALL REGIONS ------------------------------------------
  const std::set<RegionImpl*> &GameImpl::getAllRegions() const
  {
    return this->regionsList;
  }
  //------------------------------------------------- GAME DATA ----------------------------------------------
  const GameData* GameImpl::getGameData() const
  {
    return data;
  }
};

