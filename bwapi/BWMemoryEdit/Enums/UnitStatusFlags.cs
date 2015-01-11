using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.Enums
{
    [Flags]
    public enum UnitStatusFlags : uint
    {
        None = 0,
        Completed           = 0x00000001,
        GoundedBuilding     = 0x00000002,
        InAir               = 0x00000004,
        Disabled            = 0x00000008,
        Burrowed            = 0x00000010,
        InBuilding          = 0x00000020,
        InTransport         = 0x00000040,
        UNKNOWN1            = 0x00000080,
        RequiresDetection   = 0x00000100,
        Cloaked             = 0x00000200,
        DoodadStatesThing   = 0x00000400,
        CloakingForFree     = 0x00000800,
        CanNotReceiveOrders = 0x00001000,
        NoBrkCodeStart      = 0x00002000,
        UNKNOWN2            = 0x00004000,
        CanNotAttack        = 0x00008000,
        IsAUnit             = 0x00010000,
        IsABuilding         = 0x00020000,
        IgnoreTileCollision = 0x00040000,
        UNKNOWN4            = 0x00080000,
        IsNormal            = 0x00100000,
        NoCollide           = 0x00200000,
        UNKNOWN5            = 0x00400000,
        IsGathering         = 0x00800000,
        UNKNOWN6            = 0x01000000,
        UNKNOWN7            = 0x02000000,
        Invincible          = 0x04000000,
        HoldingPosition     = 0x08000000,
        SpeedUpgrade        = 0x10000000,
        CooldownUpgrade     = 0x20000000,
        IsHallucination     = 0x40000000,
        IsSelfDestructing   = 0x80000000
    }
}
