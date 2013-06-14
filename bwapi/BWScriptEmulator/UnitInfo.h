#pragma once
#include <BWAPI.h>
using namespace BWAPI;
using namespace Filter;

namespace ControlTypes
{
  enum Enum
  {
    None,
    Guard,
    Worker,
    Task,
    Captain
  };
}

namespace CaptainTypes
{
  enum Enum
  {
    None
  };
}

namespace UnitInfo
{
  enum Enum
  {
    Nothing,

    AI_Controller_Type,
    AI_Controller_Timer,

    Worker_Task,

    Guard_Return_X,
    Guard_Return_Y,

    AI_Captain_Type,

    Town,

    Order,
    OrderState,
    OrderTarget_X,
    OrderTarget_Y,
    OrderTimer,
    QueuedOrder,
    AttackTarget,

    TargetFlags
  };

}

class UnitWrap
{
public:
  UnitWrap();
  UnitWrap(BWAPI::Unit unit);

  UnitWrap &operator =(BWAPI::Unit unit);
  BWAPI::Unit operator->() const;
  operator BWAPI::Unit () const;

  // GetInfo
  int GetOrderTimer() const;
  int GetOrderState() const;
  Orders::Enum::Enum GetUnitOrder() const;
  BWAPI::Position GetOrderTargetPosition() const;
  Orders::Enum::Enum GetQueuedOrder() const;
  Unit GetAttackTarget() const;
  ControlTypes::Enum GetControlType() const;
  CaptainTypes::Enum GetCaptainType() const;
  BWAPI::Position GetGuardReturnPosition() const;
  int GetTargetFlags() const;
  int GetWorkerTask() const;

  // SetInfo
  void SetOrderState(int state);
  void SetOrderTimer(int time);
  void SetVirtualUnitOrder(BWAPI::Orders::Enum::Enum order, BWAPI::Position position = BWAPI::Positions::Origin);
  void SetQueuedOrder(BWAPI::Orders::Enum::Enum order);
  void SetAttackTarget(BWAPI::Unit pTarget);
  void SetControlType(int type);
  void SetCaptainType(int type);
  void SetGuardReturnPosition(BWAPI::Position pos);
  void SetTargetFlags(int flags);
  void SetWorkerTask(int type);

  // Procedures
  void UpdateOrderTimers();
  void EmulateOrder();
  BWAPI::Unit FindNewAttackTarget() const;
  bool IsMilitaryCaptain() const;
  bool HasNoCaptain() const;
  void AssignComputerIdleOrder();
  bool CanUseTech(TechType tech) const;
  bool UseTech(BWAPI::TechType tech, Unit pTarget, int targBit = 1);
  bool UseTechOnClosest(BWAPI::TechType tech, int range, const BWAPI::UnitFilter &pred, int targBit = 1);

  // AI Control
  void PerformSpecializedUnitControl();
  bool HasEnemyTarget();
  void DecloakCheck();
  bool DoUnitBehaviour();

  // Military
  bool TaskBunkerDefender();
  bool TaskSpellcaster(bool isAggressive);
  bool TaskAggression();
  void RunMilitaryController();

  // Task
  bool RunTaskController();

  // Worker
  bool AttendRepair();
  bool AttendTown();
  void RunWorkerController();

  // Orders
  void RunJunkYardDog();
  void RunComputerAI();
  void RunGuard();
  void RunGuardPost();
  void RunComputerReturn();

private:
  BWAPI::Unit pUnit;
};
