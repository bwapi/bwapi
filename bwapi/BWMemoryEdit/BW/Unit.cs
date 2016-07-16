using Binarysharp.MemoryManagement;
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
    public class Unit
    {
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> prev { get { return data.prev; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> next { get { return data.next; } set { } }

        [Category("Core Stats")]
        public Int32 hitPoints { get { return data.hitPoints; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "hitPoints").ToInt32(), value); } }

        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Sprite> sprite { get { return data.sprite; } set { } }

        [Category("Movement")]
        public Target moveTarget { get { return data.moveTarget; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "moveTarget").ToInt32(), value); } }

        [Category("Movement")]
        public Position nextMovementWaypoint { get { return data.nextMovementWaypoint; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "nextMovementWaypoint").ToInt32(), value); } }
        [Category("Movement")]
        public Position nextTargetWaypoint { get { return data.nextTargetWaypoint; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "nextTargetWaypoint").ToInt32(), value); } }

        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("Movement")]
        public UnitMovementFlags movementFlags { get { return data.movementFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "movementFlags").ToInt32(), (byte)value); } }
        [Category("Movement")]
        public Byte currentDirection1 { get { return data.currentDirection1; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "currentDirection1").ToInt32(), value); } }
        [Category("Movement")]
        public Byte flingyTurnRadius { get { return data.flingyTurnRadius; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flingyTurnRadius").ToInt32(), value); } }
        [Category("Movement")]
        public Byte velocityDirection1 { get { return data.velocityDirection1; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "velocityDirection1").ToInt32(), value); } }
        [Category("Movement")]
        public FlingyType flingyType { get { return data.flingyType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flingyType").ToInt32(), value); } }
        [Category("Movement")]
        public Byte _unknown_0x026 { get { return data._unknown_0x026; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "_unknown_0x026").ToInt32(), value); } }
        [Category("Movement")]
        public Byte flingyMovementType { get { return data.flingyMovementType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flingyMovementType").ToInt32(), value); } }
        [Category("Movement")]
        public Position position { get { return data.position; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "position").ToInt32(), value); } }
        [Category("Movement")]
        public LongPosition halt { get { return data.halt; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "halt").ToInt32(), value); } }
        [Category("Movement")]
        public UInt32 flingyTopSpeed { get { return data.flingyTopSpeed; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flingyTopSpeed").ToInt32(), value); } }
        [Category("Movement")]
        public Int32 current_speed1 { get { return data.current_speed1; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "current_speed1").ToInt32(), value); } }
        [Category("Movement")]
        public Int32 current_speed2 { get { return data.current_speed2; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "current_speed2").ToInt32(), value); } }
        [Category("Movement")]
        public LongPosition current_speed { get { return data.current_speed; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "current_speed").ToInt32(), value); } }
        [Category("Movement")]
        public UInt16 flingyAcceleration { get { return data.flingyAcceleration; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "flingyAcceleration").ToInt32(), value); } }
        [Category("Movement")]
        public Byte currentDirection2 { get { return data.currentDirection2; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "currentDirection2").ToInt32(), value); } }
        [Category("Movement")]
        public Byte velocityDirection2 { get { return data.velocityDirection2; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "velocityDirection2").ToInt32(), value); } }
        
        public Byte playerID { get { return data.playerID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "playerID").ToInt32(), value); } }

        [Category("PrimaryOrder")]
        public Order orderID { get { return data.orderID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderID").ToInt32(), (byte)value); } }
        [Category("PrimaryOrder")]
        public Byte orderState { get { return data.orderState; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderState").ToInt32(), value); } }
        [Category("PrimaryOrder")]
        public Byte orderSignal { get { return data.orderSignal; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderSignal").ToInt32(), value); } }
        [Category("PrimaryOrder")]
        public UnitType orderUnitType { get { return data.orderUnitType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderUnitType").ToInt32(), (ushort)value); } }
        [Category("PrimaryOrder")]
        public Byte mainOrderTimer { get { return data.mainOrderTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "mainOrderTimer").ToInt32(), value); } }


        public Byte groundWeaponCooldown { get { return data.groundWeaponCooldown; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "groundWeaponCooldown").ToInt32(), value); } }
        public Byte airWeaponCooldown { get { return data.airWeaponCooldown; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "airWeaponCooldown").ToInt32(), value); } }
        public Byte spellCooldown { get { return data.spellCooldown; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "spellCooldown").ToInt32(), value); } }

        [Category("PrimaryOrder")]
        public Target orderTarget { get { return data.orderTarget; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "orderTarget").ToInt32(), value); } }

        [Category("Core Stats")]
        public UInt32 shieldPoints { get { return data.shieldPoints; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "shieldPoints").ToInt32(), value); } }

        [Category("Core Stats")]
        public UnitType unitType { get { return data.unitType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "unitType").ToInt32(), (ushort)value); } }

        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> previousPlayerUnit { get { return data.previousPlayerUnit; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> nextPlayerUnit { get { return data.nextPlayerUnit; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> subUnit { get { return data.subUnit; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> autoTargetUnit { get { return data.autoTargetUnit; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> connectedUnit { get { return data.connectedUnit; } set { } }

        public Byte _unknown_0x086 { get { return data._unknown_0x086; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "_unknown_0x086").ToInt32(), value); } }
        public Byte attackNotifyTimer { get { return data.attackNotifyTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "attackNotifyTimer").ToInt32(), value); } }
        public UnitType previousUnitType { get { return data.previousUnitType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "previousUnitType").ToInt32(), (ushort)value); } }
        public Byte lastEventTimer { get { return data.lastEventTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "lastEventTimer").ToInt32(), value); } }
        public Byte lastEventColor { get { return data.lastEventColor; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "lastEventColor").ToInt32(), value); } }
        public Byte rankIncrease { get { return data.rankIncrease; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "rankIncrease").ToInt32(), value); } }
        public Byte killCount { get { return data.killCount; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "killCount").ToInt32(), value); } }
        public Byte lastAttackingPlayer { get { return data.lastAttackingPlayer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "lastAttackingPlayer").ToInt32(), value); } }
        
        [Category("SecondaryOrder")]
        public Byte secondaryOrderTimer { get { return data.secondaryOrderTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "secondaryOrderTimer").ToInt32(), value); } }
        
        [Category("AI")]
        public Byte AIActionFlag { get { return data.AIActionFlag; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "AIActionFlag").ToInt32(), value); } }
        public Byte userActionFlags { get { return data.userActionFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "userActionFlags").ToInt32(), value); } }

        public UInt16 currentButtonSet { get { return data.currentButtonSet; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "currentButtonSet").ToInt32(), value); } }
        [Category("Status")]
        public Boolean isCloaked { get { return data.isCloaked != 0; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "isCloaked").ToInt32(), (byte)(value ? 1 : 0)); } }

        [Category("Movement")]
        public UnitMovement movementState { get { return data.movementState; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "movementState").ToInt32(), (byte)value); } }

        [Category("Core Stats")]
        public UInt16 energy { get { return data.energy; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "energy").ToInt32(), value); } }

        public Byte uniquenessIdentifier { get { return data.uniquenessIdentifier; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "uniquenessIdentifier").ToInt32(), value); } }

        [Category("SecondaryOrder")]
        public Order secondaryOrderID { get { return data.secondaryOrderID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "secondaryOrderID").ToInt32(), (byte)value); } }

        public Byte buildingOverlayState { get { return data.buildingOverlayState; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "buildingOverlayState").ToInt32(), value); } }
        public UInt16 hpGain { get { return data.hpGain; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "hpGain").ToInt32(), value); } }
        public UInt16 shieldGain { get { return data.shieldGain; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "shieldGain").ToInt32(), value); } }
        public UInt16 remainingBuildTime { get { return data.remainingBuildTime; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "remainingBuildTime").ToInt32(), value); } }
        public UInt16 previousHP { get { return data.previousHP; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "previousHP").ToInt32(), value); } }

        //[Browsable(false), UnitType(UnitType.Terran_Vulture)]
        //public Byte spiderMineCount { get { return data.union1.vulture.spiderMineCount; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "union1.vulture.spiderMineCount").ToInt32(), value); } }

        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public UnitStatusFlags statusFlags { get { return data.statusFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "statusFlags").ToInt32(), (uint)value); } }

        public ResourceType resourceType { get { return data.resourceType; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "resourceType").ToInt32(), (byte)value); } }
        public Byte wireframeRandomizer { get { return data.wireframeRandomizer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "wireframeRandomizer").ToInt32(), value); } }

        [Category("SecondaryOrder")]
        public Byte secondaryOrderState { get { return data.secondaryOrderState; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "secondaryOrderState").ToInt32(), value); } }
        [Category("PrimaryOrder")]
        public Byte recentOrderTimer { get { return data.recentOrderTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "recentOrderTimer").ToInt32(), value); } }
        [Category("SecondaryOrder")]
        public Position secondaryOrderPosition { get { return data.secondaryOrderPosition; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "secondaryOrderPosition").ToInt32(), value); } }

        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> currentBuildUnit { get { return data.currentBuildUnit; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> previousBurrowedUnit { get { return data.previousBurrowedUnit; } set { } }
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> nextBurrowedUnit { get { return data.nextBurrowedUnit; } set { } }

        [Category("Pathing")]
        public UInt32 path { get { return data.path; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "path").ToInt32(), value); } }
        [Category("Pathing")]
        public Byte pathingCollisionInterval { get { return data.pathingCollisionInterval; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "pathingCollisionInterval").ToInt32(), value); } }
        [Category("Pathing")]
        public Byte pathingFlags { get { return data.pathingFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "pathingFlags").ToInt32(), value); } }


        [Category("Status")]
        public Boolean isBeingHealed { get { return data.isBeingHealed != 0; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "isBeingHealed").ToInt32(), (byte)(value ? 1 : 0)); } }
        [Category("Pathing")]
        public Rect contourBounds { get { return data.contourBounds; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "contourBounds").ToInt32(), value); } }

        [Category("Status")]
        public UInt16 removeTimer { get { return data.removeTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "removeTimer").ToInt32(), value); } }
        [Category("Status")]
        public UInt16 defenseMatrixDamage { get { return data.defenseMatrixDamage; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "defenseMatrixDamage").ToInt32(), value); } }
        [Category("Status")]
        public Byte defenseMatrixTimer { get { return data.defenseMatrixTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "defenseMatrixTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte stimTimer { get { return data.stimTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "stimTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte ensnareTimer { get { return data.ensnareTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "ensnareTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte lockdownTimer { get { return data.lockdownTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "lockdownTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte irradiateTimer { get { return data.irradiateTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "irradiateTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte stasisTimer { get { return data.stasisTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "stasisTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte plagueTimer { get { return data.plagueTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "plagueTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte stormTimer { get { return data.stormTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "stormTimer").ToInt32(), value); } }
        [Category("Status")]
        [Editor(typeof(JumpToReferenceTypeEditor), typeof(UITypeEditor))]
        public Reference<Unit> irradiatedBy { get { return data.irradiatedBy; } set { } }
        [Category("Status")]
        public Byte irradiatePlayerID { get { return data.irradiatePlayerID; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "irradiatePlayerID").ToInt32(), value); } }
        [Category("Status")]
        [Editor(typeof(Utils.FlagEnumUIEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public PlayerFlags parasiteFlags { get { return data.parasiteFlags; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "parasiteFlags").ToInt32(), (byte)value); } }
        [Category("Status")]
        public Byte cycleCounter { get { return data.cycleCounter; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "cycleCounter").ToInt32(), value); } }
        [Category("Status")]
        public Boolean isBlind { get { return data.isBlind != 0; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "isBlind").ToInt32(), (byte)(value ? 1 : 0)); } }
        [Category("Status")]
        public Byte maelstromTimer { get { return data.maelstromTimer; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "maelstromTimer").ToInt32(), value); } }
        [Category("Status")]
        public Byte acidSporeCount { get { return data.acidSporeCount; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "acidSporeCount").ToInt32(), value); } }

        public UInt16 bulletBehaviour3by3AttackSequence { get { return data.bulletBehaviour3by3AttackSequence; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "bulletBehaviour3by3AttackSequence").ToInt32(), value); } }
        
        [Category("AI")]
        public Int16 airStrength { get { return data.airStrength; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "airStrength").ToInt32(), value); } }
        [Category("AI")]
        public Int16 groundStrength { get { return data.groundStrength; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "groundStrength").ToInt32(), value); } }

        [Category("Sorting")]
        public Int32 posSortXL { get { return data.posSortXL; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "posSortXL").ToInt32(), value); } }
        [Category("Sorting")]
        public Int32 posSortXR { get { return data.posSortXR; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "posSortXR").ToInt32(), value); } }
        [Category("Sorting")]
        public Int32 posSortYT { get { return data.posSortYT; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "posSortYT").ToInt32(), value); } }
        [Category("Sorting")]
        public Int32 posSortYB { get { return data.posSortYB; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "posSortYB").ToInt32(), value); } }

        public Byte _repulseUnknown { get { return data._repulseUnknown; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "_repulseUnknown").ToInt32(), value); } }
        public Byte repulseAngle { get { return data.repulseAngle; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "repulseAngle").ToInt32(), value); } }
        public Byte bRepMtxX { get { return data.bRepMtxX; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "bRepMtxX").ToInt32(), value); } }
        public Byte bRepMtxY { get { return data.bRepMtxY; } set { ptr.Write(Marshal.OffsetOf(data.GetType(), "bRepMtxY").ToInt32(), value); } }
        

        /*********************************************************************************************************************/

        protected RemotePointer ptr;
        protected uint id;

        protected CUnit data { get { return ptr.Read<CUnit>(); } set { ptr.Write(value); } }

        public Unit(RemotePointer basePtr, uint id)
        {
            this.ptr = basePtr;
            this.id = id;
        }

        public override string ToString()
        {
            return "" + id + ": " + unitType.ToString();
        }
    }
}
