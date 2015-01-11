using Binarysharp.MemoryManagement.Memory;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Sprite
    {
        private RemotePointer ptr;
        private int id;

        private CSprite data { get { return ptr.Read<CSprite>(); } set { ptr.Write(value); } }
        /*********************************************************************************************************************/

        public SpriteType type { get { return data.type; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "type").ToInt32(), value); } }
        public Byte player { get { return data.player; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "player").ToInt32(), value); } }
        public Byte selectionIndex { get { return data.selectionIndex; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "selectionIndex").ToInt32(), value); } }
        public Byte visibilityFlags { get { return data.visibilityFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "visibilityFlags").ToInt32(), value); } }
        public Byte elevationLevel { get { return data.elevationLevel; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "elevationLevel").ToInt32(), value); } }
        public Byte flags { get { return data.flags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flags").ToInt32(), value); } }
        public Byte selectionTimer { get { return data.selectionTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "selectionTimer").ToInt32(), value); } }
        public UInt32 index { get { return data.index; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "index").ToInt32(), value); } }
        public Position position { get { return data.position; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "position").ToInt32(), value); } }

        /*********************************************************************************************************************/

        public Sprite(RemotePointer baseUnit, int id)
        {
            ptr = baseUnit;
            this.id = id;
        }

        public override string ToString()
        {
            return "" + id + ": " + type.ToString();
        }
    }
}
