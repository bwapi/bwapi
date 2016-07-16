using BWMemoryEdit.BW;
using BWMemoryEdit.Enums;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BWMemoryEdit
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct CUnit
    {
        public Reference<Unit> prev;
        public Reference<Unit> next;
        public Int32 hitPoints;
        public Reference<Sprite> sprite;
        public Target moveTarget;
        public Position nextMovementWaypoint;
        public Position nextTargetWaypoint;
        public UnitMovementFlags movementFlags;
        public Byte currentDirection1;
        public Byte flingyTurnRadius;
        public Byte velocityDirection1;
        public FlingyType flingyType;
        public Byte _unknown_0x026;
        public Byte flingyMovementType;
        public Position position;
        public LongPosition halt;
        public UInt32 flingyTopSpeed;
        public Int32 current_speed1;
        public Int32 current_speed2;
        public LongPosition current_speed;
        public UInt16 flingyAcceleration;
        public Byte currentDirection2;
        public Byte velocityDirection2;
        public Byte playerID;
        public Order orderID;
        public Byte orderState;
        public Byte orderSignal;
        public UnitType orderUnitType;
        public UInt16 __0x52;
        public Byte mainOrderTimer;
        public Byte groundWeaponCooldown;
        public Byte airWeaponCooldown;
        public Byte spellCooldown;
        public Target orderTarget;
        public UInt32 shieldPoints;
        public UnitType unitType;
        public UInt16 __0x66;
        public Reference<Unit> previousPlayerUnit;
        public Reference<Unit> nextPlayerUnit;
        public Reference<Unit> subUnit;
        public UInt32 orderQueueHead;
        public UInt32 orderQueueTail;
        public Reference<Unit> autoTargetUnit;
        public Reference<Unit> connectedUnit;
        public Byte orderQueueCount;
        public Byte orderQueueTimer;
        public Byte _unknown_0x086;
        public Byte attackNotifyTimer;
        public UnitType previousUnitType;
        public Byte lastEventTimer;
        public Byte lastEventColor;
        public UInt16 _unused_0x08C;
        public Byte rankIncrease;
        public Byte killCount;
        public Byte lastAttackingPlayer;
        public Byte secondaryOrderTimer;
        public Byte AIActionFlag;
        public Byte userActionFlags;
        public UInt16 currentButtonSet;
        public Byte isCloaked;
        public UnitMovement movementState;
        public fixed UInt16 buildQueue[5];
        public UInt16 energy;
        public Byte buildQueueSlot;
        public Byte uniquenessIdentifier;
        public Order secondaryOrderID;
        public Byte buildingOverlayState;
        public UInt16 hpGain;
        public UInt16 shieldGain;
        public UInt16 remainingBuildTime;
        public UInt16 previousHP;
        public fixed UInt16 loadedUnitIndex[8];

        [StructLayout(LayoutKind.Explicit)]
        public struct U1
        {
            public struct Vulture
            {
                public Byte spiderMineCount;
            }
            public struct Carrier
            {
                public UInt32 pInHanger;
                public UInt32 pOutHanger;
                public Byte inHangerCount;
                public Byte outHangerCount;
            }
            public struct Fighter
            {
                public UInt32 parent;
                public UInt32 prev;
                public UInt32 next;
                public Byte inHanger;
            }
            public struct Beacon
            {
                public UInt32 _unknown_00;
                public UInt32 _unknown_04;
                public UInt32 flagSpawnFrame;
            }
            public struct Building
            {
                public UInt32 addon;
                public UnitType addonBuildType;
                public UInt16 upgradeResearchTime;
                public TechType techType;
                public UpgradeType upgradeType;
                public Byte larvaTimer;
                public Byte landingTimer;
                public Byte creepTimer;
                public Byte upgradeLevel;
                public UInt16 __E;
            }
            public struct Worker
            {
                public UInt32 pPowerup;
                public Target targetResource;
                public UInt16 repairResourceLossTimer;
                public Byte isCarryingSomething;
                public Byte resourceCarryCount;
            }

            [FieldOffset(0)]
            public Vulture vulture;
            [FieldOffset(0)]
            public Carrier carrier;
            [FieldOffset(0)]
            public Fighter fighter;
            [FieldOffset(0)]
            public Beacon beacon;
            [FieldOffset(0)]
            public Building building;
            [FieldOffset(0)]
            public Worker worker;
        }
        [StructLayout(LayoutKind.Explicit)]
        public struct U2
        {
            public struct Resource
            {
                public UInt16 resourceCount;
                public Byte resourceIscript;
                public Byte gatherQueueCount;
                public UInt32 nextGatherer;
                public Byte resourceGroup;
                public Byte resourceBelongsToAI;
            }
            public struct Nydus
            {
                public UInt32 exit;
            }
            public struct Ghost
            {
                public UInt32 nukeDot;
            }
            public struct Pylon
            {
                public UInt32 pPowerTemplate;
            }
            public struct Silo
            {
                public UInt32 pNuke;
                public Byte bReady;
            }
            public struct Hatchery
            {
                public Rect harvestValue;
            }
            public struct Powerup
            {
                public Position origin;
            }
            public struct Gatherer
            {
                public UInt32 harvestTarget;
                public UInt32 prevHarvestUnit;
                public UInt32 nextHarvestUnit;
            }

            [FieldOffset(0)]
            public Resource resource;
            [FieldOffset(0)]
            public Nydus nydus;
            [FieldOffset(0)]
            public Ghost ghost;
            [FieldOffset(0)]
            public Pylon pylon;
            [FieldOffset(0)]
            public Silo silo;
            [FieldOffset(0)]
            public Hatchery hatchery;
            [FieldOffset(0)]
            public Powerup powerup;
            [FieldOffset(0)]
            public Gatherer gatherer;
        }

        public U1 union1;
        public U2 union2;
        public UnitStatusFlags statusFlags;
        public ResourceType resourceType;
        public Byte wireframeRandomizer;
        public Byte secondaryOrderState;
        public Byte recentOrderTimer;
        public UInt32 visibilityStatus;
        public Position secondaryOrderPosition;
        public Reference<Unit> currentBuildUnit;
        public Reference<Unit> previousBurrowedUnit;
        public Reference<Unit> nextBurrowedUnit;

        [StructLayout(LayoutKind.Explicit)]
        public struct U3
        {
            public struct Rally
            {
                public Target target;
            }
            public struct PsiProvider
            {
                public UInt32 prevPsiProvider;
                public UInt32 nextPsiProvider;
            }

            [FieldOffset(0)]
            public Rally rally;
            [FieldOffset(0)]
            public PsiProvider psiProvider;
        }
        public U3 union3;

        public UInt32 path;
        public Byte pathingCollisionInterval;
        public Byte pathingFlags;
        public Byte _unused_0x106;
        public Byte isBeingHealed;
        public Rect contourBounds;
        public UInt16 removeTimer;
        public UInt16 defenseMatrixDamage;
        public Byte defenseMatrixTimer;
        public Byte stimTimer;
        public Byte ensnareTimer;
        public Byte lockdownTimer;
        public Byte irradiateTimer;
        public Byte stasisTimer;
        public Byte plagueTimer;
        public Byte stormTimer;
        public Reference<Unit> irradiatedBy;
        public Byte irradiatePlayerID;
        public PlayerFlags parasiteFlags;
        public Byte cycleCounter;
        public Byte isBlind;
        public Byte maelstromTimer;
        public Byte _unused_0x125;
        public Byte acidSporeCount;
        public fixed Byte acidSporeTime[9];
        public UInt16 bulletBehaviour3by3AttackSequence;
        public UInt16 _padding_0x132;
        public UInt32 pAI;
        public Int16 airStrength;
        public Int16 groundStrength;
        public Int32 posSortXL;
        public Int32 posSortXR;
        public Int32 posSortYT;
        public Int32 posSortYB;
        public Byte _repulseUnknown;
        public Byte repulseAngle;
        public Byte bRepMtxX;
        public Byte bRepMtxY;

        public override string ToString()
        {
            return unitType.ToString();
        }
    }
}
