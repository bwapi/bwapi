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
    public class Sprite
    {
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Sprite> prev { get { return data.prev; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Sprite> next { get { return data.next; } set { } }

        public SpriteType type { get { return data.type; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "type").ToInt32(), (ushort)value); } }
        public Byte player { get { return data.player; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "player").ToInt32(), value); } }
        public Byte selectionIndex { get { return data.selectionIndex; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "selectionIndex").ToInt32(), value); } }
        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public PlayerFlags visibilityFlags { get { return data.visibilityFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "visibilityFlags").ToInt32(), (byte)value); } }
        public Byte elevationLevel { get { return data.elevationLevel; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "elevationLevel").ToInt32(), value); } }
        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public SpriteFlags flags { get { return data.flags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flags").ToInt32(), (byte)value); } }
        public Byte selectionTimer { get { return data.selectionTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "selectionTimer").ToInt32(), value); } }
        public UInt16 index { get { return data.index; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "index").ToInt32(), value); } }
        public Byte unkflags_12 { get { return data.unkflags_12; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "unkflags_12").ToInt32(), value); } }
        public Byte unkflags_13 { get { return data.unkflags_13; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "unkflags_13").ToInt32(), value); } }
        public Position position { get { return data.position; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "position").ToInt32(), value); } }

        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Image> mainGraphic { get { return data.mainGraphic; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Image> overlay { get { return data.overlay; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Image> underlay { get { return data.underlay; } set { } }

        /*********************************************************************************************************************/

        protected RemotePointer ptr;
        protected uint id;

        protected CSprite data { get { return ptr.Read<CSprite>(); } set { ptr.Write(value); } }

        public Sprite(RemotePointer basePtr, uint id)
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
