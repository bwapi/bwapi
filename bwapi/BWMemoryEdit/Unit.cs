using Binarysharp.MemoryManagement;
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
    public class Unit
    {
        private RemotePointer ptr;
        private int id;

        private CUnit data { get { return ptr.Read<CUnit>(); } set { ptr.Write(value); } }
        /*********************************************************************************************************************/

        public Int32 hitPoints { get { return data.hitPoints; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "hitPoints").ToInt32(), value); } }
        
        public Byte movementFlags { get { return data.movementFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "movementFlags").ToInt32(), value); } }
        public UInt16 flingyID { get { return data.flingyID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flingyID").ToInt32(), value); } }
        public Byte _unknown_0x026 { get { return data._unknown_0x026; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "_unknown_0x026").ToInt32(), value); } }
        public Byte flingyMovementType { get { return data.flingyMovementType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flingyMovementType").ToInt32(), value); } }

        public Byte playerID { get { return data.playerID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "playerID").ToInt32(), value); } }
        public Order orderID { get { return data.orderID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderID").ToInt32(), value); } }
        public Byte orderState { get { return data.orderState; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderState").ToInt32(), value); } }
        public Byte orderSignal { get { return data.orderSignal; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderSignal").ToInt32(), value); } }
        public UnitType orderUnitType { get { return data.orderUnitType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderUnitType").ToInt32(), value); } }
        public UInt16 __0x52 { get { return data.__0x52; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "__0x52").ToInt32(), value); } }
        public Byte mainOrderTimer { get { return data.mainOrderTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "mainOrderTimer").ToInt32(), value); } }
        public Byte groundWeaponCooldown { get { return data.groundWeaponCooldown; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "groundWeaponCooldown").ToInt32(), value); } }
        public Byte airWeaponCooldown { get { return data.airWeaponCooldown; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "airWeaponCooldown").ToInt32(), value); } }
        public Byte spellCooldown { get { return data.spellCooldown; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "spellCooldown").ToInt32(), value); } }

        public UInt32 shieldPoints { get { return data.shieldPoints; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "shieldPoints").ToInt32(), value); } }
        public UnitType unitType { get { return data.unitType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "unitType").ToInt32(), value); } }
        public UInt16 __0x66 { get { return data.__0x66; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "__0x66").ToInt32(), value); } }

        public Byte _unknown_0x086 { get { return data._unknown_0x086; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "_unknown_0x086").ToInt32(), value); } }
        public Byte attackNotifyTimer { get { return data.attackNotifyTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "attackNotifyTimer").ToInt32(), value); } }
        public UnitType displayedUnitID { get { return data.displayedUnitID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "displayedUnitID").ToInt32(), value); } }

        public Byte secondaryOrderTimer { get { return data.secondaryOrderTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "secondaryOrderTimer").ToInt32(), value); } }
        public Byte AIActionFlag { get { return data.AIActionFlag; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "AIActionFlag").ToInt32(), value); } }
        public Byte userActionFlags { get { return data.userActionFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "userActionFlags").ToInt32(), value); } }

        public UnitMovement movementState { get { return data.movementState; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "movementState").ToInt32(), value); } }

        public UInt16 energy { get { return data.energy; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "energy").ToInt32(), value); } }

        public Order secondaryOrderID { get { return data.secondaryOrderID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "secondaryOrderID").ToInt32(), value); } }
        public UInt16 _unknown_0x0AA { get { return data._unknown_0x0AA; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "_unknown_0x0AA").ToInt32(), value); } }

        [Browsable(false), UnitType(UnitType.Terran_Vulture)]
        public Byte spiderMineCount { get { return data.union1.vulture.spiderMineCount; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "union1.vulture.spiderMineCount").ToInt32(), value); } }

        public UInt32 statusFlags { get { return data.statusFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "statusFlags").ToInt32(), value); } }
        public Byte secondaryOrderState { get { return data.secondaryOrderState; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "secondaryOrderState").ToInt32(), value); } }
        public Byte recentOrderTimer { get { return data.recentOrderTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "recentOrderTimer").ToInt32(), value); } }
        public Position _unknown_0x0E8 { get { return data._unknown_0x0E8; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "_unknown_0x0E8").ToInt32(), value); } }

        public UInt16 airStrength { get { return data.airStrength; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "airStrength").ToInt32(), value); } }
        public UInt16 groundStrength { get { return data.groundStrength; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "groundStrength").ToInt32(), value); } }
        
        /*********************************************************************************************************************/

        public Unit(RemotePointer baseUnit, int id)
        {
            ptr = baseUnit;
            this.id = id;
        }

        public override string ToString()
        {
            return "" + id + ": " + unitType.ToString();
        }
    }
}
