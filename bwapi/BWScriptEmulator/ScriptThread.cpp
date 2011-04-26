#include "ScriptThread.h"
#include "ScriptEnum.h"
#include "BWScriptEmulator.h"

#include "Controller.h"
#include "Starcraft.h"

#include <BWAPI.h>
#include <deque>

using namespace BWAPI;

std::deque<aithread> aiThreadList;

BYTE *pbAIScriptBinary;

#define AIS_ENTRY_REQUIRES_LOCATION   0x01
#define AIS_ENTRY_STAREDIT_INVISIBLE  0x02
#define AIS_ENTRY_REQUIRES_BROODWAR   0x04

int   creep         = 0;
DWORD AISavedCall   = 0;

struct ais_entry
{
  DWORD dwScriptID;
  DWORD dwOffset;
  DWORD dwStringIndex;
  DWORD dwFlags;
};

void UpdateScripts()
{
  std::deque<aithread>::iterator i = aiThreadList.begin();
  while ( i != aiThreadList.end() )
  {
    i->showDebug(0, 0);
    if ( !i->sleep() )    // wait/sleep
    {
      if ( i->getFlags() & AI_THREAD_WANT_KILLED )
      {
        i = aiThreadList.erase(i);
        continue;
      }      

      // execute thread
      i->execute();
    }
    ++i;
  }
}

bool LoadAIBinary(const char *pszFileName)
{
  HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  if ( hFile == INVALID_HANDLE_VALUE )
    return false;

  DWORD dwFileSize = GetFileSize(hFile, NULL);
  if ( dwFileSize <= 10 )
  {
    CloseHandle(hFile);
    return false;
  }
  pbAIScriptBinary = (BYTE*)malloc(dwFileSize);
  if ( !pbAIScriptBinary )
  {
    CloseHandle(hFile);
    return false;
  }
  memset(pbAIScriptBinary, 0, dwFileSize);

  DWORD dwBytesRead = 0;
  ReadFile(hFile, pbAIScriptBinary, dwFileSize, &dwBytesRead, NULL);
  CloseHandle(hFile);
  return true;
}

int threadCount = 0;
void AICreateThread(WORD wStartBlock, Position location, void *pTown)
{
  aiThreadList.push_back(aithread(wStartBlock, location, pTown));
}

void AICreateThread(char *pszScriptID, Position location)
{
  DWORD dwScriptID = *(DWORD*)pszScriptID;
  if ( !pbAIScriptBinary )
    return;

  ais_entry *entryList = (ais_entry*)(pbAIScriptBinary + *(DWORD*)pbAIScriptBinary);
  DWORD dwScriptOffset = 0;
  for ( int i = 0; entryList[i].dwScriptID != 0 && dwScriptOffset == 0; ++i )
  {
    if ( dwScriptID == entryList[i].dwScriptID )
    {
      dwScriptOffset = entryList[i].dwOffset;
      break;
    }
  }

  if ( dwScriptOffset )
  {
    AICreateThread((WORD)dwScriptOffset, location);
    Broodwar->sendText("Beginning AI script execution: %s", pszScriptID);
  }
  else
    Broodwar->sendText("Unable to find script: %s", pszScriptID);
}

aithread::aithread()
:dwScriptOffset(0)
,dwSleepTime(0)
,dwPlayerID( (Broodwar->self()->getID()) )
,locationCenter(BWAPI::Positions::None)
,pTown(NULL)
,dwFlags(0)
,threadId(threadCount)
,dwBytesRead(0)
{
  this->locationBounds.left   = 0;
  this->locationBounds.right  = 0;
  this->locationBounds.top    = 0;
  this->locationBounds.bottom = 0;
  memset(this->bTotBuildCount, 0, sizeof(this->bTotBuildCount));
  this->debugQueue.clear();
  ++threadCount;
}

aithread::aithread(WORD wStartBlock, BWAPI::Position location, void *town)
:dwScriptOffset(wStartBlock)
,dwSleepTime(0)
,dwPlayerID(Broodwar->self()->getID())
,locationCenter(location)
,pTown(town)
,dwFlags(0)
,threadId(threadCount)
,dwBytesRead(0)
{
  this->locationBounds.left   = location.x() - 128;
  this->locationBounds.right  = location.x() + 128;
  this->locationBounds.top    = location.y() - 128;
  this->locationBounds.bottom = location.y() + 128;
  memset(this->bTotBuildCount, 0, sizeof(this->bTotBuildCount));
  this->debugQueue.clear();
  ++threadCount;
}

aithread::~aithread()
{
  for each ( char *c in this->debugQueue )
    delete c;
  this->debugQueue.clear();
  --threadCount;
}

int GetTerranRushScore(Player *player)
{
  int bunkerScore = 4 * player->completedUnitCount(UnitTypes::Terran_Bunker);
  int marineScore = player->completedUnitCount(UnitTypes::Terran_Marine);
  if ( bunkerScore <= marineScore )
    return marineScore + bunkerScore;
  else
    return 2 * marineScore;
}

