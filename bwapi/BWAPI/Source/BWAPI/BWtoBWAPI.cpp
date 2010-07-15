#pragma once
#include <BW/OrderID.h>
namespace BWAPI
{
  int BWtoBWAPI_Order[189];
  void BWtoBWAPI_init()
  {
    for(int i=0;i<189;i++)
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
  }
}