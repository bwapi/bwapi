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
        public UInt32 prev;
        public UInt32 next;
        public SpriteType type;
        public Byte player;
        public Byte selectionIndex;
        public Byte visibilityFlags;
        public Byte elevationLevel;
        public Byte flags;
        public Byte selectionTimer;
        public UInt32 index;
        public Position position;
        public UInt32 mainGraphic;
        public UInt32 underlay;
        public UInt32 overlay;
    }
}
