#include <windows.h>
#include <Shlwapi.h>
#include <vector>
#include "ExampleTournamentModule.h"
using namespace BWAPI;

bool leader = false;

char szBotName[MAX_PATH];
char *pszBotName;

bool isJoiner = false;

class coolevent
{
public:
  bool operator ==(const coolevent &evt) const
  {
    return pos      == evt.pos      &&
           unit     == evt.unit     &&
           priority == evt.priority &&
           timer    == evt.timer;
  }
  bool operator !=(const coolevent &evt) const
  {
    return !(*this == evt);
  }
  Position  pos;
  Unit      *unit;
  int       priority;
  int       timer;
};

static const coolevent NoEvent = { Positions::None, NULL, 0, 0 };

std::vector<coolevent> events;
coolevent currentEvent;

Player *self;

void AddEvent(Position pos, int priority, int timer = 25, const char *pszDebug = NULL);
void AddEvent(Unit *unit, int priority, int timer = 25);
void RemoveEvent(Unit *unit);
void ClearEvent(const coolevent &evt);
void RunEvents();

void AddEvent(Position pos, int priority, int timer, const char *pszDebug)
{
  if ( !pos )
    return;

  if ( pszDebug )
    Broodwar->printf("EVENT: %s at priority %d for %d frames", pszDebug, priority, timer);

  pos -= Position(320, 200);
  pos.makeValid();

  if ( pos.getApproxDistance(currentEvent.pos) <= 200 )
  {
    currentEvent.priority += priority;
    currentEvent.timer    += timer;
    if ( currentEvent.timer > 250 )
      currentEvent.timer = 250;
    return;
  }

  for ( std::vector<coolevent>::iterator i = events.begin(); i != events.end(); ++i )
  {
    if ( pos.getApproxDistance(i->pos) < 600 )
    {
      if ( pos.getApproxDistance(i->pos) > 200 )
      {
        pos         += i->pos + pos;
        i->pos      = Position( pos.x()/3, pos.y()/3 );
        i->pos.makeValid();
      }

      i->priority += priority;
      i->timer    += timer;
      if ( i->timer > 250 )
        i->timer = 250;
      return;
    }
  }
  coolevent evt = { pos, NULL, priority, timer };
  events.push_back(evt);
}

void AddEvent(Unit *unit, int priority, int timer)
{
  if ( !unit )
    return;

  for ( std::vector<coolevent>::iterator i = events.begin(); i != events.end(); ++i )
  {
    if ( i->unit == unit )
    {
      i->priority += priority;
      i->timer    += timer;
      if ( i->timer > 250 )
        i->timer = 250;
      return;
    }
  }
  // if unit not found
  coolevent evt = { Positions::None, unit, priority, timer };
  events.push_back(evt);
}

void RemoveEvent(Unit *unit)
{
  if ( !unit )
    return;

  for ( std::vector<coolevent>::iterator i = events.begin(); i != events.end(); ++i )
  {
    if ( i->unit == unit )
    {
      events.erase(i);
      break;
    }
  }
  if ( currentEvent.unit == unit )
    currentEvent.timer = 0;
}

void ClearEvent(const coolevent &evt)
{
  if ( evt == NoEvent )
    return;
  for ( std::vector<coolevent>::iterator i = events.begin(); i != events.end(); ++i )
  {
    if ( *i == evt )
    {
      events.erase(i);
      break;
    }
  }
}

void RunEvents()
{
  currentEvent.timer--;
  if ( currentEvent.timer <= 0 )  // assign new event
  {
    coolevent best = NoEvent;
    std::vector<coolevent>::iterator i = events.begin();
    while ( i != events.end() )
    {
      i->timer--;
      if ( i->timer <= 0 )
      {
        i = events.erase(i);
      }
      else
      {
        if ( (best == NoEvent || i->priority > best.priority) && i->timer > 25 )
          best = *i;
        ++i;
      }
    }

    if ( best != NoEvent && best.timer > 25 )
    {
      currentEvent = best;
      ClearEvent(best);
    }
  }

  if ( currentEvent.unit && currentEvent.unit->exists() && currentEvent.unit->getPosition() )
  {
    Broodwar->setScreenPosition(currentEvent.unit->getPosition() - Position(320, 200));
  }
  else if ( currentEvent.pos )
  {
    if ( Broodwar->getUnitsInRectangle(currentEvent.pos - Position(320,200), currentEvent.pos + Position(320,200)).empty() )
      currentEvent.timer = 0;
    else
      Broodwar->setScreenPosition(currentEvent.pos);
  }
  else
  {
    Broodwar->printf("\x06" "Bad event");
  }

}

