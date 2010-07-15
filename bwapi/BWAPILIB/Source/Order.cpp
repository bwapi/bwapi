#include <string>
#include <map>
#include <set>
#include <BWAPI/Order.h>

namespace BWAPI
{
  bool initializingOrder = true;
  std::string orderName[191];
  std::map<std::string, Order> orderMap;
  std::set< Order > orderSet;
  namespace Orders
  {
    const Order Die(0);
    const Order Stop(1);
    const Order Guard(2);
    const Order PlayerGuard(3);
    const Order TurretGuard(4);
    const Order BunkerGuard(5);
    const Order Move(6);
    const Order AttackUnit(10);
    const Order AttackTile(12);
    const Order Hover(13);
    const Order AttackMove(14);
    const Order InfestedCommandCenter(15);
    const Order UnusedNothing(16);
    const Order UnusedPowerup(17);
    const Order TowerGuard(18);
    const Order VultureMine(20);
    const Order Nothing(23);
    const Order Nothing3(24);
    const Order CastInfestation(27);
    const Order InfestingCommandCenter(29);
    const Order PlaceBuilding(30);
    const Order BuildProtoss2(32);
    const Order ConstructingBuilding(33);
    const Order Repair(34);
    const Order PlaceAddon(36);
    const Order BuildAddon(37);
    const Order Train(38);
    const Order RallyPointUnit(39);
    const Order RallyPointTile(40);
    const Order ZergBirth(41);
    const Order ZergUnitMorph(42);
    const Order ZergBuildingMorph(43);
    const Order IncompleteBuilding(44);
    const Order BuildNydusExit(46);
    const Order EnterNydusCanal(47);
    const Order Follow(49);
    const Order Carrier(50);
    const Order ReaverCarrierMove(51);
    const Order CarrierIgnore2(55);
    const Order Reaver(58);
    const Order TrainFighter(63);
    const Order InterceptorAttack(64);
    const Order ScarabAttack(65);
    const Order RechargeShieldsUnit(66);
    const Order RechargeShieldsBattery(67);
    const Order ShieldBattery(68);
    const Order InterceptorReturn(69);
    const Order BuildingLand(71);
    const Order BuildingLiftOff(72);
    const Order DroneLiftOff(73);
    const Order LiftingOff(74);
    const Order ResearchTech(75);
    const Order Upgrade(76);
    const Order Larva(77);
    const Order SpawningLarva(78);
    const Order Harvest1(79);
    const Order Harvest2(80);
    const Order MoveToGas(81);
    const Order WaitForGas(82);
    const Order HarvestGas(83);
    const Order ReturnGas(84);
    const Order MoveToMinerals(85);
    const Order WaitForMinerals(86);
    const Order MiningMinerals(87);
    const Order Harvest3(88);
    const Order Harvest4(89);
    const Order ReturnMinerals(90);
    const Order Interrupted(91);
    const Order EnterTransport(92);
    const Order PickupIdle(93);
    const Order PickupTransport(94);
    const Order PickupBunker(95);
    const Order Pickup4(96);
    const Order PowerupIdle(97);
    const Order Sieging(98);
    const Order Unsieging(99);
    const Order InitCreepGrowth(101);
    const Order SpreadCreep(102);
    const Order StoppingCreepGrowth(103);
    const Order GuardianAspect(104);
    const Order ArchonWarp(105);
    const Order CompletingArchonsummon(106);
    const Order HoldPosition(107);
    const Order Cloak(109);
    const Order Decloak(110);
    const Order Unload(111);
    const Order MoveUnload(112);
    const Order FireYamatoGun(113);
    const Order CastLockdown(115);
    const Order Burrowing(116);
    const Order Burrowed(117);
    const Order Unburrowing(118);
    const Order CastDarkSwarm(119);
    const Order CastParasite(120);
    const Order CastSpawnBroodlings(121);
    const Order CastEMPShockwave(122);
    const Order NukeWait(123);
    const Order NukeTrain(124);
    const Order NukeLaunch(125);
    const Order NukeUnit(127);
    const Order CastNuclearStrike(128);
    const Order NukeTrack(129);
    const Order CloakNearbyUnits(131);
    const Order PlaceMine(132);
    const Order RightClickAction(133);
    const Order CastRecall(137);
    const Order TeleporttoLocation(138);
    const Order CastScannerSweep(139);
    const Order Scanner(140);
    const Order CastDefensiveMatrix(141);
    const Order CastPsionicStorm(142);
    const Order CastIrradiate(143);
    const Order CastPlague(144);
    const Order CastConsume(145);
    const Order CastEnsnare(146);
    const Order CastStasisField(147);
    const Order CastHallucination(148);
    const Order Hallucination2(149);
    const Order ResetCollision(150);
    const Order Patrol(152);
    const Order CTFCOPInit(153);
    const Order CTFCOP1(154);
    const Order CTFCOP2(155);
    const Order ComputerAI(156);
    const Order AtkMoveEP(157);
    const Order HarassMove(158);
    const Order AIPatrol(159);
    const Order GuardPost(160);
    const Order RescuePassive(161);
    const Order Neutral(162);
    const Order ComputerReturn(163);
    const Order SelfDestrucing(165);
    const Order Critter(166);
    const Order HiddenGun(167);
    const Order OpenDoor(168);
    const Order CloseDoor(169);
    const Order HideTrap(170);
    const Order RevealTrap(171);
    const Order Enabledoodad(172);
    const Order Disabledoodad(173);
    const Order Warpin(174);
    const Order Medic(175);
    const Order MedicHeal1(176);
    const Order HealMove(177);
    const Order MedicHeal2(179);
    const Order CastRestoration(180);
    const Order CastDisruptionWeb(181);
    const Order CastMindControl(182);
    const Order DarkArchonMeld(183);
    const Order CastFeedback(184);
    const Order CastOpticalFlare(185);
    const Order CastMaelstrom(186);
    const Order JunkYardDog(187);
    const Order Fatal(188);
    const Order None(189);
    const Order Unknown(190);
    void init()
    {
      orderName[Die.getID()] = "Die";
      orderName[Stop.getID()] = "Stop";
      orderName[Guard.getID()] = "Guard";
      orderName[PlayerGuard.getID()] = "PlayerGuard";
      orderName[TurretGuard.getID()] = "TurretGuard";
      orderName[BunkerGuard.getID()] = "BunkerGuard";
      orderName[Move.getID()] = "Move";
      orderName[AttackUnit.getID()] = "AttackUnit";
      orderName[AttackTile.getID()] = "AttackTile";
      orderName[Hover.getID()] = "Hover";
      orderName[AttackMove.getID()] = "AttackMove";
      orderName[InfestedCommandCenter.getID()] = "InfestedCommandCenter";
      orderName[UnusedNothing.getID()] = "UnusedNothing";
      orderName[UnusedPowerup.getID()] = "UnusedPowerup";
      orderName[TowerGuard.getID()] = "TowerGuard";
      orderName[VultureMine.getID()] = "VultureMine";
      orderName[Nothing.getID()] = "Nothing";
      orderName[Nothing3.getID()] = "Nothing3";
      orderName[CastInfestation.getID()] = "CastInfestation";
      orderName[InfestingCommandCenter.getID()] = "InfestingCommandCenter";
      orderName[PlaceBuilding.getID()] = "PlaceBuilding";
      orderName[BuildProtoss2.getID()] = "BuildProtoss2";
      orderName[ConstructingBuilding.getID()] = "ConstructingBuilding";
      orderName[Repair.getID()] = "Repair";
      orderName[PlaceAddon.getID()] = "PlaceAddon";
      orderName[BuildAddon.getID()] = "BuildAddon";
      orderName[Train.getID()] = "Train";
      orderName[RallyPointUnit.getID()] = "RallyPointUnit";
      orderName[RallyPointTile.getID()] = "RallyPointTile";
      orderName[ZergBirth.getID()] = "ZergBirth";
      orderName[ZergUnitMorph.getID()] = "ZergUnitMorph";
      orderName[ZergBuildingMorph.getID()] = "ZergBuildingMorph";
      orderName[IncompleteBuilding.getID()] = "IncompleteBuilding";
      orderName[BuildNydusExit.getID()] = "BuildNydusExit";
      orderName[EnterNydusCanal.getID()] = "EnterNydusCanal";
      orderName[Follow.getID()] = "Follow";
      orderName[Carrier.getID()] = "Carrier";
      orderName[ReaverCarrierMove.getID()] = "ReaverCarrierMove";
      orderName[CarrierIgnore2.getID()] = "CarrierIgnore2";
      orderName[Reaver.getID()] = "Reaver";
      orderName[TrainFighter.getID()] = "TrainFighter";
      orderName[InterceptorAttack.getID()] = "InterceptorAttack";
      orderName[ScarabAttack.getID()] = "ScarabAttack";
      orderName[RechargeShieldsUnit.getID()] = "RechargeShieldsUnit";
      orderName[RechargeShieldsBattery.getID()] = "RechargeShieldsBattery";
      orderName[ShieldBattery.getID()] = "ShieldBattery";
      orderName[InterceptorReturn.getID()] = "InterceptorReturn";
      orderName[BuildingLand.getID()] = "BuildingLand";
      orderName[BuildingLiftOff.getID()] = "BuildingLiftOff";
      orderName[DroneLiftOff.getID()] = "DroneLiftOff";
      orderName[LiftingOff.getID()] = "LiftingOff";
      orderName[ResearchTech.getID()] = "ResearchTech";
      orderName[Upgrade.getID()] = "Upgrade";
      orderName[Larva.getID()] = "Larva";
      orderName[SpawningLarva.getID()] = "SpawningLarva";
      orderName[Harvest1.getID()] = "Harvest1";
      orderName[Harvest2.getID()] = "Harvest2";
      orderName[MoveToGas.getID()] = "MoveToGas";
      orderName[WaitForGas.getID()] = "WaitForGas";
      orderName[HarvestGas.getID()] = "HarvestGas";
      orderName[ReturnGas.getID()] = "ReturnGas";
      orderName[MoveToMinerals.getID()] = "MoveToMinerals";
      orderName[WaitForMinerals.getID()] = "WaitForMinerals";
      orderName[MiningMinerals.getID()] = "MiningMinerals";
      orderName[Harvest3.getID()] = "Harvest3";
      orderName[Harvest4.getID()] = "Harvest4";
      orderName[ReturnMinerals.getID()] = "ReturnMinerals";
      orderName[Interrupted.getID()] = "Interrupted";
      orderName[EnterTransport.getID()] = "EnterTransport";
      orderName[PickupIdle.getID()] = "PickupIdle";
      orderName[PickupTransport.getID()] = "PickupTransport";
      orderName[PickupBunker.getID()] = "PickupBunker";
      orderName[Pickup4.getID()] = "Pickup4";
      orderName[PowerupIdle.getID()] = "PowerupIdle";
      orderName[Sieging.getID()] = "Sieging";
      orderName[Unsieging.getID()] = "Unsieging";
      orderName[InitCreepGrowth.getID()] = "InitCreepGrowth";
      orderName[SpreadCreep.getID()] = "SpreadCreep";
      orderName[StoppingCreepGrowth.getID()] = "StoppingCreepGrowth";
      orderName[GuardianAspect.getID()] = "GuardianAspect";
      orderName[ArchonWarp.getID()] = "ArchonWarp";
      orderName[CompletingArchonsummon.getID()] = "CompletingArchonsummon";
      orderName[HoldPosition.getID()] = "HoldPosition";
      orderName[Cloak.getID()] = "Cloak";
      orderName[Decloak.getID()] = "Decloak";
      orderName[Unload.getID()] = "Unload";
      orderName[MoveUnload.getID()] = "MoveUnload";
      orderName[FireYamatoGun.getID()] = "FireYamatoGun";
      orderName[CastLockdown.getID()] = "CastLockdown";
      orderName[Burrowing.getID()] = "Burrowing";
      orderName[Burrowed.getID()] = "Burrowed";
      orderName[Unburrowing.getID()] = "Unburrowing";
      orderName[CastDarkSwarm.getID()] = "CastDarkSwarm";
      orderName[CastParasite.getID()] = "CastParasite";
      orderName[CastSpawnBroodlings.getID()] = "CastSpawnBroodlings";
      orderName[CastEMPShockwave.getID()] = "CastEMPShockwave";
      orderName[NukeWait.getID()] = "NukeWait";
      orderName[NukeTrain.getID()] = "NukeTrain";
      orderName[NukeLaunch.getID()] = "NukeLaunch";
      orderName[NukeUnit.getID()] = "NukeUnit";
      orderName[CastNuclearStrike.getID()] = "CastNuclearStrike";
      orderName[NukeTrack.getID()] = "NukeTrack";
      orderName[CloakNearbyUnits.getID()] = "CloakNearbyUnits";
      orderName[PlaceMine.getID()] = "PlaceMine";
      orderName[RightClickAction.getID()] = "RightClickAction";
      orderName[CastRecall.getID()] = "CastRecall";
      orderName[TeleporttoLocation.getID()] = "TeleporttoLocation";
      orderName[CastScannerSweep.getID()] = "CastScannerSweep";
      orderName[Scanner.getID()] = "Scanner";
      orderName[CastDefensiveMatrix.getID()] = "CastDefensiveMatrix";
      orderName[CastPsionicStorm.getID()] = "CastPsionicStorm";
      orderName[CastIrradiate.getID()] = "CastIrradiate";
      orderName[CastPlague.getID()] = "CastPlague";
      orderName[CastConsume.getID()] = "CastConsume";
      orderName[CastEnsnare.getID()] = "CastEnsnare";
      orderName[CastStasisField.getID()] = "CastStasisField";
      orderName[CastHallucination.getID()] = "CastHallucination";
      orderName[Hallucination2.getID()] = "Hallucination2";
      orderName[ResetCollision.getID()] = "ResetCollision";
      orderName[Patrol.getID()] = "Patrol";
      orderName[CTFCOPInit.getID()] = "CTFCOPInit";
      orderName[CTFCOP1.getID()] = "CTFCOP1";
      orderName[CTFCOP2.getID()] = "CTFCOP2";
      orderName[ComputerAI.getID()] = "ComputerAI";
      orderName[AtkMoveEP.getID()] = "AtkMoveEP";
      orderName[HarassMove.getID()] = "HarassMove";
      orderName[AIPatrol.getID()] = "AIPatrol";
      orderName[GuardPost.getID()] = "GuardPost";
      orderName[RescuePassive.getID()] = "RescuePassive";
      orderName[Neutral.getID()] = "Neutral";
      orderName[ComputerReturn.getID()] = "ComputerReturn";
      orderName[SelfDestrucing.getID()] = "SelfDestrucing";
      orderName[Critter.getID()] = "Critter";
      orderName[HiddenGun.getID()] = "HiddenGun";
      orderName[OpenDoor.getID()] = "OpenDoor";
      orderName[CloseDoor.getID()] = "CloseDoor";
      orderName[HideTrap.getID()] = "HideTrap";
      orderName[RevealTrap.getID()] = "RevealTrap";
      orderName[Enabledoodad.getID()] = "Enabledoodad";
      orderName[Disabledoodad.getID()] = "Disabledoodad";
      orderName[Warpin.getID()] = "Warpin";
      orderName[Medic.getID()] = "Medic";
      orderName[MedicHeal1.getID()] = "MedicHeal1";
      orderName[HealMove.getID()] = "HealMove";
      orderName[MedicHeal2.getID()] = "MedicHeal2";
      orderName[CastRestoration.getID()] = "CastRestoration";
      orderName[CastDisruptionWeb.getID()] = "CastDisruptionWeb";
      orderName[CastMindControl.getID()] = "CastMindControl";
      orderName[DarkArchonMeld.getID()] = "DarkArchonMeld";
      orderName[CastFeedback.getID()] = "CastFeedback";
      orderName[CastOpticalFlare.getID()] = "CastOpticalFlare";
      orderName[CastMaelstrom.getID()] = "CastMaelstrom";
      orderName[JunkYardDog.getID()] = "JunkYardDog";
      orderName[Fatal.getID()] = "Fatal";
      orderName[None.getID()] = "None";
      orderName[Unknown.getID()] = "Unknown";
      orderSet.insert(Die);
      orderSet.insert(Stop);
      orderSet.insert(Guard);
      orderSet.insert(PlayerGuard);
      orderSet.insert(TurretGuard);
      orderSet.insert(BunkerGuard);
      orderSet.insert(Move);
      orderSet.insert(AttackUnit);
      orderSet.insert(AttackTile);
      orderSet.insert(Hover);
      orderSet.insert(AttackMove);
      orderSet.insert(InfestedCommandCenter);
      orderSet.insert(UnusedNothing);
      orderSet.insert(UnusedPowerup);
      orderSet.insert(TowerGuard);
      orderSet.insert(VultureMine);
      orderSet.insert(Nothing);
      orderSet.insert(Nothing3);
      orderSet.insert(CastInfestation);
      orderSet.insert(InfestingCommandCenter);
      orderSet.insert(PlaceBuilding);
      orderSet.insert(BuildProtoss2);
      orderSet.insert(ConstructingBuilding);
      orderSet.insert(Repair);
      orderSet.insert(PlaceAddon);
      orderSet.insert(BuildAddon);
      orderSet.insert(Train);
      orderSet.insert(RallyPointUnit);
      orderSet.insert(RallyPointTile);
      orderSet.insert(ZergBirth);
      orderSet.insert(ZergUnitMorph);
      orderSet.insert(ZergBuildingMorph);
      orderSet.insert(IncompleteBuilding);
      orderSet.insert(BuildNydusExit);
      orderSet.insert(EnterNydusCanal);
      orderSet.insert(Follow);
      orderSet.insert(Carrier);
      orderSet.insert(ReaverCarrierMove);
      orderSet.insert(CarrierIgnore2);
      orderSet.insert(Reaver);
      orderSet.insert(TrainFighter);
      orderSet.insert(InterceptorAttack);
      orderSet.insert(ScarabAttack);
      orderSet.insert(RechargeShieldsUnit);
      orderSet.insert(RechargeShieldsBattery);
      orderSet.insert(ShieldBattery);
      orderSet.insert(InterceptorReturn);
      orderSet.insert(BuildingLand);
      orderSet.insert(BuildingLiftOff);
      orderSet.insert(DroneLiftOff);
      orderSet.insert(LiftingOff);
      orderSet.insert(ResearchTech);
      orderSet.insert(Upgrade);
      orderSet.insert(Larva);
      orderSet.insert(SpawningLarva);
      orderSet.insert(Harvest1);
      orderSet.insert(Harvest2);
      orderSet.insert(MoveToGas);
      orderSet.insert(WaitForGas);
      orderSet.insert(HarvestGas);
      orderSet.insert(ReturnGas);
      orderSet.insert(MoveToMinerals);
      orderSet.insert(WaitForMinerals);
      orderSet.insert(MiningMinerals);
      orderSet.insert(Harvest3);
      orderSet.insert(Harvest4);
      orderSet.insert(ReturnMinerals);
      orderSet.insert(Interrupted);
      orderSet.insert(EnterTransport);
      orderSet.insert(PickupIdle);
      orderSet.insert(PickupTransport);
      orderSet.insert(PickupBunker);
      orderSet.insert(Pickup4);
      orderSet.insert(PowerupIdle);
      orderSet.insert(Sieging);
      orderSet.insert(Unsieging);
      orderSet.insert(InitCreepGrowth);
      orderSet.insert(SpreadCreep);
      orderSet.insert(StoppingCreepGrowth);
      orderSet.insert(GuardianAspect);
      orderSet.insert(ArchonWarp);
      orderSet.insert(CompletingArchonsummon);
      orderSet.insert(HoldPosition);
      orderSet.insert(Cloak);
      orderSet.insert(Decloak);
      orderSet.insert(Unload);
      orderSet.insert(MoveUnload);
      orderSet.insert(FireYamatoGun);
      orderSet.insert(CastLockdown);
      orderSet.insert(Burrowing);
      orderSet.insert(Burrowed);
      orderSet.insert(Unburrowing);
      orderSet.insert(CastDarkSwarm);
      orderSet.insert(CastParasite);
      orderSet.insert(CastSpawnBroodlings);
      orderSet.insert(CastEMPShockwave);
      orderSet.insert(NukeWait);
      orderSet.insert(NukeTrain);
      orderSet.insert(NukeLaunch);
      orderSet.insert(NukeUnit);
      orderSet.insert(CastNuclearStrike);
      orderSet.insert(NukeTrack);
      orderSet.insert(CloakNearbyUnits);
      orderSet.insert(PlaceMine);
      orderSet.insert(RightClickAction);
      orderSet.insert(CastRecall);
      orderSet.insert(TeleporttoLocation);
      orderSet.insert(CastScannerSweep);
      orderSet.insert(Scanner);
      orderSet.insert(CastDefensiveMatrix);
      orderSet.insert(CastPsionicStorm);
      orderSet.insert(CastIrradiate);
      orderSet.insert(CastPlague);
      orderSet.insert(CastConsume);
      orderSet.insert(CastEnsnare);
      orderSet.insert(CastStasisField);
      orderSet.insert(CastHallucination);
      orderSet.insert(Hallucination2);
      orderSet.insert(ResetCollision);
      orderSet.insert(Patrol);
      orderSet.insert(CTFCOPInit);
      orderSet.insert(CTFCOP1);
      orderSet.insert(CTFCOP2);
      orderSet.insert(ComputerAI);
      orderSet.insert(AtkMoveEP);
      orderSet.insert(HarassMove);
      orderSet.insert(AIPatrol);
      orderSet.insert(GuardPost);
      orderSet.insert(RescuePassive);
      orderSet.insert(Neutral);
      orderSet.insert(ComputerReturn);
      orderSet.insert(SelfDestrucing);
      orderSet.insert(Critter);
      orderSet.insert(HiddenGun);
      orderSet.insert(OpenDoor);
      orderSet.insert(CloseDoor);
      orderSet.insert(HideTrap);
      orderSet.insert(RevealTrap);
      orderSet.insert(Enabledoodad);
      orderSet.insert(Disabledoodad);
      orderSet.insert(Warpin);
      orderSet.insert(Medic);
      orderSet.insert(MedicHeal1);
      orderSet.insert(HealMove);
      orderSet.insert(MedicHeal2);
      orderSet.insert(CastRestoration);
      orderSet.insert(CastDisruptionWeb);
      orderSet.insert(CastMindControl);
      orderSet.insert(DarkArchonMeld);
      orderSet.insert(CastFeedback);
      orderSet.insert(CastOpticalFlare);
      orderSet.insert(CastMaelstrom);
      orderSet.insert(JunkYardDog);
      orderSet.insert(Fatal);
      orderSet.insert(None);
      orderSet.insert(Unknown);
      for(std::set<Order>::iterator i = orderSet.begin(); i != orderSet.end(); i++)
      {
        std::string name=(*i).getName();
        for(int j=0;j<(int)name.length();j++)
        {
          if (name[j]==' ') name[j]='_';
          if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
        }
        orderMap.insert(std::make_pair(name, *i));
      }
      initializingOrder = false;
    }
  }

