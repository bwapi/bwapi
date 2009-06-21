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
namespace BWAPI { class AIModule; }

#include "BWAPI/Game.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <windows.h>

#include <Util/RectangleArray.h>
#include <Util/Types.h>

#include <BW/OrderTypes.h>
#include <BW/Offsets.h>
#include <BWAPI/Latency.h>
#include <BW/Latency.h>
#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/Position.h>

#include <BWAPI/Map.h>
#include <BWAPI/Flag.h>
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>


// unknown problem, this is what I want to declare
struct drawQueueStruct
{
  u16 x;
  u16 y;
  u16 w;
  u16 h;
  u8 c;
  u8 l;
};

extern drawQueueStruct drawQueueBox[8][4];
extern drawQueueStruct drawQueueBoxFilled[8];


/**
 * Everything in the BWAPI library that doesn't map or work directly with the bw
 * data.
 */
namespace BWAPI
{
  /** The main class wrapping the whole game data/methods. */
  class GameImpl : public Game
  {
    public :
      virtual std::set< Force* > getForces() const;
      virtual std::set< Player* > getPlayers() const;
      virtual std::set< Unit* > getAllUnits() const;
      virtual std::set< Unit* > getMinerals() const;
      virtual std::set< Unit* > getGeysers() const;
      virtual std::set< Unit* > getNeutralUnits() const;

      virtual BWAPI::Latency::Enum getLatency();
      virtual int getFrameCount() const;
      virtual int getMouseX() const;
      virtual int getMouseY() const;
      virtual int getScreenX() const;
      virtual int getScreenY() const;
      virtual bool isFlagEnabled(BWAPI::Flag::Enum flag) const;
      virtual void enableFlag(BWAPI::Flag::Enum flag);

      virtual std::set<Unit*> unitsOnTile(int x, int y) const;

      virtual int mapWidth() const;
      virtual int mapHeight() const;
      virtual std::string mapFilename() const;
      virtual std::string mapName() const;
      virtual bool buildable(int x, int y) const;
      virtual bool walkable(int x, int y) const;
      virtual bool visible(int x, int y) const;
      virtual int groundHeight(int x, int y) const; 
      virtual const std::set< TilePosition >& getStartLocations() const;
      virtual int getMapHash() const;
      /**
       * Prints text in game (only local)
       * @param text Text to be written
       */
      virtual void print(const char *text, ...);
      virtual void printPublic(const char *text, ...);
      /**
       * Changes race in the pre-game lobby.
       * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
       */
      virtual void changeRace(BWAPI::Race race);
      /**
       * Starts the game in the pre-game lobby. Should be used only in the
       * pre-game lobby, and not during counting
       */
      virtual void startGame();
      virtual void pauseGame();
      virtual void resumeGame();
      virtual const std::set<BWAPI::Unit*>& getSelectedUnits() const;
      virtual Player* self() const;
      virtual Player* enemy() const;


      //Internal BWAPI commands:
      GameImpl();
      ~GameImpl();
      void update(); /**< Updates unitArrayCopy according to bw memory */
      PlayerImpl* players[12];
      /**
       * Changes slot state in the pre-game lobby.
       * @param slot Desired state of the slot (Open/Closed/Computer)
       * @param slotID Order of the slot (0 based)
       */
      void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID);
      void setOnStartCalled(bool onStartCalled);
      void IssueCommand(PBYTE pbBuffer, u32 iSize);
      void addToCommandBuffer(Command *command);
      void onGameStart();
      void onGameEnd();
      void onCancelTrain();
      bool onSendText(const char* text);
      bool parseText(const char* text);
      void onRemoveUnit(BW::Unit *unit);
      void lockFlags();
      bool enabled;
      bool isOnStartCalled() const;
      bool isInGame() const;

      /** @todo Doesn't work */
      void refresh();
      void loadSelected();
      void printEx(s32 pID, const char *text, ...);

      UnitImpl* getFirst();
      Util::Logger *fatalError;

      std::set<UnitImpl*> units;
      bool quietSelect;
      UnitImpl* getUnit(int index);
      PlayerImpl* BWAPIPlayer;
      PlayerImpl* opponent;

/*      struct drawQueueStruct
      {
        u16 x;
        u16 y;
        u16 w;
        u16 h;
        u8 c;
        u8 l;
      };

      drawQueueStruct drawQueueBox[8][4];
      drawQueueStruct drawQueueBoxFilled[8];*/

      // Stuff for drawing to the screen
      virtual void drawBoxFilled(u16 x, u16 y, u16 width, u16 height, u8 color, u8 layer);
      virtual void drawBox(u16 x, u16 y, u16 width, u16 height, u8 color, u8 lineWidth, u8 layer);
      virtual void drawLine(u16 x, u16 y, u16 width, u16 height, u8 color, u8 lineWidth, u8 layer);
    private :
      HMODULE hMod;
      /**
       * Representation of the configuration file bw-api.ini in the starcraft
       * directory.
       */
      Util::Dictionary* configuration;
      void saveSelected();
      Map map;
      std::set<BWAPI::Unit*> selectedUnitSet;
      std::set<BWAPI::Unit*> emptySet;
      std::set<TilePosition> startLocations;
      std::set< BW::UnitType > unitTypes;
      std::set<BWAPI::Force*> forces;
      Util::RectangleArray<std::set<Unit*> > unitsOnTileData;
      /** Count of game-frames passed from game start. */
      int frameCount;
      void logUnitList();
      void logUnknownOrStrange();
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
      AIModule* client;
      bool startedClient;
  };
  /**
   * Broodwar is, and always should be the ONLY instance of the Game class, it is singleton.
   * As there is only one instance, the class is defined globaly and as value (not pointer), so the instance 
   * is constructed automatically and there is also no need to care about the destructor.
   * We wanted to save passing the Game parameter everywhere, so we expect everywhere in the code that this
   * variable is instantialised.
   */
  extern GameImpl BroodwarImpl;
};
 
  

