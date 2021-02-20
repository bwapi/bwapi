#include "GameImpl.h"

#include <sstream>
#include <string>

#include "../StringUtil.h"

#include "PlayerImpl.h"

#include "../DLLMain.h"
#include "../Config.h"
#include "../Detours.h"

#include "../BW/Constants.h"
#include "../BW/Bitmap.h"
#include "../BW/CUnit.h"
#include "../BW/OrderTypes.h"
#include "../BW/Pathing.h"
#include <BWAPI/Color.h>
#include <BWAPI4/Event.h>

#include "../svnrev.h"

using namespace BWAPI4;

//------------------------------------------------- UPDATE -------------------------------------------------
void GameImpl::update()
{
  shapes.clear(); // Reset shapes

  //this function is called every frame from a hook attached in DllMain.cpp
  this->inGame = true;

  // Calculate APM, FPS, etc.
  updateStatistics();

  //the first time update() is called, we also call onGameStart to initialize some things
  if ( !onStartCalled )
    this->onGameStart();

  if ( !this->calledMatchEnd && frameCount > 1 )
  {
    bool win     = false;
    bool allDone = false;
    if ( this->BWAPIPlayer )
    {
      if ( this->BWAPIPlayer->isVictorious() )
      {
        win     = true;
        allDone = true;
      }
      if ( this->BWAPIPlayer->isDefeated() )
      {
        win     = false;
        allDone = true;
      }
    }
    else
    {
      allDone = true;
      for(PlayerImpl* p : this->players)
      {
        if (p->getIndex() >= BW::PLAYABLE_PLAYER_COUNT)
          continue;
        if ( !p->isDefeated() && !p->isVictorious() && !p->leftGame() )
          allDone = false;
      }
    }
    if ( allDone)
    {
      if (win)
        rn_GameResult = "win";
      this->calledMatchEnd = true;
      events.push_back(Event::MatchFrame());
      events.push_back(Event::MatchEnd(win));
      server.update();
      this->inGame = false;
      events.push_back(Event::MenuFrame());
      server.update();
    }
  }

  //don't have any more MatchFrame events after MatchEnd until MatchStart is called.
  if ( this->calledMatchEnd ) return;

  // Update unit selection
  if (wantSelectionUpdate && savedUnitSelection != BW::BWDATA::ClientSelectionGroup)
  {
    wantSelectionUpdate = false;
    savedUnitSelection = BW::BWDATA::ClientSelectionGroup;
    refreshSelectionStates();
  }

  //update players and check to see if they have just left the game.
  _allies.clear();
  _enemies.clear();
  _observers.clear();
  if ( BWAPIPlayer )
  {
    for(Player p : players)
    {
      if ( p->leftGame() || p->isDefeated() || p == BWAPIPlayer )
        continue;
      if ( BWAPIPlayer->isAlly(p) )
        _allies.insert(p);
      if ( BWAPIPlayer->isEnemy(p) )
        _enemies.insert(p);
      if ( p->isObserver() )
        _observers.insert(p);
    }
  }
  for (int i = 0; i < BW::PLAYER_COUNT; ++i)
  {
    bool prevLeftGame = this->players[i]->leftGame();
    this->players[i]->updateData();
    if (!prevLeftGame && this->players[i]->leftGame())
      events.push_back(Event::PlayerLeft(this->players[i]));
  }
  //update properties of Unit and Bullet objects
  this->updateUnits();
  this->updateBullets();

  //iterate through the list of intercepted messages
  for(std::string &s : sentMessages)
    BroodwarImpl.onSendText(s);
  this->sentMessages.clear();

  //on the first frame we check to see if the client process has connected.
  //if not, then we load the AI dll specified in bwapi.ini
  if ( !this->startedClient )
  {
    this->initializeAIModule();

    //push the MatchStart event to the front of the queue so that it is the first event in the queue.
    events.push_front(Event::MatchStart());
    this->startedClient = true;
  }

  //each frame we add a MatchFrame event to the queue
  events.push_back(Event::MatchFrame());

  //if the AI is a client process, this will signal the client to process the next frame
  //if the AI is a DLL, this will translate the events into AIModule callbacks.
  server.update();

  //Before returning control to starcraft, we clear the unit data for units that are no longer accessible
  for(Unit u : evadeUnits)
    static_cast<UnitImpl*>(u)->updateData();

  //We also kill the units that are dying on this frame.
  //We wait until after server.update() and processEvents() to do this so that the AI can
  //access the last frame of unit data during the onUnitDestroy callback.
  for(Unit ui : dyingUnits)
  {
    UnitImpl *u = static_cast<UnitImpl*>(ui);
    deadUnits.insert(u);
    u16 index = u->getIndex();
    unitArray[index] = new UnitImpl(&BW::BWDATA::UnitNodeTable[index], index);
    u->die();
  }

  //Kill or remove units starcraft-side, not sure if this should go here or elsewhere but here it is
  updateKillAndRemoveUnits();

  /* In case we ever want to add a Flag::UnitPermanence cheat flag...
  bool UnitPermanence = false;
  if ( !UnitPermanence )
  {
    for(Unit ui : evadeUnits)
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      deadUnits.push_back(u);
      int index = u->getIndex();
      unitArray[index] = new UnitImpl(&BW::BWDATA::UnitNodeTable->unit[index],(u16)index);
      u->die();
    }
  }
  */

  //increment frame count if the game is not paused
  if ( BW::BWDATA::isGamePaused == 0 )
    this->frameCount++;

  // Set the replay time, this is a workaround to fixing the replay DIVIDE_BY_ZERO exception bug
  if ( !this->isReplay() )
    BW::BWDATA::ReplayHead.frameCount = this->getFrameCount()+20;

  // Update any graphical/debug overlays
  updateOverlays();
  setTextSize(); // Reset text size
  //finally return control to starcraft
}
namespace
{
  bool __fastcall alwaysKillCondition(int player, u16 type, BW::CUnit *unit)
  {
    return true;
  }
}
void GameImpl::updateKillAndRemoveUnits()
{
  BW::TrigKillRemoveProperties properties = {};
  properties.condition = &alwaysKillCondition;

  BW::BWDATA::TrigRemoveInsteadOfKill = false;
  for (Unit u : unitsToKill)
  {
    if (static_cast<UnitImpl*>(u)->getOriginalRawData)
      BW::BWFXN_KillRemoveUnit(static_cast<UnitImpl*>(u)->getOriginalRawData, &properties);
    unitsToRemove.erase(u);
  }
  unitsToKill.clear();

  BW::BWDATA::TrigRemoveInsteadOfKill = true;
  for (Unit u : unitsToRemove)
  {
    if (static_cast<UnitImpl*>(u)->getOriginalRawData)
      BW::BWFXN_KillRemoveUnit(static_cast<UnitImpl*>(u)->getOriginalRawData, &properties);
  }
  unitsToRemove.clear();
}
//------------------------------------------------- STATS -------------------------------------------------
void GameImpl::updateStatistics()
{
  apmCounter.update(getFrameCount());
  fpsCounter.update(getFrameCount());
}

