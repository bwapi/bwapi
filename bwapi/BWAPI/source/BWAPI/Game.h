#pragma once

namespace BW { struct UnitArray; }
namespace BW { struct Unit; }
namespace BWAPI { class Player; }
namespace BWAPI { class Unit; }
namespace BWAPI { class Command; }
class Logger;

#include "..//BW//OrderTypes.h"
#include "..//BW//Offsets.h"

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
    void printXY(int x, int y, const char* text);
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
    void onCancelTrain();
    /** 
     * Starts the game in the pre-game lobby.
     * Should be used only in the pre-game lobby, and not during counting
     */
    void startGame();
    /** Gets mouse cursor horizontal position in pixels. */
    int getMouseX();
    /** Gets mouse cursor vertical position in pixels. */
    int getMouseY();
    /** Gets horizontal position of game screen in pixels. */
    int getScreenX();
    /** Gets vertical position of game screen in pixels. */
    int getScreenY();
    /** @todo Doesn't work */
    void drawBox(DWORD x, DWORD y, DWORD w, DWORD h, BYTE clr); 
    void refresh();
    Unit* getUnit(int index);
    BW::Unit** saveSelected();
    void loadSelected(BW::Unit** selected);
    void onRemoveUnit(BW::Unit *unit);
    Player* marwin;
    Unit* getFirst();
    template <class Type>
    std::string getBinary(Type value);
    void logUnitList();
    void logUnknownOrStrange();
    /** Count of game-frames passed from game start. */
    int frameCount;
  private :
    bool inGame;
    BW::UnitArray* unitArrayCopy;
    BW::UnitArray* unitArrayCopyLocal;
    Unit* units[BW::UNIT_ARRAY_MAX_LENGTH];
    unsigned int latency;
    std::vector<std::vector<Command *>> commandBuffer;
    std::vector<char *> buffers;
    Unit *first; /** Precomputed address corresponding to BWXFN_FirstUnit */
    Logger *commandLog;
  };
};
 
  

