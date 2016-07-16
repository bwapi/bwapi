using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.Enums
{
    [Flags]
    public enum PlayerFlags : byte
    {
        None    = 0,
        Player1 = 0x01,
        Player2 = 0x02,
        Player3 = 0x04,
        Player4 = 0x08,
        Player5 = 0x10,
        Player6 = 0x20,
        Player7 = 0x40,
        Player8 = 0x80
    }
}
