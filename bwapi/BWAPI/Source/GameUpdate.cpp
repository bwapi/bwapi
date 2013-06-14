#include "GameImpl.h"

#include "PlayerImpl.h"

#include "DLLMain.h"
#include "Config.h"
#include "Detours.h"
#include "Recording.h"

#include <BW/Offsets.h>
#include <BW/Bitmap.h>
#include <BW/CUnit.h>
#include <BW/OrderTypes.h>
#include <BW/Pathing.h>

#include <Util/Foreach.h>
#include "../../../svnrev.h"
#include "../../../Debug.h"

#define TOURNAMENT_STR "BWAPI r" SVN_REV_STR " Tournament Mode Engaged!"

using namespace BWAPI;

//------------------------------------------------- UPDATE -------------------------------------------------
void GameImpl::update()
{
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
      foreach(PlayerImpl *p, this->players)
      {
        if ( p->getIndex() >= 8 )
          continue;
        if ( !p->isDefeated() && !p->isVictorious() && !p->leftGame() )
          allDone = false;
      }
    }
    if ( allDone)
    {
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
  if ( wantSelectionUpdate && memcmp(savedUnitSelection, BW::BWDATA::ClientSelectionGroup, sizeof(savedUnitSelection)) != 0 )
  {
    wantSelectionUpdate = false;
    memcpy(savedUnitSelection, BW::BWDATA::ClientSelectionGroup, sizeof(savedUnitSelection));
    refreshSelectionStates();
  }

  //update players and check to see if they have just left the game.
  _allies.clear();
  _enemies.clear();
  _observers.clear();
  if ( BWAPIPlayer )
  {
    foreach(Player p, players)
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
  for (int i = 0; i < PLAYER_COUNT; ++i)
  {
    bool prevLeftGame = this->players[i]->leftGame();
    this->players[i]->updateData();
    if (!prevLeftGame && this->players[i]->leftGame())
      events.push_back(Event::PlayerLeft((Player)this->players[i]));
  }
  //update properties of Unit and Bullet objects
  this->updateUnits();
  this->updateBullets();

  //iterate through the list of intercepted messages
  foreach(std::string i, sentMessages)
    BroodwarImpl.onSendText(i.c_str());

  //clear all intercepted messages
  this->sentMessages.clear();

  //on the first frame we check to see if the client process has connected.
  //if not, then we load the AI dll specified in bwapi.ini
  if ( !this->startedClient )
  {
    this->initializeTournamentModule();
    this->initializeAIModule();

    //push the MatchStart event to the front of the queue so that it is the first event in the queue.
    events.push_front(Event::MatchStart());
    this->startedClient = true;
  }

  if ( !this->bTournamentMessageAppeared && 
        hTournamentModule && 
        this->frameCount > _currentPlayerId()*8 )
  {
    this->bTournamentMessageAppeared = true;
    this->isTournamentCall = true;
    sendText("%s", TOURNAMENT_STR);
    if ( this->tournamentController )
      this->tournamentController->onFirstAdvertisement();
    this->isTournamentCall = false;
  }

  //each frame we add a MatchFrame event to the queue
  events.push_back(Event::MatchFrame());

  //if the AI is a client process, this will signal the client to process the next frame
  //if the AI is a DLL, this will translate the events into AIModule callbacks.
  server.update();

  // Process events that were created for interface objects
  this->processInterfaceEvents();

  //Before returning control to starcraft, we clear the unit data for units that are no longer accessible
  foreach(UnitImpl* u, evadeUnits)
    u->updateData();

  //We also kill the units that are dying on this frame.
  //We wait until after server.update() and processEvents() to do this so that the AI can
  //access the last frame of unit data during the onUnitDestroy callback.
  foreach(UnitImpl* u, dyingUnits)
  {
    deadUnits.push_back(u);
    int index = u->getIndex();
    unitArray[index] = new UnitImpl(&BW::BWDATA::UnitNodeTable[index], (u16)index);
    u->die();
  }

  /* In case we ever want to add a Flag::UnitPermanence cheat flag...
  bool UnitPermanence = false;
  if ( !UnitPermanence )
  {
    foreach(UnitImpl* u, evadeUnits)
    {
      deadUnits.push_back(u);
      int index = u->getIndex();
      unitArray[index] = new UnitImpl(&BW::BWDATA::UnitNodeTable->unit[index],(u16)index);
      u->die();
    }
  }
  */

  //increment frame count if the game is not paused
  if ( *BW::BWDATA::isGamePaused == 0 )
    this->frameCount++;

  // Set the replay time, this is a workaround to fixing the replay DIVIDE_BY_ZERO exception bug
  if ( !this->isReplay() )
    *BW::BWDATA::ReplayFrames = this->getFrameCount()+20;

  // Check if the window is iconic, if so, go super fast!
  static bool bLastIconic = false;
  if ( !!IsIconic(SDrawGetFrameWindow()) != bLastIconic && !this->isMultiplayer() )
  {
    this->setLocalSpeed(bLastIconic ? -1 : 0);
    this->setFrameSkip(bLastIconic ? 1 : 16);
    bLastIconic = !bLastIconic;
  }

  // Execute commands that have been buffered by the command optimizer
  updateCommandOptimizer();

  // Update any graphical/debug overlays
  updateOverlays();

  // Record the current frame for video recording
  if ( !this->isPaused()  && 
        recordingStarted  && 
        pVidBuffer        && 
        recordingUpdated )
  {
    recordingUpdated = false;
    RecordFrame(wmode ? pVidBuffer : BW::BWDATA::GameScreenBuffer->getData());
  }
  setTextSize(); // Reset text size

  //finally return control to starcraft
}

//------------------------------------------------- STATS -------------------------------------------------
void GameImpl::updateStatistics()
{
  // Compute frame rate
  accumulatedFrames++;
  DWORD currentTickCount = GetTickCount();
  if ( currentTickCount >= lastTickCount + 1000 )
  {
    fps               = accumulatedFrames;
    averageFPS        = averageFPS*0.7+fps*0.3;

    double APMInterval = 0.95;
    int duration = (currentTickCount - lastTickCount);
    int totalDuration = (currentTickCount - startTickCount);
    botAPMCounter_selects     = botAPMCounter_selects * exp(-(duration)/(APMInterval*60000));
    botAPMCounter_noselects   = botAPMCounter_noselects * exp(-(duration)/(APMInterval*60000));
    double gameDurationFactor = 1-exp(-totalDuration/(APMInterval*60000));
    if (gameDurationFactor < 0.01) gameDurationFactor = 0.01; //Prevent division by 0
    botAPM_selects   = (int)floor(botAPMCounter_noselects+botAPMCounter_selects/(APMInterval*gameDurationFactor));
    botAPM_noselects = (int)floor(botAPMCounter_noselects/(APMInterval*gameDurationFactor));

    lastTickCount     = currentTickCount;
    accumulatedFrames = 0;
  }
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
    for ( int y = scrPos.y/32; y < (scrPos.y + BW::BWDATA::GameScreenBuffer->height())/32 + 1; ++y )
    {
      for ( int x = scrPos.x/32; x < (scrPos.x + BW::BWDATA::GameScreenBuffer->width())/32 + 1; ++x )
      {
        for ( int i = 0; i < 32; i += 4 )
        {
          drawLineMap(x*32 + 32, y*32 + i, x*32 + 32, y*32 + i + 2, BWAPI::Colors::Grey);
          drawLineMap(x*32 + i, y*32 + 32, x*32 + i + 2, y*32 + 32, BWAPI::Colors::Grey);
        }
      }
    }
    setTextSize(0);
    drawTextScreen(64, 288, "%c(%u, %u)", Text::White, (scrPos.x+this->getMousePosition().x)/32, (scrPos.y+this->getMousePosition().y)/32);
    setTextSize();
  } // grid

  // fps display
  if ( showfps )
  {
    setTextSize(0);
    drawTextScreen(1,1, "%cFrame Rate (Logical Frames)\nLFPS: %d\nAvg LFPS:%.3lf", BWAPI::Text::Yellow, this->getFPS(), this->getAverageFPS());
    setTextSize();
  } // fps

#ifdef _DEBUG
  setTextSize(0);
  // unitdebug
  if ( unitDebug && BWAPIPlayer )
  {
    foreach ( BWAPI::UnitImpl *u, this->getAllUnits() )
    {
      auto raw = u->getOriginalRawData;
      if ( raw->autoTargetUnit )
        this->drawLineMap(u->getPosition(), BWAPI::Position(raw->autoTargetUnit->position), Colors::Red);
    }
    /*
    BWAPI::Color c = Colors::Red;
    int x = this->getMousePosition().x + this->getScreenPosition().x;
    int y = this->getMousePosition().y + this->getScreenPosition().y;
    if ( BW::isCollidingWithContour((*BW::BWDATA::SAIPathing)->contours, 
                                x,
                                y,
                                UnitTypes::Terran_Marine.dimensionLeft(),
                                UnitTypes::Terran_Marine.dimensionUp(),
                                UnitTypes::Terran_Marine.dimensionRight(),
                                UnitTypes::Terran_Marine.dimensionDown()) )
        c = Colors::Green;
    drawBoxMap(x - UnitTypes::Terran_Marine.dimensionLeft(),
                                y - UnitTypes::Terran_Marine.dimensionUp(),
                                x + UnitTypes::Terran_Marine.dimensionRight(),
                                y + UnitTypes::Terran_Marine.dimensionDown(), c);
    */
  } // unitdebug

  // pathdebug
  if ( pathDebug && *BW::BWDATA::SAIPathing )
  {
    BWAPI::Position mouse   = getMousePosition() + getScreenPosition();
    BW::region *selectedRgn = BW::getRegionAt(mouse);
    int scrx = (getScreenPosition().x/32 - 1)*32;
    int scry = (getScreenPosition().y/32 - 1)*32;
    for ( int x = (scrx > 0 ? scrx : 0); x < getScreenPosition().x + BW::BWDATA::ScreenLayers[5].width && x/32 < this->mapWidth(); x += 32 )
    {
      for ( int y = (scry > 0 ? scry : 0); y < getScreenPosition().y + BW::BWDATA::ScreenLayers[5].height && y/32 < this->mapHeight(); y += 32 )
      {
        BW::TilePosition tp((u16)x/32, (u16)y/32);

        u16 idx = (*BW::BWDATA::SAIPathing)->mapTileRegionId[tp.y][tp.x];
        if ( idx & 0x2000 )
        {
          BW::split *t = &(*BW::BWDATA::SAIPathing)->splitTiles[idx & 0x1FFF];
          for ( int mTileY = 0; mTileY < 4; ++mTileY )
          {
            for ( int mTileX = 0; mTileX < 4; ++mTileX )
            {
              BW::region *rgn1 = getRegionFromId(t->rgn1);
              BWAPI::Color c = selectedRgn == rgn1 ? BWAPI::Colors::Brown : BWAPI::Colors::Grey;
              if ( rgn1->accessabilityFlags == 0x1FFD )
                c = selectedRgn == rgn1 ? BWAPI::Colors::Yellow : BWAPI::Colors::Red;
              if ( ((t->minitileMask >> ( mTileX + mTileY * 4 )) & 1) )
              {
                BW::region *rgn2 = getRegionFromId(t->rgn2);
                c = selectedRgn == rgn2 ? BWAPI::Colors::Brown : BWAPI::Colors::Grey;
                if ( rgn2->accessabilityFlags == 0x1FFD )
                  c = selectedRgn == rgn2 ? BWAPI::Colors::Yellow : BWAPI::Colors::Red;
              }
              drawLineMap(x + mTileX * 8,     y + mTileY * 8, x + mTileX * 8 + 8, y + mTileY * 8 + 8, c);
              drawLineMap(x + mTileX * 8 + 8, y + mTileY * 8, x + mTileX * 8,     y + mTileY * 8 + 8, c);
            } // minitile X
          }// minitile Y
        } // index & 0x2000
        else
        {
          BW::region *r = getRegionFromId(idx);
          BWAPI::Color c = selectedRgn == r ? BWAPI::Colors::Brown : BWAPI::Colors::Grey;
          if ( r->accessabilityFlags == 0x1FFD )
            c = selectedRgn == r ? BWAPI::Colors::Yellow : BWAPI::Colors::Red;
          drawLineMap(x,    y,    x + 32,      y + 32,      c);
          drawLineMap(x+8,  y,    x + 32,      y + 32 - 8,  c);
          drawLineMap(x+16, y,    x + 32,      y + 32 - 16, c);
          drawLineMap(x+24, y,    x + 32,      y + 32 - 24, c);
          drawLineMap(x,    y+16, x + 32 - 16, y + 32,      c);
          drawLineMap(x,    y+8,  x + 32 - 8,  y + 32,      c);
          drawLineMap(x,    y+24, x + 32 - 24, y + 32,      c);
            
          drawLineMap(x,    y+32, x + 32,      y,           c);
          drawLineMap(x,    y+16, x + 32 - 16, y,           c);
          drawLineMap(x,    y+8,  x + 32 - 24, y,           c);
          drawLineMap(x,    y+24, x + 32 - 8,  y,           c);
          drawLineMap(x+16, y+32, x + 32,      y + 32 - 16, c);
          drawLineMap(x+8,  y+32, x + 32,      y + 32 - 24, c);
          drawLineMap(x+24, y+32, x + 32,      y + 32 - 8,  c);
        }
      } // iterate y
    } // iterate x
    for ( unsigned int i = 0; i < (*BW::BWDATA::SAIPathing)->regionCount; ++i )
    {
      BW::region *r = &(*BW::BWDATA::SAIPathing)->regions[i];
      if ( r->accessabilityFlags != 0x1FFD )
        drawBoxMap(r->rgnBox.left, r->rgnBox.top, r->rgnBox.right, r->rgnBox.bottom, r == selectedRgn ? Colors::Cyan : Colors::Purple);

      for ( u8 n = 0; n < r->neighborCount; ++n )
      {
        BW::region *neighbor = r->getNeighbor(n);
        if ( r->accessabilityFlags != 0x1FFD && neighbor->accessabilityFlags != 0x1FFD )
          drawLineMap(r->getCenter(), neighbor->getCenter(), neighbor->groupIndex == r->groupIndex ? Colors::Green : Colors::Red);
      }
      if ( r == selectedRgn )
        drawTextMap(r->getCenter(), "%cTiles: %u\nPaths: %u\nFlags: %u\nGroupID: %u", Text::White, r->tileCount, r->pathCount, r->defencePriority, r->groupIndex);
    }
    for ( int i = 0; i < 4; ++i )
    {
      BW::contourHub *hub = (*BW::BWDATA::SAIPathing)->contours;
      for ( int c = 0; c < hub->contourCount[i]; ++c )
      {
        BW::contour *cont = &hub->contours[i][c];
        bool select = false;
        int l = getScreenPosition().x;
        int r = getScreenPosition().x + BW::BWDATA::ScreenLayers[5].width;
        int t = getScreenPosition().y;
        int b = getScreenPosition().y + BW::BWDATA::ScreenLayers[5].height;

        switch ( cont->type )
        {
        case 0:
        case 2:
          if ( !((cont->v[1] > l || cont->v[2] > l) && (cont->v[1] < r || cont->v[2] < r) && cont->v[0] > t && cont->v[0] < b) )
            continue;
          if ( mouse.x < cont->v[2] && mouse.x > cont->v[1] && mouse.y > cont->v[0] - 4 && mouse.y < cont->v[0] + 4 )
            select = true;
          drawLineMap(cont->v[1], cont->v[0], cont->v[2], cont->v[0], select ? Colors::Cyan : Colors::Green);
          break;
        case 1:
        case 3:
          if ( !((cont->v[1] > t || cont->v[2] > t) && (cont->v[1] < b || cont->v[2] < b) && cont->v[0] > l && cont->v[0] < r) )
            continue;
          if ( mouse.x < cont->v[0] + 4 && mouse.x > cont->v[0] - 4 && mouse.y > cont->v[1] && mouse.y < cont->v[2] )
            select = true;
          drawLineMap(cont->v[0], cont->v[1], cont->v[0], cont->v[2], select ? Colors::Cyan : Colors::Green);
          break;
        default:
          Broodwar << "Default case! " << cont->type << std::endl;
          break;
        }
        if ( select )
        {
          BYTE b = cont->unk_relation;
          unsigned int len = abs(cont->v[2] - cont->v[1]);
          drawTextMouse(32, 0, "%cLength: %u\n%cUnknown: %u (0x%02X) (b%u%u%u%u%u%u%u%u)", Text::White, len, Text::White, b, b, b&0x80 ? 1:0, b&0x40 ? 1:0, b&0x20 ? 1:0, b&0x10 ? 1:0, b&8 ? 1:0, b&4 ? 1:0, b&2 ? 1:0, b&1 ? 1:0);
        }
      }
    }
    /*
    foreach (BWAPI::RegionImpl *r, this->regionsList )
    {
      drawTextMap(r->getCenter(), "%u", r->getRegionGroupID());
        
      std::vector<BWAPI::Position> poly = r->getSimplePolygon();
      BWAPI::Position prev = Positions::None;
      for ( auto j = poly.begin(), jend = poly.end(); j != jend; ++j )
      {
        if ( prev != Positions::None )
          drawLineMap(prev, *j, Colors::Yellow);
        prev = *j;
      }
    }*/
  } // pathdebug
#endif

}

