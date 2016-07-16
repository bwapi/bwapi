using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.Enums
{
    public enum RLE : byte
    {
        RLE_NORMAL,
        RLE_1,
        RLE_2,
        RLE_3,
        RLE_4,
        RLE_CLOAK,
        RLE_CLOAKED,
        RLE_DECLOAK,
        RLE_8,
        RLE_EFFECT,
        RLE_SHADOW,
        RLE_HPFLOATDRAW,
        RLE_WARP,
        RLE_OUTLINE,
        RLE_PLAYER_SIDE,
        RLE_15,
        RLE_SHIFT,
        RLE_FIRE
    }
}