void ExampleTournamentAI::onStart()
{
  // Set the command optimization level (reduces high APM, size of bloated replays, etc)
  Broodwar->setCommandOptimizationLevel(MINIMUM_COMMAND_OPTIMIZATION);
  Broodwar->enableFlag(Flag::UserInput);

  char szTmp[8] = { };
  GetPrivateProfileString("auto_menu", "map", "", szTmp, 8, "C:\\Starcraft\\bwapi-data\\bwapi.ini");
  if ( strlen(szTmp) == 0 )
    isJoiner = true;

  GetPrivateProfileString("ai", "ai", "UNKNOWN", szBotName, MAX_PATH, "C:\\Starcraft\\bwapi-data\\bwapi.ini");
  pszBotName = szBotName;
  // Move to last backslash
  char *pszTmp = strrchr(pszBotName, '\\');
  if ( pszTmp )
    pszBotName = &pszTmp[1];

  // Move to last forward slash
  pszTmp = strrchr(pszBotName, '/');
  if ( pszTmp )
    pszBotName = &pszTmp[1];

  // eliminate underscore
  pszTmp = strchr(pszBotName, '_');
  if ( pszTmp )
    *pszTmp = 0;

  // eliminate extension
  pszTmp = strchr(pszBotName, '.');
  if ( pszTmp )
    *pszTmp = 0;

  self = Broodwar->self();

  Broodwar->setLocalSpeed(0);
  Broodwar->setFrameSkip(64);
  AddEvent(Position(self->getStartLocation()) + Position(64,48), 100, 100);
}

int maxAPM;
void ExampleTournamentAI::onEnd(bool isWinner)
{
  // save maxAPM or something
}

