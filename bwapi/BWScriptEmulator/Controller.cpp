#include "Controller.h"
#include "BWScriptEmulator.h"

AIController *MainController;

AIController::AIController()
{
  memset(this, 0, sizeof(AIController));
}

AIController::~AIController()
{

}

void AIController::AttackAdd(int count, BWAPI::UnitType type)
{
  if ( count <= 0 || count >= 64 || type >= BWAPI::UnitTypes::None )
    return;

  int u = 0;
  for ( ; this->attackGroups[u] && u < 64; ++u )
  {}
  for ( int c = 0; c < count && u + c < 64; ++c )
    this->attackGroups[u + c] = type + 1;
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
  if ( type < 0 || type > 3 || count < 0 || count > 10 || uType >= BWAPI::UnitTypes::None )
    return;
  int u = 0;
  for ( ; this->defensebuild[type][u] && u < 20; ++u )
  {}
  for ( int c = 0; c < count && u + c < 20; ++c )
    this->defensebuild[type][u + c] = uType + 1;
}
void AIController::DefenseUse(int type, int count, BWAPI::UnitType uType)
{
  if ( type < 0 || type > 3 || count < 0 || count > 10 || uType >= BWAPI::UnitTypes::None )
    return;
  int u = 0;
  for ( ; this->defenseuse[type][u] && u < 20; ++u )
  {}
  for ( int c = 0; c < count && u + c < 20; ++c )
    this->defenseuse[type][u + c] = uType + 1;
}