//------------------------------------------------- OPTIMIZER -------------------------------------------------
void GameImpl::updateCommandOptimizer()
{
  // @TODO: Only process on the frame before commands are sent
  //if ( *BW::BWDATA::FramesUntilNextTurn == 1 )
  {
    // Iterate the command types
    for ( int i = 0; i < UnitCommandTypes::Enum::None; ++i )
    {
      // Declare our temporary variables
      Unitset groupOf12;

      Position pos  = Positions::None;
      int       e   = 0;
      Unit t  = nullptr;
      UnitType  ut;
      bool      o   = false;

      // Iterate the list
      auto cmd = commandOptimizer[i].begin();
      
      // Re-Iterate all remaining commands
      while ( cmd != commandOptimizer[i].end() )
      {
        // Iterate all commands, and only process those that are equal
        while ( cmd != commandOptimizer[i].end() )
        {
          // Ignore anything but the command that the unit last processed
          if ( //static_cast<UnitImpl*>(cmd->unit)->lastImmediateCommandFrame == this->getFrameCount() &&
                  static_cast<UnitImpl*>(cmd->unit)->lastImmediateCommand != *cmd )
          {
            cmd = commandOptimizer[i].erase(cmd);
            continue;
          }

          // If we are starting a new command grouping
          if ( groupOf12.empty() )
          {
            // Assign our comparison variables to determine which commands should be grouped
            // Note: Using individual variables instead of comparing UnitCommand operator== because
            //       it will also compare the type which is not necessary, and we may create a new
            //       optimization type that does a fuzzy position comparison
            e = cmd->extra;
            t = cmd->target;
            pos = cmd->getTargetPosition();
            if (  i == UnitCommandTypes::Attack_Unit ||
                  i == UnitCommandTypes::Unload_All  ||
                  i == UnitCommandTypes::Load        ||
                  i == UnitCommandTypes::Cancel_Morph )
              o = cmd->unit->getType().isBuilding();
            else if ( i == UnitCommandTypes::Use_Tech )
              o = cmd->unit->isSieged() || cmd->unit->isCloaked() || cmd->unit->isBurrowed();
            else
              o = false;
            groupOf12.push_back( cmd->unit );
            cmd = commandOptimizer[i].erase(cmd);
          } // otherwise if this command is the same as the first, the units can be grouped
          else if ( e == cmd->extra && t == cmd->target && pos == cmd->getTargetPosition() )
          {
            bool oTmp;
            if (  i == UnitCommandTypes::Attack_Unit ||
                  i == UnitCommandTypes::Unload_All  ||
                  i == UnitCommandTypes::Load        ||
                  i == UnitCommandTypes::Cancel_Morph )
              oTmp = cmd->unit->getType().isBuilding();
            else if ( i == UnitCommandTypes::Use_Tech )
              oTmp = cmd->unit->isSieged() || cmd->unit->isCloaked() || cmd->unit->isBurrowed();
            else
              oTmp = false;

            if ( o == oTmp )
            {
              groupOf12.push_back( cmd->unit );
              cmd = commandOptimizer[i].erase(cmd);
            }
            else
              ++cmd;
          } // otherwise skip this command for now
          else
          {
            ++cmd;
          }

          // If our group of 12 is full
          if ( groupOf12.size() == 12 )
          {
            // Select the group
            BW::Orders::Select sel(groupOf12);
            ++botAPM_select;
            QueueGameCommand(&sel, sel.size);

            // Workaround for doing certain actions
            Unit unit = (i == UnitCommandTypes::Load         || 
                          i == UnitCommandTypes::Attack_Unit  ||
                          i == UnitCommandTypes::Train        ||
                          i == UnitCommandTypes::Unload_All   ||
                          i == UnitCommandTypes::Cancel_Morph ||
                          i == UnitCommandTypes::Use_Tech) ? groupOf12.front() : nullptr;

            // execute command
            executeCommand(UnitCommand(unit, i, t, pos.x, pos.y, e));
            groupOf12.clear();
          } // groupOf12 max execute
        } // second while

        // If we iterated the entire command list and don't have an empty group, then give a command
        // to the remaining units in the group
        if ( !groupOf12.empty() )
        {
          // Select the group
          BW::Orders::Select sel(groupOf12);
          ++botAPM_select;
          QueueGameCommand(&sel, sel.size);

          // Workaround for doing certain actions
          Unit unit = (i == UnitCommandTypes::Load         || 
                        i == UnitCommandTypes::Attack_Unit  ||
                        i == UnitCommandTypes::Train        ||
                        i == UnitCommandTypes::Unload_All   ||
                        i == UnitCommandTypes::Cancel_Morph ||
                        i == UnitCommandTypes::Use_Tech) ? groupOf12.front() : nullptr;

          // execute command
          executeCommand(UnitCommand(unit, i, t, pos.x, pos.y, e));
          groupOf12.clear();
        }
        // Reset iterator
        cmd = commandOptimizer[i].begin();
      } // first while
    } // iterate command types
  } // execute all stored commands
}

