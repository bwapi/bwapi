#pragma once

namespace BW { struct UnitArray; }
namespace BW { struct UnitData; }
namespace BWAPI { class Player; }
namespace BWAPI { class Unit; }
namespace BWAPI { class Command; }
#include "..\\BW\\OrderTypes.h"
#include <vector>
#include <windows.h>
/** Everything in the BWAPI library that doesn't map or work directly with the bw data. */
namespace BWAPI
{
  /** The main class wrapping the whole game data/methods. */
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
    bool isInGame();
    void setInGame(bool inGame);
    /** 
     * Changes slot state in the pre-game lobby.
     * @param slot Desired state of the slot (Open/Closed/Computer)
     * @param slotID Order of the slot (0 based)
     */
    void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID);
    /** 
     * Changes race in the pre-game lobby.
     * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
     * @param slotID Order of the slot (0 based)
     */
    void changeRace(BW::Orders::ChangeRace::Race race, u8 slotID);
    /** 
     * This function is called after every update (in DLLMain), and is 
     * used for debug and other reasons
     */
    void test(void);
    void __fastcall IssueCommand(PBYTE pbBuffer, int iSize);
    void addToCommandBuffer(Command *command);
    void onGameStart();
    void onGameEnd();
    /** 
     * Starts the game in the pre-game lobby.
     * Should be used only in the pre-game lobby, and not during counting
     */
    void startGame();
    int getMouseX();
    int getMouseY();
    int getScreenX();
    int getScreenY();
  private :
    bool inGame;
    BW::UnitArray* unitArrayCopy;
    BW::UnitArray* unitArrayCopyLocal;
    Unit* units[1700];
    unsigned int latency;
    std::vector<std::vector<Command *>> commandBuffer;
    /** Count of game-frames passed from game start. */
    int frameCount;
  };
};
 
  

