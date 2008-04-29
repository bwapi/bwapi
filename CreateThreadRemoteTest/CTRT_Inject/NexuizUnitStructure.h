#pragma once

#include "Types.h"
#include "StaticAssert.h"

class Nexuiz_Unit {
public:
   /*000*/Nexuiz_Unit*  pPrev;              // ptr to previous unit struct
   /*004*/Nexuiz_Unit*  pNext;              // ptr to next unit struct
   /*008*/_UNKNOWN z_25[1];            
   /*009*/WORD     iHealthPoints;      // health points (part of the dword at 008)
   /*011*/_UNKNOWN z_1[1];             // part of the dword at 008
   /*012*/PVOID    pSprite;            // sprite info for unit
   /*016*/WORD     iDest_X;            // destination x
   /*018*/WORD     iDest_Y;            // destination y 
   /*020*/Nexuiz_Unit*  pDestinationUnit;   // ptr to neutral this units destination (be it minerals, or cc, etc)
   /*024*/WORD     iDest_X2;           // destination x II (same as above)
   /*026*/WORD     iDest_Y2;           // destination y II (same as above)
   /*028*/_UNKNOWN z_2[12];   
   /*040*/WORD     iTrue_X;            // current x
   /*042*/WORD     iTrue_Y;            // current y
   /*044*/_UNKNOWN z_3[32];   
   /*076*/BYTE     iPlayerOwner;       // owner id
   /*077*/BYTE     iCurrentState;      // units current state
   /*078*/_UNKNOWN z_4[14];   
   /*092*/Nexuiz_Unit*  pTargetUnit;        // current target unit for attacks/spell cast
   /*096*/_UNKNOWN z_12[1];           
   /*097*/WORD     iShieldPoints;      // shield points
   /*099*/_UNKNOWN z_5[1];    
   /*100*/DWORD     iUnitType;          // unit type
   /*101*/_UNKNOWN z_6[3];    
   /*104*/Nexuiz_Unit*  pSecPrev;           // pointer to previous unit (units in cargo have original prev/next ptrs NULLed)
   /*108*/Nexuiz_Unit*  pSecNext;           // pointer to next unit (units in cargo have original prev/next ptrs NULLed)
   /*112*/_UNKNOWN z_20[21];
   /*133*/BYTE     iAliveTime;         // alive tick count
   /*134*/_UNKNOWN z_24[9];
   /*143*/BYTE     iKillCount;         // frag count
   /*144*/_UNKNOWN z_7[4];
   /*148*/BYTE     iUnitType2;         // unit type II
   /*149*/_UNKNOWN z_8[3];
   /*152*/WORD     iBuildingQueue[5];  // unit -types- for building queue (NOT UNIT TOKENS)
   /*162*/_UNKNOWN z_13[1];
   /*163*/WORD     iManaPoints;        // mana points
   /*165*/BYTE     nTokenBase;         // token base
   /*166*/BYTE     iBuildingUnits;     // (?) 0x26 = constructing, 0x17 = not constructing
   /*167*/_UNKNOWN z_10[9];    
   /*176*/WORD     iLoadedUnitArray[8];// array of unit unique ids which are loaded into this unit (transport)
   /*192*/Nexuiz_Unit*  pProtossParent;     // first interceptor/scarab if carrier/reaver, else the carrier/reaver if its interceptor/scarab
   /*196*/_UNKNOWN z_14[4]; 
   /*200*/Nexuiz_Unit*  pLastClickedUnit;   // last clicked unit (stays the same if state->idle)
   /*204*/_UNKNOWN z_11[32];
   /*236*/Nexuiz_Unit*  pChild;             // child unit (for terran buildings, its the addon)
   /*240*/_UNKNOWN z_21[28];
   /*268*/BYTE     iDefenseMatrixRemaining; // defense matrix time remaining
   /*269*/BYTE     iStimPackRemaining;      // stim pack remaining
   /*270*/BYTE     iEnsnareTimeRemaining;   // ensnare time remaining
   /*271*/BYTE     iLockdownTimeRemaining;  // lockdown time remaining
   /*272*/_UNKNOWN z_27[2];
   /*274*/BYTE     iPlagueTimeRemaining;
   /*275*/_UNKNOWN z_28[6];
   /*281*/BYTE     iParasiteStatus;
   /*282*/_UNKNOWN z_29[35];
};

struct Nexuiz_UnitArray
{
   Nexuiz_Unit Unit[1700];
};

BOOST_STATIC_ASSERT(sizeof(Nexuiz_Unit) == 336);
BOOST_STATIC_ASSERT(sizeof(Nexuiz_UnitArray) == 336*1700);
