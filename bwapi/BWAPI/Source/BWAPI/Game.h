#pragma once

namespace Util  { class Logger; }
namespace Util  { class Dictionary; }
namespace BW    { struct UnitArray; }
namespace BW    { struct Unit; }
namespace BWAPI { class Player; }
namespace BWAPI { class Unit; }
namespace BWAPI { class Command; }

#include <vector>
#include <list>
#include <map>
#include <set>
#include <windows.h>

#include <Util/RectangleArray.h>

#include <BW/OrderTypes.h>
#include <BW/Offsets.h>
#include <BW/Latency.h>
#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/Position.h>

#include <BWAPI/Map.h>
#include <BWAPI/Flag.h>
/**
 * Everything in the BWAPI library that doesn't map or work directly with the bw
 * data.
 */
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

      const std::set< BW::TilePosition >& getStartLocations() const;
      const std::set< BW::UnitType >& allUnitTypes() const;

      /**
       * Prints text in game (only local)
       * @param text Text to be written
       */
      void print(const char *text, ...);
      void printEx(s32 pID, const char *text, ...);
      void printPublic(const char *text, ...);
      bool isOnStartCalled() const;
      void setOnStartCalled(bool onStartCalled);
      bool isInGame() const;
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
      void changeRace(BW::Race::Enum race, u8 slotID);
      /**
       * This function is called after every update (in DLLMain), and is used
       * for debug and other reasons
       */
      void test(void);
      void IssueCommand(PBYTE pbBuffer, u32 iSize);
      void addToCommandBuffer(Command *command);
      void onGameStart();
      void onGameEnd();
      void onCancelTrain();
      bool onSendText(const char* text);
      /**
       * Starts the game in the pre-game lobby. Should be used only in the
       * pre-game lobby, and not during counting
       */
      void startGame();
      void pauseGame();
      void resumeGame();
      /** Gets mouse cursor horizontal position in pixels. */
      int getMouseX() const;
      /** Gets mouse cursor vertical position in pixels. */
      int getMouseY() const;
      /** Gets horizontal position of game screen in pixels. */
      int getScreenX() const;
      /** Gets vertical position of game screen in pixels. */
      int getScreenY() const;
      /** @todo Doesn't work */
      void refresh();
      Unit* getUnit(int index);
      void saveSelected();
      void loadSelected();
      const std::set<BWAPI::Unit*>& getSelectedUnits() const;
      void onRemoveUnit(BW::Unit *unit);
      Player* BWAPIPlayer;
      Player* opponent;
      Util::Logger *fatalError;
      bool quietSelect;
      BW::Latency::Enum getLatency();
      /**
       * Representation of the configuration file bw-api.ini in the starcraft
       * directory.
       */
      Util::Dictionary* configuration;
      /** Every tile will have pointers to units touching it. */
      BW::UnitType getUnitType(std::string &name) const;
      BW::TechType getTechType(std::string &name) const;
      BW::UpgradeType getUpgradeType(std::string &name) const;
      std::list<Unit*> unitsOnTile(int x, int y) const;
      int getFrameCount() const;
      bool enabled;
      Map map;
      Unit* getFirst();

      bool isFlagEnabled(BWAPI::Flag::Enum flag) const;
      void enableFlag(BWAPI::Flag::Enum flag);
      void lockFlags();
    private :
      std::set<BWAPI::Unit*> selectedUnitSet;
      std::set<BWAPI::Unit*> emptySet;
      std::set<BW::TilePosition> startLocations;
      std::set< BW::UnitType > unitTypes;
      Util::RectangleArray<std::list<Unit*> > unitsOnTileData;
      std::vector<Unit*> units;
      /** Count of game-frames passed from game start. */
      int frameCount;
      void logUnitList();
      void logUnknownOrStrange();
      std::map<std::string, BW::UnitType> unitNameToType;
      std::map<std::string, BW::TechType> techNameToType;
      std::map<std::string, BW::UpgradeType> upgradeNameToType;
      bool onStartCalled;
      BW::UnitArray* unitArrayCopy;
      BW::UnitArray* unitArrayCopyLocal;
      Unit* unitArray[BW::UNIT_ARRAY_MAX_LENGTH];
      BW::Latency::Enum latency;
      std::vector<std::vector<Command *> > commandBuffer;
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
      /** Will update the unitsOnTile content, should be called every frame. */
      void updateUnitsOnTile();
      /** Made for debug reasons, may come later someday, so I leave it*/
      void printUnitCountPerTile();
      /**
       * Specifies if some order was given, so the loadSelect function will have
       * to be called.
       */
      bool reselected;
      bool flags[BWAPI::FLAG_COUNT];
      bool flagsLocked;
      BW::Unit* savedSelectionStates[13];
      void refreshSelectionStates();
  };
};
 
  

