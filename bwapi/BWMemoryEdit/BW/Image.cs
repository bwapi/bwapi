using Binarysharp.MemoryManagement.Memory;
using BWMemoryEdit.BW;
using BWMemoryEdit.Enums;
using BWMemoryEdit.Util;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing.Design;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Image
    {
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Image> prev { get { return data.prev; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Image> next { get { return data.next; } set { } }

        public ImageType type { get { return data.type; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "type").ToInt32(), (ushort)value); } }
        public RLE rle { get { return data.rle; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "rle").ToInt32(), (byte)value); } }
        public Byte direction { get { return data.direction; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "direction").ToInt32(), value); } }
        
        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public ImageFlags flags { get { return data.flags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flags").ToInt32(), (ushort)value); } }

        public SByte horizontalOffset { get { return data.horizontalOffset; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "horizontalOffset").ToInt32(), value); } }
        public SByte verticalOffset { get { return data.verticalOffset; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "verticalOffset").ToInt32(), value); } }
        public UInt16 iscriptHeader { get { return data.iscriptHeader; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "iscriptHeader").ToInt32(), value); } }
        public UInt16 iscriptOffset { get { return data.iscriptOffset; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "iscriptOffset").ToInt32(), value); } }
        public UInt16 unknown_14 { get { return data.unknown_14; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "unknown_14").ToInt32(), value); } }

        public AnimType anim { get { return data.anim; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "anim").ToInt32(), (byte)value); } }
        public Byte sleep { get { return data.sleep; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "sleep").ToInt32(), value); } }

        public UInt16 frameSet { get { return data.frameSet; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "frameSet").ToInt32(), value); } }
        public UInt16 frameIndex { get { return data.unknown_14; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "frameIndex").ToInt32(), value); } }
        public Position mapPosition { get { return data.mapPosition; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "mapPosition").ToInt32(), value); } }
        public Position screenPosition { get { return data.screenPosition; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "screenPosition").ToInt32(), value); } }
        public Rect grpBounds { get { return data.grpBounds; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "grpBounds").ToInt32(), value); } }

        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Sprite> spriteOwner { get { return data.spriteOwner; } set { } }

        /*********************************************************************************************************************/

        protected RemotePointer ptr;
        protected uint id;

        protected CImage data { get { return ptr.Read<CImage>(); } set { ptr.Write(value); } }

        public Image(RemotePointer basePtr, uint id)
        {
            this.ptr = basePtr;
            this.id = id;
        }

        public override string ToString()
        {
            return "" + id + ": " + type.ToString();
        }
    }
}
