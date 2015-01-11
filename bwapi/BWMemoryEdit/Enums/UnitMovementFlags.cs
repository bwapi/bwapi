using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.Enums
{
    [Flags]
    public enum UnitMovementFlags : byte
    {
        None = 0,
        OrderedAtLeastOnce  = 0x01,
        Accelerating        = 0x02,
        Braking             = 0x04,
        StartingAttack      = 0x08,
        Moving              = 0x10,
        Lifted              = 0x20,
        unknown1            = 0x40,
        _alwaysZero1        = 0x80
    }
}
