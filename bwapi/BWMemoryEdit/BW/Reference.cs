using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit.BW
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Reference <T>
    {
        public UInt32 value;

        public override string ToString()
        {
            return "[" + value.ToString("X") + "]";
        }
    }
}