int GetZergGroundRushScore(Player *player)
{
  return player->completedUnitCount(UnitTypes::Zerg_Hydralisk) + 
     2 * player->completedUnitCount(UnitTypes::Zerg_Sunken_Colony);
}

int GetZergAirRushScore(Player *player)
{
  return player->completedUnitCount(UnitTypes::Zerg_Hydralisk) + 
    player->completedUnitCount(UnitTypes::Zerg_Mutalisk) + 
     2 * player->completedUnitCount(UnitTypes::Zerg_Spore_Colony);
}

int GetProtossAirRushScore(Player *player)
{
  return player->completedUnitCount(UnitTypes::Protoss_Dragoon) + 
         player->completedUnitCount(UnitTypes::Protoss_Scout);
}

int GetStandardUnitCount(UnitType type, Player *player, bool bCompleted)
{
  int count = 0;
  Player *pl = (player ? player : self);
  if ( !pl )
    return 0;
  if ( bCompleted )
  {
    count = pl->completedUnitCount(type);
    if ( type == UnitTypes::Terran_Siege_Tank_Siege_Mode )
      count += pl->completedUnitCount(UnitTypes::Terran_Siege_Tank_Tank_Mode);
    else if ( type == UnitTypes::Terran_Siege_Tank_Tank_Mode )
      count += pl->completedUnitCount(UnitTypes::Terran_Siege_Tank_Siege_Mode);
  }
  else
  {
    count = pl->allUnitCount(type);
    if ( type == UnitTypes::Terran_Siege_Tank_Siege_Mode )
      count += pl->allUnitCount(UnitTypes::Terran_Siege_Tank_Tank_Mode);
    else if ( type == UnitTypes::Terran_Siege_Tank_Tank_Mode )
      count += pl->allUnitCount(UnitTypes::Terran_Siege_Tank_Siege_Mode);
  }
  return count;
}

