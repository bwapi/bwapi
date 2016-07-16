using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.Enums
{
    [Flags]
    public enum ImageFlags : ushort
    {
        None = 0,
        Redraw      = 0x0001,
        Flipped     = 0x0002,
        Unk_0004    = 0x0004,
        HasRotation = 0x0008,
        Unk_0010    = 0x0010,
        Clickable   = 0x0020,
        Hidden      = 0x0040,
        Unk_0080    = 0x0080,
        Unk_0100    = 0x0100,
        Unk_0200    = 0x0200,
        Unk_0400    = 0x0400,
        Unk_0800    = 0x0800,
        Unk_1000    = 0x1000,
        Unk_2000    = 0x2000,
        Unk_4000    = 0x4000,
        Unk_8000    = 0x8000        
    }
}
