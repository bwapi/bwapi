#pragma once

#include <list>
#include "../Types.h"
namespace BW { struct UnitData; }

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
    s32   getGas() const;
    u8    getID() const;
    /** 
    * List of units owned by the current player, this structure owns (so the player needs 
    * to be deleted upon removal.
    */
    std::list<Unit*> units; 
    BW::UnitData** selectedUnit();

  private :
    u8 id;  /**< Order of the player, is used to load player's information from the memory */
  };
};
