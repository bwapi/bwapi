#pragma once
#include <BW/OrderID.h>
#include <BW/UnitID.h>

#include "../../Debug.h"

namespace BWAPI
{
  int BWtoBWAPI_Order[191];
  int AttackAnimationRestFrame[234];
  void BWtoBWAPI_init()
  {
    for(int i=0;i<191;i++)
    {
      BWtoBWAPI_Order[i]=i;
    }
    BWtoBWAPI_Order[BW::OrderID::ReaverStop]  = BW::OrderID::Stop;
    BWtoBWAPI_Order[BW::OrderID::CarrierStop] = BW::OrderID::Stop;

    BWtoBWAPI_Order[BW::OrderID::Attack1]          = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::Attack2]          = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::CarrierAttack1]   = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::CarrierAttack2]   = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::CarrierFight]     = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::ReaverAttack1]    = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::ReaverAttack2]    = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::ReaverFight]      = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::StayinRange]      = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::SapUnit]          = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::WatchTarget]      = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::TowerAttack]      = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::TurretAttack]     = BW::OrderID::AttackUnit;
    BWtoBWAPI_Order[BW::OrderID::AttackFixedRange] = BW::OrderID::AttackUnit;

    BWtoBWAPI_Order[BW::OrderID::CarrierHoldPosition] = BW::OrderID::HoldPosition;
    BWtoBWAPI_Order[BW::OrderID::QueenHoldPosition]   = BW::OrderID::HoldPosition;
    BWtoBWAPI_Order[BW::OrderID::SuicideHoldPosition] = BW::OrderID::HoldPosition;
    BWtoBWAPI_Order[BW::OrderID::MedicHoldPosition]   = BW::OrderID::HoldPosition;
    BWtoBWAPI_Order[BW::OrderID::ReaverHoldPosition]  = BW::OrderID::HoldPosition;

    BWtoBWAPI_Order[BW::OrderID::BuildProtoss1]   = BW::OrderID::BuildTerran;
    BWtoBWAPI_Order[BW::OrderID::DroneStartBuild] = BW::OrderID::BuildTerran;
    BWtoBWAPI_Order[BW::OrderID::DroneLand]       = BW::OrderID::BuildTerran;

    BWtoBWAPI_Order[BW::OrderID::DroneBuild]      = BW::OrderID::TerranBuildSelf;
    BWtoBWAPI_Order[BW::OrderID::ZergBuildSelf]   = BW::OrderID::TerranBuildSelf;
    BWtoBWAPI_Order[BW::OrderID::ProtossBuildSelf]= BW::OrderID::TerranBuildSelf;

    BWtoBWAPI_Order[BW::OrderID::SapLocation]     = BW::OrderID::AttackMove;
    BWtoBWAPI_Order[BW::OrderID::Repair2]         = BW::OrderID::Repair1;
    BWtoBWAPI_Order[BW::OrderID::FireYamatoGun2]  = BW::OrderID::FireYamatoGun1;
    BWtoBWAPI_Order[BW::OrderID::InitArbiter]     = BW::OrderID::PlayerGuard;
    BWtoBWAPI_Order[BW::OrderID::InitPsiProvider] = BW::OrderID::Nothing;
    BWtoBWAPI_Order[BW::OrderID::InfestMine3]     = BW::OrderID::InfestMine2;
    BWtoBWAPI_Order[BW::OrderID::ResetCollision2] = BW::OrderID::ResetCollision1;
    BWtoBWAPI_Order[BW::OrderID::NukePaint]       = BW::OrderID::NukeGround;


    for(int i=0;i<234;i++)
    {
      AttackAnimationRestFrame[i]=10000;
    }
    AttackAnimationRestFrame[BW::UnitID::Terran_Marine] = 34;
    AttackAnimationRestFrame[BW::UnitID::Terran_Ghost] = 34;
    AttackAnimationRestFrame[BW::UnitID::Terran_Vulture] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_Goliath] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_SiegeTankSiegeMode] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_SiegeTankTankMode] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_SCV] = 17;
    AttackAnimationRestFrame[BW::UnitID::Terran_Wraith] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_ScienceVessel] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_Battlecruiser] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_Firebat] = 0;
    AttackAnimationRestFrame[BW::UnitID::Terran_Valkyrie] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Terran_MissileTurret] = 10000;

    AttackAnimationRestFrame[BW::UnitID::Protoss_Corsair] = 0;
    AttackAnimationRestFrame[BW::UnitID::Protoss_DarkTemplar] = 153;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Probe] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Zealot] = 0;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Dragoon] = 357;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Archon] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Scout] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Arbiter] = 0;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Carrier] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Interceptor] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Reaver] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Protoss_Scarab] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Protoss_PhotonCannon] = 10000;

    AttackAnimationRestFrame[BW::UnitID::Zerg_Zergling] = 85;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Hydralisk] = 51;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Ultralisk] = 153;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Broodling] = 17;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Drone] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Lurker] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Mutalisk] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Guardian] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Zerg_Devourer] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Zerg_SporeColony] = 10000;
    AttackAnimationRestFrame[BW::UnitID::Zerg_SunkenColony] = 33;
  }
}