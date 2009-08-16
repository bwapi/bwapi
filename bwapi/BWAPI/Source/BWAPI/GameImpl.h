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
#include "Shape.h"
#include "ShapeBox.h"
#include "ShapeLine.h"

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
      virtual std::set< Force* > getForces();
      virtual std::set< Player* > getPlayers();
      virtual std::set< Unit* > getAllUnits();
      virtual std::set< Unit* > getMinerals();
      virtual std::set< Unit* > getGeysers();
      virtual std::set< Unit* > getNeutralUnits();

      virtual BWAPI::Latency::Enum getLatency();
      virtual int getFrameCount();
      virtual int getMouseX();
      virtual int getMouseY();
      virtual int getScreenX();
      virtual int getScreenY();
      virtual void setScreenPosition(int x, int y);

      virtual bool isFlagEnabled(BWAPI::Flag::Enum flag);
      virtual void enableFlag(BWAPI::Flag::Enum flag);
      virtual std::set<Unit*> unitsOnTile(int x, int y);
      virtual Error getLastError() const;

      virtual int mapWidth();
      virtual int mapHeight();
      virtual std::string mapFilename();
      virtual std::string mapName();
      virtual bool buildable(int x, int y);
      virtual bool walkable(int x, int y);
      virtual bool visible(int x, int y);
      virtual bool hasCreep(int x, int y);
      virtual bool hasPower(int x, int y, int tileWidth, int tileHeight);
      virtual bool canBuildHere(Unit* builder, TilePosition position, UnitType type);
      virtual bool canMake(Unit* builder, UnitType type);
      virtual bool canResearch(Unit* unit, TechType type);
      virtual bool canUpgrade(Unit* unit, UpgradeType type);
      virtual int groundHeight(int x, int y);
      virtual const std::set< TilePosition >& getStartLocations();
      virtual int getMapHash();
      /**
       * Prints text in game (only local)
       * @param text Text to be written
       */
      virtual void printf(const char* text, ...);
      virtual void sendText(const char* text, ...);
      virtual void text(CoordinateType::Enum ctype, int x, int y, const char* text, ...);
      /**
       * Changes race in the pre-game lobby.
       * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
       */
      virtual void changeRace(BWAPI::Race race);
      /**
       * Starts the game in the pre-game lobby. Should be used only in the
       * pre-game lobby, and not during counting
       */
      virtual bool inReplay();
      virtual void startGame();
      virtual void pauseGame();
      virtual void resumeGame();
      virtual void surrender();
      virtual const std::set<BWAPI::Unit*>& getSelectedUnits();
      virtual Player* self();
      virtual Player* enemy();
      virtual void drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
      virtual void drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      virtual void drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid = false);
      virtual void drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      virtual void drawDot(CoordinateType::Enum ctype, int x, int y, Color color);
      virtual void drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color);


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
      void addToCommandBuffer(Command* command);
      void onGameStart();
      void onGameEnd();
      void onCancelTrain();
      bool onSendText(const char* text);
      bool parseText(const char* text);
      void onRemoveUnit(BWAPI::UnitImpl* unit);
      void onRemoveUnit(BW::Unit* unit);
      void onAddUnit(BWAPI::Unit* unit);
      void lockFlags();
      bool enabled;
      bool isSinglePlayer() const;
      bool isOnStartCalled() const;
      bool inGame() const;
      void setLastError(BWAPI::Error e);

      /** @todo Doesn't work */
      void refresh();
      void loadSelected();
      void printEx(s32 pID, const char* text, ...);

      UnitImpl* getFirst();
      Util::Logger* fatalError;

      std::set<UnitImpl*> units;
      UnitImpl* getUnit(int index);
      PlayerImpl* BWAPIPlayer;
      PlayerImpl* opponent;

      int _getMouseX() const;
      int _getMouseY() const;
      int _getScreenX() const;
      int _getScreenY() const;
      void addShape(Shape* s);
      std::vector<Shape*> shapes;
      std::vector<Shape*> cachedShapes;
      HANDLE hcachedShapesMutex;
      /** Unknown unitID's */
      Util::Logger* newUnitLog;
      std::set<int> invalidIndices;
      bool flagsLocked;
      bool inUpdate;
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
      std::list<BWAPI::UnitImpl*> myPylons;
      Util::RectangleArray<std::set<Unit*> > unitsOnTileData;
      /** Count of game-frames passed from game start. */
      int frameCount;
      void logUnitList();
      void logUnknownOrStrange();
      bool onStartCalled;
      BW::UnitArray* unitArrayCopyLocal;
      UnitImpl* unitArray[BW::UNIT_ARRAY_MAX_LENGTH];
      std::vector<std::vector<Command *> > commandBuffer;
      /** All commands ordered from BWAPI */
      Util::Logger* commandLog;
      /** Unknown orderID's */
      Util::Logger* newOrderLog;
      /** Failed check of assumption */
      Util::Logger* badAssumptionLog;
      /** Sum of all units*/
      Util::Logger* unitSum;
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
      BW::Unit* savedSelectionStates[13];
      void refreshSelectionStates();
      AIModule* client;
      bool startedClient;
      BWAPI::Error lastError;
      std::list<UnitImpl*> deadUnits;
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



