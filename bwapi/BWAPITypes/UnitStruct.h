#pragma once
namespace BWAPI
{
  // access for a unit the agent has knowledge of
  typedef int ClearanceLevel;
  enum ClearanceLevels
  {
    None = 0,
    Basic = 1,
    Detected = 2,
    Visible = 3,
    FullyObservable = 4  // highest clearance level
  };
  // Unit's state we know about, depending on clearance level
  struct StateBasic
  {
    int getID;
    int getInitialType;
    //PlayerStruct* getInitialPlayer;
  };
  struct StateDetected : StateBasic
  {
    //replace with position object later
    int x;
    int y;
  };
  struct StateVisible : StateDetected
  {
    //PlayerStruct* getPlayer;
    int getType;
    int getHitPoints;
  };
  struct StateFullyObservable : StateVisible
  {
    bool isResearching;//can not get this from visible enemy units
  };
  typedef StateFullyObservable State;      // all states unified

}