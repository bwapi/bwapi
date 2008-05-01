
#define _CRT_SECURE_NO_DEPRECATE

#include "BW_UnitTypes.h"
#include "BW_Unit.h"
#include "NexuizUnitStructure.h"
#include "Offsets.h"

#include "windows.h"
#include "assert.h"

#include <vector>
#include <map>
#include <algorithm>


// OFFSETS


//////////////////////////////////////////////////////////////////////////

void* UnitArray_LastElement = (void*)0x00628280;    // Last Element
void* UnitArray_FirstElement = (void*)0x0059CB40;   // First Element

DWORD WINAPI CTRT_Thread( LPVOID lpThreadParameter )
{
   while(true)
   {
      //////////////////////////////////////////////////////////////////////////
      // Read the Unit Array
      
      BW_UnitArray* unitArray = (BW_UnitArray*)UnitArray_FirstElement;
      for (int i = 0; i < 1700; ++i)
      {
         if (unitArray->Unit[i].unitID == BW_UnitType::Terran_SCV)
         {
            i = i;
         }
      }

      {
         BW_Unit* curUnit = (BW_Unit*)UnitArray_LastElement;
         int unitCount = 0;
         bool hasSCV = false;
         bool hasProbe = false;
         bool hasMinerals = false;
         std::map<BW_UnitType::Enum, u32> unitTypesFound;
         while (true)
         {
            ++unitCount;

            BW_UnitType::Enum unitType = (BW_UnitType::Enum)curUnit->unitID;
            std::map<BW_UnitType::Enum, u32>::iterator findIt = unitTypesFound.find(unitType);
            if (findIt != unitTypesFound.end())
            {
               unitTypesFound[unitType] += 1;
            }
            else
            {
               unitTypesFound[unitType] = 1;
            }

            // If this is an SCV, let me break
            if (unitType == BW_UnitType::Terran_SCV)
            {
               unitType = unitType;
            }

            // Move to the prev unit
            if (curUnit->previousUnit)
            {
               curUnit = curUnit->previousUnit;
            }
            else
            {
               // finished looping
               break;
            }
         }
         unitCount = unitCount;
      }

//       //////////////////////////////////////////////////////////////////////////
//       // Set Players minerals to 101 + playerNum. This is determined at game load time (depending on where you spawn on the map, determines your player number)
//       for (int i = 0; i < 12; ++i)
//       {
//          BW_PlayerMinerals->Player[i] = 101 + i;
//       }
// 
//       //////////////////////////////////////////////////////////////////////////
//       // Set Players gas to 201 + playerNum. This is determined at game load time (depending on where you spawn on the map, determines your player number)   
//       for (int i = 0; i < 12; ++i)
//       {
//          BW_PlayerGas->Player[i] = 301 + i;
//       }

      //////////////////////////////////////////////////////////////////////////
      // Read Player Names
      //BW_PlayerNames->Player[1];

      Sleep(500);
   }

   return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
   switch (ul_reason_for_call)
   {
     case DLL_PROCESS_ATTACH: 
     {
       CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CTRT_Thread, NULL, 0, NULL);
        return true;
     }
     case DLL_THREAD_ATTACH:
     case DLL_THREAD_DETACH:
     case DLL_PROCESS_DETACH:
      break;
   }

   return true;
}