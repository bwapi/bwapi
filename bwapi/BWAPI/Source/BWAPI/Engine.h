#pragma once

namespace Util  { class Logger; }
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
  namespace Engine
  {
      extern std::set< Force* >&  getForces();
      extern std::set< Player* >&  getPlayers();
      extern std::set< Unit* >&  getAllUnits();
      extern std::set< Unit* >&  getMinerals();
      extern std::set< Unit* >&  getGeysers();
      extern std::set< Unit* >&  getNeutralUnits();

      extern std::set< Unit* >& getStaticMinerals();
      extern std::set< Unit* >& getStaticGeysers();
      extern std::set< Unit* >& getStaticNeutralUnits();

      extern int  getLatency();
      extern int  getFrameCount();
      extern int  getMouseX();
      extern int  getMouseY();
      extern int  getScreenX();
      extern int  getScreenY();

      extern bool  isFlagEnabled(int flag);
      extern void  enableFlag(int flag);
      extern std::set<Unit*>& unitsOnTile(int x, int y);
      extern Error  getLastError();

      extern int  mapWidth();
      extern int  mapHeight();
      extern std::string  mapFilename();
      extern std::string  mapName();
      extern int getMapHash();

      extern int  getGroundHeight(int x, int y);
      extern bool isWalkable(int x, int y);
      extern bool isBuildable(int x, int y);
      extern bool isVisible(int x, int y);
      extern bool isExplored(int x, int y);
      extern bool hasCreep(int x, int y);
      extern bool hasPower(int x, int y, int tileWidth, int tileHeight);


      extern bool isBuildable(TilePosition position);
      extern bool isVisible(TilePosition position);
      extern bool isExplored(TilePosition position);
      extern bool hasCreep(TilePosition position);
      extern bool hasPower(TilePosition position, int tileWidth, int tileHeight);

      extern bool canBuildHere(Unit* builder, TilePosition position, UnitType type);
      extern bool canMake(Unit* builder, UnitType type);
      extern bool canResearch(Unit* unit, TechType type);
      extern bool canUpgrade(Unit* unit, UpgradeType type);
      extern std::set< TilePosition >& getStartLocations();
      /**
       * Prints text in game (only local)
       * @param text Text to be written
       */
      extern void  printf(const char* text, ...);
      extern void  sendText(const char* text, ...);

      /**
       * Changes race in the pre-game lobby.
       * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
       */
      extern void  changeRace(BWAPI::Race race);
      extern bool  isMultiplayer();
      extern bool  isPaused();
      extern bool  isReplay();
      /**
       * Starts the game in the pre-game lobby. Should be used only in the
       * pre-game lobby, and not during counting
       */
      extern void  startGame();
      extern void  pauseGame();
      extern void  resumeGame();
      extern void  leaveGame();
      extern void  restartGame();
      extern void  setLocalSpeed(int speed = -1);
      extern std::set<BWAPI::Unit*>& getSelectedUnits();
      extern Player*  self();
      extern Player*  enemy();

      extern void drawText(int ctype, int x, int y, const char* text, ...);
      extern void drawTextMap(int x, int y, const char* text, ...);
      extern void drawTextMouse(int x, int y, const char* text, ...);
      extern void drawTextScreen(int x, int y, const char* text, ...);

      extern void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
      extern void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      extern void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      extern void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);

      extern void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      extern void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      extern void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      extern void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);

      extern void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false);
      extern void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
      extern void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
      extern void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);

      extern void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      extern void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      extern void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      extern void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);

      extern void drawDot(int ctype, int x, int y, Color color);
      extern void drawDotMap(int x, int y, Color color);
      extern void drawDotMouse(int x, int y, Color color);
      extern void drawDotScreen(int x, int y, Color color);

      extern void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color);
      extern void drawLineMap(int x1, int y1, int x2, int y2, Color color);
      extern void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
      extern void drawLineScreen(int x1, int y1, int x2, int y2, Color color);

      extern bool gluMessageBox(char* message, int type);
      extern bool gluEditBox(char* message, char* dest, size_t destsize, char* restricted);

      extern void update(); /**< Updates unitArrayCopy according to bw memory */
      extern void onMenuFrame();
      extern void setScreenPosition(int x, int y);
      extern void init();
      /**
       * Changes slot state in the pre-game lobby.
       * @param slot Desired state of the slot (Open/Closed/Computer)
       * @param slotID Order of the slot (0 based)
       */
      extern void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID);
      extern void setOnStartCalled(bool onStartCalled);
      extern void IssueCommand(PBYTE pbBuffer, u32 iSize);
      extern void addToCommandBuffer(Command* command);
      extern void onGameStart();
      extern void onGameEnd();
      extern bool onSendText(const char* text);
      extern bool parseText(const char* text);
      extern void onUnitDeath(BWAPI::UnitImpl* unit);
      extern void onUnitDeath(BW::Unit* unit);
      extern void onAddUnit(BWAPI::Unit* unit);
      extern bool inScreen(int ctype, int x, int y);
      extern bool inScreen(int ctype, int x1, int y1, int x2, int y2);
      extern bool inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3);
      extern void lockFlags();
      extern bool isOnStartCalled();
      extern bool inGame();
      extern bool _isSinglePlayer();
      extern bool _isReplay();
      extern void setLastError(BWAPI::Error e);
      extern void addInterceptedMessage(const char* message);
      extern UnitImpl* getUnit(int index);
      extern bool flagsAreLocked();
      extern bool isInUpdate();
      extern PlayerImpl* players[12];
      extern bool enabled;
      extern HANDLE hcachedShapesMutex;
      extern std::set<int> invalidIndices;
      extern std::vector<Shape*> cachedShapes;
      extern Util::Logger* newUnitLog;
  };
  /**
   * Broodwar is, and always should be the ONLY instance of the Game class, it is singleton.
   * As there is only one instance, the class is defined globaly and as value (not pointer), so the instance
   * is constructed automatically and there is also no need to care about the destructor.
   * We wanted to save passing the Game parameter everywhere, so we expect everywhere in the code that this
   * variable is instantialised.
   */
};
