#include <string>
#include <map>
#include <set>
#include <BWAPITypes\WeaponType.h>
#include <BWAPITypes\WeaponTypeId.h>
#include <BWAPITypes\UnitTypeId.h>
#include <BWAPITypes\TechTypeId.h>
#include <BWAPITypes\UpgradeTypeId.h>
#include <BWAPITypes\DamageTypeId.h>
#include <BWAPITypes\ExplosionTypeId.h>

namespace BWAPI
{
  WeaponType weaponTypeData[WeaponTypeIds::count];
  std::map<std::string, WeaponTypeId> weaponTypeMap;
  std::set< WeaponTypeId > weaponTypeSet;
  std::set< WeaponTypeId > specialWeaponTypeSet;
  std::set< WeaponTypeId > normalWeaponTypeSet;
  void fillWeaponType(WeaponTypeId id, const char* name, TechTypeId techType, int damageAmount, int damageBonus, int damageCooldown, int damageFactor, UpgradeTypeId upgradeType, DamageTypeId damageType, ExplosionTypeId explosionType, int minRange, int maxRange, int innerSplashRadius, int medianSplashRadius, int outerSplashRadius, bool targetsAir, bool targetsGround, bool targetsMechanical, bool targetsOrganic, bool targetsNonBuilding, bool targetsNonRobotic, bool targetsTerrain, bool targetsOrgOrMech, bool targetsOwn, UnitTypeId whatUses)
  {
    WeaponType &target = weaponTypeData[id];
    target.name = name;
    target.techType = techType;
    target.damageAmount = damageAmount;
    target.damageBonus = damageBonus;
    target.damageCooldown = damageCooldown;
    target.damageFactor = damageFactor;
    target.upgradeType = upgradeType;
    target.damageType = damageType;
    target.explosionType = explosionType;
    target.minRange = minRange;
    target.maxRange = maxRange;
    target.innerSplashRadius = innerSplashRadius;
    target.medianSplashRadius = medianSplashRadius;
    target.outerSplashRadius = outerSplashRadius;
    target.targetsAir = targetsAir;
    target.targetsGround = targetsGround;
    target.targetsMechanical = targetsMechanical;
    target.targetsOrganic = targetsOrganic;
    target.targetsNonBuilding = targetsNonBuilding;
    target.targetsNonRobotic = targetsNonRobotic;
    target.targetsTerrain = targetsTerrain;
    target.targetsOrgOrMech = targetsOrgOrMech;
    target.targetsOwn = targetsOwn;
    target.whatUses = whatUses;
    target.valid = true;
  }
  namespace WeaponTypes
  {
    void init()
    {
      fillWeaponType(WeaponTypeIds::Gauss_Rifle, "Gauss Rifle", TechTypeIds::None, 6, 1, 15, 1, UpgradeTypeIds::Terran_Infantry_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Marine);
      fillWeaponType(WeaponTypeIds::C_10_Canister_Rifle, "C-10 Canister Rifle", TechTypeIds::None, 10, 1, 22, 1, UpgradeTypeIds::Terran_Infantry_Weapons, DamageTypeIds::Concussive, ExplosionTypeIds::Normal, 0, 224, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Ghost);
      fillWeaponType(WeaponTypeIds::Fragmentation_Grenade, "Fragmentation Grenade", TechTypeIds::None, 20, 2, 30, 1, UpgradeTypeIds::Terran_Vehicle_Weapons, DamageTypeIds::Concussive, ExplosionTypeIds::Normal, 0, 160, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Vulture);
      fillWeaponType(WeaponTypeIds::Spider_Mines, "Spider Mines", TechTypeIds::Spider_Mines, 125, 0, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Explosive, ExplosionTypeIds::Radial_Splash, 0, 10, 50, 75, 100, 0, 1, 0, 0, 1, 0, 0, 0, 0, UnitTypeIds::Terran_Vulture_Spider_Mine);
      fillWeaponType(WeaponTypeIds::Twin_Autocannons, "Twin Autocannons", TechTypeIds::None, 12, 1, 22, 1, UpgradeTypeIds::Terran_Vehicle_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 192, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Goliath);
      fillWeaponType(WeaponTypeIds::Hellfire_Missile_Pack, "Hellfire Missile Pack", TechTypeIds::None, 10, 2, 22, 2, UpgradeTypeIds::Terran_Vehicle_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 160, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Goliath);
      fillWeaponType(WeaponTypeIds::Arclite_Cannon, "Arclite Cannon", TechTypeIds::None, 30, 3, 37, 1, UpgradeTypeIds::Terran_Vehicle_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 224, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Siege_Tank_Tank_Mode);
      fillWeaponType(WeaponTypeIds::Fusion_Cutter, "Fusion Cutter", TechTypeIds::None, 5, 1, 15, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 10, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_SCV);
      fillWeaponType(WeaponTypeIds::Gemini_Missiles, "Gemini Missiles", TechTypeIds::None, 20, 2, 22, 1, UpgradeTypeIds::Terran_Ship_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 160, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Wraith);
      fillWeaponType(WeaponTypeIds::Burst_Lasers, "Burst Lasers", TechTypeIds::None, 8, 1, 30, 1, UpgradeTypeIds::Terran_Ship_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 160, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Wraith);
      fillWeaponType(WeaponTypeIds::ATS_Laser_Battery, "ATS Laser Battery", TechTypeIds::None, 25, 3, 30, 1, UpgradeTypeIds::Terran_Ship_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 192, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Battlecruiser);
      fillWeaponType(WeaponTypeIds::ATA_Laser_Battery, "ATA Laser Battery", TechTypeIds::None, 25, 3, 30, 1, UpgradeTypeIds::Terran_Ship_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 192, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Battlecruiser);
      fillWeaponType(WeaponTypeIds::Flame_Thrower, "Flame Thrower", TechTypeIds::None, 8, 1, 22, 1, UpgradeTypeIds::Terran_Infantry_Weapons, DamageTypeIds::Concussive, ExplosionTypeIds::Enemy_Splash, 0, 32, 15, 20, 25, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Firebat);
      fillWeaponType(WeaponTypeIds::Arclite_Shock_Cannon, "Arclite Shock Cannon", TechTypeIds::None, 70, 5, 75, 1, UpgradeTypeIds::Terran_Vehicle_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Radial_Splash, 64, 384, 10, 25, 40, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Siege_Tank_Siege_Mode);
      fillWeaponType(WeaponTypeIds::Longbolt_Missile, "Longbolt Missile", TechTypeIds::None, 20, 0, 15, 1, UpgradeTypeIds::None, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 224, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Missile_Turret);
      fillWeaponType(WeaponTypeIds::Yamato_Gun, "Yamato Gun", TechTypeIds::Yamato_Gun, 260, 0, 15, 1, UpgradeTypeIds::None, DamageTypeIds::Explosive, ExplosionTypeIds::Yamato_Gun, 0, 320, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Battlecruiser);
      fillWeaponType(WeaponTypeIds::Nuclear_Strike, "Nuclear Strike", TechTypeIds::Nuclear_Strike, 600, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Explosive, ExplosionTypeIds::Nuclear_Missile, 0, 3, 128, 192, 256, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Ghost);
      fillWeaponType(WeaponTypeIds::Lockdown, "Lockdown", TechTypeIds::Lockdown, 0, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Concussive, ExplosionTypeIds::Lockdown, 0, 256, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, UnitTypeIds::Terran_Ghost);
      fillWeaponType(WeaponTypeIds::EMP_Shockwave, "EMP Shockwave", TechTypeIds::EMP_Shockwave, 0, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Concussive, ExplosionTypeIds::EMP_Shockwave, 0, 256, 64, 64, 64, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypeIds::Terran_Science_Vessel);
      fillWeaponType(WeaponTypeIds::Irradiate, "Irradiate", TechTypeIds::Irradiate, 250, 0, 75, 1, UpgradeTypeIds::None, DamageTypeIds::Ignore_Armor, ExplosionTypeIds::Irradiate, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypeIds::Terran_Science_Vessel);
      fillWeaponType(WeaponTypeIds::Claws, "Claws", TechTypeIds::None, 5, 1, 8, 1, UpgradeTypeIds::Zerg_Melee_Attacks, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Zergling);
      fillWeaponType(WeaponTypeIds::Needle_Spines, "Needle Spines", TechTypeIds::None, 10, 1, 15, 1, UpgradeTypeIds::Zerg_Missile_Attacks, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Hydralisk);
      fillWeaponType(WeaponTypeIds::Kaiser_Blades, "Kaiser Blades", TechTypeIds::None, 20, 3, 15, 1, UpgradeTypeIds::Zerg_Melee_Attacks, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 25, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Ultralisk);
      fillWeaponType(WeaponTypeIds::Toxic_Spores, "Toxic Spores", TechTypeIds::None, 4, 1, 15, 1, UpgradeTypeIds::Zerg_Melee_Attacks, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Broodling);
      fillWeaponType(WeaponTypeIds::Spines, "Spines", TechTypeIds::None, 5, 0, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Drone);
      fillWeaponType(WeaponTypeIds::Acid_Spore, "Acid Spore", TechTypeIds::None, 20, 2, 30, 1, UpgradeTypeIds::Zerg_Flyer_Attacks, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 256, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Guardian);
      fillWeaponType(WeaponTypeIds::Glave_Wurm, "Glave Wurm", TechTypeIds::None, 9, 1, 30, 1, UpgradeTypeIds::Zerg_Flyer_Attacks, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 96, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Mutalisk);
      fillWeaponType(WeaponTypeIds::Seeker_Spores, "Seeker Spores", TechTypeIds::None, 15, 0, 15, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 224, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Spore_Colony);
      fillWeaponType(WeaponTypeIds::Subterranean_Tentacle, "Subterranean Tentacle", TechTypeIds::None, 40, 0, 32, 1, UpgradeTypeIds::None, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 224, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Sunken_Colony);
      fillWeaponType(WeaponTypeIds::Suicide_Infested_Terran, "Suicide Infested Terran", TechTypeIds::None, 500, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Explosive, ExplosionTypeIds::Radial_Splash, 0, 3, 20, 40, 60, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Infested_Terran);
      fillWeaponType(WeaponTypeIds::Suicide_Scourge, "Suicide Scourge", TechTypeIds::None, 110, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Scourge);
      fillWeaponType(WeaponTypeIds::Parasite, "Parasite", TechTypeIds::Parasite, 0, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Parasite, 0, 384, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,UnitTypeIds::Zerg_Queen);
      fillWeaponType(WeaponTypeIds::Spawn_Broodlings, "Spawn Broodlings", TechTypeIds::Spawn_Broodlings, 0, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Broodlings, 0, 288, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0,UnitTypeIds::Zerg_Queen);
      fillWeaponType(WeaponTypeIds::Ensnare, "Ensnare", TechTypeIds::Ensnare, 0, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Ensnare, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0,UnitTypeIds::Zerg_Queen);
      fillWeaponType(WeaponTypeIds::Dark_Swarm, "Dark Swarm", TechTypeIds::Dark_Swarm, 0, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Dark_Swarm, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Defiler);
      fillWeaponType(WeaponTypeIds::Plague, "Plague", TechTypeIds::Plague, 300, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Plague, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0,UnitTypeIds::Zerg_Defiler);
      fillWeaponType(WeaponTypeIds::Consume, "Consume", TechTypeIds::Consume, 0, 0, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Consume, 0, 16, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1,UnitTypeIds::Zerg_Defiler);
      fillWeaponType(WeaponTypeIds::Particle_Beam, "Particle Beam", TechTypeIds::None, 5, 0, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Probe);
      fillWeaponType(WeaponTypeIds::Psi_Blades, "Psi Blades", TechTypeIds::None, 8, 1, 22, 1, UpgradeTypeIds::Protoss_Ground_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Zealot);
      fillWeaponType(WeaponTypeIds::Phase_Disruptor, "Phase Disruptor", TechTypeIds::None, 20, 2, 30, 1, UpgradeTypeIds::Protoss_Ground_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Dragoon);
      fillWeaponType(WeaponTypeIds::Psionic_Shockwave, "Psionic Shockwave", TechTypeIds::None, 30, 3, 20, 1, UpgradeTypeIds::Protoss_Ground_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Enemy_Splash, 0, 64, 3, 15, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Archon);
      fillWeaponType(WeaponTypeIds::Dual_Photon_Blasters, "Dual Photon Blasters", TechTypeIds::None, 8, 1, 30, 1, UpgradeTypeIds::Protoss_Air_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 128, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Scout);
      fillWeaponType(WeaponTypeIds::Anti_Matter_Missiles, "Anti-Matter Missiles", TechTypeIds::None, 14, 1, 22, 2, UpgradeTypeIds::Protoss_Air_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 128, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Scout);
      fillWeaponType(WeaponTypeIds::Phase_Disruptor_Cannon, "Phase Disruptor Cannon", TechTypeIds::None, 10, 1, 45, 1, UpgradeTypeIds::Protoss_Air_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Normal, 0, 160, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Arbiter);
      fillWeaponType(WeaponTypeIds::Pulse_Cannon, "Pulse Cannon", TechTypeIds::None, 6, 1, 1, 1, UpgradeTypeIds::Protoss_Air_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 128, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Interceptor);
      fillWeaponType(WeaponTypeIds::STS_Photon_Cannon, "STS Photon Cannon", TechTypeIds::None, 20, 0, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 224, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Photon_Cannon);
      fillWeaponType(WeaponTypeIds::STA_Photon_Cannon, "STA Photon Cannon", TechTypeIds::None, 20, 0, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 224, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Photon_Cannon);
      fillWeaponType(WeaponTypeIds::Scarab, "Scarab", TechTypeIds::None, 100, 25, 1, 1, UpgradeTypeIds::Scarab_Damage, DamageTypeIds::Normal, ExplosionTypeIds::Enemy_Splash, 0, 128, 20, 40, 60, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Scarab);
      fillWeaponType(WeaponTypeIds::Stasis_Field, "Stasis Field", TechTypeIds::Stasis_Field, 0, 1, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Stasis_Field, 0, 288, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypeIds::Protoss_Arbiter);
      fillWeaponType(WeaponTypeIds::Psionic_Storm, "Psionic Storm", TechTypeIds::Psionic_Storm, 14, 1, 45, 1, UpgradeTypeIds::None, DamageTypeIds::Ignore_Armor, ExplosionTypeIds::Radial_Splash, 0, 288, 48, 48, 48, 1, 1, 0, 0, 1, 0, 1, 0, 0,UnitTypeIds::Protoss_High_Templar);
      fillWeaponType(WeaponTypeIds::Neutron_Flare, "Neutron Flare", TechTypeIds::None, 5, 1, 8, 1, UpgradeTypeIds::Protoss_Air_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Air_Splash, 0, 160, 5, 50, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Corsair);
      fillWeaponType(WeaponTypeIds::Disruption_Web, "Disruption Web", TechTypeIds::Disruption_Web, 0, 0, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Ignore_Armor, ExplosionTypeIds::Disruption_Web, 0, 288, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Corsair);
      fillWeaponType(WeaponTypeIds::Restoration, "Restoration", TechTypeIds::Restoration, 20, 0, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Ignore_Armor, ExplosionTypeIds::Restoration, 0, 192, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypeIds::Terran_Medic);
      fillWeaponType(WeaponTypeIds::Halo_Rockets, "Halo Rockets", TechTypeIds::None, 6, 1, 64, 2, UpgradeTypeIds::Terran_Ship_Weapons, DamageTypeIds::Explosive, ExplosionTypeIds::Air_Splash, 0, 192, 5, 50, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Valkyrie);
      fillWeaponType(WeaponTypeIds::Corrosive_Acid, "Corrosive Acid", TechTypeIds::None, 25, 2, 100, 1, UpgradeTypeIds::Zerg_Flyer_Attacks, DamageTypeIds::Explosive, ExplosionTypeIds::Corrosive_Acid, 0, 192, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Devourer);
      fillWeaponType(WeaponTypeIds::Mind_Control, "Mind Control", TechTypeIds::Mind_Control, 8, 1, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Normal, ExplosionTypeIds::Mind_Control, 0, 256, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypeIds::Protoss_Dark_Archon);
      fillWeaponType(WeaponTypeIds::Feedback, "Feedback", TechTypeIds::Feedback, 8, 1, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Ignore_Armor, ExplosionTypeIds::Feedback, 0, 320, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypeIds::Protoss_Dark_Archon);
      fillWeaponType(WeaponTypeIds::Optical_Flare, "Optical Flare", TechTypeIds::Optical_Flare, 8, 1, 22, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Optical_Flare, 0, 288, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Terran_Medic);
      fillWeaponType(WeaponTypeIds::Maelstrom, "Maelstrom", TechTypeIds::Maelstrom, 0, 1, 1, 1, UpgradeTypeIds::None, DamageTypeIds::Independent, ExplosionTypeIds::Maelstrom, 0, 320, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, UnitTypeIds::Protoss_Dark_Archon);
      fillWeaponType(WeaponTypeIds::Subterranean_Spines, "Subterranean Spines", TechTypeIds::None, 20, 2, 37, 1, UpgradeTypeIds::Zerg_Missile_Attacks, DamageTypeIds::Normal, ExplosionTypeIds::Enemy_Splash, 0, 192, 20, 20, 20, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Zerg_Lurker);
      fillWeaponType(WeaponTypeIds::Warp_Blades, "Warp Blades", TechTypeIds::None, 40, 3, 30, 1, UpgradeTypeIds::Protoss_Ground_Weapons, DamageTypeIds::Normal, ExplosionTypeIds::Normal, 0, 15, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::Protoss_Dark_Templar);
      fillWeaponType(WeaponTypeIds::None, "None", TechTypeIds::None, 0, 0, 0, 0, UpgradeTypeIds::None, DamageTypeIds::None, ExplosionTypeIds::None, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, UnitTypeIds::None);

      weaponTypeSet.insert(WeaponTypeIds::Gauss_Rifle);
      weaponTypeSet.insert(WeaponTypeIds::C_10_Canister_Rifle);
      weaponTypeSet.insert(WeaponTypeIds::Fragmentation_Grenade);
      weaponTypeSet.insert(WeaponTypeIds::Spider_Mines);
      weaponTypeSet.insert(WeaponTypeIds::Twin_Autocannons);
      weaponTypeSet.insert(WeaponTypeIds::Hellfire_Missile_Pack);
      weaponTypeSet.insert(WeaponTypeIds::Arclite_Cannon);
      weaponTypeSet.insert(WeaponTypeIds::Fusion_Cutter);
      weaponTypeSet.insert(WeaponTypeIds::Gemini_Missiles);
      weaponTypeSet.insert(WeaponTypeIds::Burst_Lasers);
      weaponTypeSet.insert(WeaponTypeIds::ATS_Laser_Battery);
      weaponTypeSet.insert(WeaponTypeIds::ATA_Laser_Battery);
      weaponTypeSet.insert(WeaponTypeIds::Flame_Thrower);
      weaponTypeSet.insert(WeaponTypeIds::Arclite_Shock_Cannon);
      weaponTypeSet.insert(WeaponTypeIds::Longbolt_Missile);
      weaponTypeSet.insert(WeaponTypeIds::Claws);
      weaponTypeSet.insert(WeaponTypeIds::Needle_Spines);
      weaponTypeSet.insert(WeaponTypeIds::Kaiser_Blades);
      weaponTypeSet.insert(WeaponTypeIds::Toxic_Spores);
      weaponTypeSet.insert(WeaponTypeIds::Spines);
      weaponTypeSet.insert(WeaponTypeIds::Acid_Spore);
      weaponTypeSet.insert(WeaponTypeIds::Glave_Wurm);
      weaponTypeSet.insert(WeaponTypeIds::Seeker_Spores);
      weaponTypeSet.insert(WeaponTypeIds::Subterranean_Tentacle);
      weaponTypeSet.insert(WeaponTypeIds::Suicide_Infested_Terran);
      weaponTypeSet.insert(WeaponTypeIds::Suicide_Scourge);
      weaponTypeSet.insert(WeaponTypeIds::Particle_Beam);
      weaponTypeSet.insert(WeaponTypeIds::Psi_Blades);
      weaponTypeSet.insert(WeaponTypeIds::Phase_Disruptor);
      weaponTypeSet.insert(WeaponTypeIds::Psionic_Shockwave);
      weaponTypeSet.insert(WeaponTypeIds::Dual_Photon_Blasters);
      weaponTypeSet.insert(WeaponTypeIds::Anti_Matter_Missiles);
      weaponTypeSet.insert(WeaponTypeIds::Phase_Disruptor_Cannon);
      weaponTypeSet.insert(WeaponTypeIds::Pulse_Cannon);
      weaponTypeSet.insert(WeaponTypeIds::STS_Photon_Cannon);
      weaponTypeSet.insert(WeaponTypeIds::STA_Photon_Cannon);
      weaponTypeSet.insert(WeaponTypeIds::Scarab);
      weaponTypeSet.insert(WeaponTypeIds::Neutron_Flare);
      weaponTypeSet.insert(WeaponTypeIds::Halo_Rockets);
      weaponTypeSet.insert(WeaponTypeIds::Corrosive_Acid);
      weaponTypeSet.insert(WeaponTypeIds::Subterranean_Spines);
      weaponTypeSet.insert(WeaponTypeIds::Warp_Blades);
      normalWeaponTypeSet.insert(WeaponTypeIds::Gauss_Rifle);
      normalWeaponTypeSet.insert(WeaponTypeIds::C_10_Canister_Rifle);
      normalWeaponTypeSet.insert(WeaponTypeIds::Fragmentation_Grenade);
      normalWeaponTypeSet.insert(WeaponTypeIds::Spider_Mines);
      normalWeaponTypeSet.insert(WeaponTypeIds::Twin_Autocannons);
      normalWeaponTypeSet.insert(WeaponTypeIds::Hellfire_Missile_Pack);
      normalWeaponTypeSet.insert(WeaponTypeIds::Arclite_Cannon);
      normalWeaponTypeSet.insert(WeaponTypeIds::Fusion_Cutter);
      normalWeaponTypeSet.insert(WeaponTypeIds::Gemini_Missiles);
      normalWeaponTypeSet.insert(WeaponTypeIds::Burst_Lasers);
      normalWeaponTypeSet.insert(WeaponTypeIds::ATS_Laser_Battery);
      normalWeaponTypeSet.insert(WeaponTypeIds::ATA_Laser_Battery);
      normalWeaponTypeSet.insert(WeaponTypeIds::Flame_Thrower);
      normalWeaponTypeSet.insert(WeaponTypeIds::Arclite_Shock_Cannon);
      normalWeaponTypeSet.insert(WeaponTypeIds::Longbolt_Missile);
      normalWeaponTypeSet.insert(WeaponTypeIds::Claws);
      normalWeaponTypeSet.insert(WeaponTypeIds::Needle_Spines);
      normalWeaponTypeSet.insert(WeaponTypeIds::Kaiser_Blades);
      normalWeaponTypeSet.insert(WeaponTypeIds::Toxic_Spores);
      normalWeaponTypeSet.insert(WeaponTypeIds::Spines);
      normalWeaponTypeSet.insert(WeaponTypeIds::Acid_Spore);
      normalWeaponTypeSet.insert(WeaponTypeIds::Glave_Wurm);
      normalWeaponTypeSet.insert(WeaponTypeIds::Seeker_Spores);
      normalWeaponTypeSet.insert(WeaponTypeIds::Subterranean_Tentacle);
      normalWeaponTypeSet.insert(WeaponTypeIds::Suicide_Infested_Terran);
      normalWeaponTypeSet.insert(WeaponTypeIds::Suicide_Scourge);
      normalWeaponTypeSet.insert(WeaponTypeIds::Particle_Beam);
      normalWeaponTypeSet.insert(WeaponTypeIds::Psi_Blades);
      normalWeaponTypeSet.insert(WeaponTypeIds::Phase_Disruptor);
      normalWeaponTypeSet.insert(WeaponTypeIds::Psionic_Shockwave);
      normalWeaponTypeSet.insert(WeaponTypeIds::Dual_Photon_Blasters);
      normalWeaponTypeSet.insert(WeaponTypeIds::Anti_Matter_Missiles);
      normalWeaponTypeSet.insert(WeaponTypeIds::Phase_Disruptor_Cannon);
      normalWeaponTypeSet.insert(WeaponTypeIds::Pulse_Cannon);
      normalWeaponTypeSet.insert(WeaponTypeIds::STS_Photon_Cannon);
      normalWeaponTypeSet.insert(WeaponTypeIds::STA_Photon_Cannon);
      normalWeaponTypeSet.insert(WeaponTypeIds::Scarab);
      normalWeaponTypeSet.insert(WeaponTypeIds::Neutron_Flare);
      normalWeaponTypeSet.insert(WeaponTypeIds::Halo_Rockets);
      normalWeaponTypeSet.insert(WeaponTypeIds::Corrosive_Acid);
      normalWeaponTypeSet.insert(WeaponTypeIds::Subterranean_Spines);
      normalWeaponTypeSet.insert(WeaponTypeIds::Warp_Blades);
      weaponTypeSet.insert(WeaponTypeIds::Yamato_Gun);
      weaponTypeSet.insert(WeaponTypeIds::Nuclear_Strike);
      weaponTypeSet.insert(WeaponTypeIds::Lockdown);
      weaponTypeSet.insert(WeaponTypeIds::EMP_Shockwave);
      weaponTypeSet.insert(WeaponTypeIds::Irradiate);
      weaponTypeSet.insert(WeaponTypeIds::Parasite);
      weaponTypeSet.insert(WeaponTypeIds::Spawn_Broodlings);
      weaponTypeSet.insert(WeaponTypeIds::Ensnare);
      weaponTypeSet.insert(WeaponTypeIds::Dark_Swarm);
      weaponTypeSet.insert(WeaponTypeIds::Plague);
      weaponTypeSet.insert(WeaponTypeIds::Consume);
      weaponTypeSet.insert(WeaponTypeIds::Stasis_Field);
      weaponTypeSet.insert(WeaponTypeIds::Psionic_Storm);
      weaponTypeSet.insert(WeaponTypeIds::Disruption_Web);
      weaponTypeSet.insert(WeaponTypeIds::Restoration);
      weaponTypeSet.insert(WeaponTypeIds::Mind_Control);
      weaponTypeSet.insert(WeaponTypeIds::Feedback);
      weaponTypeSet.insert(WeaponTypeIds::Optical_Flare);
      weaponTypeSet.insert(WeaponTypeIds::Maelstrom);
      specialWeaponTypeSet.insert(WeaponTypeIds::Yamato_Gun);
      specialWeaponTypeSet.insert(WeaponTypeIds::Nuclear_Strike);
      specialWeaponTypeSet.insert(WeaponTypeIds::Lockdown);
      specialWeaponTypeSet.insert(WeaponTypeIds::EMP_Shockwave);
      specialWeaponTypeSet.insert(WeaponTypeIds::Irradiate);
      specialWeaponTypeSet.insert(WeaponTypeIds::Parasite);
      specialWeaponTypeSet.insert(WeaponTypeIds::Spawn_Broodlings);
      specialWeaponTypeSet.insert(WeaponTypeIds::Ensnare);
      specialWeaponTypeSet.insert(WeaponTypeIds::Dark_Swarm);
      specialWeaponTypeSet.insert(WeaponTypeIds::Plague);
      specialWeaponTypeSet.insert(WeaponTypeIds::Consume);
      specialWeaponTypeSet.insert(WeaponTypeIds::Stasis_Field);
      specialWeaponTypeSet.insert(WeaponTypeIds::Psionic_Storm);
      specialWeaponTypeSet.insert(WeaponTypeIds::Disruption_Web);
      specialWeaponTypeSet.insert(WeaponTypeIds::Restoration);
      specialWeaponTypeSet.insert(WeaponTypeIds::Mind_Control);
      specialWeaponTypeSet.insert(WeaponTypeIds::Feedback);
      specialWeaponTypeSet.insert(WeaponTypeIds::Optical_Flare);
      specialWeaponTypeSet.insert(WeaponTypeIds::Maelstrom);

      weaponTypeSet.insert(WeaponTypeIds::None);
      for(std::set<WeaponTypeId>::iterator i = weaponTypeSet.begin(); i != weaponTypeSet.end(); i++)
      {
        weaponTypeMap.insert(std::make_pair(std::string(weaponTypeData[*i].name), *i));
      }
    }
  }
}
