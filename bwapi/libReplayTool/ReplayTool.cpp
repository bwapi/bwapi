#include "ReplayTool.h"
#include "ResearchAction.h"
#include "SaveGameAction.h"
#include "BuildAction.h"
#include "CloakAction.h"
#include "LiftOffAction.h"
#include "StopAction.h"
#include "TankSiegeAction.h"
#include "SetSpeedAction.h"
#include "UnloadAllAction.h"
#include "BurrowAction.h"
#include "PingMinimapAction.h"
#include "UpgradeAction.h"
#include "LatencyAction.h"
#include "LeaveGameAction.h"
#include "RightClickAction.h"
#include "TargetClickAction.h"
#include "SetReplaySpeedAction.h"
#include "ChatReplayAction.h"
#include "SelectAction.h"
#include "TrainAction.h"
#include "GroupUnitsAction.h"
#include "CancelTrainAction.h"

using namespace ReplayTool;

void InitActionFactory();

void ReplayTool::init()
{
  InitActionFactory();
}

void InitActionFactory()
{
  REGISTER_ACTION(ResearchAction, ReplayTool::Research);
  REGISTER_ACTION(SaveGameAction, ReplayTool::Save_Game);
  REGISTER_ACTION(LoadGameAction, ReplayTool::Load_Game);
  REGISTER_ACTION(BuildAction, ReplayTool::Placebox);
  REGISTER_ACTION(ChatReplayAction, ReplayTool::Chat_Replay);
  REGISTER_ACTION_WITH_1_PARAM(SelectAction, ReplayTool::Select_Delta_Add, SelectAction::SELECT_DeltaAdd);
  REGISTER_ACTION_WITH_1_PARAM(SelectAction, ReplayTool::Select_Delta_Del, SelectAction::SELECT_DeltaDel);
  REGISTER_ACTION_WITH_1_PARAM(SelectAction, ReplayTool::Select_Units, SelectAction::SELECT_Units);
  REGISTER_ACTION_WITH_1_PARAM(CloakAction, ReplayTool::Cloak_On, true);
  REGISTER_ACTION_WITH_1_PARAM(CloakAction, ReplayTool::Cloak_Off, false);
  REGISTER_ACTION(LiftOffAction, ReplayTool::Lift_Off);
  REGISTER_ACTION_WITH_1_PARAM(StopAction, ReplayTool::Stop, true);
  REGISTER_ACTION_WITH_1_PARAM(StopAction, ReplayTool::Return, false);
  REGISTER_ACTION_WITH_1_PARAM(TankSiegeAction, ReplayTool::Tank_Siege, true);
  REGISTER_ACTION_WITH_1_PARAM(TankSiegeAction, ReplayTool::Tank_Unsiege, false);
  REGISTER_ACTION(SetSpeedAction, ReplayTool::Set_Speed);
  REGISTER_ACTION_WITH_1_PARAM(UnloadAllAction, ReplayTool::Unload_All, true);
  REGISTER_ACTION_WITH_1_PARAM(UnloadAllAction, ReplayTool::Hold_Position, false);
  REGISTER_ACTION_WITH_1_PARAM(BurrowAction, ReplayTool::Burrow_Up, true);
  REGISTER_ACTION_WITH_1_PARAM(BurrowAction, ReplayTool::Burrow_Down, false);
  REGISTER_ACTION(PingMinimapAction, ReplayTool::Ping_Minimap);
  REGISTER_ACTION(UpgradeAction, ReplayTool::Upgrade);
  REGISTER_ACTION(LatencyAction, ReplayTool::Set_Latency);
  REGISTER_ACTION(LeaveGameAction, ReplayTool::Leave_Game);
  REGISTER_ACTION(RightClickAction, ReplayTool::Right_Click);
  REGISTER_ACTION(TargetClickAction, ReplayTool::Target_Click);
  REGISTER_ACTION(SetReplaySpeedAction, ReplayTool::Set_Replay_Speed);
  REGISTER_ACTION(TrainAction, ReplayTool::Train);
  REGISTER_ACTION(GroupUnitsAction, ReplayTool::Group_Units);
  REGISTER_ACTION(CancelTrainAction, ReplayTool::Cancel_Train);
}