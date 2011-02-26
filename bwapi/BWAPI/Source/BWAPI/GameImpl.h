#pragma once

namespace Util  { class  Logger; }
namespace BW    { class  Unit; }
namespace BW    { struct Bullet; }
namespace BWAPI { class  Player; }
namespace BWAPI { class  PlayerImpl; }
namespace BWAPI { class  Unit; }
namespace BWAPI { class  UnitImpl; }
namespace BWAPI { class  Bullet; }
namespace BWAPI { class  BulletImpl; }
namespace BWAPI { class  Command; }
namespace BWAPI { class  AIModule; }

#include "BWAPI/Game.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <windows.h>
#include <winuser.h>

#include <Util/RectangleArray.h>
#include <Util/Types.h>

#include <BW/OrderTypes.h>
#include <BW/Offsets.h>
#include <BWAPI/Latency.h>
#include <BW/Latency.h>
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
#include "Server.h"

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
      virtual std::set< Force* >&  getForces();
      virtual std::set< Player* >& getPlayers();
      virtual std::set< Unit* >&   getAllUnits();
      virtual std::set< Unit* >&   getMinerals();
      virtual std::set< Unit* >&   getGeysers();
      virtual std::set< Unit* >&   getNeutralUnits();
 
      virtual std::set< Unit* >&   getStaticMinerals();
      virtual std::set< Unit* >&   getStaticGeysers();
      virtual std::set< Unit* >&   getStaticNeutralUnits();

      virtual std::set< Bullet* >& getBullets();
      virtual std::set< Position >& getNukeDots();
      virtual std::list< Event >&  getEvents();

      virtual Force   *getForce(int forceID);
      virtual Player  *getPlayer(int playerID);
      virtual Unit    *getUnit(int unitID);
      virtual Unit    *indexToUnit(int unitIndex);

      virtual GameType getGameType();

      virtual int      getLatency();
      virtual int      getFrameCount();
      virtual int      getFPS();
      virtual double   getAverageFPS();

      virtual Position getMousePosition();
      virtual bool     getMouseState(MouseButton button);
      virtual bool     getMouseState(int button);
      virtual bool     getKeyState(Key key);
      bool             getKeyState(int key);

      virtual Position getScreenPosition();
      virtual void     setScreenPosition(int x, int y);
      virtual void     setScreenPosition(BWAPI::Position p);

      virtual void     pingMinimap(int x, int y);
      virtual void     pingMinimap(BWAPI::Position p);

      virtual bool     isFlagEnabled(int flag);
      virtual void     enableFlag(int flag);

      virtual std::set<Unit*>& getUnitsOnTile(int x, int y);
      virtual std::set<Unit*>& getUnitsInRectangle(int left, int top, int right, int bottom) const;
      virtual std::set<Unit*>& getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight) const;
      virtual std::set<Unit*>& getUnitsInRadius(BWAPI::Position center, int radius) const;

      virtual Error       getLastError() const;
      virtual bool        setLastError(BWAPI::Error e);

      virtual int         mapWidth();
      virtual int         mapHeight();
      virtual std::string mapFileName();
      virtual std::string mapPathName();
      virtual std::string mapName();
      virtual std::string mapHash();

      virtual bool isWalkable(int x, int y);
      virtual int  getGroundHeight(int x, int y);
      virtual int  getGroundHeight(TilePosition position);
      virtual bool isBuildable(int x, int y, bool includeBuildings = false);
      virtual bool isBuildable(TilePosition position, bool includeBuildings = false);
      virtual bool isVisible(int x, int y);
      virtual bool isVisible(TilePosition position);
      virtual bool isExplored(int x, int y);
      virtual bool isExplored(TilePosition position);
      virtual bool hasCreep(int x, int y);
      virtual bool hasCreep(TilePosition position);
      virtual bool hasPower(int x, int y, int tileWidth, int tileHeight);
      virtual bool hasPower(TilePosition position, int tileWidth, int tileHeight);

      virtual bool canBuildHere(const Unit* builder, TilePosition position, UnitType type, bool checkExplored = false);
      virtual bool canMake(const Unit* builder, UnitType type);
      virtual bool canResearch(const Unit* unit, TechType type);
      virtual bool canUpgrade(const Unit* unit, UpgradeType type);

      virtual std::set< TilePosition >& getStartLocations();

      /**
       * Prints text in game (only local)
       * @param format Text to be written
       */
      virtual void printf(const char *format, ...);
      virtual void sendText(const char *format, ...);
      virtual void sendTextEx(bool toAllies, const char *format, ...);

      /**
       * Changes race in the pre-game lobby.
       * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
       */
      virtual void changeRace(BWAPI::Race race);
      virtual bool isInGame();
      virtual bool isMultiplayer();
      virtual bool isBattleNet();
      virtual bool isPaused();
      virtual bool isReplay();

      /**
       * Starts the game in the pre-game lobby. Should be used only in the
       * pre-game lobby, and not during counting
       */
      virtual void startGame();
      virtual void pauseGame();
      virtual void resumeGame();
      virtual void leaveGame();
      virtual void restartGame();
      virtual void setLocalSpeed(int speed = -1);
      virtual bool issueCommand(const std::set<BWAPI::Unit*>& units, UnitCommand command);
      virtual std::set<BWAPI::Unit*>& getSelectedUnits();
      virtual Player *self();
      virtual Player *enemy();
      virtual Player *neutral();
      virtual std::set<BWAPI::Player*>& allies();
      virtual std::set<BWAPI::Player*>& enemies();

      virtual void setTextSize(int size = 1);
      virtual void drawText(int ctype, int x, int y, const char *format, ...);
      virtual void drawTextMap(int x, int y, const char *format, ...);
      virtual void drawTextMouse(int x, int y, const char *format, ...);
      virtual void drawTextScreen(int x, int y, const char *format, ...);

      virtual void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
      virtual void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      virtual void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      virtual void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);

      virtual void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      virtual void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      virtual void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      virtual void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);

      virtual void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false);
      virtual void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
      virtual void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
      virtual void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);

      virtual void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      virtual void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      virtual void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      virtual void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);

      virtual void drawDot(int ctype, int x, int y, Color color);
      virtual void drawDotMap(int x, int y, Color color);
      virtual void drawDotMouse(int x, int y, Color color);
      virtual void drawDotScreen(int x, int y, Color color);

      virtual void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color);
      virtual void drawLineMap(int x1, int y1, int x2, int y2, Color color);
      virtual void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
      virtual void drawLineScreen(int x1, int y1, int x2, int y2, Color color);

      virtual void *getScreenBuffer();

      virtual int getLatencyFrames();
      virtual int getLatencyTime();
      virtual int getRemainingLatencyFrames();
      virtual int getRemainingLatencyTime();

      virtual int  getRevision();
      virtual bool isDebug();
      virtual bool isLatComEnabled();
      virtual void setLatCom(bool isEnabled);
      virtual int  getReplayFrameCount();
      virtual void setGUI(bool enabled = true);

      virtual int getInstanceNumber();
      virtual int getAPM(bool includeSelects = false);

      virtual bool setMap(const char *mapFileName);
      virtual void setFrameSkip(int frameSkip = 1);

      //Internal BWAPI commands:
      GameImpl();
      ~GameImpl();

      void update(); /**< Updates unitArrayCopy according to bw memory */
      void loadAutoMenuData();
      void onMenuFrame();
      PlayerImpl* players[12];
      void pressKey(int key);
      void mouseDown(int x, int y);
      void mouseUp(int x, int y);

      /**
       * Changes slot state in the pre-game lobby.
       * @param slot Desired state of the slot (Open/Closed/Computer)
       * @param slotID Order of the slot (0 based)
       */
      void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID);
      void addToCommandBuffer(Command* command);
      void onGameStart();
      void onGameEnd();
      int  stormIdToPlayerId(int dwStormId);
      void onSendText(const char* text);
      void onReceiveText(int playerId, std::string text);
      bool parseText(const char* text);
      bool inScreen(int ctype, int x, int y);
      bool inScreen(int ctype, int x1, int y1, int x2, int y2);
      bool inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3);
      void lockFlags();
      bool enabled;
      bool _isInGame() const;
      bool _isSinglePlayer() const;
      bool _isBattleNet();
      bool _isReplay() const;
      void _changeRace(int slot, BWAPI::Race race);

      void loadSelected();
      void copyMapToSharedMemory();
      void moveToSelected();
      void executeCommand(UnitCommand command, bool addCommandToLatComBuffer = true);

      UnitImpl   *getUnitFromIndex(int index);
      BulletImpl *getBulletFromIndex(int index);
      PlayerImpl *BWAPIPlayer;
      PlayerImpl *enemyPlayer;

      std::vector<Shape*> shapes;
      Util::Logger* newUnitLog;
      Util::Logger* commandLog;
      std::set<int> invalidIndices;
      std::list<std::string > sentMessages;
      void onSaveGame(char *name);
      std::list<Event> events;
      int bulletCount;
      BW::dialog *myDlg;
      Server server;
      std::vector<BWAPI::UnitImpl*> lastEvadedUnits;
      bool onStartCalled;
      std::string autoMenuMode;
      std::string autoMenuMapPath;
      bool wasJustInGame;
      bool wantNewMapGen;
      int  autoMapTryCount;
      std::vector<std::string> autoMapPool;
      /** Count of game-frames passed from game start. */
      int frameCount;
      void refreshSelectionStates();
      BW::Unit *savedUnitSelection[12];
      bool wantSelectionUpdate;

      bool noGUI;
      bool startedClient;

      UnitImpl *unitArray[UNIT_ARRAY_MAX_LENGTH];
    private :
      HMODULE hMod;
      Map map;

      std::set<BWAPI::UnitImpl*> aliveUnits; //units alive on current frame
      std::set<BWAPI::UnitImpl*> dyingUnits; //units leaving aliveUnits set on current frame

      std::vector<BWAPI::UnitImpl*> discoverUnits; //units entering accessibleUnits set on current frame
      std::set<BWAPI::Unit*> accessibleUnits; //units that are accessible to the client on current frame
      std::vector<BWAPI::UnitImpl*> evadeUnits; //units leaving accessibleUnits set on current frame

      std::set<BWAPI::Unit*>  selectedUnitSet;
      std::set<BWAPI::Unit*>  emptySet;
      std::set<TilePosition>  startLocations;

      std::set<BWAPI::Force*>  forces;
      std::set<BWAPI::Player*> playerSet;

      std::set<BWAPI::Unit*>   minerals;
      std::set<BWAPI::Unit*>   geysers;
      std::set<BWAPI::Unit*>   neutralUnits;
      std::set<BWAPI::Bullet*> bullets;
      std::set<Position>       nukeDots;
      std::set<BWAPI::UnitImpl*> pylons;
      Util::RectangleArray<std::set<Unit*> > unitsOnTileData;

      std::set<BWAPI::Unit*> staticMinerals;
      std::set<BWAPI::Unit*> staticGeysers;
      std::set<BWAPI::Unit*> staticNeutralUnits;

      BulletImpl* bulletArray[BULLET_ARRAY_MAX_LENGTH];
      std::vector<std::vector<Command *> > commandBuffer;
      /** Will update the unitsOnTile content, should be called every frame. */
      void updateUnits();
      void updateBullets();
      void computeUnitExistence();
      void computePrimaryUnitSets();
      void extractUnitData();
      void augmentUnitData();
      void applyLatencyCompensation();
      void computeSecondaryUnitSets();
      void processEvents();
      /**
       * Specifies if some order was given, so the loadSelect function will have
       * to be called.
       */
      std::string savedMapHash;
      bool flags[BWAPI::Flag::Max];
      AIModule* client;
      BWAPI::Error lastError;
      std::list<UnitImpl*> deadUnits;
      u32 cheatFlags;
      std::string autoMenuLanMode;
      std::string autoMenuRace;
      std::string autoMenuEnemyRace[8];
      unsigned int autoMenuEnemyCount;
      std::string autoMenuGameType;
      std::string autoMenuRestartGame;
      std::string autoMenuSaveReplay;
      DWORD endTick;
      std::set<Player*> _allies;
      std::set<Player*> _enemies;

      bool inGame;

      bool actMainMenu;
      bool actRegistry;
      bool actCreate;
      bool actConnSel;
      bool actGameSel;
      bool actRaceSel;
      bool actEnd;
      bool actBriefing;

      DWORD  startTickCount;
      DWORD  lastTickCount;
      int    accumulatedFrames;
      int    fps;
      double averageFPS;
      int botAPM_noselects;
      int botAPM_selects;
      double botAPMCounter_noselects;
      double botAPMCounter_selects;

      int  textSize;
      bool hasLatCom;

      bool pathDebug;
      bool unitDebug;
      bool grid;

      bool externalModuleConnected;
      bool calledMatchEnd;
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
