using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.Enums
{
    public enum AnimType : byte
    {
        Init = 0,
        Death,
        GndAttkInit,
        AirAttkInit,
        Unused1,
        GndAttkRpt,
        AirAttkRpt,
        CastSpell,
        GndAttkToIdle,
        AirAttkToIdle,
        Unused2,
        Walking,
        WalkingToIdle,
        SpecialState1,
        SpecialState2,
        AlmostBuilt,
        Built,
        Landing,
        LiftOff,
        IsWorking,
        WorkingToIdle,
        WarpIn,
        Unused3,
        StarEditInit,
        Disable,
        Burrow,
        UnBurrow,
        Enable        
    }
}
