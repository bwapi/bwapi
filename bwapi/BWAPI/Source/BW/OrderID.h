#pragma once

#include <Util/Types.h>
#include <string>

namespace BW
{
  /** 
   * Specifies type of action performed by unit. 
   * from http://www.staredit.net/wiki/Orders.dat_Entry_Listing
   */
  namespace OrderID
  {
    enum Enum : u8
    {
      Die                    = 0x00, /**< Causes the unit to die. Normal units run the death iscript animation, while hallucinated units have the sound/sprite spawned and then are removed.
                                      * Default Requirements: Allow on hallucinated units. */
      Stop                   = 0x01, /**< Normal unit stop command. Stops current order chain, and then goes to idle.
                                      * Default Requirements: Allow on hallucinated units. */
      Guard                  = 0x02, /**< Generic Guard order. Determines what guard command a unit uses.
                                      * Default Requirements: Unit responds to Battle Orders. Allow on hallucinated units. */
      PlayerGuard            = 0x03, /**< Attacking Mobile unit guard order.
                                      * Default Requirements: Unit responds to Battle Orders. Allow on hallucinated units. */
      TurretGuard            = 0x04, /**< Attacking unit turret guard.
                                      * Default Requirements: Unit is a subunit. Allow on hallucinated units. */
      BunkerGuard            = 0x05, /**< Transport building guard. Set when a building picks up a unit.
                                      * Default Requirements: Unit must be a Terran Bunker. */
      Move                   = 0x06, /**< Unit move. Ignores enemies on way to destination.
                                      * Default Requirements: Unit is able to move. Allow on hallucinated units. */
      ReaverStop             = 0x07, /**< Stop order for the reaver.
                                      * Default Requirements: Unit must be Reaver/Warbringer. Allow on hallucinated units. */
      Attack1                = 0x08, /**< Generic attack order.
                                      * Default Requirements: Unit responds to battle orders. Allow on hallucinated units. */
      Attack2                = 0x09, /**< Move to attack shrouded building.
                                      * Default Requirements: Unit responds to battle orders. Allow on hallucinated units. */
      AttackUnit             = 0x0A, /**< Mobile unit attacking a unit/building.
                                      * Default Requirements: Unit is able to move. Allow on hallucinated units. */
      AttackFixedRange       = 0x0B, /**< Attack for an immobile unit. Lurker attack.
                                      * Default Requirements: Unit responds to battle orders. Allow on hallucinated units. */
      AttackTile             = 0x0C, /**< Unused
                                      * Default Requirements: Unit responds to battle orders. Allow on hallucinated units. */
      Hover                  = 0x0D, /**< Unused
                                      * Default Requirements: Unit is able to move. Allow on hallucinated units. */
      AttackMove             = 0x0E, /**< Unit move, attack enemies along path to target.
                                      * Default Requirements: Unit is able to move. Allow on hallucinated units. */
      InfestMine1            = 0x0F, /**< Ran when a unit is being infested.
                                      * Default Requirements: Unit must be Terran Command Center. */
      Nothing1               = 0x10, /**< Unknown
                                      * Default Requirements: Unused. */
      Powerup1               = 0x11, /**< Unknown. Speculated to be a Powerup being built order.
                                      * Default Requirements: Unused. */
      TowerGuard             = 0x12, /**< Building tower guard.
                                      * Default Requirements: Unit must be either Photon Cannon, Missile Turret, Sunken Colony, or Spore Colony. */
      TowerAttack            = 0x13, /**< Building tower attack.
                                      * Default Requirements: Unit must be either Photon Cannon, Missile Turret, Sunken Colony, Spore Colony, Floor Gun Trap, Left Wall Missile Trap, Left Wall Flame Trap, Right Wall Flame Trap, Right Wall Missile Trap, */
      VultureMine            = 0x14, /**< Spidermine idle order.
                                      * Default Requirements: Unit must be Vulture Spider Mine. */
      StayinRange            = 0x15, /**< Mobile unit base attack.
                                      * Default Requirements: Unit is able to move. Allow on hallucinated units. */
      TurretAttack           = 0x16, /**< Mobile Unit Turret attack.
                                      * Default Requirements: Must be subunit. Allow on hallucinated units. */
      Nothing2               = 0x17, /**< Do nothing, next order.
                                      * Default Requirements: Allow on hallucinated units. */
      Nothing3               = 0x18, /**< Unknown, used when a unit is changing state between siege <-> normal.
                                      * Default Requirements: Unused. */
      DroneStartBuild        = 0x19, /**< Move to target position and run drone build.
                                      * Default Requirements: Unit must be Zerg Drone. */
      DroneBuild             = 0x1A, /**< Check resources and run drone land.
                                      * Default Requirements: Unit must be Zerg Drone. */
      InfestMine2            = 0x1B, /**< Move to Infest a unit.
                                      * Default Requirements: Unit must be Zerg Queen/Matriarch. */
      InfestMine3            = 0x1C, /**< Move to Infest shrouded unit
                                      * Default Requirements: Unit must be Zerg Queen/Matriarch. */
      InfestMine4            = 0x1D, /**< Infest Unit. Hides unit, runs infest 1 on target, then reshows unit.
                                      * Default Requirements: Unit must be Zerg Queen/Matriarch. */
      BuildTerran            = 0x1E, /**< Move/Start Terran Building.
                                      * Default Requirements: Unit must be Terran SCV. */
      BuildProtoss1          = 0x1F, /**< Full Protoss Building order.
                                      * Default Requirements: Unit must be Protoss Probe. */
      BuildProtoss2          = 0x20, /**< Creates the Protoss Building.
                                      * Default Requirements: Unit must be Protoss Probe. */
      ConstructingBuilding   = 0x21, /**< SCV is building.
                                      * Default Requirements: Unit must be Terran SCV. */
      Repair1                = 0x22, /**< Repair Unit.
                                      * Default Requirements: Unit must be Terran SCV. */
      Repair2                = 0x23, /**< Move to repair shrouded building.
                                      * Default Requirements: Unit must be Terran SCV. */
      PlaceAddon             = 0x24, /**< Move and start addon.
                                      * Default Requirements: Blank. */
      BuildAddon             = 0x25, /**< Building Addon.
                                      * Default Requirements: Unused. */
      Train                  = 0x26, /**< Training Unit.
                                      * Default Requirements: Unused. */
      RallyPoint1            = 0x27, /**< Rally to Visible Unit. Causes units to follow the selected unit.
                                      * Default Requirements: Unit is able to set Rally Point. */
      RallyPoint2            = 0x28, /**< Rally to tile.
                                      * Default Requirements: Unit is able to set Rally Point. */
      ZergBirth              = 0x29, /**< Unit is being born.
                                      * Default Requirements: Unit must be Zerg Egg or Zerg Cocoon. */
      Morph1                 = 0x2A, /**< Unit Morph
                                      * Default Requirements: Unused. */
      Morph2                 = 0x2B, /**< Building Morph
                                      * Default Requirements: Unused */
      BuildSelf1             = 0x2C, /**< Terran Building, Is being built.
                                      * Default Requirements: Unused */
      ZergBuildSelf          = 0x2D, /**< Zerg Building build order.
                                      * Default Requirements: Unused. */
      Build5                 = 0x2E, /**< Nydus canal exit build order.
                                      * Default Requirements: Unit must be Zerg Nydus Canal and must not have exit. */
      Enternyduscanal        = 0x2F, /**< Enter/transport through nydus canal
                                      * Default Requirements: Unused. */
      BuildSelf2             = 0x30, /**< Protoss Building being built order.
                                      * Default Requirements: Unused. */
      Follow                 = 0x31, /**< Move to/with unit or building. Causes units to load into transports or enter nydus canal or recharge shields.
                                      * Default Requirements: Unit is able to move. Allow on hallucinated units. */
      Carrier                = 0x32, /**< Idle command for the carrier.
                                      * Default Requirements: Unit must be Carrier/Gantrithor. Allow on hallucinated units. */
      CarrierIgnore1         = 0x33, /**< Carrier move command. Ignores enemies
                                      * Default Requirements: Unit must be Carrier/Gantrithor or Reaver/Warbringer. Allow on hallucinated units. */
      CarrierStop            = 0x34, /**< Carrier stop command. Runs idle.
                                      * Default Requirements: Unit must be Carrier/Gantrithor. Allow on hallucinated units. */
      CarrierAttack1         = 0x35, /**< Generic Carrier attack command.
                                      * Default Requirements: Unit must be Carrier/Gantrithor. Allow on hallucinated units. */
      CarrierAttack2         = 0x36, /**< Move to attack shrouded building.
                                      * Default Requirements: Unit must be Carrier/Gantrithor. Allow on hallucinated units. */
      CarrierIgnore2         = 0x37, /**< Unknown. Possibly a secondary move.
                                      * Default Requirements: Unit must be Carrier/Gantrithor or Reaver/Warbringer. Allow on hallucinated units. */
      CarrierFight           = 0x38, /**< Carrier Attack Unit.
                                      * Default Requirements: Unit must be Carrier/Gantrithor. Allow on hallucinated units. */
      HoldPosition1          = 0x39, /**< Carrier Hold Position.
                                      * Default Requirements: Unit must be Carrier/Gantrithor. Allow on hallucinated units. */
      Reaver                 = 0x3A, /**< Reaver Idle order.
                                      * Default Requirements: Unit must be Reaver/Warbringer. Allow on hallucinated units. */
      ReaverAttack1          = 0x3B, /**< Generic reaver attack order.
                                      * Default Requirements: Unit must be Reaver/Warbringer. Allow on hallucinated units. */
      ReaverAttack2          = 0x3C, /**< Move to attack shrouded building
                                      * Default Requirements: Unit must be Reaver/Warbringer. Allow on hallucinated units. */
      ReaverFight            = 0x3D, /**< Reaver attack unit.
                                      * Default Requirements: Unit must be Reaver/Warbringer. Allow on hallucinated units. */
      ReaverHold             = 0x3E, /**< Reaver hold position.
                                      * Default Requirements: Unit must be Reaver/Warbringer. Allow on hallucinated units. */
      TrainFighter           = 0x3F, /**< Training subunit(scarab, interceptor). Causes all interceptors within a carrier to be healed when not attacking.
                                      * Default Requirements: Unused. */
      StrafeUnit1            = 0x40, /**< Interceptor move and attack.
                                      * Default Requirements: Unit must be Protoss Interceptor. Allow on hallucinated units. */
      StrafeUnit2            = 0x41, /**< Scarab move and attack.
                                      * Default Requirements: Unit must be Protoss Scarab. */
      RechargeShields1       = 0x42, /**< Unit recharge shields.
                                      * Default Requirements: Unused. */
      Rechargeshields2       = 0x43, /**< Shield Battery, recharge shield cast on unit or ground. Unit runs recharge shields 1, shield battery runs shield battery. If cast on ground, recharges all units within rechargeable radius.
                                      * Default Requirements: Unit must be Protoss Shield Battery. */
      ShieldBattery          = 0x44, /**< Shield Battery, is recharging.
                                      * Default Requirements: Unused. */
      Return                 = 0x45, /**< Interceptor return to parent.
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      DroneLand              = 0x46, /**< Drone landing order. Used when building.
                                      * Default Requirements: Unit must be Zerg Drone. */
      BuildingLand           = 0x47, /**< Building land order.
                                      * Default Requirements: Unit must be a building(can lift off) that is lifted off. */
      BuildingLiftoff        = 0x48, /**< Begin Building Liftoff
                                      * Default Requirements: Unit must be a building(can lift off) that is on the ground. */
      DroneLiftoff           = 0x49, /**< Begin Drone liftoff
                                      * Default Requirements: Unit must be Zerg Drone. */
      Liftoff                = 0x4A, /**< Unit is lifting off.
                                      * Default Requirements: Unit must be a building(can lift off). */
      ResearchTech           = 0x4B, /**< Building researching tech.
                                      * Default Requirements: Unused. */
      Upgrade                = 0x4C, /**< Building researching upgrade.
                                      * Default Requirements: Unused. */
      Larva                  = 0x4D, /**< Idle order for larva. Make sure it stays on creep, dies if off, and says within the range of the parent it came from.
                                      * Default Requirements: Unit must be Zerg Larva. Allow on hallucinated units. */
      SpawningLarva          = 0x4E, /**< Building is spawning larva.
                                      * Default Requirements: Unit must be Hatchery/Lair/Hive. */
      Harvest1               = 0x4F, /**< Generic move to harvest order.
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      Harvest2               = 0x50, /**< Move to harvest shrouded minerals/gas
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      HarvestGas1            = 0x51, /**< Move to harvest gas.
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      HarvestGas2            = 0x52, /**< Check if it can enter the gas mine(no unit in it).
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      HarvestGas3            = 0x53, /**< Enter/exit mine, set return order.
                                      * Default Requirements: Unused. */
      ReturnGas              = 0x54, /**< Return order, has gas.
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      MoveToMinerals         = 0x55, /**< Move to harvest minerals.
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      HarvestMinerals2       = 0x56, /**< Can harvest minerals(one unit per field).
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      MiningMinerals         = 0x57, /**< Harvesting minerals. Runs iscript to spawn weapon.
                                      * Default Requirements: Unused. */
      Harvest3               = 0x58, /**< Harvesting minerals is interrupted.
                                      * Default Requirements: Unused. */
      Harvest4               = 0x59, /**< Unknown harvest command.
                                      * Default Requirements: Unused. */
      ReturnMinerals         = 0x5A, /**< Return resources /B Has minerals.
                                      * Default Requirements: Unit must be a Worker (has harvest orders). */
      Harvest5               = 0x5B, /**< Harvest Interrupt /B recharge shields.
                                      * Default Requirements: Unused. */
      EnterTransport         = 0x5C, /**< Move/enter a transport.
                                      * Default Requirements: Allow on Hallucinated Units. */
      Pickup1                = 0x5D, /**< Transport Idle command.
                                      * Default Requirements: Unit must be Transport(Can carry units) or Zerg Overlord. */
      Pickup2                = 0x5E, /**< Mobile Transport unit pickup.
                                      * Default Requirements: Unit must be Transport(Can carry units). */
      Pickup3                = 0x5F, /**< Building pickup.
                                      * Default Requirements: Unit must be Terran Bunker. */
      Pickup4                = 0x60, /**< Unknown /B AI pickup?
                                      * Default Requirements: Unused. */
      Powerup2               = 0x61, /**< Idle for powerups.
                                      * Default Requirements: Unit must be a powerup. */
      SiegeMode              = 0x62, /**< Switch to Siege mode.
                                      * Default Requirements: Unused. */
      TankMode               = 0x63, /**< Switch to Tank mode.
                                      * Default Requirements: Unused. */
      WatchTarget            = 0x64, /**< Immobile Unit base, watch the target.
                                      * Default Requirements: Allow on hallucinated units. */
      InitCreepGrowth        = 0x65, /**< Start Spreading Creep.
                                      * Default Requirements: Unit must be Hatchery/Lair/Hive or Creep Colony. */
      SpreadCreep            = 0x66, /**< Spreads creep. If it is a larva producer, runs that order also.
                                      * Default Requirements: Blank. */
      StoppingCreepGrowth    = 0x67, /**< Stops creep growth.
                                      * Default Requirements: Unit must be Sunken/Spore Colony. */
      GuardianAspect         = 0x68, /**< Unused, Morph 1 is used for unit morphing.
                                      * Default Requirements: Unit must be Zerg Mutalisk. */
      WarpingArchon          = 0x69, /**< Move and start archon merge.
                                      * Default Requirements: Unused. */
      CompletingArchonsummon = 0x6A, /**< Archon build self order.
                                      * Default Requirements: Unused. */
      HoldPosition2          = 0x6B, /**< Attacking Unit hold position.
                                      * Default Requirements: Unit is able to Hold Position. Allow on hallucinated units. */
      HoldPosition3          = 0x6C, /**< Queen Hold position.
                                      * Default Requirements: Unit must be Zerg Queen/Matriarch. */
      Cloak                  = 0x6D, /**< Cloak Unit.
                                      * Default Requirements: Unused. */
      Decloak                = 0x6E, /**< Decloak Unit.
                                      * Default Requirements: Unused. */
      Unload                 = 0x6F, /**< Unload a unit from the transport.
                                      * Default Requirements: Unit must be a transport. */
      MoveUnload             = 0x70, /**< Move to unload site and run unload order.
                                      * Default Requirements: Unit must be a transport. */
      FireYamatoGun1         = 0x71, /**< Cast Spell: Yamato.
                                      * Default Requirements: Unused. */
      FireYamatoGun2         = 0x72, /**< Move to cast spell on shrouded building.
                                      * Default Requirements: Unused. */
      MagnaPulse             = 0x73, /**< Cast Spell: Lockdown.
                                      * Default Requirements: Unused. */
      Burrow                 = 0x74, /**< Burrow Unit.
                                      * Default Requirements: Unused. */
      Burrowed               = 0x75, /**< Burrowed Unit idle.
                                      * Default Requirements: Unused. */
      Unburrow               = 0x76, /**< Unburrow unit.
                                      * Default Requirements: Unused. */
      DarkSwarm              = 0x77, /**< Cast Spell: Dark Swarm.
                                      * Default Requirements: Unused. */
      CastParasite           = 0x78, /**< Cast Spell: Parasite.
                                      * Default Requirements: Unused. */
      SummonBroodlings       = 0x79, /**< Cast Spell: Spawn Broodings.
                                      * Default Requirements: Unused. */
      EmpShockwave           = 0x7A, /**< Cast Spell: EMP Shockwave.
                                      * Default Requirements: Unused. */
      NukeWait               = 0x7B, /**< Unknown.
                                      * Default Requirements: Unit must be Terran Nuclear Missile. */
      NukeTrain              = 0x7C, /**< Silo Idle
                                      * Default Requirements: Unused. */
      NukeLaunch             = 0x7D, /**< Launch for nuclear missile.
                                      * Default Requirements: Unit must be Terran Nuclear Missile. */
      NukePaint              = 0x7E, /**< Move to and set nuke target.
                                      * Default Requirements: Unit must be Terran Ghost. */
      NukeUnit               = 0x7F, /**< Nuke the ground location of the unit.
                                      * Default Requirements: Unit must be Terran Nuclear Missile. */
      NukeGround             = 0x80, /**< Nuke ground.
                                      * Default Requirements: Unit must be Terran Nuclear Missile. */
      NukeTrack              = 0x81, /**< Ghost order during nuke.
                                      * Default Requirements: Unit must be Terran Nuclear Missile. */
      InitArbiter            = 0x82, /**< Run nearby cloaking.
                                      * Default Requirements: Unit must be Protoss Arbiter/Danimoth. Allow on Hallucinated units. */
      CloakNearbyUnits       = 0x83, /**< Cloak non arbiters within range.
                                      * Default Requirements: Unit must be Protoss Arbiter/Danimoth. */
      PlaceMine              = 0x84, /**< Place spider mine.
                                      * Default Requirements: Unused. */
      Rightclickaction       = 0x85, /**< right click, sets correct order based on target.
                                      * Default Requirements: Allow on Hallucinated units. */
      SapUnit                = 0x86, /**< Suicide Attack Unit.
                                      * Default Requirements: Unit must be Zerg infested Terran. Allow on Hallucinated units. */
      SapLocation            = 0x87, /**< Suicide Attack tile.
                                      * Default Requirements: Unit must be Zerg infested Terran. Allow on Hallucinated units. */
      HoldPosition4          = 0x88, /**< Suicide Hold Position.
                                      * Default Requirements: Unit must be Zerg Infested Terran or Zerg Scourge. Allow on Hallucinated units. */
      Teleport               = 0x89, /**< Recall(units within range of target pos).
                                      * Default Requirements: Unused. */
      TeleporttoLocation     = 0x8A, /**< Causes units to teleport when being recalled.
                                      * Default Requirements: Unused. */
      PlaceScanner           = 0x8B, /**< Place Scanner Sweep Unit at position.
                                      * Default Requirements: Unused. */
      Scanner                = 0x8C, /**< Scanner Sweep Unit idle.
                                      * Default Requirements: Unit must be Scanner Sweep. */
      DefensiveMatrix        = 0x8D, /**< Defensive Matrix cast on target.
                                      * Default Requirements: Unused. */
      PsiStorm               = 0x8E, /**< Cast Spell: Psi Storm.
                                      * Default Requirements: Unused. */
      Irradiate              = 0x8F, /**< Cast Spell: Irradiate.
                                      * Default Requirements: Unused. */
      Plague                 = 0x90, /**< Cast Spell: Plague.
                                      * Default Requirements: Unused. */
      Consume                = 0x91, /**< Cast Spell: Consume.
                                      * Default Requirements: Unused. */
      Ensnare                = 0x92, /**< Cast Spell: Ensnare.
                                      * Default Requirements: Unused. */
      StasisField            = 0x93, /**< Cast Spell: Stasis Field.
                                      * Default Requirements: Unused. */
      Hallucianation1        = 0x94, /**< Hallucination Cast on target.
                                      * Default Requirements: Unused. */
      Hallucination2         = 0x95, /**< Kill Halluciation on spell cast.
                                      * Default Requirements: Unused. */
      ResetCollision1        = 0x96, /**< Collision Reset between 2 units.
                                      * Default Requirements: Unused. */
      ResetCollision2        = 0x97, /**< Collision reset between harvester and mine.
                                      * Default Requirements: Unused. */
      Patrol                 = 0x98, /**< Patrol to target, queue patrol to original position.
                                      * Default Requirements: Unit is able to move. Allow on Hallucinated units. */
      CTFCOPInit             = 0x99, /**< CTF Initialization
                                      * Default Requirements: Unused. */
      CTFCOP1                = 0x9A, /**< CTF Idle
                                      * Default Requirements: Unused. */
      CTFCOP2                = 0x9B, /**< Unknown? Reset COP?
                                      * Default Requirements: Unit must be Zerg Flag Beacon, Terran Flag Beacon, or Protoss Flag Beacon. */
      ComputerAI             = 0x9C, /**< AI Control.
                                      * Default Requirements: Unused. */
      AtkMoveEP              = 0x9D, /**< AI Attack Move?
                                      * Default Requirements: Unit is able to move. Allow on Hallucinated units. */
      HarassMove             = 0x9E, /**< Aggressive Attack Move? Units wonâ‚Žâ„t give up on a target. If they see it, theyâ‚Žâ„ll attack it, even worse than attack move. Might be a computer attack move?
                                      * Default Requirements: Unused. */
      AIPatrol               = 0x9F, /**< Moves units to the center of the current â‚ŽË¶areaâ‚Žâ„ they are at? Not sure if the spacing is meant to allow for detectors to cover an area or not.
                                      * Default Requirements: Unused. */
      GuardPost              = 0xA0, /**< Immobile Unit Guard.
                                      * Default Requirements: Unused. */
      RescuePassive          = 0xA1, /**< Rescuable unit idle.
                                      * Default Requirements: Unused. */
      Neutral                = 0xA2, /**< Neutral Unit idle.
                                      * Default Requirements: Unused. */
      ComputerReturn         = 0xA3, /**< Return computer units to defensive position? Was seen returning units that had followed a unit outside of a base and killed it.
                                      * Default Requirements: Unused. */
      InitPsiProvider        = 0xA4, /**< Init Psi Provider. Adds to some kind of linked list.
                                      * Default Requirements: Unused. */
      SelfDestrucing         = 0xA5, /**< Remove unit.
                                      * Default Requirements: Unit must be Protoss Scarab. */
      Critter                = 0xA6, /**< Critter idle.
                                      * Default Requirements: Unit must be Rhynadon, Bengalaas, Ragnasaur, Scantid, Kakaru, or Ursadon. Allow on hallucinated units. */
      HiddenGun              = 0xA7, /**< Trap idle order.
                                      * Default Requirements: Unit must be Floor Gun Trap, Left Wall Missile Trap, Left Wall Flame Trap, Right Wall Missile Trap, or Right Wall Flame Trap. */
      OpenDoor               = 0xA8, /**< Opens the door.
                                      * Default Requirements: Unit must be Left Upper Level Door, Right Upper Level Door, Left Pit Door, or Right pit Door. */
      CloseDoor              = 0xA9, /**< Closes the door.
                                      * Default Requirements: Unit must be Left Upper Level Door, Right Upper Level Door, Left Pit Door, or Right pit Door. */
      HideTrap               = 0xAA, /**< Trap return to idle.
                                      * Default Requirements: Unit must be Floor Missile Trap, Floor Gun Trap, Left Wall Missile Trap, Left Wall Flame Trap, Right Wall Missile Trap, or Right Wall Flame Trap. */
      RevealTrap             = 0xAB, /**< Trap attack.
                                      * Default Requirements: Unit must be Floor Missile Trap, Floor Gun Trap, Left Wall Missile Trap, Left Wall Flame Trap, Right Wall Missile Trap, or Right Wall Flame Trap. */
      Enabledoodad           = 0xAC, /**< Enable Doodad State.
                                      * Default Requirements: Unused. */
      Disabledoodad          = 0xAD, /**< Disable Doodad State.
                                      * Default Requirements: Unused. */
      Warpin                 = 0xAE, /**< Unused. Left over from unit warp in which now exists in Starcraft 2.
                                      * Default Requirements: Unused. */
      Medic                  = 0xAF, /**< Idle command for the Terran Medic.
                                      * Default Requirements: Unit must be Terran Medic. */
      MedicHeal1             = 0xB0, /**< Heal cast on target.
                                      * Default Requirements: Unit must be Terran Medic. */
      HealMove               = 0xB1, /**< Attack move command for the Terran Medic.
                                      * Default Requirements: Unit must be Terran Medic. */
      MedicHoldPosition      = 0xB2, /**< Holds Position for Terran Medics, heals units within range.
                                      * Default Requirements: Unit must be Terran Medic. */
      MedicHeal2             = 0xB3, /**< Return to idle after heal.
                                      * Default Requirements: Unused. */
      Restoration            = 0xB4, /**< Cast Spell: Restoration.
                                      * Default Requirements: Unused. */
      CastDisruptionWeb      = 0xB5, /**< Cast Spell: Disruption Web.
                                      * Default Requirements: Unused. */
      CastMindControl        = 0xB6, /**< Mind Control Cast on Target.
                                      * Default Requirements: Unit must be Protoss Dark Archon. */
      WarpingDarkArchon      = 0xB7, /**< Dark Archon Meld.
                                      * Default Requirements: Unused. */
      CastFeedback           = 0xB8, /**< Feedback cast on target.
                                      * Default Requirements: Unused. */
      CastOpticalFlare       = 0xB9, /**< Cast Spell: Optical Flare.
                                      * Default Requirements: Unused. */
      CastMaelstrom          = 0xBA, /**< Cast Spell: Maelstrom.
                                      * Default Requirements: Unused. */
      JunkYardDog            = 0xBB, /**< Junk yard dog movement.
                                      * Default Requirements: Unused. */
      Fatal                  = 0xBC, /**< Nothing.
                                      * Default Requirements: Unused. */
    };

