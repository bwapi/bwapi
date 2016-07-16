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
    public struct Rect
    {
        public UInt16 left { get; set; }
        public UInt16 top { get; set; }
        public UInt16 right { get; set; }
        public UInt16 bottom { get; set; }

        public override string ToString()
        {
            return "(" + left + ", " + top + ", " + right + ", " + bottom + ")";
        }
    }
}
