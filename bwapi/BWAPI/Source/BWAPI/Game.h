#pragma once

namespace Util  { class Logger; }
namespace Util  { class Dictionary; }
namespace BW    { struct UnitArray; }
namespace BW    { struct Unit; }
namespace BWAPI { class Player; }
namespace BWAPI { class PlayerImpl; }
namespace BWAPI { class Unit; }
namespace BWAPI { class UnitImpl; }
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

      std::set< Player* > getPlayers() const;
      std::set< Unit* > getAllUnits() const;
      std::set< Unit* > getMinerals() const;
      std::set< Unit* > getGeysers() const;
      std::set< Unit* > getNeutralUnits() const;

      BW::Latency::Enum getLatency();
      int getFrameCount() const;
      int getMouseX() const;
      int getMouseY() const;
      int getScreenX() const;
      int getScreenY() const;
      bool isFlagEnabled(BWAPI::Flag::Enum flag) const;
      void enableFlag(BWAPI::Flag::Enum flag);

      std::list<Unit*> unitsOnTile(int x, int y) const;

      const std::set< BW::UnitType >& allUnitTypes() const;
      BW::UnitType getUnitType(std::string &name) const;

      BW::UpgradeType getUpgradeType(std::string &name) const;

      BW::TechType getTechType(std::string &name) const;

      int mapWidth() const;
      int mapHeight() const;
      std::string mapFilename() const;
      std::string mapName() const;
      bool buildable(int x, int y) const;
      bool walkable(int x, int y) const;
      bool visible(int x, int y) const;
      int groundHeight(int x, int y) const; 
      const std::set< BW::TilePosition >& getStartLocations() const;

      /**
       * Prints text in game (only local)
       * @param text Text to be written
       */
      static void print(const char *text, ...);
      static void printEx(s32 pID, const char *text, ...);
      static void printPublic(const char *text, ...);
      bool isOnStartCalled() const;
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
       * Starts the game in the pre-game lobby. Should be used only in the
       * pre-game lobby, and not during counting
       */
      void startGame();
      void pauseGame();
      void resumeGame();
      const std::set<BWAPI::Unit*>& getSelectedUnits() const;
      Player* self() const;
      Player* enemy() const;


      //Internal BWAPI commands:
      Game();
      ~Game();
      void update(); /**< Updates unitArrayCopy according to bw memory */
      PlayerImpl* players[12];

      void setOnStartCalled(bool onStartCalled);
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
      void onRemoveUnit(BW::Unit *unit);
      void lockFlags();
      bool enabled;
      /** @todo Doesn't work */
      void refresh();
      UnitImpl* getUnit(int index);
      void saveSelected();
      void loadSelected();
      /**
       * Representation of the configuration file bw-api.ini in the starcraft
       * directory.
       */
      Util::Dictionary* configuration;

      UnitImpl* getFirst();
      Util::Logger *fatalError;
      bool quietSelect;

      PlayerImpl* BWAPIPlayer;
      PlayerImpl* opponent;
      std::set<UnitImpl*> units;
    private :
      Map map;
      std::set<BWAPI::Unit*> selectedUnitSet;
      std::set<BWAPI::Unit*> emptySet;
      std::set<BW::TilePosition> startLocations;
      std::set< BW::UnitType > unitTypes;
      Util::RectangleArray<std::list<Unit*> > unitsOnTileData;
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
      UnitImpl* unitArray[BW::UNIT_ARRAY_MAX_LENGTH];
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
 
  

