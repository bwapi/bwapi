using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.Enums
{
    [Flags]
    public enum SpriteFlags : byte
    {
        None = 0,
        ShowSelectionCircle = 0x01,
        Unk02 = 0x02,
        Unk04 = 0x04,
        Selected = 0x08,
        Unk10 = 0x10,
        Hidden = 0x20,
        Burrowed = 0x40,
        UnbreakableCode = 0x80
    }
}
