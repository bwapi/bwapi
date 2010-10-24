#include "Path.h"
namespace BW
{
  char *getMoveStateName(u8 bMoveState)
  {
    switch ( bMoveState )
    {
      case UM_Init:
        return "Init";
      case UM_InitSeq:
        return "InitSeq";
      case UM_Lump:
        return "Lump";
      case UM_Turret:
        return "Turret";
      case UM_Bunker:
        return "Bunker";
      case UM_BldgTurret:
        return "BldgTurret";
      case UM_Hidden:
        return "Hidden";
      case UM_Flyer:
        return "Flyer";
      case UM_FakeFlyer:
        return "FakeFlyer";
      case UM_AtRest:
        return "AtRest";
      case UM_Dormant:
        return "Dormant";
      case UM_AtMoveTarget:
        return "AtMoveTarget";
      case UM_CheckIllegal:
        return "CheckIllegal";
      case UM_MoveToLegal:
        return "MoveToLegal";
      case UM_LumpWannabe:
        return "LumpWannabe";
      case UM_FailedPath:
        return "FailedPath";
      case UM_RetryPath:
        return "RetryPath";
      case UM_StartPath:
        return "StartPath";
      case UM_UIOrderDelay:
        return "UIOrderDelay";
      case UM_TurnAndStart:
        return "TurnAndStart";
      case UM_FaceTarget:
        return "FaceTarget";
      case UM_NewMoveTarget:
        return "NewMoveTarget";
      case UM_AnotherPath:
        return "AnotherPath";
      case UM_Repath:
        return "Repath";
      case UM_RepathMovers:
        return "RepathMovers";
      case UM_FollowPath:
        return "FollowPath";
      case UM_ScoutPath:
        return "ScoutPath";
      case UM_ScoutFree:
        return "ScoutFree";
      case UM_FixCollision:
        return "FixCollision";
      case UM_WaitFree:
        return "WaitFree";
      case UM_GetFree:
        return "GetFree";
      case UM_SlidePrep:
        return "SlidePrep";
      case UM_SlideFree:
        return "SlideFree";
      case UM_ForceMoveFree:
        return "ForceMoveFree";
      case UM_FixTerrain:
        return "FixTerrain";
      case UM_TerrainSlide:
        return "TerrainSlide";
      default:
        break;
    }
    return "UNKNOWN";
  }

}