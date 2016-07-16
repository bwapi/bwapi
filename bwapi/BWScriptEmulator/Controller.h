#pragma once
#include <windows.h>
#include <BWAPI.h>

#include "Location.h"

#define CONTROLLER_SECURE_FINISHED    0x0001
#define CONTROLLER_TOWN_STARTED       0x0002
#define CONTROLLER_DEFAULT_BUILD_OFF  0x0004
#define CONTROLLER_TRANSPORTS_OFF     0x0008
#define CONTROLLER_FARMS_TIMING_OFF   0x0010
#define CONTROLLER_IS_CAMPAIGN        0x0020
#define CONTROLLER_DOING_FARMS        0x0040
#define CONTROLLER_SPREAD_CREEP       0x0080
#define CONTROLLER_REQUEST_DETECTOR   0x0100
#define CONTROLLER_SENT_SUICIDE       0x0200
#define CONTROLLER_UPGRADES_FINISHED  0x0400
#define CONTROLLER_TARGET_EXPANSION   0x0800

#define DEFENSE_GROUND_TO_GROUND  0
#define DEFENSE_AIR_TO_GROUND     1
#define DEFENSE_GROUND_TO_AIR     2
#define DEFENSE_AIR_TO_AIR        3

class AIController
{
public:
  AIController();
  ~AIController();

  bool IsCampaign() const;
  int GetCastingCooldown() const;
  void SetCastingCooldown(int time);


  void AttackAdd(int count, BWAPI::UnitType type);
  void DefenseClear(int type);
  void DefenseBuild(int type, int count, BWAPI::UnitType uType);
  void DefenseUse(int type, int count, BWAPI::UnitType uType);

  WORD getFlags() const;
  void  setFlags(WORD wFlags);
  void  clearFlags(WORD wFlags);

  BYTE  bNukeRate;
  BYTE  bAttacks;
  DWORD dwLastNukeTime;
  WORD  wFlags;
  WORD  wPanicBlock;
  WORD  wMaxForce;
  WORD  wAttackGroup;
  WORD  wWaitForType;
  BYTE  bDefaultMin;

  int castingCooldown;
  
  DWORD dwLastIndividualUnitUpdateTime;
  DWORD dwAttackTime;
  
  BYTE  bIfDif;
  WORD  attackGroups[64];
  DWORD defensebuild[4][10];
  DWORD defenseuse[4][10];
  BYTE  bDefineMax[228];

  Location genCmdTarget;
};

extern AIController MainController;