void ExampleTournamentAI::onFrame()
{
  if ( Broodwar->getFrameCount() < 64 )
    return;

  /*if ( Broodwar->enemy()->allUnitCount(UnitTypes::AllUnits) > 0 )
    Broodwar->setLocalSpeed(30);
  else
    Broodwar->setLocalSpeed(0);*/

  int thisAPM = Broodwar->getAPM();
  if ( thisAPM > maxAPM )
    maxAPM = thisAPM;

  char hcolor = 2;
  if ( maxAPM < 500 )
    hcolor = 7;
  else if ( maxAPM < 1500 )
    hcolor = 4;
  else if ( maxAPM < 2500 )
    hcolor = 3;
  else
    hcolor = 6;

  char color = 2;
  if ( thisAPM < 500 )
    color = 7;
  else if ( thisAPM < 1500 )
    color = 4;
  else if ( thisAPM < 2500 )
    color = 3;
  else
    color = 6;
  
  Broodwar->setTextSize(3);
  Broodwar->drawTextScreen(4, 4, "\x13" "\x07" "%s", pszBotName);
  Broodwar->setTextSize();

  Broodwar->drawTextScreen(2, 2, "\x04" "%s\n" "\x07" "APM: %c%d\n" "\x07" "Highest APM: %c%d", Broodwar->mapName().c_str(), color, thisAPM, hcolor, maxAPM);
  // draw supplies
  if ( isJoiner )
    Broodwar->drawTextScreen(560, 40, "\x07" "Supply: %d/%d", Broodwar->self()->supplyUsed()/2, Broodwar->self()->supplyTotal()/2);

  // iterate self units for event identification
  for ( std::set<Unit*>::const_iterator i = self->getUnits().begin(),
        iend = self->getUnits().end();
        i != iend;
        ++i )
  {
    Unit *u = *i;

    if ( !u->exists() )
      continue;

    if ( u->isStartingAttack() )
      AddEvent(u->getPosition(), 20, 30);//, "starting attack");

    if ( u->isUnderAttack() )
      AddEvent(u->getPosition(), 20, 50);//, "under attack");
/*
    if ( u->isBeingHealed() )
      AddEvent(u->getPosition(), 2, 5, "being healed");

    if ( u->isRepairing() )
      AddEvent(u->getPosition(), 1, 3);//, "repairing");
*/
    if ( u->isCompleted() && !u->getType().isBuilding() && !u->isInvincible() && u->isUnderStorm() )
      AddEvent(u->getPosition(), 100, 50);//, "under psi storm");

    if ( u->isCompleted() && u->getType().maxHitPoints() && (u->getHitPoints()*100)/u->getType().maxHitPoints() <= 2 )
    {
      std::set<Unit*> urad = u->getUnitsInRadius(512);
      bool enemyInRad = false;
      for ( std::set<Unit*>::iterator ur = urad.begin(),
            urend = urad.end();
            ur != urend;
            ++ur )
      {
        if ( (*ur)->exists() && (self->isEnemy((*ur)->getPlayer()) || (*ur)->getPlayer()->isEnemy(self)) )
        {
          enemyInRad = true;
          break;
        }
      }
      if ( !enemyInRad )
        AddEvent(u->getPosition(), 20, 30);//, "really low hp");
    }
/*
    if ( u->getKillCount() != 0 && u->getKillCount() % 50 == 0 )
      AddEvent(u->getPosition(), 100, 20, "massive kill count");
*/

    Order o = u->getOrder();

    // states
    if ( o == Orders::BuildingLiftOff ||
         o == Orders::BuildingLand    ||
         o == Orders::Burrowing       ||
         o == Orders::Unburrowing     ||
         o == Orders::Sieging         ||
         o == Orders::Unsieging       ||
         o == Orders::OpenDoor        ||
         o == Orders::CloseDoor       ||
         o == Orders::Cloak           ||
         o == Orders::Decloak )
      AddEvent(u->getPosition(), 5, 15);//, o.c_str());

    // abilities
    if (  o == Orders::CastConsume            ||
          o == Orders::CastDarkSwarm          ||
          o == Orders::CastDefensiveMatrix    ||
          o == Orders::CastDisruptionWeb      ||
          o == Orders::CastEMPShockwave       ||
          o == Orders::CastEnsnare            ||
          o == Orders::CastFeedback           ||
          o == Orders::CastHallucination      ||
          o == Orders::CastInfestation        ||
          o == Orders::CastIrradiate          ||
          o == Orders::CastLockdown           ||
          o == Orders::CastMaelstrom          ||
          o == Orders::CastMindControl        ||
          o == Orders::CastOpticalFlare       ||
          o == Orders::CastParasite           ||
          o == Orders::CastPlague             ||
          o == Orders::CastPsionicStorm       ||
          o == Orders::CastRecall             ||
          o == Orders::CastRestoration        ||
          o == Orders::CastSpawnBroodlings    ||
          o == Orders::CastStasisField        ||
          o == Orders::FireYamatoGun          ||
          o == Orders::RechargeShieldsUnit )
    {
      if ( u->getTarget() )
        AddEvent(u->getTarget()->getPosition(), 30, 50);//, o.c_str());
      else
        AddEvent(u->getOrderTargetPosition(), 30, 50);
    }
    // scanner
    if ( o == Orders::CastScannerSweep && 
          u->getLastCommand().getType() == UnitCommandTypes::Use_Tech_Position && 
          u->getLastCommand().getTechType() == TechTypes::Scanner_Sweep )
      AddEvent(u->getLastCommand().getTargetPosition(), 100, 50);

    // more abilities
    if ( o == Orders::PlaceMine )
      AddEvent(u->getPosition(), 2, 10);

    // other
    if (  o == Orders::EnterNydusCanal  ||
          //o == Orders::EnterTransport ||
          o == Orders::GuardianAspect   ||
          o == Orders::MoveUnload       ||
          o == Orders::PickupBunker     ||
          o == Orders::PickupTransport  ||
          o == Orders::PlaceAddon       ||
          o == Orders::SelfDestrucing   ||
          o == Orders::Unload           ||
          o == Orders::ZergBirth )
      AddEvent(u->getPosition(), 50, 40);//, o.c_str());
  }
  RunEvents();
}