//--------------------------------------------- TOURNAMENT MODULE ----------------------------------------------
void GameImpl::initializeTournamentModule()
{
  // Declare typedefs for function pointers
  typedef void (*PFNGameInit)(Game *);
  typedef AIModule* (*PFNCreateA1)();
  typedef TournamentModule* (*PFNCreateTournament)();

  // Initialize Tournament Variables
#ifndef _DEBUG
  // Load tournament string and module if string exists
  std::string TournamentDllPath = LoadConfigString("ai", "tournament");
  if ( TournamentDllPath.size() > 0 )
    hTournamentModule = LoadLibrary(TournamentDllPath.c_str());

  // If tournament module exists
  if ( hTournamentModule )
  {
    // Obtain our tournament functions
    PFNGameInit         newGameInit         = (PFNGameInit)GetProcAddress(hTournamentModule, TEXT("gameInit"));
    PFNCreateA1         newTournamentAI     = (PFNCreateA1)GetProcAddress(hTournamentModule, TEXT("newTournamentAI"));
    PFNCreateTournament newTournamentModule = (PFNCreateTournament)GetProcAddress(hTournamentModule, TEXT("newTournamentModule"));

    // Call the tournament functions if they exist
    if ( newTournamentAI && newTournamentModule && newGameInit )
    {
      newGameInit(this);
      this->tournamentAI         = newTournamentAI();
      this->tournamentController = newTournamentModule();
    }
    else // error when one function is not found
    {
      // Free the tournament module
      FreeLibrary(hTournamentModule);
      hTournamentModule = NULL;

      // Create our error string
      std::string missing;
      if ( !newTournamentAI )
        missing += "newTournamentAI";
          
      if ( !newTournamentModule )
      {
        if ( !missing.empty() )
          missing += " and ";
        missing += "newTournamentModule";
      }
      missing += " function";

      // print error message
      Broodwar << Text::Red << "ERROR: Failed to find the " << missing << " in tournament module." << std::endl;
    }
  }
  this->bTournamentMessageAppeared = false;
#else
  this->bTournamentMessageAppeared = true;
#endif
}

