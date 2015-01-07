using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit
{
    public class PropertyGridInitialExpandedAttribute : Attribute
    {
        public bool InitialExpanded { get; set; }

        public PropertyGridInitialExpandedAttribute(bool initialExpanded)
        {
            InitialExpanded = initialExpanded;
        }
    }
}
