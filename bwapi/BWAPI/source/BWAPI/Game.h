#pragma once

namespace BW { struct UnitArray; };
namespace BWAPI { class Player; };
namespace BWAPI { class Unit; };
#include "..\\BW\\OrderTypes.h"


namespace BWAPI
{
  /** 
  * The main class wrapping the whole game data/methods.
  */
  class Game
  {
  public :
    Game();
    ~Game();
    void update(); /**< Updates unitArrayCopy according to bw memory */
    Player* players[12];
    void printXY(int x, int y, char* text) const;
    /** 
     * Prints text in game (only local)
     * @param text Text to be written
     */
    void print(char *text) const;
    void printPublic(char *text) const; // doesn't work now
    bool inGame(); // doesn't work now
    /** 
     * Changes slot state in the pre-game lobby.
     * @param Slot Desired state of the slot (Open/Closed/Computer)
     * @param slotID Order of the slot (0 based)
     */
    void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID);
    /** 
     * Changes race in the pre-game lobby.
     * @param Race Desired race of the slot (Zerg/Protoss/Terran/Random)
     * @param slotID Order of the slot (0 based)
     */
    void changeRace(BW::Orders::ChangeRace::Race race, u8 slotID);
    /** 
     * This function is called after every update (in DLLMain), and is 
     * used for debug and other reasons
     */
    void test(void);
  private :
    BW::UnitArray* unitArrayCopy;
    Unit* units[1700];
  };
};

