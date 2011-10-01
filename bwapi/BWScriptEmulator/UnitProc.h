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
  void EmulateIdleOrder();
  void execute();

  int       iEmulatedOrder;
  Position  guardLocation;
  Position  destLocation;
  Unit      *thisUnit;
  bool      initialized;
  int       dwState;

};

UnitProc *getUnitProc(BWAPI::Unit *pUnit);
