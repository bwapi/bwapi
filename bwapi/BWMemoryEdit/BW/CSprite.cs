using BWMemoryEdit.BW;
using BWMemoryEdit.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct CSprite
    {
        public Reference<Sprite> prev;
        public Reference<Sprite> next;
        public SpriteType type;
        public Byte player;
        public Byte selectionIndex;
        public PlayerFlags visibilityFlags;
        public Byte elevationLevel;
        public SpriteFlags flags;
        public Byte selectionTimer;
        public UInt16 index;
        public Byte unkflags_12;
        public Byte unkflags_13;
        public Position position;
        public Reference<Image> mainGraphic;
        public Reference<Image> overlay;
        public Reference<Image> underlay;
    }
}
