#pragma once

#include <list>

#include "../Types.h"

#include "../BW/UnitType.h"
#include "../BW/Offsets.h"

#include "../BW/Race.h"

namespace BW { struct Unit; };

namespace BWAPI
{
  class Unit;
  /** 
  * Represents one player in game, note that there is always player 12 who owns resources.
  */
  class Player
  {
  public :
    Player(u8 id);
    ~Player();
    char* getName() const;
    void  setName(const char* const name);
    s32   getMinerals() const;
    s32   getMineralsLocal() const;
    s32   getGas() const;
    s32   getGasLocal() const;
    u8    getID() const;
    void  spendLocal(s32 minerals, s32 gas);

	  s32   getSuppliesAvailable(BW::Race::Enum race);
    s32   getSuppliesUsed(BW::Race::Enum race);
    s32   getSuppliesAvailableLocal(BW::Race::Enum race);
    s32   getSuppliesUsedLocal(BW::Race::Enum race);
    s32   freeSuppliesLocal(BW::Race::Enum race);
    s32   usedSuppliesLocal(BW::Race::Enum race);

    void  useSuppliesProtossLocal(u8 supplies);
    void  useSuppliesTerranLocal(u8 supplies);
    void  useSuppliesZergLocal(u8 supplies);

    /** 
    * List of units owned by the current player, this structure owns (so the player needs 
    * to be deleted upon removal.
    */
    std::list<Unit*> units; 
    BW::Unit** selectedUnit();
    void update();
    bool canBuild(BW::UnitType unit);
    u16 unitTypeCount[BW::unitTypeCount];
    /** 
     * Finds out if the current player can afford to build the specified unit atm
     * Checks minerals, gas and supply need
     * @param unit Unit type to be checked
     * @param Money that must the player keep (reserved to make building or other reason)
     * @returns @c true if the unit can be trained, @c false otherwise.
     */
    bool canAfford(BW::UnitType unit, u16 mineralsToKeep = 0);
  private :
    u8 id;  /**< Order of the player, is used to load player's information from the memory */
    s32 mineralsLocal, gasLocal;

    s32 suppliesAvailableProtossLocal;
    s32 suppliesUsedProtossLocal;

    s32 suppliesAvailableTerranLocal;
    s32 suppliesUsedTerranLocal;
    
    s32 suppliesAvailableZergLocal;
    s32 suppliesUsedZergLocal;
  };
};
