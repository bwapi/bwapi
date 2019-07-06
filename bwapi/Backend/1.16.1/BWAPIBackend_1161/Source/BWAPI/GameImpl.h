#pragma once
#include <string>
#include <list>
#include <deque>

#include "../Types.h"
#include <BW/Constants.h>

#include <BWAPI/Server.h>
#include <BWAPI/Map.h>
#include <BWAPI/Client/GameData.h>
#include <BWAPI/CoordinateType.h>

#include "Command.h"
#include "APMCounter.h"
#include "FPSCounter.h"
#include "AutoMenuManager.h"

namespace BW
{
  class CUnit;
  class CBullet;
  class Dialog;
}
/**
 * Everything in the BWAPI library that doesn't map or work directly with the bw
 * data.
 */
namespace BWAPI
{
  // forwards
  class BulletImpl;
  class Bulletset;

  class PlayerInterface;
  typedef PlayerInterface *Player;

  class PlayerImpl;
  class Playerset;
  class UnitImpl;
  class Unitset;

  /** The main class wrapping the whole game data/methods. */
  class GameImpl
  {
    public :
      const std::set<ForceImpl*>&   getForces() const;
      const Playerset&  getPlayers() const;
      const Unitset&    getAllUnits() const;
      const Unitset&    getMinerals() const;
      const Unitset&    getGeysers() const;
      const Unitset&    getNeutralUnits() const;

      const Unitset&  getStaticMinerals() const;
      const Unitset&  getStaticGeysers() const;
      const Unitset&  getStaticNeutralUnits() const;

      const Bulletset&     getBullets() const;
      const Position::list& getNukeDots() const;
      const std::list< Event >&   getEvents() const;

      Force     getForce(int forceID) const;
      Player    getPlayer(int playerID) const;
      Unit getUnit(int unitID) const;
      Unit indexToUnit(int unitIndex) const;
      Region    getRegion(int regionID) const;

      GameType  getGameType() const;

      int       getLatency() const;
      int       getFrameCount() const;
      int       getReplayFrameCount() const;
      int       getFPS() const;
      double    getAverageFPS() const;

      Position  getMousePosition() const;
      bool      getMouseState(MouseButton button) const;
      bool      getKeyState(Key key) const;

      Position  getScreenPosition() const;
      void      setScreenPosition(int x, int y);
      void      pingMinimap(int x, int y);

      bool    isFlagEnabled(int flag) const;
      void    enableFlag(int flag);