void aithread::execute()
{
  this->dwBytesRead = 0;
  if ( !pbAIScriptBinary )
  {
    Broodwar->printf("%cNo binary loaded!", 6);
    return this->killThread();
  }
  if ( this->locationCenter == BWAPI::Positions::None )
  {
    Broodwar->printf("%cInvalid location!", 6);
    return this->killThread();
  }
  for (;;)
  {
    if ( this->dwScriptOffset == 0 )
    {
      Broodwar->printf("%cScript offset is 0 !", 6);
      return this->killThread();
    }

    this->dwBytesRead = 0;
    BYTE bOpcode = read<BYTE>();
    switch ( bOpcode )
    {
    case AISCRIPT::GOTO:  // COMPLETE
      this->dwScriptOffset = this->read<WORD>();
      this->saveDebug(bOpcode, "p_%X", this->dwScriptOffset);
      continue;
    case AISCRIPT::NOTOWNS_JUMP:  // COMPLETE
      {
        UnitType uType    = UnitType(this->read<WORD>());
        WORD     jOffset  = this->read<WORD>();
        this->saveDebug(bOpcode, "%s p_%X", AISCRIPT::getUnitName(uType), jOffset);
        if ( self->allUnitCount(uType) == 0 )
          this->dwScriptOffset = jOffset;
        continue;
      }
    case AISCRIPT::WAIT:  // COMPLETE
      this->dwSleepTime = this->read<WORD>();
      this->saveDebug(bOpcode, "%3u", this->dwSleepTime);
      return;
    case AISCRIPT::START_TOWN:      // not started
      this->saveDebug(bOpcode);
      MainController->wFlags |= CONTROLLER_TOWN_STARTED;
      // townStart stuff
      continue;
    case AISCRIPT::START_AREATOWN:  // not started
      this->saveDebug(bOpcode);
      MainController->wFlags |= CONTROLLER_TOWN_STARTED;
      // townStart stuff
      continue;
    case AISCRIPT::EXPAND:          // not started
      {
        BYTE bExpandID    = this->read<BYTE>();
        WORD wExpandBlock = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u p_%X", bExpandID, wExpandBlock);
        //ai expand
        continue;
      }
    case AISCRIPT::BUILD: // 
      {
        BYTE bBuildCount    = this->read<BYTE>();
        UnitType wBuildType = UnitType(this->read<WORD>());
        BYTE bPriority      = this->read<BYTE>();

        this->saveDebug(bOpcode, "%3u %s %3u", bBuildCount, AISCRIPT::getUnitName(wBuildType), bPriority);
        bTotBuildCount[wBuildType.getID()] = bBuildCount;

        //
        continue;
      }
    case AISCRIPT::UPGRADE:
      {
        BYTE bUpgLevel = this->read<BYTE>();
        UpgradeType wUpgType  = UpgradeType(this->read<WORD>());
        BYTE bPriority = this->read<BYTE>();
        this->saveDebug(bOpcode, "%3u %s %3u", bUpgLevel, wUpgType.getName().c_str(), bPriority);
        //        
        continue;
      }
    case AISCRIPT::TECH:
      {
        TechType wTechType = TechType(this->read<WORD>());
        BYTE bPriority = this->read<BYTE>();
        this->saveDebug(bOpcode, "%s %3u", wTechType.getName().c_str(), bPriority);
        //
        continue;
      }
    case AISCRIPT::WAIT_BUILD: // incomplete
    case AISCRIPT::WAIT_BUILDSTART: // incomplete
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        if ( wType == self->getRace().getWorker() )
        {
          // TOWN STUFF
        }
/*
        if ( // AI_BuildSomething >= bCount )
        {
          this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
          continue;
        }
        */
        this->retry();
        return;
      }
    case AISCRIPT::ATTACK_CLEAR:  // unfinished
      this->saveDebug(bOpcode);
      memset(MainController->attackGroups, 0, sizeof(MainController->attackGroups));
      MainController->wAttackGroup = 0;
      MainController->dwAttackTime = 0;
      continue;
    case AISCRIPT::ATTACK_ADD:  // COMPLETED
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->AttackAdd(bCount, wType);
        continue;
      }
    case AISCRIPT::ATTACK_PREPARE:  // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::ATTACK_DO:  // not started
      // if ( AISomeAttackManager )
      {
        this->saveDebug(bOpcode);
        continue;
      }
      // this->retry();
      // return;
    case AISCRIPT::WAIT_SECURE:  // not started
      if ( MainController->wFlags & CONTROLLER_SECURE_FINISHED )
      {
        this->saveDebug(bOpcode);
        continue;
      }
      this->retry();
      return;
    case AISCRIPT::CAPT_EXPAND:  // not started
      // Stands for capture expansion, run in default expansion block, set by what???
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::BUILD_BUNKERS:  // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::WAIT_BUNKERS:  // not started
      // if ( AIWaitBunkersFinished )
      {
        this->saveDebug(bOpcode);
        continue;
      }
      //this->retry();
      //return;
    case AISCRIPT::DEFENSEBUILD_GG:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseBuild(DEFENSE_GROUND_TO_GROUND, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEBUILD_AG:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseBuild(DEFENSE_AIR_TO_GROUND, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEBUILD_GA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseBuild(DEFENSE_GROUND_TO_AIR, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEBUILD_AA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseBuild(DEFENSE_AIR_TO_AIR, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEUSE_GG:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseUse(DEFENSE_GROUND_TO_GROUND, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEUSE_AG:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseUse(DEFENSE_AIR_TO_GROUND, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEUSE_GA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseUse(DEFENSE_GROUND_TO_AIR, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSEUSE_AA:  // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        MainController->DefenseUse(DEFENSE_AIR_TO_AIR, bCount, wType);
        continue;
      }
    case AISCRIPT::DEFENSECLEAR_GG:  // COMPLETED
      this->saveDebug(bOpcode);
      MainController->DefenseClear(DEFENSE_GROUND_TO_GROUND);
      continue;
    case AISCRIPT::DEFENSECLEAR_AG:  // COMPLETED
      this->saveDebug(bOpcode);
      MainController->DefenseClear(DEFENSE_AIR_TO_GROUND);
      continue;
    case AISCRIPT::DEFENSECLEAR_GA:  // COMPLETED
      this->saveDebug(bOpcode);
      MainController->DefenseClear(DEFENSE_GROUND_TO_AIR);
      continue;
    case AISCRIPT::DEFENSECLEAR_AA:  // COMPLETED
      this->saveDebug(bOpcode);
      MainController->DefenseClear(DEFENSE_AIR_TO_AIR);
      continue;
    case AISCRIPT::SEND_SUICIDE:  // not started
      {
        BYTE bSuicideType = this->read<BYTE>();
        this->saveDebug(bOpcode, "%3u", bSuicideType);
        memset(MainController->attackGroups, 0, sizeof(MainController->attackGroups));
        MainController->wAttackGroup = 0;
        MainController->dwAttackTime = 0;
        continue;
      }
    case AISCRIPT::PLAYER_ENEMY:  // not started
      this->saveDebug(bOpcode);
      
      continue;
    case AISCRIPT::PLAYER_ALLY:  // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::DEFAULT_MIN:  // COMPLETED
      MainController->bDefaultMin = this->read<BYTE>();
      this->saveDebug(bOpcode, "%3u", MainController->bDefaultMin);
      continue;
    case AISCRIPT::DEFAULTBUILD_OFF:  // not started
      this->saveDebug(bOpcode);
      MainController->wFlags |= CONTROLLER_DEFAULT_BUILD_OFF;
      continue;
    case AISCRIPT::STOP:    // COMPLETE
      this->saveDebug(bOpcode);
      this->killThread();
      return;
    case AISCRIPT::SWITCH_RESCUE:  // COMPLETED (can't actually emulate)
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::MOVE_DT: // 
      this->saveDebug(bOpcode);
      for each ( Unit *u in self->getUnits() )
      {
        UnitType ut = u->getType();
        if ( (ut == UnitTypes::Protoss_Dark_Templar || ut == UnitTypes::Hero_Dark_Templar) &&
             u->exists() && u->isCompleted() )
        {
          //special re-assignment
        }
      }
      continue;
    case AISCRIPT::DEBUG:   // COMPLETE
      {
        WORD wJump = this->read<WORD>();
        Broodwar->sendText("%s", pbAIScriptBinary[this->dwScriptOffset]);
        this->saveDebug(bOpcode, "p_%X %s", wJump, pbAIScriptBinary[this->dwScriptOffset]);
        this->dwScriptOffset = wJump;
        continue;
      }
    case AISCRIPT::FATAL_ERROR: // COMPLETED
      Broodwar->sendText("Illegal AI script executed.");
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::ENTER_BUNKER: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::VALUE_AREA: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::TRANSPORTS_OFF: // COMPLETE
      this->saveDebug(bOpcode);
      MainController->wFlags |= CONTROLLER_TRANSPORTS_OFF;
      continue;
    case AISCRIPT::CHECK_TRANSPORTS: // COMPLETE
      this->saveDebug(bOpcode);
      MainController->wFlags &= ~CONTROLLER_TRANSPORTS_OFF;
      continue;
    case AISCRIPT::NUKE_RATE: // COMPLETE
      MainController->bNukeRate = this->read<BYTE>();
      this->saveDebug(bOpcode, "%3u", MainController->bNukeRate);
      continue;
    case AISCRIPT::MAX_FORCE: // COMPLETED
      MainController->wMaxForce = UnitType(this->read<WORD>());
      this->saveDebug(bOpcode, "%s", AISCRIPT::getUnitName(MainController->wMaxForce) );
      continue;
    case AISCRIPT::CLEAR_COMBATDATA: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::RANDOM_JUMP: // completed?
      {
        BYTE bRandChance = this->read<BYTE>();
        WORD wJmpOffset = this->read<WORD>();
        
        this->saveDebug(bOpcode, "%3u p_%X", bRandChance, wJmpOffset);
        if ( LOBYTE(Random()) <= bRandChance )
          this->dwScriptOffset = wJmpOffset;
        continue;
      }
    case AISCRIPT::TIME_JUMP: // completed?
      {
        BYTE bTimePast  = 60 * this->read<BYTE>();
        WORD wJmpOffset = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u p_%X", bTimePast, wJmpOffset);
        if ( (Broodwar->getFrameCount() / 24) >= bTimePast )
          this->dwScriptOffset = wJmpOffset;
        continue;
      }
    case AISCRIPT::FARMS_NOTIMING: // completed?
      this->saveDebug(bOpcode);
      MainController->wFlags |= CONTROLLER_FARMS_TIMING_OFF;
      continue;
    case AISCRIPT::FARMS_TIMING: // completed?
      this->saveDebug(bOpcode);
      MainController->wFlags &= ~CONTROLLER_FARMS_TIMING_OFF;
      continue;
    case AISCRIPT::BUILD_TURRETS: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::WAIT_TURRETS: // not started
      // if ( AIWaitTurretsFinished )
      {
        this->saveDebug(bOpcode);
        continue;
      }
      //this->retry();
      //return;
    case AISCRIPT::DEFAULT_BUILD: // not started
      this->saveDebug(bOpcode);
      MainController->wFlags &= ~CONTROLLER_DEFAULT_BUILD_OFF;
      continue;
    case AISCRIPT::HARASS_FACTOR: // not started
      {
        WORD wUnk = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u", wUnk);
        continue;
      }
    case AISCRIPT::START_CAMPAIGN: // not started
      this->saveDebug(bOpcode);
      MainController->wFlags |= CONTROLLER_IS_CAMPAIGN;
      continue;
    case AISCRIPT::RACE_JUMP: // completed?
      {
        WORD t_jmp = this->read<WORD>();
        WORD z_jmp = this->read<WORD>();
        WORD p_jmp = this->read<WORD>();
        
        this->saveDebug(bOpcode, "p_%X p_%X p_%X", t_jmp, z_jmp, p_jmp);

        // should be AI_getClosestEnemyRace; gets closest enemy unit to region
        Race race = Races::None;
        if ( bw->enemy() )
          race = bw->enemy()->getRace();
        if ( race == Races::Protoss )
          this->dwScriptOffset = p_jmp;
        else if ( race == Races::Zerg )
          this->dwScriptOffset = z_jmp;
        else
          this->dwScriptOffset = t_jmp;
        continue;
      }
    case AISCRIPT::REGION_SIZE: // not started
      {
        BYTE bSize  = this->read<BYTE>();
        WORD wBlock = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u p_%X", bSize, wBlock);
        // unknown region calculations
        continue;
      }
    case AISCRIPT::GET_OLDPEONS: // not started
      {
        BYTE bCount = this->read<BYTE>();
        this->saveDebug(bOpcode, "%3u", bCount);
        // getOldPeons
        continue;
      }
    case AISCRIPT::GROUNDMAP_JUMP: // COMPLETED
      {
        WORD wJmpOffset = this->read<WORD>();
        this->saveDebug(bOpcode, "p_%X", wJmpOffset);
        for each ( Unit *u in bw->getAllUnits() )
        {
          if (  u->exists() &&
                self->isEnemy(u->getPlayer()) &&
                u->getType().isBuilding() &&
                !u->isLifted() &&
                u->hasPath(this->locationCenter) )
          {
            this->dwScriptOffset = wJmpOffset;
            break;
          }
        }
        continue;
      }
    case AISCRIPT::PLACE_GUARD: // not started
      {
        UnitType wType = UnitType(this->read<WORD>());
        BYTE bIndex    = this->read<BYTE>();
        this->saveDebug(bOpcode, "%s %3u", AISCRIPT::getUnitName(wType), bIndex);
        continue;
      }
    case AISCRIPT::WAIT_FORCE:  //
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        
        if ( GetStandardUnitCount(wType, Broodwar->self()) < bCount )
        {
          MainController->wWaitForType = wType + 1;
          this->retry();
          return;
        }
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType) );
        continue;
      }
    case AISCRIPT::GUARD_RESOURCES: // not started
      {
        UnitType wType = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%s", AISCRIPT::getUnitName(wType));
        continue;
      }
    case AISCRIPT::CALL:    // COMPLETE
      AISavedCall           = this->dwScriptOffset;
      this->dwScriptOffset  = this->read<WORD>();
      this->saveDebug(bOpcode, "p_%X", this->dwScriptOffset);
      continue;
    case AISCRIPT::RETURN:    // COMPLETE
      this->saveDebug(bOpcode);
      this->dwScriptOffset = AISavedCall;
      AISavedCall = 0;
      continue;
    case AISCRIPT::EVAL_HARASS: // not started
      {
        WORD wBlock = this->read<WORD>();
        this->saveDebug(bOpcode, "p_%X", wBlock);
        //if ( !EvalHarass )
        //  this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::CREEP:   // COMPLETE
      {
        BYTE bCreepFlag = this->read<BYTE>();
        this->saveDebug(bOpcode, "%3u", bCreepFlag);
        if ( bCreepFlag == 4 )
          MainController->wFlags |= CONTROLLER_SPREAD_CREEP;
        else
          MainController->wFlags &= ~CONTROLLER_SPREAD_CREEP;
        continue;
      }
    case AISCRIPT::PANIC: // COMPLETED
      MainController->wPanicBlock = this->read<WORD>();
      this->saveDebug(bOpcode, "p_%X", MainController->wPanicBlock);
      continue;
    case AISCRIPT::PLAYER_NEED: // 
      {
        BYTE bBuildCount    = this->read<BYTE>();
        UnitType wBuildType = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bBuildCount, AISCRIPT::getUnitName(wBuildType));

        continue;
      }
    case AISCRIPT::DO_MORPH:  // 
      {
        BYTE bCount     = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        if ( GetStandardUnitCount(wType, Broodwar->self()) < bCount )
          MainController->wWaitForType = wType + 1;

        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        continue;
      }
    case AISCRIPT::WAIT_UPGRADES: // not started
      if ( MainController->wFlags & CONTROLLER_UPGRADES_FINISHED )
      {
        this->saveDebug(bOpcode);
        continue;
      }
      this->retry();
      return;
    case AISCRIPT::MULTIRUN:    // COMPLETE
      {
        WORD wMultirunBlock = this->read<WORD>();
        this->saveDebug(bOpcode, "p_%X", wMultirunBlock);
        AICreateThread(wMultirunBlock, this->locationCenter, this->pTown);
        continue;
      }
    case AISCRIPT::RUSH:  // completed ?
      {
        BYTE bRushCategory = this->read<BYTE>();
        WORD wRushBlock    = this->read<WORD>();

        this->saveDebug(bOpcode, "%3u p_%X", bRushCategory, wRushBlock);
        bool isRushed = false;
        
        int bestDistance = 99999999;
        Player *closestEnemy = NULL;
        for each (Unit *u in Broodwar->getAllUnits())
        {
          if ( u->exists() && 
              self->isEnemy(u->getPlayer()) && 
              !u->getPlayer()->isNeutral() && 
              (!u->getType().isBuilding() || u->isLifted()) )
          {
            int thisDistance = u->getPosition().getApproxDistance(this->locationCenter);
            if ( thisDistance <= 320 && closestEnemy == u->getPlayer() )
              break;

            if ( thisDistance <= bestDistance )
            {
              bestDistance = thisDistance;
              closestEnemy = u->getPlayer();
            }
          } // reqs
        } // iterate units

        if ( !closestEnemy )
          continue;

        switch ( bRushCategory )
        {
        case 0:
          if ( closestEnemy->allUnitCount(UnitTypes::Terran_Barracks)    ||
               closestEnemy->allUnitCount(UnitTypes::Protoss_Gateway)    ||
               closestEnemy->allUnitCount(UnitTypes::Zerg_Spawning_Pool) )
            isRushed = true;
          break;
        case 1:
          if ( GetTerranRushScore(closestEnemy)     > 16 ||
               GetZergGroundRushScore(closestEnemy) > 10 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 6 )
            isRushed = true;
          break;
        case 2:
          if ( GetTerranRushScore(closestEnemy)  > 24 ||
               GetZergAirRushScore(closestEnemy) > 10 )
            isRushed = true;
          break;
        case 3:
          if ( GetTerranRushScore(closestEnemy)     > 5 ||
               closestEnemy->completedUnitCount(UnitTypes::Zerg_Hydralisk_Den) ||
               GetZergGroundRushScore(closestEnemy) > 2 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 1 )
            isRushed = true;
          break;
        case 4:
          if ( GetTerranRushScore(closestEnemy)     > 16 ||
               GetZergGroundRushScore(closestEnemy) > 10 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 8 )
            isRushed = true;
          break;
        case 5:
          if ( GetTerranRushScore(closestEnemy)     > 6 ||
               GetZergGroundRushScore(closestEnemy) > 6 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 3 )
            isRushed = true;
          break;
        case 6:
          if ( GetTerranRushScore(closestEnemy) > 12 ||
               closestEnemy->completedUnitCount(UnitTypes::Zerg_Sunken_Colony) > 1 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Dragoon)    > 1 )
            isRushed = true;
          break;
        case 7:
          if ( closestEnemy->completedUnitCount(UnitTypes::Terran_Siege_Tank_Tank_Mode) > 0 ||
               closestEnemy->completedUnitCount(UnitTypes::Zerg_Queen) ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 6 )
            isRushed = true;
          break;
        case 8:
          if ( GetTerranRushScore(closestEnemy)     > 5 ||
               GetZergGroundRushScore(closestEnemy) > 2 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 1 )
            isRushed = true;
          break;
        case 9:
          if ( GetTerranRushScore(closestEnemy)     > 9 ||
               GetZergGroundRushScore(closestEnemy) > 4 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 5 )
            isRushed = true;
          break;
        case 10:
          if ( GetTerranRushScore(closestEnemy)     > 4 ||
               GetZergGroundRushScore(closestEnemy) > 4 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 2 )
            isRushed = true;
          break;
        case 11:
          if ( GetTerranRushScore(closestEnemy)     > 10 ||
               GetZergGroundRushScore(closestEnemy) > 10 ||
               closestEnemy->completedUnitCount(UnitTypes::Protoss_Zealot) > 5 )
            isRushed = true;
          break;
        case 12:
          if ( GetTerranRushScore(closestEnemy)     > 16  ||
               GetZergAirRushScore(closestEnemy)    > 5   ||
               GetProtossAirRushScore(closestEnemy) > 2 )
            isRushed = true;
          break;
        case 13:
          if ( GetTerranRushScore(closestEnemy)     > 24 ||
               GetZergAirRushScore(closestEnemy)    > 10 ||
               GetProtossAirRushScore(closestEnemy) > 7 )
            isRushed = true;
          break;
        default:
          isRushed = false;
          break;
        }
        if ( isRushed )
          this->dwScriptOffset = wRushBlock;
        continue;
      }
    case AISCRIPT::SCOUT_WITH:  // COMPLETE (this function does nothing in Starcraft)
      {
        UnitType wScout = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%s", AISCRIPT::getUnitName(wScout));
        continue;
      }
    case AISCRIPT::DEFINE_MAX: // COMPLETED
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        if ( wType < UnitTypes::None )
          MainController->bDefineMax[wType] = bCount;
        continue;
      }
    case AISCRIPT::TRAIN: // 
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        if ( GetStandardUnitCount(wType, Broodwar->self(), false) < bCount )
        {
          MainController->wWaitForType = wType + 1;
          this->retry();
          return;
        }
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        continue;
      }
    case AISCRIPT::TARGET_EXPANSION: // COMPLETE
      this->saveDebug(bOpcode);
      MainController->wFlags |= CONTROLLER_TARGET_EXPANSION;
      continue;
    case AISCRIPT::WAIT_TRAIN: // FINISHED!
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wType  = UnitType(this->read<WORD>());
        if ( GetStandardUnitCount(wType) < bCount )
        {
          this->retry();
          return;
        }
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wType));
        continue;
      }
    case AISCRIPT::SET_ATTACKS: // COMPLETE
      MainController->bAttacks = this->read<BYTE>();
      this->saveDebug(bOpcode, "%3u", MainController->bAttacks);
      continue;
    case AISCRIPT::SET_GENCMD: // COMPLETED
      this->saveDebug(bOpcode);
      MainController->genCmdTarget.left   = this->locationBounds.left;
      MainController->genCmdTarget.top    = this->locationBounds.top;
      MainController->genCmdTarget.right  = this->locationBounds.right;
      MainController->genCmdTarget.bottom = this->locationBounds.bottom;
      continue;
    case AISCRIPT::MAKE_PATROL: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::GIVE_MONEY:  // COMPLETE
      this->saveDebug(bOpcode);
      if ( Broodwar->self()->minerals() < 500 )
      {
        Broodwar->sendText("whats mine is mine");
        Broodwar->sendText("whats mine is mine");
        Broodwar->sendText("whats mine is mine");
        Broodwar->sendText("whats mine is mine");
      }
      if ( Broodwar->self()->gas() < 500 )
      {
        Broodwar->sendText("breathe deep");
        Broodwar->sendText("breathe deep");
        Broodwar->sendText("breathe deep");
        Broodwar->sendText("breathe deep");
      }
      continue;
    case AISCRIPT::PREP_DOWN: // not started
      {
        BYTE bSaveCount    = this->read<BYTE>();
        BYTE bMinimum      = this->read<BYTE>();
        UnitType wMilitary = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %3u %s", bSaveCount, bMinimum, AISCRIPT::getUnitName(wMilitary));

        DWORD dwCountToAdd = GetStandardUnitCount(wMilitary, self, false) - bSaveCount;
        if ( dwCountToAdd < bMinimum )
          dwCountToAdd = bMinimum;
        // AI_AttackAdd
        continue;
      }
    case AISCRIPT::RESOURCES_JUMP:  // COMPLETE
      {
        WORD wOre = this->read<WORD>();
        WORD wGas = this->read<WORD>();
        WORD wJmp = this->read<WORD>();
        if ( Broodwar->self()->minerals() >= wOre &&
             Broodwar->self()->gas()      >= wGas )
          this->dwScriptOffset = wJmp;
        this->saveDebug(bOpcode, "%3u %3u p_%X", wOre, wGas, wJmp);
        continue;
      }
    case AISCRIPT::ENTER_TRANSPORT: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::EXIT_TRANSPORT: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::SHAREDVISION_ON: // not started
      {
        BYTE bPlayer = this->read<BYTE>();
        this->saveDebug(bOpcode, "%3u", bPlayer);
        continue;
      }
    case AISCRIPT::SHAREDVISION_OFF: // not started
      {
        BYTE bPlayer = this->read<BYTE>();
        this->saveDebug(bOpcode, "%3u", bPlayer);
        continue;
      }
    case AISCRIPT::NUKE_LOCATION: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::HARASS_LOCATION: // FINISHED!
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::IMPLODE: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::GUARD_ALL: // not started
      this->saveDebug(bOpcode);
      for each ( Unit *u in self->getUnits() )
      {
        // MakeGuard
      }
      continue;
    case AISCRIPT::ENEMYOWNS_JUMP:  // COMPLETED
      {
        UnitType wType = UnitType(this->read<WORD>());
        WORD wJump     = this->read<WORD>();
        this->saveDebug(bOpcode, "%s p_%X", AISCRIPT::getUnitName(wType), wJump);
        for each ( Unit *u in bw->getAllUnits() )
        {
          if ( u->getType() != wType || !self->isEnemy(u->getPlayer()) )
            continue;
          this->dwScriptOffset = wJump;
          break;
        }
        continue;
      }
    case AISCRIPT::ENEMYRESOURCES_JUMP:  // completed ?
      {
        WORD wOre = this->read<WORD>();
        WORD wGas = this->read<WORD>();
        WORD wJmp = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u %3u p_%X", wOre, wGas, wJmp);
        Player *plr = NULL;
        for each (Unit *u in bw->getAllUnits())
        {
          if ( self->isEnemy(u->getPlayer()) )
          {
            plr = u->getPlayer();
            break;
          }
        }
        if ( plr                     &&
             plr->minerals() >= wOre &&
             plr->gas()      >= wGas )
          this->dwScriptOffset = wJmp;
        continue;
      }
    case AISCRIPT::IF_DIF:  // COMPLETE
      {
        BYTE bModifier = this->read<BYTE>();
        BYTE bVal      = this->read<BYTE>();
        WORD wBlock    = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u %3u p_%X", bModifier, bVal, wBlock);
        BYTE bSaved = MainController->bIfDif;
        if ( bModifier ? (bSaved > bVal) : (bSaved < bVal) )
          this->dwScriptOffset = wBlock;
        continue;
      }
    case AISCRIPT::EASY_ATTACK: // not started 
      {
        BYTE     bCount = this->read<BYTE>();
        UnitType wUnit  = UnitType(this->read<WORD>());
        this->saveDebug(bOpcode, "%3u %s", bCount, AISCRIPT::getUnitName(wUnit));
        if ( !MainController->bIfDif )
        {
          // AI_AttackAdd 
        }
        continue;
      }
    case AISCRIPT::KILL_THREAD:  // COMPLETE
      {
        for ( std::deque<aithread>::iterator i = aiThreadList.begin(); i != aiThreadList.end(); ++i )
        {
          if ( i->getFlags() & AI_THREAD_KILLABLE )
            i->killThread();
        }
        this->saveDebug(bOpcode);
        continue;
      }
    case AISCRIPT::KILLABLE:  // COMPLETE
      this->saveDebug(bOpcode);
      this->setFlags(AI_THREAD_KILLABLE);
      continue;
    case AISCRIPT::WAIT_FINISHATTACK: // not started
      // if ( AIIsAttacking )
      {
        this->saveDebug(bOpcode);
        continue;
      }
      // this->retry();
      // return;
    case AISCRIPT::QUICK_ATTACK:  // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::JUNKYARD_DOG:  // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::FAKE_NUKE:   // not started
      this->saveDebug(bOpcode);
      //MainController->dwLastNukeTime = elapsedTime + 1;
      continue;
    case AISCRIPT::DISRUPTION_WEB:  // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::RECALL_LOCATION: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::SET_RANDOMSEED:  // COMPLETED
      dwRandomSeed = this->read<DWORD>();
      this->saveDebug(bOpcode, "%X", dwRandomSeed);
      continue;
    case AISCRIPT::IF_OWNED:  // COMPLETED
      {
        UnitType wType = UnitType(this->read<WORD>());
        WORD wJump     = this->read<WORD>();
        this->saveDebug(bOpcode, "%s p_%X", AISCRIPT::getUnitName(wType), wJump);
        if ( self->allUnitCount(wType) )
          this->dwScriptOffset = wJump;
        continue;
      }
    case AISCRIPT::CREATE_NUKE: // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::CREATE_UNIT: // not started
      {
        UnitType wType  = UnitType(this->read<WORD>()); // type
        WORD wX         = this->read<WORD>(); // x
        WORD wY         = this->read<WORD>(); // y
        this->saveDebug(bOpcode, "%s %3u %3u", AISCRIPT::getUnitName(wType), wX, wY);
        continue;
      }
    case AISCRIPT::NUKE_POS:  // not started
      {
        WORD wX         = this->read<WORD>(); // x
        WORD wY         = this->read<WORD>(); // y
        this->saveDebug(bOpcode, "%3u %3u", wX, wY);
        continue;
      }
    case AISCRIPT::HELP_IFTROUBLE:  // not started
      this->saveDebug(bOpcode);
      continue;
    case AISCRIPT::ALLIES_WATCH: // not started
      {
        BYTE bUnk1  = this->read<BYTE>();
        WORD wBlock = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u p_%X", bUnk1, wBlock);
        continue;
      }
    case AISCRIPT::TRY_TOWNPOINT: // not started
      {
        BYTE bUnk1  = this->read<BYTE>();
        WORD wBlock = this->read<WORD>();
        this->saveDebug(bOpcode, "%3u p_%X", bUnk1, wBlock);
        continue;
      }
    default:
      Broodwar->printf("%cBad script opcode %02X", 6, bOpcode);
      
      return;
    } // switch
  } // loop
}