//------------------------------------------------- OVERLAYS -------------------------------------------------
void GameImpl::updateOverlays()
{
  if ( !data->hasGUI )
    return;

  // grid
  if ( grid )
  {
    BWAPI::Position scrPos = getScreenPosition();

    // draw mtx grid
    for ( int y = scrPos.y/32; y < (scrPos.y + BW::BWDATA::GameScreenBuffer.height())/32 + 1; ++y )
    {
      for ( int x = scrPos.x/32; x < (scrPos.x + BW::BWDATA::GameScreenBuffer.width())/32 + 1; ++x )
      {
        for ( int i = 0; i < 32; i += 4 )
        {
          drawLine(CoordinateType::Map, x*32 + 32, y*32 + i, x*32 + 32, y*32 + i + 2, BWAPI4::Colors::Grey);
          drawLine(CoordinateType::Map, x*32 + i, y*32 + 32, x*32 + i + 2, y*32 + 32, BWAPI4::Colors::Grey);
        }
      }
    }
    setTextSize(Text::Size::Small);
    drawText(CoordinateType::Screen, 64, 288, "%c(%u, %u)", Text::White, (scrPos.x+this->getMousePosition().x)/32, (scrPos.y+this->getMousePosition().y)/32);
    setTextSize();
  } // grid

  // fps display
  if ( showfps )
  {
    setTextSize(Text::Size::Small);
    drawText(CoordinateType::Screen, 1,1, "%cFrame Rate (Logical Frames)\nLFPS: %d\nAvg LFPS:%.3lf", BWAPI4::Text::Yellow, this->getFPS(), this->getAverageFPS());
    setTextSize();
  } // fps

}

//--------------------------------------------- AI MODULE ----------------------------------------------
void GameImpl::initializeAIModule()
{
  // Connect to external module if it exists
  externalModuleConnected = false;
  std::string moduleName("<Nothing>");
  if ( server.isConnected() ) //check to see if the server is connected to the client
  {
    // Set the module string
    moduleName = "<Client Connection>";
    externalModuleConnected = true;
  }
  else // if not, allow interaction and print error message
  {
    // enable flags to allow interaction
    BroodwarImpl.enableFlag(Flag::UserInput);

    // print error string
    BroodwarImpl.printf("%cERROR: No Client is connected.", Text::Red);
    externalModuleConnected = false;
  }

  // Load flags
  std::string completemapinformation = LoadConfigString("flags", "completemapinformation", "OFF");
  if (completemapinformation == "ON")
    BroodwarImpl.enableFlag(Flag::CompleteMapInformation);

  std::string userinput = LoadConfigString("flags", "userinput", "ON");
  if (userinput == "ON")
    BroodwarImpl.enableFlag(Flag::UserInput);

  sendText("BWAPI %s.%d %s is now live using \"%s\".", BWAPI_VER, SVN_REV, BUILD_STR, moduleName.c_str());
}