    static const std::string orderName(OrderID::Enum order)
    {
      switch (order)
      {
        case Die : return "Die";
        case Stop : return "Stop";
        case Guard : return "Guard";
        case PlayerGuard : return "Player Guard";
        case TurretGuard : return "Turret Guard";
        case BunkerGuard : return "Bunker Guard";
        case Move : return "Move";
        case ReaverStop : return "Reaver Stop";
        case Attack1 : return "Attack1";
        case Attack2 : return "Attack2";
        case AttackUnit : return "Attack Unit";
        case AttackFixedRange : return "Attack Fixed Range";
        case AttackTile : return "Attack Tile";
        case Hover : return "Hover";
        case AttackMove : return "Attack Move";
        case InfestMine1 : return "Infest Mine1";
        case Nothing1 : return "Nothing1";
        case Powerup1 : return "Powerup1";
        case TowerGuard : return "Tower Guard";
        case TowerAttack : return "Tower Attack";
        case VultureMine : return "Vulture Mine";
        case StayinRange : return "Stayin Range";
        case TurretAttack : return "Turret Attack";
        case Nothing2 : return "Nothing2";
        case Nothing3 : return "Nothing3";
        case DroneStartBuild : return "Drone Start Build";
        case DroneBuild : return "Drone Build";
        case InfestMine2 : return "Infest Mine2";
        case InfestMine3 : return "Infest Mine3";
        case InfestMine4 : return "Infest Mine4";
        case BuildTerran : return "Build terran";
        case BuildProtoss1 : return "Build Protoss 1";
        case BuildProtoss2 : return "Build Protoss 2";
        case ConstructingBuilding : return "Constructing building";
        case Repair1 : return "Repair1";
        case Repair2 : return "Repair2";
        case PlaceAddon : return "Place Addon";
        case BuildAddon : return "*BuildAddon";
        case Train : return "*Train";
        case RallyPoint1 : return "Rally Point1";
        case RallyPoint2 : return "Rally Point2";
        case ZergBirth : return "Zerg Birth";
        case Morph1 : return "Morph1";
        case Morph2 : return "Morph2";
        case BuildSelf1 : return "Build Self1";
        case ZergBuildSelf : return "Zerg Build Self";
        case Build5 : return "Build5";
        case Enternyduscanal : return "Enternyduscanal";
        case BuildSelf2 : return "Build Self2";
        case Follow : return "Follow";
        case Carrier : return "Carrier";
        case CarrierIgnore1 : return "Carrier Ignore1";
        case CarrierStop : return "Carrier Stop";
        case CarrierAttack1 : return "Carrier Attack1";
        case CarrierAttack2 : return "Carrier Attack2";
        case CarrierIgnore2 : return "Carrier Ignore2";
        case CarrierFight : return "Carrier Fight";
        case HoldPosition1 : return "Hold Position1";
        case Reaver : return "Reaver";
        case ReaverAttack1 : return "Reaver Attack1";
        case ReaverAttack2 : return "Reaver Attack2";
        case ReaverFight : return "Reaver Fight";
        case ReaverHold : return "Reaver Hold";
        case TrainFighter : return "*TrainFighter";
        case StrafeUnit1 : return "Strafe Unit1";
        case StrafeUnit2 : return "Strafe Unit2";
        case RechargeShields1 : return "Recharge Shields1";
        case Rechargeshields2 : return "Rechargeshields2";
        case ShieldBattery : return "*ShieldBattery";
        case Return : return "Return";
        case DroneLand : return "Drone Land";
        case BuildingLand : return "Building Land";
        case BuildingLiftoff : return "Building Liftoff";
        case DroneLiftoff : return "Drone Liftoff";
        case Liftoff : return "Liftoff";
        case ResearchTech : return "Research Tech";
        case Upgrade : return "Upgrade";
        case Larva : return "Larva";
        case SpawningLarva : return "*SpawningLarva";
        case Harvest1 : return "Harvest1";
        case Harvest2 : return "Harvest2";
        case HarvestGas1 : return "Harvest Gas1";
        case HarvestGas2 : return "Harvest Gas2";
        case HarvestGas3 : return "Harvest Gas3";
        case ReturnGas : return "Return Gas";
        case MoveToMinerals : return "Harvest Minerals1";
        case HarvestMinerals2 : return "Harvest Minerals2";
        case MiningMinerals : return "Harvest Minerals3";
        case Harvest3 : return "Harvest3";
        case Harvest4 : return "Harvest4";
        case ReturnMinerals : return "Return Minerals";
        case Harvest5 : return "Harvest5";
        case EnterTransport : return "Enter Transport";
        case Pickup1 : return "Pickup1";
        case Pickup2 : return "Pickup2";
        case Pickup3 : return "Pickup3";
        case Pickup4 : return "Pickup4";
        case Powerup2 : return "Powerup2";
        case SiegeMode : return "Siege Mode";
        case TankMode : return "Tank Mode";
        case WatchTarget : return "Watch Target";
        case InitCreepGrowth : return "Init Creep Growth";
        case SpreadCreep : return "*SpreadCreep";
        case StoppingCreepGrowth : return "Stopping Creep Growth";
        case GuardianAspect : return "Guardian Aspect";
        case WarpingArchon : return "Warping Archon";
        case CompletingArchonsummon : return "Completing Archonsummon";
        case HoldPosition2 : return "Hold Position2";
        case HoldPosition3 : return "Hold Position3";
        case Cloak : return "*Cloak";
        case Decloak : return "*Decloak";
        case Unload : return "Unload";
        case MoveUnload : return "Move Unload";
        case FireYamatoGun1 : return "Fire Yamato Gun1";
        case FireYamatoGun2 : return "Fire Yamato Gun2";
        case MagnaPulse : return "Magna Pulse";
        case Burrow : return "Burrow";
        case Burrowed : return "Burrowed";
        case Unburrow : return "Unburrow";
        case DarkSwarm : return "Dark Swarm";
        case CastParasite : return "Cast Parasite";
        case SummonBroodlings : return "Summon Broodlings";
        case EmpShockwave : return "Emp Shockwave";
        case NukeWait : return "Nuke Wait";
        case NukeTrain : return "Nuke Train";
        case NukeLaunch : return "Nuke Launch";
        case NukePaint : return "Nuke Paint";
        case NukeUnit : return "Nuke Unit";
        case NukeGround : return "Nuke Ground";
        case NukeTrack : return "Nuke Track";
        case InitArbiter : return "Init Arbiter";
        case CloakNearbyUnits : return "*CloakNearbyUnits";
        case PlaceMine : return "Place Mine";
        case Rightclickaction : return "Rightclickaction";
        case SapUnit : return "Sap Unit";
        case SapLocation : return "Sap Location";
        case HoldPosition4 : return "Hold Position4";
        case Teleport : return "Teleport";
        case TeleporttoLocation : return "Teleportto Location";
        case PlaceScanner : return "Place Scanner";
        case Scanner : return "Scanner";
        case DefensiveMatrix : return "Defensive Matrix";
        case PsiStorm : return "Psi Storm";
        case Irradiate : return "Irradiate";
        case Plague : return "Plague";
        case Consume : return "Consume";
        case Ensnare : return "Ensnare";
        case StasisField : return "Stasis Field";
        case Hallucianation1 : return "Hallucianation1";
        case Hallucination2 : return "*Hallucination2";
        case ResetCollision1 : return "Reset Collision1";
        case ResetCollision2 : return "Reset Collision2";
        case Patrol : return "Patrol";
        case CTFCOPInit : return "CTFCOPInit";
        case CTFCOP1 : return "CTFCOP1";
        case CTFCOP2 : return "CTFCOP2";
        case ComputerAI : return "Computer AI";
        case AtkMoveEP : return "Atk Move EP";
        case HarassMove : return "Harass Move";
        case AIPatrol : return "AIPatrol";
        case GuardPost : return "Guard Post";
        case RescuePassive : return "Rescue Passive";
        case Neutral : return "Neutral";
        case ComputerReturn : return "Computer Return";
        case InitPsiProvider : return "Init Psi Provider";
        case SelfDestrucing : return "Self Destrucing";
        case Critter : return "Critter";
        case HiddenGun : return "Hidden Gun";
        case OpenDoor : return "Open Door";
        case CloseDoor : return "Close Door";
        case HideTrap : return "Hide Trap";
        case RevealTrap : return "Reveal Trap";
        case Enabledoodad : return "Enabledoodad";
        case Disabledoodad : return "Disabledoodad";
        case Warpin : return "Warpin";
        case Medic : return "Medic";
        case MedicHeal1 : return "Medic Heal1";
        case HealMove : return "Heal Move";
        case MedicHoldPosition : return "Medic Hold Position";
        case MedicHeal2 : return "Medic Heal2";
        case Restoration : return "Restoration";
        case CastDisruptionWeb : return "Cast Disruption Web";
        case CastMindControl : return "Cast Mind Control";
        case WarpingDarkArchon : return "Warping Dark Archon";
        case CastFeedback : return "Cast Feedback";
        case CastOpticalFlare : return "Cast Optical Flare";
        case CastMaelstrom : return "Cast Maelstrom";
        case JunkYardDog : return "Junk Yard Dog";
        case Fatal : return "Fatal";
        default :
        {
          char message[50];
          sprintf_s(message, 50, "Unknown command %d", order);
          return message;
        }
      }
    }
  };
};
