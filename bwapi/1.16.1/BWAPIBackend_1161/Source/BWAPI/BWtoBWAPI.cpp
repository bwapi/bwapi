#include "BWtoBWAPI.h"

namespace BWAPI4
{
  using namespace BWAPI;

  int BWtoBWAPI_Order[Orders::Enum::MAX];
  int AttackAnimationRestFrame[UnitTypes::Enum::MAX];
  void BWtoBWAPI_init()
  {
    for(int i = 0; i < Orders::Enum::MAX; ++i)
      BWtoBWAPI_Order[i] = i;

    BWtoBWAPI_Order[Orders::Enum::ReaverStop]  = Orders::Enum::Stop;
    BWtoBWAPI_Order[Orders::Enum::CarrierStop] = Orders::Enum::Stop;

    BWtoBWAPI_Order[Orders::Enum::Attack1]              = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::Attack2]              = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::CarrierAttack]        = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::CarrierMoveToAttack]  = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::CarrierFight]         = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::ReaverAttack]         = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::ReaverMoveToAttack]   = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::ReaverFight]          = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::StayInRange]          = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::SuicideUnit]          = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::WatchTarget]          = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::TowerAttack]          = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::TurretAttack]         = Orders::Enum::AttackUnit;
    BWtoBWAPI_Order[Orders::Enum::AttackFixedRange]     = Orders::Enum::AttackUnit;

    BWtoBWAPI_Order[Orders::Enum::CarrierHoldPosition] = Orders::Enum::HoldPosition;
    BWtoBWAPI_Order[Orders::Enum::QueenHoldPosition]   = Orders::Enum::HoldPosition;
    BWtoBWAPI_Order[Orders::Enum::SuicideHoldPosition] = Orders::Enum::HoldPosition;
    BWtoBWAPI_Order[Orders::Enum::MedicHoldPosition]   = Orders::Enum::HoldPosition;
    BWtoBWAPI_Order[Orders::Enum::ReaverHoldPosition]  = Orders::Enum::HoldPosition;

    BWtoBWAPI_Order[Orders::Enum::PlaceProtossBuilding] = Orders::Enum::PlaceBuilding;
    BWtoBWAPI_Order[Orders::Enum::DroneStartBuild]      = Orders::Enum::PlaceBuilding;
    BWtoBWAPI_Order[Orders::Enum::DroneLand]            = Orders::Enum::PlaceBuilding;

    BWtoBWAPI_Order[Orders::Enum::DroneBuild]         = Orders::Enum::IncompleteBuilding;
    BWtoBWAPI_Order[Orders::Enum::IncompleteMorphing] = Orders::Enum::IncompleteBuilding;
    BWtoBWAPI_Order[Orders::Enum::IncompleteWarping]  = Orders::Enum::IncompleteBuilding;

    BWtoBWAPI_Order[Orders::Enum::SuicideLocation]        = Orders::Enum::AttackMove;
    BWtoBWAPI_Order[Orders::Enum::MoveToRepair]           = Orders::Enum::Repair;
    BWtoBWAPI_Order[Orders::Enum::MoveToFireYamatoGun]    = Orders::Enum::FireYamatoGun;
    BWtoBWAPI_Order[Orders::Enum::InitializeArbiter]      = Orders::Enum::PlayerGuard;
    BWtoBWAPI_Order[Orders::Enum::InitializePsiProvider]  = Orders::Enum::Nothing;
    BWtoBWAPI_Order[Orders::Enum::MoveToInfest]           = Orders::Enum::CastInfestation;
    BWtoBWAPI_Order[Orders::Enum::ResetHarvestCollision]  = Orders::Enum::ResetCollision;
    BWtoBWAPI_Order[Orders::Enum::NukePaint]              = Orders::Enum::CastNuclearStrike;

    for(int i = 0; i < UnitTypes::Enum::MAX; ++i)
      AttackAnimationRestFrame[i] = 10000;

    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Marine]             = 34;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Ghost]              = 34;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Vulture]            = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Goliath]            = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Siege_Tank_Siege_Mode] = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Siege_Tank_Tank_Mode]  = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_SCV]                = 17;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Wraith]             = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Science_Vessel]      = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Battlecruiser]      = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Firebat]            = 0;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Valkyrie]           = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Terran_Missile_Turret]      = 10000;

    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Corsair]           = 0;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Dark_Templar]       = 153;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Probe]             = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Zealot]            = 0;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Dragoon]           = 357;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Archon]            = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Scout]             = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Arbiter]           = 0;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Carrier]           = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Interceptor]       = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Reaver]            = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Scarab]            = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Protoss_Photon_Cannon]      = 10000;

    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Zergling]             = 85;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Hydralisk]            = 51;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Ultralisk]            = 153;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Broodling]            = 17;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Drone]                = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Lurker]               = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Mutalisk]             = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Guardian]             = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Devourer]             = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Spore_Colony]          = 10000;
    AttackAnimationRestFrame[UnitTypes::Enum::Zerg_Sunken_Colony]         = 33;
  }
}
