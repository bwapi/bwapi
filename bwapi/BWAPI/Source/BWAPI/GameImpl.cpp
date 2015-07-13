#include "../../../svnrev.h"
#include "GameImpl.h"

#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

#include <Util/clamp.h>
#include <Util/Convenience.h>

#include <BWAPI/ForceImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/BulletImpl.h>
#include <Templates.h>
#include <BWAPI/Command.h>
#include <BWAPI/Map.h>
#include <BWAPI/Flag.h>
#include <BWAPI/UnaryFilter.h>

#include <BWAPI/Unitset.h>

#include <BW/CUnit.h>
#include <BW/CBullet.h>
#include <BW/Offsets.h>
#include <BW/UnitTarget.h>
#include <BW/OrderTypes.h>
#include <BWAPI/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/CheatType.h>
#include <BW/Dialog.h>

#include "BWAPI/AIModule.h"
#include "../DLLMain.h"
#include "../Config.h"
#include "../Detours.h"
#include "../NewHackUtil.h"
#include "BWtoBWAPI.h"

#include "../BW/Pathing.h"

#include "../../../Debug.h"

namespace BWAPI
{
  //----------------------------------------------------------------------------------------------------------
  Force GameImpl::getForce(int forceID) const
  {
    return server.getForce(forceID);
  }
  //----------------------------------------------------------------------------------------------------------
  Region GameImpl::getRegion(int regionID) const
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
      return BWAPI::Latency::SinglePlayer;

