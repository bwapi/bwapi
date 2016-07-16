using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit
{
    public enum UnitMovement : byte
    {
        UM_Init = 0,
        UM_InitSeq,
        UM_Lump,
        UM_Turret,
        UM_Bunker,
        UM_BldgTurret,
        UM_Hidden,
        UM_Flyer,
        UM_FakeFlyer,
        UM_AtRest,
        UM_Dormant,
        UM_AtMoveTarget,
        UM_CheckIllegal,
        UM_MoveToLegal,
        UM_LumpWannabe,
        UM_FailedPath,
        UM_RetryPath,
        UM_StartPath,
        UM_UIOrderDelay,
        UM_TurnAndStart,
        UM_FaceTarget,
        UM_NewMoveTarget,
        UM_AnotherPath,
        UM_Repath,
        UM_RepathMovers,
        UM_FollowPath,
        UM_ScoutPath,
        UM_ScoutFree,
        UM_FixCollision,
        UM_WaitFree,
        UM_GetFree,
        UM_SlidePrep,
        UM_SlideFree,
        UM_ForceMoveFree,
        UM_FixTerrain,
        UM_TerrainSlide
    }
}