      Unitset getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred = nullptr) const;
      Unit getClosestUnitInRectangle(Position center, const UnitFilter &pred = nullptr, int left = 0, int top = 0, int right = 999999, int bottom = 999999) const;
      Unit getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center = Positions::None, int radius = 999999) const;

      Error   getLastError() const;
      bool    setLastError(BWAPI::Error e = Errors::None) const;

      int         mapWidth() const;
      int         mapHeight() const;
      std::string mapFileName() const;
      std::string mapPathName() const;
      std::string mapName() const;
      std::string mapHash() const;

      bool  isWalkable(int x, int y) const;
      int   getGroundHeight(int x, int y) const;
      bool  isBuildable(int x, int y, bool includeBuildings = false) const;
      bool  isVisible(int x, int y) const;
      bool  isExplored(int x, int y) const;
      bool  hasCreep(int x, int y) const;
      bool  hasPowerPrecise(int x, int y, UnitType unitType = UnitTypes::None ) const;

      bool  canBuildHere(TilePosition position, UnitType type, Unit builder = nullptr, bool checkExplored = false);
      bool  canMake(UnitType type, Unit builder = nullptr) const;
      bool  canResearch(TechType type, Unit unit = nullptr, bool checkCanIssueCommandType = true);
      bool  canUpgrade(UpgradeType type, Unit unit = nullptr, bool checkCanIssueCommandType = true);

      const TilePosition::list& getStartLocations() const;

      void vPrintf(const char *format, va_list arg);
      void vSendTextEx(bool toAllies, const char *format, va_list arg);

      bool isInGame() const;
      bool isMultiplayer() const;
      bool isBattleNet() const;
      bool isPaused() const;
      bool isReplay() const;

      void pauseGame();
      void resumeGame();
      void leaveGame();
      void restartGame();
      void setLocalSpeed(int speed);
      bool issueCommand(const Unitset& units, UnitCommand command);
      const Unitset& getSelectedUnits() const;
      Player self() const;
      Player enemy() const;
      Player neutral() const;
      Playerset& allies();
      Playerset& enemies();
      Playerset& observers();

      void setTextSize(Text::Size::Enum size = Text::Size::Default);
      void vDrawText(CoordinateType::Enum ctype, int x, int y, const char *format, va_list arg);

      void drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
      void drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      void drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid = false);
      void drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      void drawDot(CoordinateType::Enum ctype, int x, int y, Color color);
      void drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color);

      int getLatencyFrames() const;
      int getLatencyTime() const;
      int getRemainingLatencyFrames() const;
      int getRemainingLatencyTime() const;

      int  getRevision() const;
      int  getClientVersion() const;
      bool isDebug() const;
      bool isLatComEnabled() const;
      void setLatCom(bool isEnabled);
      bool isGUIEnabled() const;
      void setGUI(bool enabled);

      int getInstanceNumber() const;
      int getAPM(bool includeSelects = false) const;

      bool setMap(const std::string &mapFileName);
      void setFrameSkip(int frameSkip);

      bool setAlliance(Player player, bool allied = true, bool alliedVictory = true);
      bool setVision(Player player, bool enabled = true);
      int  elapsedTime() const;

      int  countdownTimer() const;

      const Regionset &getAllRegions() const;
      BWAPI::Region   getRegionAt(int x, int y) const;

      int getLastEventTime() const;

      bool setRevealAll(bool reveal = true);

      unsigned getRandomSeed() const;

      void setScreenPosition(BWAPI::Position p);
      void pingMinimap(BWAPI::Position p);
      Unitset getUnitsOnTile(int tileX, int tileY, const UnitFilter &pred = nullptr) const;
      Unitset getUnitsOnTile(BWAPI::TilePosition tile, const UnitFilter &pred = nullptr) const;
      Unitset getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight, const UnitFilter &pred = nullptr) const;
      Unitset getUnitsInRadius(int x, int y, int radius, const UnitFilter &pred = nullptr) const;
      Unitset getUnitsInRadius(BWAPI::Position center, int radius, const UnitFilter &pred = nullptr) const;
      Unit getClosestUnit(Position center, const UnitFilter &pred = nullptr, int radius = 999999) const;
      bool isWalkable(BWAPI::WalkPosition position) const;
      int  getGroundHeight(TilePosition position) const;
      bool isBuildable(TilePosition position, bool includeBuildings = false) const;
      bool isVisible(TilePosition position) const;
      bool isExplored(TilePosition position) const;
      bool hasCreep(TilePosition position) const;
      bool hasPowerPrecise(Position position, UnitType unitType = UnitTypes::None) const;
      bool hasPower(int tileX, int tileY, UnitType unitType = UnitTypes::None) const;
      bool hasPower(TilePosition position, UnitType unitType = UnitTypes::None) const;
      bool hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;
      bool hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;
      void printf(const char *format, ...);
      void sendText(const char *format, ...);
      void vSendText(const char *format, va_list args);
      void sendTextEx(bool toAllies, const char *format, ...);
      void drawText(CoordinateType::Enum ctype, int x, int y, const char *format, ...);
      void drawTextMap(int x, int y, const char *format, ...);
      void drawTextMap(Position p, const char *format, ...);
      void drawTextMouse(int x, int y, const char *format, ...);
      void drawTextMouse(Position p, const char *format, ...);
      void drawTextScreen(int x, int y, const char *format, ...);
      void drawTextScreen(Position p, const char *format, ...);
      void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      void drawBoxMap(Position leftTop, Position rightBottom, Color color, bool isSolid = false);
      void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      void drawBoxMouse(Position leftTop, Position rightBottom, Color color, bool isSolid = false);
      void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      void drawBoxScreen(Position leftTop, Position rightBottom, Color color, bool isSolid = false);
      void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      void drawTriangleMap(Position a, Position b, Position c, Color color, bool isSolid = false);
      void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      void drawTriangleMouse(Position a, Position b, Position c, Color color, bool isSolid = false);
      void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      void drawTriangleScreen(Position a, Position b, Position c, Color color, bool isSolid = false);
      void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
      void drawCircleMap(Position p, int radius, Color color, bool isSolid = false);
      void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
      void drawCircleMouse(Position p, int radius, Color color, bool isSolid = false);
      void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);
      void drawCircleScreen(Position p, int radius, Color color, bool isSolid = false);
      void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      void drawEllipseMap(Position p, int xrad, int yrad, Color color, bool isSolid = false);
      void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      void drawEllipseMouse(Position p, int xrad, int yrad, Color color, bool isSolid = false);
      void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      void drawEllipseScreen(Position p, int xrad, int yrad, Color color, bool isSolid = false);
      void drawDotMap(int x, int y, Color color);
      void drawDotMap(Position p, Color color);
      void drawDotMouse(int x, int y, Color color);
      void drawDotMouse(Position p, Color color);
      void drawDotScreen(int x, int y, Color color);
      void drawDotScreen(Position p, Color color);
      void drawLineMap(int x1, int y1, int x2, int y2, Color color);
      void drawLineMap(Position a, Position b, Color color);
      void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
      void drawLineMouse(Position a, Position b, Color color);
      void drawLineScreen(int x1, int y1, int x2, int y2, Color color);
      void drawLineScreen(Position a, Position b, Color color);
      bool hasPath(Position source, Position destination) const;
      BWAPI::Region getRegionAt(BWAPI::Position position) const;
      int getDamageFrom(UnitType fromType, UnitType toType, Player fromPlayer = nullptr, Player toPlayer = nullptr) const;
      int getDamageTo(UnitType toType, UnitType fromType, Player toPlayer = nullptr, Player fromPlayer = nullptr) const;
      void updateKillAndRemoveUnits();
      Unitset unitsToKill;
      Unitset unitsToRemove;

      void killUnit(Unit unit);
      void removeUnit(Unit unit);
      void createUnit(
        UnitType unitType,
        Player player,
        int x,
        int y,
        int hpPercent = 100,
        int shieldsPercent = 100,
        int energyPercent = 100,
        int resources = 5000,
        int hangarCount = 0,
        bool cloaked = false,
        bool burrowed = false,
        bool lifted = false,
        bool hallucinated = false,
        bool invincible = false);

      //Internal BWAPI commands:
      GameImpl();
      ~GameImpl();

      void initializeData();
      void update(); // Updates unitArrayCopy according to bw memory
      void updateStatistics();
      void updateOverlays();
      void initializeAIModule();

      void loadAutoMenuData();

      void onMenuFrame();
      static Race getMenuRace(const std::string &sChosenRace);
      PlayerImpl *_getPlayer(int id);
      static int _currentPlayerId();
      static void pressKey(int key);

      static void mouseDown(int x, int y);
      static void mouseUp(int x, int y);

      void addToCommandBuffer(Command command);
      void onGameStart();
      void onGameEnd();
      static int stormIdToPlayerId(int dwStormId);
      void onSendText(const std::string &text);
      void onReceiveText(int playerId, const std::string &text);
      bool parseText(const std::string &text);
      bool inScreen(CoordinateType::Enum ctype, int x, int y) const;
      bool inScreen(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2) const;
      bool inScreen(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int x3, int y3) const;
      static void _startGame();
      static void _changeRace(int slot, BWAPI::Race race);

      void moveToSelectedUnits();
      void executeCommand(UnitCommand command);

      void queueSentMessage(std::string const &message);

      UnitImpl   *getUnitFromIndex(int index);
      BulletImpl *getBulletFromIndex(int index);

      void onSaveGame(const char *name);

      void refreshSelectionStates();
      const GameData* getGameData() const;

      void dropPlayers();

      int drawShapes();
      Unit _unitFromIndex(int index);

    public:
      std::array<Race, BW::PLAYABLE_PLAYER_COUNT> lastKnownRaceBeforeStart;
      PlayerImpl *BWAPIPlayer;
      PlayerImpl *enemyPlayer;
      Server server;
      std::list<Event> events;
    private:
      std::vector<std::string> sentMessages;

      // Count of game-frames passed from game start.
      int frameCount = 0;
      std::array<BW::CUnit*, BW::PLAYER_COUNT> savedUnitSelection;

      static void setLocalSpeedDirect(int speed);
    public:
      bool onStartCalled = false;
      AutoMenuManager autoMenuManager;

      int seedOverride = std::numeric_limits<int>::max();
      int speedOverride = std::numeric_limits<int>::min();
      bool wantDropPlayers = true;

      bool wantSelectionUpdate;
      bool startedClient;

      std::array<UnitImpl*, BW::UNIT_ARRAY_MAX_LENGTH> unitArray;

      GameData* data = server.data;

      // NOTE: This MUST be a POD array (NOT std::array) because of the crappy assembly hacks that are being used
      // Until we can get rid of the assembly hacks, this must be treated like a pissed off cat
      PlayerImpl* players[BW::PLAYER_COUNT];

      Unitset evadeUnits; //units leaving accessibleUnits set on current frame
    private:
      std::vector<PlayerImpl*> droppedPlayers;

      Unitset aliveUnits; //units alive on current frame
      Unitset dyingUnits; //units leaving aliveUnits set on current frame

      Unitset discoverUnits; //units entering accessibleUnits set on current frame
      Unitset accessibleUnits; //units that are accessible to the client on current frame
      Unitset selectedUnitSet;

      TilePosition::list startLocations;

      std::set<ForceImpl*> forces;
      Playerset playerSet;

      Unitset       minerals;
      Unitset       geysers;
      Unitset       neutralUnits;
      Bulletset     bullets;
      Position::list nukeDots;
      Unitset pylons;

      Unitset staticMinerals;
      Unitset staticGeysers;
      Unitset staticNeutralUnits;

      Regionset regionsList;
      std::unordered_map<int, Region> regionMap;

      std::array<BulletImpl*, BW::BULLET_ARRAY_MAX_LENGTH> bulletArray;
      std::vector<std::vector<Command>> commandBuffer;

      void updateUnits();
      void updateBullets();
      void computeUnitExistence();
      void computePrimaryUnitSets();
      void extractUnitData();
      void augmentUnitData();
      void applyLatencyCompensation();
      void computeSecondaryUnitSets();

      std::array<bool,BWAPI::Flag::Max> flags;
      mutable BWAPI::Error lastError;
      Unitset deadUnits;    // Keeps track of units that were removed from the game, used only to deallocate them
      u32 cheatFlags;

      std::string rn_BWAPIName;
      std::string rn_BWAPIRace;
      std::string rn_MapName;
      std::string rn_AlliesNames;
      std::string rn_AlliesRaces;
      std::string rn_EnemiesNames;
      std::string rn_EnemiesRaces;
      std::string rn_GameResult;
      Playerset _allies;
      Playerset _enemies;
      Playerset _observers;

      bool inGame = false;

      FPSCounter fpsCounter;

      Text::Size::Enum textSize;

      bool grid = false;
      bool showfps = false;

      bool externalModuleConnected = false;
      bool calledMatchEnd = false;

      int lastEventTime = 0;

    public:
      APMCounter apmCounter;

    private:
      int addShape(const BWAPIC::Shape &s);
      int addString(const char* text);
      int addText(BWAPIC::Shape &s, const char* text);
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