    if ( this->isBattleNet() )
    {
      switch(BW::BWDATA::Latency)
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
      switch(BW::BWDATA::Latency)
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
  int GameImpl::getFrameCount() const
  {
    return this->frameCount;
  }
  //--------------------------------------------- GET REPLAY FRAME COUNT -------------------------------------
  int GameImpl::getReplayFrameCount() const
  {
    return BW::BWDATA::ReplayFrames;
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
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return BWAPI::Positions::Unknown;
    return BWAPI::Position(BW::BWDATA::Mouse.x, BW::BWDATA::Mouse.y);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(MouseButton button) const
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return false;

    int vkValue;
    switch ( button )
    {
      case BWAPI::M_LEFT:
        vkValue = VK_LBUTTON;
        break;
      case BWAPI::M_RIGHT:
        vkValue = VK_RBUTTON;
        break;
      case BWAPI::M_MIDDLE:
        vkValue = VK_MBUTTON;
        break;
      default:
        return false;
    }
    return (GetKeyState(vkValue) & 0x80) > 0;
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(Key key) const
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return false;

    if ( key < 0 || key >= K_MAX )
      return false;

    return (GetKeyState(key) & 128) > 0;
  }
  //------------------------------------------- GET SCREEN POSITION ------------------------------------------
  BWAPI::Position GameImpl::getScreenPosition() const
  {
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
      return BWAPI::Positions::Unknown;
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
    BW::BWDATA::MoveToTile = BW::TilePosition(movePos);
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
    if ( flag < 0 || flag >= BWAPI::Flag::Max ) 
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
    if ( flag < 0 || flag >= BWAPI::Flag::Max )
    {
      this->setLastError(Errors::Invalid_Parameter);
      return;
    }
    
    // check if tournament will allow the call
    if ( !this->tournamentCheck(Tournament::EnableFlag, &flag) )
      return;

    // Modify flag state 
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
  //--------------------------------------------- GET UNITS IN RECTANGLE -------------------------------------
  Unit GameImpl::_unitFromIndex(int index)
  {
    return this->unitArray[index-1];
  }
  Unitset GameImpl::getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred) const
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
                                                 [&](Unit u){ if ( !pred.isValid() || pred(u) )
                                                                 unitFinderResults.insert(u); });
    // Return results
    return unitFinderResults;
  }
  Unit GameImpl::getClosestUnitInRectangle(Position center, const UnitFilter &pred, int left, int top, int right, int bottom) const
  {
    // cppcheck-suppress variableScope
    int bestDistance = 99999999;
    Unit pBestUnit = nullptr;

    Templates::iterateUnitFinder<BW::unitFinder>( BW::BWDATA::UnitOrderingX.data(),
                                                  BW::BWDATA::UnitOrderingY.data(),
                                                  BW::BWDATA::UnitOrderingCount,
                                                  left,
                                                  top,
                                                  right,
                                                  bottom,
                                                  [&](Unit u){ if ( !pred.isValid() || pred(u) )
                                                                {
                                                                  int newDistance = u->getDistance(center);
                                                                  if ( newDistance < bestDistance )
                                                                  {
                                                                    pBestUnit = u;
                                                                    // cppcheck-suppress unreadVariable
                                                                    bestDistance = newDistance;
                                                                  }
                                                                } } );
    return pBestUnit;
  }
  Unit GameImpl::getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center, int radius) const
  {
    Unit pBestUnit = nullptr;
    Position rad(radius,radius);
    
    Position topLeft(center - rad);
    Position botRight(center + rad);

    topLeft.makeValid();
    botRight.makeValid();

    Templates::iterateUnitFinder<BW::unitFinder>( BW::BWDATA::UnitOrderingX.data(),
                                                  BW::BWDATA::UnitOrderingY.data(),
                                                  BW::BWDATA::UnitOrderingCount,
                                                  topLeft.x,
                                                  topLeft.y,
                                                  botRight.x,
                                                  botRight.y,
                                                  [&](Unit u){ if ( !pred.isValid() || pred(u) )
                                                                {
                                                                  if ( pBestUnit == nullptr )
                                                                    pBestUnit = u;
                                                                  else
                                                                    pBestUnit = best(pBestUnit,u); 
                                                                } } );

    return pBestUnit;
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

    if ( !this->tournamentCheck(Tournament::Printf, buffer) )
      return;

    // Dispatch message using existing Storm library function (lobby+game)
    S_EVT evt = { 4, -1, buffer, strlen(buffer) + 1 };
    SEvtDispatch('SNET', 1, 4, &evt);
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::vSendTextEx(bool toAllies, const char *format, va_list arg)
  {
    // safety
    if ( !format ) return;
    
    // Expand format and store in buffer
    char buffer[80]; // Use maximum size of 80 since there is a hardcoded limit in Broodwar of 80 characters
    VSNPrintf(buffer, format, arg);

    // Check if tournament module allows sending text
    if ( !this->tournamentCheck(Tournament::SendText, buffer) )
      return;

    if ( buffer[0] == '/' )    // If we expect a battle.net command
    {
      SNetSendServerChatCommand(buffer);
      return;
    }

    if ( this->isReplay() )  // Just print the text if in a replay
    {
      Broodwar << buffer << std::endl;
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
        Broodwar << this->BWAPIPlayer->getTextColor() << this->BWAPIPlayer->getName()
                  << ": " << Text::Green << buffer << std::endl;
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
          SNetSendMessage(-1, szMessage, msgLen + 3);
        } // toAllies
      }
      else  // assume in lobby, then send lobby message
      {
        szMessage[1] = 0x4C;
        SNetSendMessage(-1, &szMessage[1], msgLen + 2);
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
    if ( !this->tournamentCheck(Tournament::PauseGame) )
      return;
    QUEUE_COMMAND(BW::Orders::PauseGame);
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    // Resumes the game 
    this->setLastError();
    if ( !this->tournamentCheck(Tournament::ResumeGame) )
      return;
    QUEUE_COMMAND(BW::Orders::ResumeGame);
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    // Leaves the current game. Moves directly to the post-game score screen 
    this->setLastError();
    if ( !this->tournamentCheck(Tournament::LeaveGame) )
      return;
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
  bool GameImpl::setAlliance(BWAPI::Player player, bool allied, bool alliedVictory)
  {
    // Set the current player's alliance status 
    if ( !BWAPIPlayer || isReplay() || !player || player == BWAPIPlayer )
      return this->setLastError(Errors::Invalid_Parameter);

    u32 alliance = 0;
    for (int i = 0; i < BW::PLAYER_COUNT; ++i)
      alliance |= (BW::BWDATA::Alliance[BWAPIPlayer->getIndex()][i] & 3) << (i*2);
    
    u8 newAlliance = allied ? (alliedVictory ? 2 : 1) : 0;
    if ( allied )
      alliance |= newAlliance << ( static_cast<PlayerImpl*>(player)->getIndex()*2);
    else
      alliance &= ~(3 << ( static_cast<PlayerImpl*>(player)->getIndex()*2) );
    QUEUE_COMMAND(BW::Orders::SetAllies, alliance);
    return this->setLastError();
  }
  //----------------------------------------------- SET VISION -----------------------------------------------
  bool GameImpl::setVision(BWAPI::Player player, bool enabled)
  {
    if ( !player )  // Parameter check
      return this->setLastError(Errors::Invalid_Parameter);

    if ( this->isReplay() )
    {
      u32 vision = BW::BWDATA::ReplayVision;
      if ( enabled )
        vision |= 1 << static_cast<PlayerImpl*>(player)->getIndex();
      else
        vision &= ~(1 <<  static_cast<PlayerImpl*>(player)->getIndex() );
      BW::BWDATA::ReplayVision = vision;
    }
    else
    {
      if ( !BWAPIPlayer || player == BWAPIPlayer )
        return this->setLastError(Errors::Invalid_Parameter);

      u16 vision = static_cast<u16>(BW::BWDATA::PlayerVision[BWAPIPlayer->getIndex()]);
      if ( enabled )
        vision |= 1 << static_cast<PlayerImpl*>(player)->getIndex();
      else
        vision &= ~(1 <<  static_cast<PlayerImpl*>(player)->getIndex() );
      QUEUE_COMMAND(BW::Orders::SetVision, vision);
    }
    return this->setLastError();
  }
  //--------------------------------------------------- GAME SPEED -------------------------------------------
  void GameImpl::setLocalSpeed(int speed)
  {
    // Sets the frame rate of the client 
    if (!this->tournamentCheck(Tournament::SetLocalSpeed, &speed) ||
      this->speedOverride != std::numeric_limits<decltype(this->speedOverride)>::min()) return;

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
    if ( !this->tournamentCheck(Tournament::SetFrameSkip, &frameSkip) )
      return;

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
      if ( static_cast<UnitImpl*>(u)->prepareIssueCommand(command) )
        continue;

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
      // TODO this piece should be extracted to the CommandOptimizer
      if ( command.type != BWAPI::UnitCommandTypes::Unload || commandOptimizer.level < 2 )
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
    if ( !this->isFlagEnabled(BWAPI::Flag::UserInput) )
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
  Playerset& GameImpl::allies()
  {
    return _allies;
  }
  //----------------------------------------------------- ENEMIES --------------------------------------------
  Playerset& GameImpl::enemies()
  {
    return _enemies;
  }
  //---------------------------------------------------- OBSERVERS -------------------------------------------
  Playerset& GameImpl::observers()
  {
    return _observers;
  }
  //--------------------------------------------------- LATENCY ----------------------------------------------
  int GameImpl::getLatencyFrames() const
  {
    DWORD dwCallDelay = 1;
    if ( BW::BWDATA::NetMode )
    {
      CAPS caps;
      caps.dwSize = sizeof(CAPS);
      SNetGetProviderCaps(&caps);

      dwCallDelay = clamp<DWORD>(caps.dwCallDelay, 2, 8);
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
  int GameImpl::getRevision() const
  {
    return SVN_REV;
  }
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
    if ( !this->tournamentCheck(Tournament::SetLatCom, &isEnabled) )
      return;
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
  bool GameImpl::setMap(const char *mapFileName)
  {
    if ( !mapFileName || strlen(mapFileName) >= MAX_PATH || !mapFileName[0] )
      return setLastError(Errors::Invalid_Parameter);

    if ( !std::ifstream(mapFileName).is_open() )
      return setLastError(Errors::File_Not_Found);

    if ( !this->tournamentCheck(Tournament::SetMap, (void*)mapFileName) )
      return setLastError(Errors::None);


    strcpy(BW::BWDATA::CurrentMapFileName.data(), mapFileName);
    return setLastError(Errors::None);
  }
  //------------------------------------------------- ELAPSED TIME -------------------------------------------
  int GameImpl::elapsedTime() const
  {
    return BW::BWDATA::ElapsedTime;
  }
  //-------------------------------------- SET COMMAND OPTIMIZATION LEVEL ------------------------------------
  void GameImpl::setCommandOptimizationLevel(int level)
  {
    level = clamp<int>(level, 0, 4);
    if ( !this->tournamentCheck(Tournament::SetCommandOptimizationLevel, &level) )
      return;
    this->commandOptimizer.level = level;
  }
  //----------------------------------------------- COUNTDOWN TIMER ------------------------------------------
  int GameImpl::countdownTimer() const
  {
    return BW::BWDATA::CountdownTimer;
  }
  //------------------------------------------------- GET REGION AT ------------------------------------------
  BWAPI::Region GameImpl::getRegionAt(int x, int y) const
  {
    this->setLastError();
    if ( !Position(x, y) )
    {
      this->setLastError(BWAPI::Errors::Invalid_Parameter);
      return nullptr;
    }
    const BW::region * const rgn = BW::getRegionAt(x,y);
    if ( !rgn )
    {
      this->setLastError(BWAPI::Errors::Invalid_Parameter);
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
};

