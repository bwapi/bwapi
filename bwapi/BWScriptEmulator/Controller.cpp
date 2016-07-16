#include "Controller.h"
#include "BWScriptEmulator.h"

AIController MainController;

AIController::AIController()
{
  memset(this, 0, sizeof(AIController));
}

AIController::~AIController()
{
}

bool AIController::IsCampaign() const
{
  return !!(this->wFlags & CONTROLLER_IS_CAMPAIGN);
}

int AIController::GetCastingCooldown() const
{
  return this->castingCooldown;
}
void AIController::SetCastingCooldown(int time)
{
  this->castingCooldown = time;
}

void AIController::AttackAdd(int count, BWAPI::UnitType type)
{
  if ( count <= 0 || count >= countof(this->attackGroups) || type >= BWAPI::UnitTypes::None )
    return;

  int u = 0;
  while ( u < countof(this->attackGroups) && this->attackGroups[u] )
    ++u;
  for ( int c = 0; c < count && u + c < countof(this->attackGroups); ++c )
    this->attackGroups[u + c] = (int)type + 1;
}

void AIController::DefenseClear(int type)
{
  if ( type < 0 || type > 3 )
    return;
  memset(defensebuild[type], 0, sizeof(defensebuild[0]));
  memset(defenseuse[type],   0, sizeof(defenseuse[0]));
}

void AIController::DefenseBuild(int type, int count, BWAPI::UnitType uType)
{
  if ( type < 0   || 
       type > 3   ||
       count < 0  || 
       count > countof(this->defensebuild[0]) || uType >= BWAPI::UnitTypes::None )
    return;
  int u = 0;
  while ( u < countof(this->defensebuild[0]) && this->defensebuild[type][u] )
    ++u;
  for ( int c = 0; c < count && u + c < countof(this->defensebuild[0]); ++c )
    this->defensebuild[type][u + c] = (int)uType + 1;
}
void AIController::DefenseUse(int type, int count, BWAPI::UnitType uType)
{
  if ( type < 0 || type > 3 || count < 0 || count > countof(this->defenseuse[0]) || uType >= BWAPI::UnitTypes::None )
    return;
  int u = 0;
  while ( u < countof(this->defenseuse[0]) && this->defenseuse[type][u] )
    ++u;
  for ( int c = 0; c < count && u + c < countof(this->defenseuse[0]); ++c )
    this->defenseuse[type][u + c] = (int)uType + 1;
}

WORD AIController::getFlags() const
{
  return this->wFlags;
}
void AIController::setFlags(WORD wFlags)
{
  this->wFlags |= wFlags;
}
void AIController::clearFlags(WORD wFlags)
{
  this->wFlags &= ~wFlags;
}