//--------------------------------------------- AI MODULE ----------------------------------------------
void GameImpl::initializeAIModule()
{
  // Declare typedefs for function pointers
  typedef void (*PFNGameInit)(Game *);
  typedef AIModule* (*PFNCreateA1)();

  // Connect to external module if it exists
  externalModuleConnected = false;
  std::string moduleName("<Nothing>");
  if ( server.isConnected() ) //check to see if the server is connected to the client
  {
    // assign a blank AI module to our variable
    this->client = new AIModule();
    // Hide success strings in tournament mode
    if ( !hTournamentModule )
      Broodwar << "BWAPI: Connected to AI Client process" << std::endl;
    // Set the module string
    moduleName = "<Client Connection>";
    externalModuleConnected = true;
  }
  else // if not, load the AI module DLL
  {
    // declare/assign variables
    hAIModule         = nullptr;

    std::string aicfg = LoadConfigString("ai", BUILD_DEBUG ? "ai_dbg" : "ai", "_NULL");
    std::stringstream aiList(aicfg);

    // Get DLL name
    std::string dll = aicfg.substr(0, aicfg.find_first_of(','));

    // Skip to current intended instance
    for ( int i = 0; i < (int)gdwProcNum && aiList; ++i )
      std::getline(aiList, dll, ',');

    // ignore the deprecated revision extension
    size_t tmp = dll.find_first_of(':');
    if ( tmp != std::string::npos )
      dll.erase(tmp);

    // Trim leading and trailing spaces
    while ( isspace(dll.front()) )
      dll.erase(0);
    while ( isspace(dll.back())  )
      dll.pop_back();

    // Check if string was loaded
    if ( aicfg == "_NULL" )
      BWAPIError("Could not find %s under ai in \"%s\".", BUILD_DEBUG ? "ai_dbg" : "ai", configPath.c_str());
    else  // Load DLL
      hAIModule = LoadLibrary(dll.c_str());

    if ( !hAIModule )
    {
      //if hAIModule is nullptr, there there was a problem when trying to load the AI Module
      this->client = new AIModule();

      // enable flags to allow interaction
      Broodwar->enableFlag(Flag::CompleteMapInformation);
      Broodwar->enableFlag(Flag::UserInput);

      // print error string
      Broodwar << Text::Red << "ERROR: Failed to load the AI Module \"" << dll << "\"." << std::endl;
      externalModuleConnected = false;
    }
    else
    {
      // Obtain the AI module function
      PFNGameInit newGame     = (PFNGameInit)GetProcAddress(hAIModule, TEXT("gameInit"));
      PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hAIModule, TEXT("newAIModule"));
      if ( newAIModule && newGame )
      {
        // Call the AI module function and assign the client variable
        newGame(this);
        this->client = newAIModule();

        // Hide success strings in tournament mode
        if ( !hTournamentModule )
          Broodwar << Text::Green << "Loaded the AI Module: " << dll << std::endl;
        externalModuleConnected = true;

        moduleName = dll;

        // Strip the path from the module name
        size_t tmp = moduleName.find_last_of("/\\");
        if ( tmp != std::string::npos )
          moduleName.erase(0, tmp+1);
      }
      else  // If the AIModule function is not found
      {
        // Create a dummy AI module
        this->client = new AIModule();

        // Enable flags to allow interaction
        Broodwar->enableFlag(Flag::CompleteMapInformation);
        Broodwar->enableFlag(Flag::UserInput);

        // Create error string
        std::string missing;
        if ( !newGame )
          missing += "gameInit";
          
        if ( !newAIModule )
        {
          if ( !missing.empty() )
            missing += " and ";
          missing += "newAIModule";
        }
        missing += " function";

        // Print an error message
        Broodwar << Text::Red << "ERROR: Failed to find the " << missing << " in " << dll << std::endl;
        externalModuleConnected = false;
      }
    }
  }

  if ( !hTournamentModule ) // If tournament mode wasn't initialized
    sendText("BWAPI r" SVN_REV_STR " " BUILD_STR " is now live using \"%s\".", moduleName.c_str() );
}

