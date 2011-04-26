#pragma once
#include <BWAPI.h>
#include <set>
#include <vector>

using namespace BWAPI;

typedef struct __spell
{
  Unit      *pUnit;
  TechType  tech;
} spell;

extern std::vector<spell> spellsCast;

class UnitProc
{
public:
  UnitProc();
  UnitProc(Unit *pUnit);
  ~UnitProc();
  void Init();
  void Init(Unit *pUnit);
  void OnIdle();
  bool HealSelf();
  bool ReturnToGuard();
  bool CanUse(TechType tech);
  bool CanHit(WeaponType wpn, Unit *target);
  bool WorthHitting(Unit *target);
  bool CheckForSpellcasting();
  bool UseAbility(TechType tech, Unit *target);
  bool AbilityInUse(TechType tech, Unit *target);

  Position  guardLocation;
  Position  destLocation;
  Unit      *thisUnit;
  bool      initialized;
  int       dwState;

  virtual bool GenericIdle();
};
