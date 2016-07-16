using BWMemoryEdit.BW;
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
    [StructLayout(LayoutKind.Sequential)]
    public struct Target
    {
        public Position position { get; set; }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> unit { get; set; }

        public override string ToString()
        {
            return position.ToString() + unit.ToString();
        }
    }
}
