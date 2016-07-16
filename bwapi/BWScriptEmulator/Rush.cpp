#include "Rush.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Rush);


int GetTerranRushScore(Player player)
{
  int bunkerScore = 4 * player->completedUnitCount(UnitTypes::Terran_Bunker);
  int marineScore = player->completedUnitCount(UnitTypes::Terran_Marine);
  if ( bunkerScore <= marineScore )
    return marineScore + bunkerScore;
  else
    return 2 * marineScore;
}

int GetZergGroundRushScore(Player player)
{
  return player->completedUnitCount(UnitTypes::Zerg_Hydralisk) + 
     2 * player->completedUnitCount(UnitTypes::Zerg_Sunken_Colony);
}

int GetZergAirRushScore(Player player)
{
  return player->completedUnitCount(UnitTypes::Zerg_Hydralisk) + 
    player->completedUnitCount(UnitTypes::Zerg_Mutalisk) + 
     2 * player->completedUnitCount(UnitTypes::Zerg_Spore_Colony);
}

int GetProtossAirRushScore(Player player)
{
  return player->completedUnitCount(UnitTypes::Protoss_Dragoon) + 
         player->completedUnitCount(UnitTypes::Protoss_Scout);
}


bool Rush::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bRushCategory;
  WORD wOffset;
  thread.readTuple( std::tie(bRushCategory, wOffset) );

  // Debug
  thread.saveDebug(Text::Yellow, this->getOpcode(), "%u p_%04X", bRushCategory, wOffset);
  bool isRushed = false;

  // Get closest enemy owner
  Unit pClosest = Broodwar->getClosestUnit(thread.getLocation().center(), IsEnemy && (!IsBuilding || IsLifted) );
  Player closestEnemy = pClosest ? pClosest->getPlayer() : nullptr;
  if ( !closestEnemy )  return true;

  // Check rush categories
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
  }

  // Jump to offset if rushed
  if ( isRushed )
    thread.setScriptOffset( wOffset );

  return true;
}
