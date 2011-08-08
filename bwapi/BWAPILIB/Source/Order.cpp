#include <string>
#include <map>
#include <set>
#include <BWAPI/Order.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

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
    const Order NukePaint(126);
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
    const Order CTFCOPStarted(154);
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

#define SET_ORDER(x) orderName[x] = underscoresToSpaces(#x); orderSet.insert(x);
    void init()
    {
      SET_ORDER(Die);
      SET_ORDER(Stop);
      SET_ORDER(Guard);
      SET_ORDER(PlayerGuard);
      SET_ORDER(TurretGuard);
      SET_ORDER(BunkerGuard);
      SET_ORDER(Move);
      SET_ORDER(AttackUnit);
      SET_ORDER(AttackTile);
      SET_ORDER(Hover);
      SET_ORDER(AttackMove);
      SET_ORDER(InfestedCommandCenter);
      SET_ORDER(UnusedNothing);
      SET_ORDER(UnusedPowerup);
      SET_ORDER(TowerGuard);
      SET_ORDER(VultureMine);
      SET_ORDER(Nothing);
      SET_ORDER(Nothing3);
      SET_ORDER(CastInfestation);
      SET_ORDER(InfestingCommandCenter);
      SET_ORDER(PlaceBuilding);
      SET_ORDER(BuildProtoss2);
      SET_ORDER(ConstructingBuilding);
      SET_ORDER(Repair);
      SET_ORDER(PlaceAddon);
      SET_ORDER(BuildAddon);
      SET_ORDER(Train);
      SET_ORDER(RallyPointUnit);
      SET_ORDER(RallyPointTile);
      SET_ORDER(ZergBirth);
      SET_ORDER(ZergUnitMorph);
      SET_ORDER(ZergBuildingMorph);
      SET_ORDER(IncompleteBuilding);
      SET_ORDER(BuildNydusExit);
      SET_ORDER(EnterNydusCanal);
      SET_ORDER(Follow);
      SET_ORDER(Carrier);
      SET_ORDER(ReaverCarrierMove);
      SET_ORDER(CarrierIgnore2);
      SET_ORDER(Reaver);
      SET_ORDER(TrainFighter);
      SET_ORDER(InterceptorAttack);
      SET_ORDER(ScarabAttack);
      SET_ORDER(RechargeShieldsUnit);
      SET_ORDER(RechargeShieldsBattery);
      SET_ORDER(ShieldBattery);
      SET_ORDER(InterceptorReturn);
      SET_ORDER(BuildingLand);
      SET_ORDER(BuildingLiftOff);
      SET_ORDER(DroneLiftOff);
      SET_ORDER(LiftingOff);
      SET_ORDER(ResearchTech);
      SET_ORDER(Upgrade);
      SET_ORDER(Larva);
      SET_ORDER(SpawningLarva);
      SET_ORDER(Harvest1);
      SET_ORDER(Harvest2);
      SET_ORDER(MoveToGas);
      SET_ORDER(WaitForGas);
      SET_ORDER(HarvestGas);
      SET_ORDER(ReturnGas);
      SET_ORDER(MoveToMinerals);
      SET_ORDER(WaitForMinerals);
      SET_ORDER(MiningMinerals);
      SET_ORDER(Harvest3);
      SET_ORDER(Harvest4);
      SET_ORDER(ReturnMinerals);
      SET_ORDER(Interrupted);
      SET_ORDER(EnterTransport);
      SET_ORDER(PickupIdle);
      SET_ORDER(PickupTransport);
      SET_ORDER(PickupBunker);
      SET_ORDER(Pickup4);
      SET_ORDER(PowerupIdle);
      SET_ORDER(Sieging);
      SET_ORDER(Unsieging);
      SET_ORDER(InitCreepGrowth);
      SET_ORDER(SpreadCreep);
      SET_ORDER(StoppingCreepGrowth);
      SET_ORDER(GuardianAspect);
      SET_ORDER(ArchonWarp);
      SET_ORDER(CompletingArchonsummon);
      SET_ORDER(HoldPosition);
      SET_ORDER(Cloak);
      SET_ORDER(Decloak);
      SET_ORDER(Unload);
      SET_ORDER(MoveUnload);
      SET_ORDER(FireYamatoGun);
      SET_ORDER(CastLockdown);
      SET_ORDER(Burrowing);
      SET_ORDER(Burrowed);
      SET_ORDER(Unburrowing);
      SET_ORDER(CastDarkSwarm);
      SET_ORDER(CastParasite);
      SET_ORDER(CastSpawnBroodlings);
      SET_ORDER(CastEMPShockwave);
      SET_ORDER(NukeWait);
      SET_ORDER(NukeTrain);
      SET_ORDER(NukeLaunch);
      SET_ORDER(NukePaint);
      SET_ORDER(NukeUnit);
      SET_ORDER(CastNuclearStrike);
      SET_ORDER(NukeTrack);
      SET_ORDER(CloakNearbyUnits);
      SET_ORDER(PlaceMine);
      SET_ORDER(RightClickAction);
      SET_ORDER(CastRecall);
      SET_ORDER(TeleporttoLocation);
      SET_ORDER(CastScannerSweep);
      SET_ORDER(Scanner);
      SET_ORDER(CastDefensiveMatrix);
      SET_ORDER(CastPsionicStorm);
      SET_ORDER(CastIrradiate);
      SET_ORDER(CastPlague);
      SET_ORDER(CastConsume);
      SET_ORDER(CastEnsnare);
      SET_ORDER(CastStasisField);
      SET_ORDER(CastHallucination);
      SET_ORDER(Hallucination2);
      SET_ORDER(ResetCollision);
      SET_ORDER(Patrol);
      SET_ORDER(CTFCOPInit);
      SET_ORDER(CTFCOPStarted);
      SET_ORDER(CTFCOP2);
      SET_ORDER(ComputerAI);
      SET_ORDER(AtkMoveEP);
      SET_ORDER(HarassMove);
      SET_ORDER(AIPatrol);
      SET_ORDER(GuardPost);
      SET_ORDER(RescuePassive);
      SET_ORDER(Neutral);
      SET_ORDER(ComputerReturn);
      SET_ORDER(SelfDestrucing);
      SET_ORDER(Critter);
      SET_ORDER(HiddenGun);
      SET_ORDER(OpenDoor);
      SET_ORDER(CloseDoor);
      SET_ORDER(HideTrap);
      SET_ORDER(RevealTrap);
      SET_ORDER(Enabledoodad);
      SET_ORDER(Disabledoodad);
      SET_ORDER(Warpin);
      SET_ORDER(Medic);
      SET_ORDER(MedicHeal1);
      SET_ORDER(HealMove);
      SET_ORDER(MedicHeal2);
      SET_ORDER(CastRestoration);
      SET_ORDER(CastDisruptionWeb);
      SET_ORDER(CastMindControl);
      SET_ORDER(DarkArchonMeld);
      SET_ORDER(CastFeedback);
      SET_ORDER(CastOpticalFlare);
      SET_ORDER(CastMaelstrom);
      SET_ORDER(JunkYardDog);
      SET_ORDER(Fatal);
      SET_ORDER(None);
      SET_ORDER(Unknown);

      foreach(Order i, orderSet)
      {
        std::string name(i.getName());
        fixName(&name);
        orderMap.insert(std::make_pair(name, i));
      }
      initializingOrder = false;
    }
  }

  Order::Order()
  {
    this->id = Orders::None;
  }
  Order::Order(int id)
  {
    this->id = id;
    if (!initializingOrder && (id < 0 || id >= 191 || orderName[id].length() == 0))
      this->id = Orders::Unknown;
  }
  Order::Order(const Order& other)
  {
    this->id = other;
  }
  Order& Order::operator=(const Order& other)
  {
    this->id = other;
    return *this;
  }
  Order::operator int() const
  {
    return id;
  }
  int Order::getID() const
  {
    return this->id;
  }
  std::string Order::getName() const
  {
    return orderName[this->id];
  }
  const char *Order::c_str() const
  {
    return orderName[this->id].c_str();
  }
  Order Orders::getOrder(std::string name)
  {
    fixName(&name);
    std::map<std::string, Order>::iterator i = orderMap.find(name);
    if (i == orderMap.end())
      return Orders::Unknown;
    return (*i).second;
  }
  std::set<Order>& Orders::allOrders()
  {
    return orderSet;
  }
}
