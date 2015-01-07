using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit
{
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = true)]
    public class UnitTypeAttribute : Attribute
    {
        private readonly UnitType[] units;
        public UnitTypeAttribute(params UnitType[] units)
        {
            this.units = units;
        }
        public override bool Match(object obj)
        {
            var browsable = obj as BrowsableAttribute;
            if (browsable != null && browsable.Browsable) return true;
            
            var other = obj as UnitTypeAttribute;
            if (other == null) return true;
            
            return units.Intersect(other.units).Count() > 0;
        }
    }
}
