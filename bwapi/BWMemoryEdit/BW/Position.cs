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
    [StructLayout(LayoutKind.Sequential)]
    public struct Position
    {
        public Int16 x { get; set; }
        public Int16 y { get; set; }

        public override string ToString()
        {
            return "(" + x + ", " + y + ")";
        }
    }
}
