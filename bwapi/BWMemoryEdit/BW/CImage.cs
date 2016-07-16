using BWMemoryEdit.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.BW
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct CImage
    {
        public Reference<Image> prev;
        public Reference<Image> next;
        public ImageType type;
        public RLE rle;
        public byte direction;
        public ImageFlags flags;
        public SByte horizontalOffset;
        public SByte verticalOffset;
        public UInt16 iscriptHeader;
        public UInt16 iscriptOffset;
        public UInt16 unknown_14;
        public AnimType anim;
        public Byte sleep;
        public UInt16 frameSet;
        public UInt16 frameIndex;
        public Position mapPosition;
        public Position screenPosition;
        public Rect grpBounds;
        public UInt32 grpFile;
        public UInt32 coloringData;
        public UInt32 renderFunction;
        public UInt32 updateFunction;
        public Reference<Sprite> spriteOwner;
    }
}