DWORD aithread::getFlags()
{
  return this->dwFlags;
}

void aithread::setFlags(DWORD dwFlags)
{
  this->dwFlags |= dwFlags;
}

void aithread::clearFlags(DWORD dwFlags)
{
  this->dwFlags &= ~dwFlags;
}

DWORD aithread::sleep()
{
  DWORD rval = this->dwSleepTime;
  if ( rval )
    this->dwSleepTime = rval - 1;
  return rval;
}

void aithread::killThread()
{
  this->setFlags(AI_THREAD_WANT_KILLED);
}

void aithread::showDebug(int x, int y)
{
  int _y = y;
  int _x = x + (threadCount)*200 + 4;
  bw->setTextSize(0);
  for each ( char *s in this->debugQueue )
  {
    char n[512];
    strcpy(n, s);
    
    char *mid = strchr(n, ' ');
    if ( mid )
    {
      *mid = 0;
      ++mid;
    }
    
    bw->drawTextScreen(_x, _y, "%s", n);
    if ( mid )
      bw->drawTextScreen(_x+100, _y, "%s", mid);
    _y += 9;
  }
  bw->setTextSize();
}

void aithread::saveDebug(int iOpcode, const char *pszFormat, ...)
{
  char szFinalBuffer[512];
  char szBuffer[256];

  szFinalBuffer[0] = 0;
  szBuffer[0]      = 0;

  if ( pszFormat )
  {
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf_s(szBuffer, 256, 256, pszFormat, ap);
    va_end(ap);
  }
  sprintf_s(szFinalBuffer, 512, "%s %s", AISCRIPT::getOpcodeName(iOpcode), szBuffer);

  char *tmp = new char[strlen(szFinalBuffer)+1];
  strcpy(tmp, szFinalBuffer);

  if ( debugQueue.size() > 40 )
  {
    char *front = this->debugQueue.front();
    if ( front )
      delete front;
    this->debugQueue.pop_front();
  }
  this->debugQueue.push_back(tmp);
}
void aithread::retry()
{
  this->dwScriptOffset -= this->dwBytesRead;
  this->dwSleepTime     = 30;
  this->dwBytesRead     = 0;
}