void ExampleTournamentAI::onSendText(std::string text)
{
  if ( text == "/freeze" )
  {
    for (;;)
    {
      Sleep(1000);
    }
  }

  const char *pszTxt = text.c_str();
  if ( strstr(pszTxt, "TRNSPD") )
  {
    int spd = 0;
    sscanf(pszTxt, "TRNSPD %d", &spd);
    Broodwar->setLocalSpeed(spd);
    Broodwar->sendText("TRNSPD %d", spd);
    Broodwar->sendText("Set speed to %d", spd);
  }
  else if ( strstr(pszTxt, "RACES") )
  {
    for ( std::set<Player*>::iterator r = Broodwar->getPlayers().begin(), rend = Broodwar->getPlayers().end(); r != rend; ++r )
      Broodwar->printf("%s: %s", (*r)->getName().c_str(), (*r)->getRace().c_str());
  }
}

void ExampleTournamentAI::onReceiveText(BWAPI::Player* player, std::string text)
{
  const char *pszTxt = text.c_str();
  if ( strstr(pszTxt, "TRNSPD") )
  {
    int spd = 0;
    sscanf(pszTxt, "TRNSPD %d", &spd);
    Broodwar->setLocalSpeed(spd);
    Broodwar->sendText("Set speed to %d", spd);
  }
}

void ExampleTournamentAI::onPlayerLeft(BWAPI::Player* player)
{
  Broodwar->printf("%s left", player->getName().c_str());
}

void ExampleTournamentAI::onPlayerDropped(BWAPI::Player* player)
{
  Broodwar->printf("%s DROPPED", player->getName().c_str());
}

void ExampleTournamentAI::onNukeDetect(BWAPI::Position target)
{
  AddEvent(target, 1000, 100);
}

void ExampleTournamentAI::onUnitDiscover(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitEvade(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitShow(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitHide(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onUnitCreate(BWAPI::Unit* unit)
{
  if ( unit->getPlayer() == self && unit->getType().isBuilding() )
    AddEvent(unit->getPosition(), unit->getType().buildScore()/10);
}

void ExampleTournamentAI::onUnitDestroy(BWAPI::Unit* unit)
{
  if ( unit->getPlayer() == self )
  {
    RemoveEvent(unit);
    AddEvent(unit->getPosition(), unit->getType().destroyScore()/10);
  }
}

void ExampleTournamentAI::onUnitMorph(BWAPI::Unit* unit)
{
  if ( unit->getPlayer() == self )
    AddEvent(unit->getPosition(), unit->getType().buildScore()/10);
}

void ExampleTournamentAI::onUnitComplete(BWAPI::Unit *unit)
{
  if ( unit->getPlayer() == self )
    AddEvent(unit->getPosition(), unit->getType().buildScore()/8, 40);
}

void ExampleTournamentAI::onUnitRenegade(BWAPI::Unit* unit)
{
}

void ExampleTournamentAI::onSaveGame(std::string gameName)
{
}

bool ExampleTournamentModule::onAction(int actionType, void *parameter)
{
  switch ( actionType )
  {
  case Tournament::EnableFlag:
    switch ( *(int*)parameter )
    {
    case Flag::CompleteMapInformation:
    case Flag::UserInput:
      // Disallow these two flags
      return false;
    }
    // Allow other flags if we add more that don't affect gameplay specifically
    return true;
  case Tournament::PauseGame:
  case Tournament::RestartGame:
  case Tournament::ResumeGame:
  case Tournament::SetFrameSkip:
  case Tournament::SetGUI:
  case Tournament::SetLocalSpeed:
  case Tournament::SetMap:
    return false; // Disallow these actions
  case Tournament::LeaveGame:
  case Tournament::ChangeRace:
  case Tournament::SetLatCom:
  case Tournament::SetTextSize:
  case Tournament::SendText:
  case Tournament::Printf:
    return true; // Allow these actions
  case Tournament::SetCommandOptimizationLevel:
    return *(int*)parameter > MINIMUM_COMMAND_OPTIMIZATION; // Set a minimum command optimization level 
                                                            // to reduce APM with no action loss
  default:
    break;
  }
  return true;
}

void ExampleTournamentModule::onFirstAdvertisement()
{
  leader = true;
  Broodwar->sendText("Welcome to " TOURNAMENT_NAME "!");
  Broodwar->sendText("Brought to you by " SPONSORS ".");
}
