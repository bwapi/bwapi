#include "Unit.h"

#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Player.h"
#include "../BWAPI/Globals.h"

#include "../BW/UnitData.h"
#include "../BW/Offsets.h"
#include "../BW/UnitTypes.h"

namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  Unit::Unit(BW::UnitData* unitData)
    :bwUnitData(unitData)
  {
  }
  //----------------------------- DESTRUCTOR -----------------------------------
  Unit::~Unit()
  {
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getHealthPoints() const
  {
    return bwUnitData->healthPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getHealthPointsFraction() const
  {
    return bwUnitData->healthPointsFraction;
  }
  //-------------------------- GET HEALTH POINTS --------------------------------
  u16 Unit::getShieldPoints() const
  {
    return bwUnitData->shieldPoints;
  }
  //----------------------- GET HEALT POINTS FACTION ----------------------------
  u8 Unit::getShieldPointsFraction() const
  {
    return bwUnitData->shieldPointsFraction;
  }
  //--------------------------  GET UNIT PROTOTYPE -----------------------------
  const UnitPrototype* const Unit::getPrototype() const
  {
     switch (this->bwUnitData->unitID)
     {
       case BW::UnitType::Terran_SCV             : return BWAPI::Prototypes::SCV;
       case BW::UnitType::Terran_CommandCenter   : return BWAPI::Prototypes::CommandCenter;
       case BW::UnitType::Terran_SupplyDepot     : return BWAPI::Prototypes::SupplyDepot;
       case BW::UnitType::Terran_Barracks        : return BWAPI::Prototypes::Barracks;
       case BW::UnitType::Terran_Marine          : return BWAPI::Prototypes::Marine;
       /** Protoss*/
       case BW::UnitType::Protoss_Probe          : return BWAPI::Prototypes::Probe;
       case BW::UnitType::Protoss_Zealot         : return BWAPI::Prototypes::Zealot;
       case BW::UnitType::Protoss_Nexus          : return BWAPI::Prototypes::Nexus;
       case BW::UnitType::Protoss_Pylon          : return BWAPI::Prototypes::Pylon;
       case BW::UnitType::Protoss_Gateway        : return BWAPI::Prototypes::Gateway;
       /** Zerg */
       case BW::UnitType::Zerg_Queen             : return BWAPI::Prototypes::Queen;
       case BW::UnitType::Zerg_Larva             : return BWAPI::Prototypes::Larva;
       case BW::UnitType::Zerg_Drone             : return BWAPI::Prototypes::Drone;
       case BW::UnitType::Zerg_Zergling          : return BWAPI::Prototypes::Zergling;
       case BW::UnitType::Zerg_Overlord          : return BWAPI::Prototypes::Overlord;
       case BW::UnitType::Zerg_Hatchery          : return BWAPI::Prototypes::Hatchery;
       case BW::UnitType::Zerg_SpawningPool      : return BWAPI::Prototypes::SpawningPool;
       /** Neutral */
       case BW::UnitType::Resource_MineralPatch1 :
       case BW::UnitType::Resource_MineralPatch2 :
       case BW::UnitType::Resource_MineralPatch3 : return BWAPI::Prototypes::Minerals;
       default : return NULL;
     }

    return NULL;
  }
  //------------------------------ CAN ORDER -----------------------------------
  bool Unit::canOrder(const AbilityPrototype* const ability, Unit* target) const
  {
    if (!this->getPrototype()->canOrder(ability, target))
      return false;

    if (this == target &&
      !(ability->getTargetFlags() & TargetType::TARGET_SELF))
      return false;

    if (this->getOwner() == target->getOwner() &&
      !(ability->getTargetFlags() & TargetType::TARGET_MY_UNITS))
      return false;

    if (this->getOwner() == target->getOwner() &&
      !(ability->getTargetFlags() & TargetType::TARGET_OPPONENTS_UNITS))
      return false;

    return true;
  }
  //------------------------------ CAN ORDER -----------------------------------
  bool Unit::canOrder(const AbilityPrototype* const ability, const BW::Position& target) const
  {
    if (!this->getPrototype()->canOrder(ability, target))
      return false;
    return true;
  }
  //-------------------------------- ORDER -------------------------------------
  void Unit::order(const AbilityPrototype* const ability, Unit* target)
  {

  }

#pragma warning(push)
#pragma warning(disable:4312)
  void (_stdcall*sendCommand)(int, int, int, int) = (void(_stdcall*)(int, int, int, int))0x4858F0;
  void (_stdcall*cancel)() = (void(_stdcall*)())0x423480;
  void (_stdcall*printXY)(int x, int y, char * text) = (void(_stdcall*)(int x, int y, char * text))0x4202A0;


  void (_stdcall*drawBox)(int x, int y, int width, int height, u8 color) = (void(_stdcall*)(int x, int y, int width, int height, u8 color))0x004E18C0;
  void (_stdcall*printText)(int a, char * text, int b) = (void(_stdcall*)(int a, char * text, int b))0x0048C9D0;
  
#pragma warning(pop)

  //--------------------------------- ORDER ------------------------------------
  void Unit::order(const AbilityPrototype* const ability, const BW::Position& target)
  {
    sendCommand(6, 0xe4, target.y, 0);
    //sendcommand(command, 0xe4, y, x);
  }
  //--------------------------------- ORDER ------------------------------------
  void Unit::order(int commandCode, const BW::Position& target)
  {
   //printText(3, "Hello", 0);
    //sendCommand(commandCode, 0xe4, target.x, target.y);
   /*DWORD x = 100, y = 100 ,w = 100 ,h = 100;
   BYTE clr = 50;
   __asm {
        mov al, clr
	       mov ds:[6CF494h], al
        push h 
        push w 
        push y
        push x
        call dword ptr ds:[4E18E0h]
   }*/

   //printXY(100, 100, "Hello");
    //sendcommand(command, 0xe4, y, x);
  }
  //------------------------------- GET OWNER ----------------------------------
  Player* Unit::getOwner() const
  {
    return Broodwar.players[this->bwUnitData->playerID];
  }
  //-------------------------------- IS VALID ----------------------------------
  bool Unit::isValid() const
  {
    return this->getHealthPoints() > 0 || 
           this->getHealthPointsFraction() > 0;
  }
  //----------------------------------------------------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return bwUnitData->currentPos;
  }
  //----------------------------------------------------------------------------
};