  Order::Order()
  {
    this->id = Orders::None.id;
  }
  Order::Order(int id)
  {
    this->id = id;
    if (!initializingOrder)
    {
      if (id < 0 || id >= 191 || orderName[id].length() == 0)
      {
        this->id = Orders::Unknown.id;
      }
    }
  }
  Order::Order(const Order& other)
  {
    this->id = other.id;
  }
  Order& Order::operator=(const Order& other)
  {
    this->id = other.id;
    return *this;
  }
  bool Order::operator==(const Order& other) const
  {
    return this->id == other.id;
  }
  bool Order::operator!=(const Order& other) const
  {
    return this->id != other.id;
  }
  bool Order::operator<(const Order& other) const
  {
    return this->id < other.id;
  }
  int Order::getID() const
  {
    return this->id;
  }
  std::string Order::getName() const
  {
    return orderName[this->id];
  }
  Order Orders::getOrder(std::string name)
  {
    for(int j=0;j<(int)name.length();j++)
    {
      if (name[j]==' ') name[j]='_';
      if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
    }
    std::map<std::string, Order>::iterator i = orderMap.find(name);
    if (i == orderMap.end()) return Orders::Unknown;
    return (*i).second;
  }
  std::set<Order>& Orders::allOrders()
  {
    return orderSet;
  }
}
