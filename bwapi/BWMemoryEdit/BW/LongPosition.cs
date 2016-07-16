using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.BW
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    [StructLayout(LayoutKind.Sequential)]
    public struct LongPosition
    {
        public Int32 x { get; set; }
        public Int32 y { get; set; }

        public override string ToString()
        {
            return "(" + x + ", " + y + ")";
        }
    }
}
