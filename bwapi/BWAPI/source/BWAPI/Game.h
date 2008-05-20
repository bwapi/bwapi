#pragma once

namespace Util  { class Logger; }
namespace BW    { struct UnitArray; }
namespace BW    { struct Unit; }
namespace BWAPI { class Player; }
namespace BWAPI { class Unit; }
namespace BWAPI { class Command; }

class Dictionary;

#include <vector>
#include <list>
#include <windows.h>

#include <RectangleArray.h>

#include "..//BW/OrderTypes.h"
#include "..//BW/Offsets.h"
#include "..//BW/Latency.h"

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
      /** 
       * Prints text in game (only local)
       * @param text Text to be written
       */
      static void print(char *text);
      /** 
       * Doesn't work now
       */
      void printPublic(char *text) const;
      bool isInGame() const;
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
      void IssueCommand(PBYTE pbBuffer, u32 iSize);
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
      int getMouseX() const;
      /** Gets mouse cursor vertical position in pixels. */
      int getMouseY() const;
      /** Gets horizontal position of game screen in pixels. */
      int getScreenX() const;
      /** Gets vertical position of game screen in pixels. */
      int getScreenY() const;
      /** @todo Doesn't work */
      void drawBox(DWORD x, DWORD y, DWORD w, DWORD h, BYTE clr); 
      void refresh();
      Unit* getUnit(int index);
      BW::Unit** saveSelected();
      void loadSelected(BW::Unit** selected);
      void onRemoveUnit(BW::Unit *unit);
      Player* BWAPIPlayer;
      Unit* getFirst();
      template <class Type>
      std::string getBinary(Type value);
      void logUnitList();
      void logUnknownOrStrange();
      /** Count of game-frames passed from game start. */
      int frameCount;
      Util::Logger *fatalError;
      bool quietSelect;
      BW::Latency::Enum getLatency();
      /** Representation of the configuration file bw-api.ini in the starcraft directory. */
      Dictionary* configuration;
    private :
      bool inGame;
      BW::UnitArray* unitArrayCopy;
      BW::UnitArray* unitArrayCopyLocal;
      Unit* units[BW::UNIT_ARRAY_MAX_LENGTH];
      BW::Latency::Enum latency;
      std::vector<std::vector<Command *> > commandBuffer;
      std::vector<char *> buffers;
      Unit *first; /** Precomputed address corresponding to BWXFN_FirstUnit */
      /** All commands ordered from BWAPI */
      Util::Logger *commandLog;
      /** Unknown orderID's */
      Util::Logger *newOrderLog;
      /** Unknown unitID's */
      Util::Logger *newUnitLog;
      /** Failed check of assumption */
      Util::Logger *badAssumptionLog;
      /** Sum of all units*/
      Util::Logger *unitSum;
      /** Every tile will have pointers to units touching it. */
      RectangleArray<std::list<Unit*> > unitsOnTile;
      /** Will update the unitsOnTile content, should be called every frame. */
      void updateUnitsOnTile();
      /** Made for debug reasons, may come later someday, so I leave it*/
      void printUnitCountPerTile();
  };
};
 
  

