#include "GameImpl.h"
#include <vector>
#include <string>

#include "../WMode.h"
#include "../Detours.h"
#include "../DLLMain.h"
#include "../Resolution.h"

#include <BWAPI/BWtoBWAPI.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/BulletImpl.h>
#include <BWAPI/RegionImpl.h>
#include "Command.h"

#include <BW/CBullet.h>
#include <BW/CUnit.h>
#include <BW/Dialog.h>
#include <BW/Offsets.h>

#include "../../../Debug.h"

/*
  This files holds all functions of the GameImpl class that are not part of the Game interface.
 */

namespace BWAPI
{
  GameImpl BroodwarImpl;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  GameImpl::GameImpl()
  {
    BWAPI::BroodwarPtr = static_cast<Game*>(this);

    BWtoBWAPI_init();

    // iterate through players and create PlayerImpl for each
    for (u8 i = 0; i < BW::PLAYER_COUNT; ++i)
      players[i] = new PlayerImpl(i);

    // iterate through units and create UnitImpl for each
    for (u16 i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; ++i)
      unitArray[i] = new UnitImpl(&BW::BWDATA::UnitNodeTable[i], i);

    // iterate through bullets and create BulletImpl for each
    for (u16 i = 0; i < BW::BULLET_ARRAY_MAX_LENGTH; ++i)
      bulletArray[i] = new BulletImpl(&BW::BWDATA::BulletNodeTable[i], i);

    this->initializeData();
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {
    this->initializeData();

    // destroy all UnitImpl
    for (UnitImpl* u : unitArray)
    {
      if (u) delete u;
    }
    unitArray.fill(nullptr);

    // destroy all PlayerImpl
    for (int i = 0; i < std::extent<decltype(players)>::value; ++i)
    {
      if (players[i]) delete players[i];
      players[i] = nullptr;
    }

    // destroy all bullets
    for (BulletImpl* b : bulletArray)
    {
      if (b) delete b;
    }
    bulletArray.fill(nullptr);
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (UnitImpl* u : unitArray)
    {
      if (u) u->setSelected(false);
    }

    selectedUnitSet.clear();
    for (int i = 0; i < BW::BWDATA::ClientSelectionCount && i < BW::MAX_SELECTION_COUNT; ++i)
    {
      BWAPI::UnitImpl *u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA::ClientSelectionGroup[i]);
      if (u)
      {
        u->setSelected(true);
        selectedUnitSet.insert(u);
      }
    }
  }
  void GameImpl::dropPlayers()
  {
    for ( int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( BW::BWDATA::playerStatusArray[i] & 0x10000 )
      {
        int iplr = this->stormIdToPlayerId(i);
        if ( iplr != -1 && iplr != BW::BWDATA::g_LocalHumanID )
        {
          this->droppedPlayers.push_back(this->players[iplr]);
          SNetDropPlayer(i, 0x40000006);  // The value used when dropping
        }
      }
    }
  }
  //------------------------------------------------ MOUSE/KEY INPUT -----------------------------------------
  void GameImpl::pressKey(int key)
  {
    // Don't do anything if key is 0
    // used when auto-menu dialogs are not found, performance
    if ( !key )
      return;

    // Press and release the key
    PostMessage(SDrawGetFrameWindow(), WM_CHAR, (WPARAM)key, NULL);
  }
  void GameImpl::mouseDown(int x, int y)
  {
    // Press the left mouse button
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONDOWN, NULL, (LPARAM)MAKELONG(x,y));
  }
  void GameImpl::mouseUp(int x, int y)
  {
    // Release the left mouse button
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONUP, NULL, (LPARAM)MAKELONG(x,y));
  }
  //------------------------------------------- PLAYER ID CONVERT --------------------------------------------
  int GameImpl::stormIdToPlayerId(int dwStormId)
  {
    /* Translates a storm ID to a player Index */
    for (int i = 0; i < BW::PLAYER_COUNT; ++i)
    {
      if ( BW::BWDATA::Players[i].dwStormId == dwStormId )
        return i;
    }
    return -1;
  }
  //----------------------------------------------- PARSE TEXT -----------------------------------------------
  bool GameImpl::parseText(const std::string &text)
  {
    // analyze the string
    std::stringstream ss(text);
    std::string cmd;
    int n;

    ss >> cmd;

    // commands list
    if (cmd == "/leave")
    {
      this->leaveGame();
    }
    else if (cmd == "/speed")
    {
      n = -1;
      ss >> n;
      setLocalSpeedDirect(n);
      Broodwar << "Changed game speed" << std::endl;
    }
    else if (cmd == "/fs")
    {
      n = 1;
      ss >> n;
      setFrameSkip(n);
      Broodwar << "Altered frame skip" << std::endl;
    }
    else if (cmd == "/cheats")
    {
      sendText("power overwhelming");
      sendText("operation cwal");
      sendText("the gathering");
      sendText("medieval man");
      sendText("black sheep wall");
      sendText("food for thought");
      sendText("modify the phase variance");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
    }
    else if (cmd == "/restart")
    {
      restartGame();
    }
    else if (cmd == "/nogui")
    {
      setGUI(!data->hasGUI);
      Broodwar << "GUI: " << (data->hasGUI ? "enabled" : "disabled") << std::endl;
    }
    else if (cmd == "/wmode")
    {
      SetWMode(BW::BWDATA::GameScreenBuffer.width(), BW::BWDATA::GameScreenBuffer.height(), !wmode);
      Broodwar << "Toggled windowed mode." << std::endl;
    }
    else if (cmd == "/grid")
    {
      grid = !grid;
      Broodwar << "Matrix grid " << (grid ? "enabled" : "disabled") << std::endl;
    }
    else if ( cmd == "/fps" )
    {
      this->showfps = !this->showfps;
      Broodwar << "FPS display " << (showfps ? "enabled" : "disabled") << std::endl;
    }
#ifdef _DEBUG
    else if (cmd == "/latency")
    {
      Broodwar << "Latency: " << getLatency() << std::endl;
      Broodwar << "New latency: " << getLatencyFrames() << " frames (" << getLatencyTime() << "ms)" << std::endl;
    }
    else if (cmd == "/test")
    {
    }
#endif
    else
    {
      return false;
    }
    return true;
  }
  //------------------------------------------- INTERFACE EVENT UPDATE ---------------------------------------
  void GameImpl::processInterfaceEvents()
  {
    // GameImpl events
    this->updateEvents();

    // UnitImpl events
    for(Unit u : this->accessibleUnits)
    {
      u->exists() ? u->updateEvents() : u->interfaceEvents.clear();
    }

    // ForceImpl events
    for (Force f : this->forces)
      f->updateEvents();

    // BulletImpl events
    for (Bullet b : this->bullets)
    {
      b->exists() ? b->updateEvents() : b->interfaceEvents.clear();
    }

    // RegionImpl events
    for (Region r : this->regionsList)
      r->updateEvents();

    // PlayerImpl events
    for (Player p : this->playerSet)
      p->updateEvents();
  }
  //------------------------------------------- GET PLAYER INTERNAL ------------------------------------------
  PlayerImpl *GameImpl::_getPlayer(int id)
  {
    if (id < 0 || id >= BW::PLAYER_COUNT)
      return players[BW::PLAYER_COUNT - 1];
    return players[id];
  }
  int GameImpl::_currentPlayerId()
  {
    return BW::BWDATA::g_LocalHumanID;
  }
  bool GameImpl::tournamentCheck(Tournament::ActionID type, void *parameter)
  {
    if ( this->tournamentController && !isTournamentCall )
    {
      isTournamentCall  = true;
      bool allow        = this->tournamentController->onAction(type, parameter);
      isTournamentCall  = false;
      return allow;
    }
    return true;
  }
  void GameImpl::initializeData()
  {
    // Delete forces
    for ( Forceset::iterator f = this->forces.begin(); f != this->forces.end(); ++f)
      delete (static_cast<ForceImpl*>(*f));
    this->forces.clear();

    // Remove player references
    this->BWAPIPlayer = nullptr;
    this->enemyPlayer = nullptr;

    // Set random seed
    srand(GetTickCount());

    // clear all sets
    this->aliveUnits.clear();
    this->dyingUnits.clear();
    this->discoverUnits.clear();
    this->accessibleUnits.clear();
    this->evadeUnits.clear();
    this->selectedUnitSet.clear();
    this->startLocations.clear();
    this->playerSet.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->neutralUnits.clear();
    this->bullets.clear();
    this->pylons.clear();
    this->staticMinerals.clear();
    this->staticGeysers.clear();
    this->staticNeutralUnits.clear();
    this->_allies.clear();
    this->_enemies.clear();
    this->_observers.clear();

    // Reset saved selection
    this->savedUnitSelection.fill(nullptr);
    this->wantSelectionUpdate = false;

    // Disable all game flags
    flags.fill(false);

    // Clear the latency buffer
    for(unsigned int j = 0; j < this->commandBuffer.size(); ++j)
      for (unsigned int i = 0; i < this->commandBuffer[j].size(); ++i)
        delete this->commandBuffer[j][i];
    this->commandBuffer.clear();
    this->commandBuffer.reserve(16);

    commandOptimizer.init();

    // Delete all dead units
    for ( Unitset::iterator d = this->deadUnits.begin(); d != this->deadUnits.end(); ++d )
      delete static_cast<UnitImpl*>(*d);
    this->deadUnits.clear();

    // Delete all regions
    for ( Regionset::iterator r = this->regionsList.begin(); r != this->regionsList.end(); ++r )
      delete static_cast<RegionImpl*>(*r);
    this->regionsList.clear();
    this->regionMap.clear();

    // Reset game speeds and text size
    this->setLocalSpeedDirect(this->speedOverride);
    this->setFrameSkip(1);
    this->setTextSize();
    this->setGUI(true);

    // Reset all Unit objects in the unit array
    for (UnitImpl* u : unitArray)
    {
      if (!u) continue;
      u->clear();
      u->userSelected = false;
      u->isAlive = false;
      u->wasAlive = false;
      u->wasCompleted = false;
      u->wasAccessible = false;
      u->wasVisible = false;
      u->nukeDetected = false;
      u->lastType = UnitTypes::Unknown;
      u->lastPlayer = nullptr;

      u->setID(-1);
    }

    BulletImpl::nextId = 0;
    this->cheatFlags  = 0;
    //this->frameCount  = -1;
    this->frameCount = 0;

    this->clientInfo.clear();
    this->interfaceEvents.clear();

    //reload auto menu data (in case the AI set the location of the next map/replay)
    this->loadAutoMenuData();

    //clear everything in the server
    this->server.clearAll();

    // clear messages so they are not stored until the next match
    this->sentMessages.clear();

    // Some other variables
    apmCounter.init();
    fpsCounter.init();

    // @NOTE: Freeing libraries comes after because of some destructors for functionals in Interface Events

    // Destroy the AI Module client
    if ( this->client )
    {
      delete this->client;
      this->client = nullptr;
    }

    // Unload the AI Module library
    if ( hAIModule )
    {
      FreeLibrary(hAIModule);
      hAIModule = nullptr;
    }

    this->startedClient = false;

    // Destroy the Tournament Module controller
    if ( this->tournamentController )
    {
      delete this->tournamentController;
      this->tournamentController = nullptr;
    }

    // Destroy the Tournament Module AI
    if ( this->tournamentAI )
    {
      delete this->tournamentAI;
      this->tournamentAI = nullptr;
    }

    // Destroy the Tournament Module Library
    if ( hTournamentModule )
    {
      FreeLibrary(hTournamentModule);
      hTournamentModule = nullptr;
    }

    this->bTournamentMessageAppeared = false;
  }

  void GameImpl::queueSentMessage(std::string const &message)
  {
    if (!message.empty())
      this->sentMessages.emplace_back(message);
  }
};
