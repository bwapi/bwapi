#pragma once
#include <BWAPI.h>
#include <set>
#include <vector>

using namespace BWAPI;

class UnitProc
{
public:
  UnitProc(Unit *pUnit = NULL);
  ~UnitProc();
  void Init(Unit *pUnit = NULL);
  void EmulateIdleOrder();
  void execute();
  void StandardUnitProc();

  int       iEmulatedOrder;
  Position  guardLocation;
  Position  destLocation;
  Unit      *thisUnit;
  bool      initialized;
  int       dwState;
  int       aiCaptain;
};

UnitProc *getUnitProc(BWAPI::Unit *pUnit);
