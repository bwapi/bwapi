#pragma once
#include <string>
#include <list>

#include <BW/Offsets.h>

#include <BWAPI/Game.h>
#include <BWAPI/Server.h>
#include <BWAPI/Map.h>
#include <BWAPI/Client/GameData.h>
#include <BWAPI/TournamentAction.h>
#include <BWAPI/CoordinateType.h>

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
  class AIModule;
  class Command;
  class BulletImpl;
  class Bulletset;
  
  class PlayerInterface;
  typedef PlayerInterface *Player;

  class PlayerImpl;
  class Playerset;
  class UnitImpl;
  class Unitset;

  /** The main class wrapping the whole game data/methods. */
  class GameImpl : public Game
  {
    public :
      virtual const Forceset&   getForces() const override;
      virtual const Playerset&  getPlayers() const override;
      virtual const Unitset&    getAllUnits() const override;
      virtual const Unitset&    getMinerals() const override;
      virtual const Unitset&    getGeysers() const override;
      virtual const Unitset&    getNeutralUnits() const override;
 
      virtual const Unitset&  getStaticMinerals() const override;
      virtual const Unitset&  getStaticGeysers() const override;
      virtual const Unitset&  getStaticNeutralUnits() const override;

      virtual const Bulletset&     getBullets() const override;
      virtual const Position::set& getNukeDots() const override;
      virtual const std::list< Event >&   getEvents() const override;

      virtual Force     getForce(int forceID) const override;
      virtual Player    getPlayer(int playerID) const override;
      virtual Unit getUnit(int unitID) const override;
      virtual Unit indexToUnit(int unitIndex) const override;
      virtual Region    getRegion(int regionID) const override;

      virtual GameType  getGameType() const override;

      virtual int       getLatency() const override;
      virtual int       getFrameCount() const override;
      virtual int       getReplayFrameCount() const override;
      virtual int       getFPS() const override;
      virtual double    getAverageFPS() const override;

      virtual Position  getMousePosition() const override;
      virtual bool      getMouseState(MouseButton button) const override;
      virtual bool      getKeyState(Key key) const override;

      virtual Position  getScreenPosition() const override;
      virtual void      setScreenPosition(int x, int y) override;
      virtual void      pingMinimap(int x, int y) override;

      virtual bool    isFlagEnabled(int flag) const override;
      virtual void    enableFlag(int flag) override;

      virtual Unitset getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred = nullptr) const override;
      virtual Unit getClosestUnitInRectangle(Position center, const UnitFilter &pred = nullptr, int left = 0, int top = 0, int right = 999999, int bottom = 999999) const override;
      virtual Unit getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center = Positions::None, int radius = 999999) const override;

      virtual Error   getLastError() const override;
      virtual bool    setLastError(BWAPI::Error e = Errors::None) const override;

      virtual int         mapWidth() const override;
      virtual int         mapHeight() const override;
      virtual std::string mapFileName() const override;
      virtual std::string mapPathName() const override;
      virtual std::string mapName() const override;
      virtual std::string mapHash() const override;

      virtual bool  isWalkable(int x, int y) const override;
      virtual int   getGroundHeight(int x, int y) const override;
      virtual bool  isBuildable(int x, int y, bool includeBuildings = false) const override;
      virtual bool  isVisible(int x, int y) const override;
      virtual bool  isExplored(int x, int y) const override;
      virtual bool  hasCreep(int x, int y) const override;
      virtual bool  hasPowerPrecise(int x, int y, UnitType unitType = UnitTypes::None ) const override;

      virtual bool  canBuildHere(TilePosition position, UnitType type, Unit builder = nullptr, bool checkExplored = false) override;
      virtual bool  canMake(UnitType type, Unit builder = nullptr) const override;
      virtual bool  canResearch(TechType type, Unit unit = nullptr, bool checkCanIssueCommandType = true) override;
      virtual bool  canUpgrade(UpgradeType type, Unit unit = nullptr, bool checkCanIssueCommandType = true) override;

      virtual const TilePosition::set& getStartLocations() const override;

      virtual void vPrintf(const char *format, va_list arg) override;
      virtual void vSendTextEx(bool toAllies, const char *format, va_list arg) override;

      virtual bool isInGame() const override;
      virtual bool isMultiplayer() const override;
      virtual bool isBattleNet() const override;
      virtual bool isPaused() const override;
      virtual bool isReplay() const override;

      virtual void pauseGame() override;
      virtual void resumeGame() override;
      virtual void leaveGame() override;
      virtual void restartGame() override;
      virtual void setLocalSpeed(int speed) override;
      virtual bool issueCommand(const Unitset& units, UnitCommand command) override;
      virtual const Unitset& getSelectedUnits() const override;
      virtual Player self() const override;
      virtual Player enemy() const override;
      virtual Player neutral() const override;
      virtual Playerset& allies() override;
      virtual Playerset& enemies() override;
      virtual Playerset& observers() override;

      virtual void setTextSize(int size = 1) override;
      virtual void vDrawText(CoordinateType::Enum ctype, int x, int y, const char *format, va_list arg) override;

      virtual void drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false) override;
      virtual void drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) override;
      virtual void drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid = false) override;
      virtual void drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false) override;
      virtual void drawDot(CoordinateType::Enum ctype, int x, int y, Color color) override;
      virtual void drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color) override;

      virtual int getLatencyFrames() const override;
      virtual int getLatencyTime() const override;
      virtual int getRemainingLatencyFrames() const override;
      virtual int getRemainingLatencyTime() const override;

      virtual int  getRevision() const override;
      virtual bool isDebug() const override;
      virtual bool isLatComEnabled() const override;
      virtual void setLatCom(bool isEnabled) override;
      virtual bool isGUIEnabled() const override;
      virtual void setGUI(bool enabled) override;

      virtual int getInstanceNumber() const override;
      virtual int getAPM(bool includeSelects = false) const override;

      virtual bool setMap(const char *mapFileName) override;
      virtual void setFrameSkip(int frameSkip) override;

      virtual bool hasPath(Position source, Position destination) const override;

      virtual bool setAlliance(Player player, bool allied = true, bool alliedVictory = true) override;
      virtual bool setVision(Player player, bool enabled = true) override;
      virtual int  elapsedTime() const override;

      virtual void setCommandOptimizationLevel(int level = 0) override;
      virtual int  countdownTimer() const override;

      virtual const Regionset &getAllRegions() const override;
      virtual BWAPI::Region   getRegionAt(int x, int y) const override;

      virtual int getLastEventTime() const override;

      virtual bool setRevealAll(bool reveal = true) override;

      //Internal BWAPI commands:
      GameImpl();
      ~GameImpl();

      void initializeData();
      void update(); // Updates unitArrayCopy according to bw memory
      void updateStatistics();
      void updateOverlays();
      void updateCommandOptimizer();
      void initializeTournamentModule();
      void initializeAIModule();

      void loadAutoMenuData();
      void onMenuFrame();
      PlayerImpl *_getPlayer(int id);
      int _currentPlayerId();
      void pressKey(int key);
      void mouseDown(int x, int y);
      void mouseUp(int x, int y);

      void addToCommandBuffer(Command* command);
      void onGameStart();
      void onGameEnd();
      int  stormIdToPlayerId(int dwStormId);
      void onSendText(const char* text);
      void onReceiveText(int playerId, std::string text);
      bool parseText(const char* text);
      bool inScreen(CoordinateType::Enum ctype, int x, int y);
      bool inScreen(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2);
      bool inScreen(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int x3, int y3);
      void lockFlags();
      void _startGame();
      void _changeRace(int slot, BWAPI::Race race);
      Race lastKnownRaceBeforeStart[8];

      void loadSelected();
      void copyMapToSharedMemory();
      void moveToSelectedUnits();
      void executeCommand(UnitCommand command);
      bool addToCommandOptimizer(UnitCommand command);

      void chooseNewRandomMap();
      static void SendClientEvent(BWAPI::AIModule *module, Event &e);

      void processInterfaceEvents();

      UnitImpl   *getUnitFromIndex(int index);
      BulletImpl *getBulletFromIndex(int index);
      PlayerImpl *BWAPIPlayer;
      PlayerImpl *enemyPlayer;

      Vectorset<int> invalidIndices;
      std::list<std::string > sentMessages;
      void onSaveGame(char *name);
      std::list<Event> events;
      int bulletCount;
      Server server;
      Unitset lastEvadedUnits;
      bool onStartCalled;
      std::string lastMapGen;
      std::string autoMenuMode;
      std::string autoMenuMapPath;
      std::string autoMenuGameName;
      bool isHost;
      int  autoMapTryCount;
      std::vector<std::string> autoMapPool;
      std::string autoMapIteration;
      std::string lastAutoMapString;
      unsigned int lastAutoMapEntry;

      /** Count of game-frames passed from game start. */
      int frameCount;
      void refreshSelectionStates();
      BW::CUnit *savedUnitSelection[12];
      bool wantSelectionUpdate;

      bool startedClient;

      UnitImpl *unitArray[UNIT_ARRAY_MAX_LENGTH];
      bool isTournamentCall;
      const GameData* getGameData() const;
      GameData* data;
      
      int drawShapes();
      void processEvents();
      Unit _unitFromIndex(int index);

      int  commandOptimizerLevel;

      HMODULE hAIModule;
      AIModule *client;

      HMODULE hTournamentModule;
      AIModule *tournamentAI;

      PlayerImpl *players[PLAYER_COUNT];

      void dropPlayers();
      std::vector<PlayerImpl*> droppedPlayers;
    private :
      Map map;

      Unitset aliveUnits; //units alive on current frame
      Unitset dyingUnits; //units leaving aliveUnits set on current frame

      Unitset discoverUnits; //units entering accessibleUnits set on current frame
      Unitset accessibleUnits; //units that are accessible to the client on current frame
      Unitset evadeUnits; //units leaving accessibleUnits set on current frame

      Unitset selectedUnitSet;

      TilePosition::set startLocations;

      Forceset  forces;
      Playerset playerSet;

      Unitset       minerals;
      Unitset       geysers;
      Unitset       neutralUnits;
      Bulletset     bullets;
      Position::set nukeDots;
      Unitset pylons;

      Unitset staticMinerals;
      Unitset staticGeysers;
      Unitset staticNeutralUnits;

      Regionset regionsList;

      BulletImpl* bulletArray[BULLET_ARRAY_MAX_LENGTH];
      std::vector< std::vector<Command *> > commandBuffer;
      /** Will update the unitsOnTile content, should be called every frame. */
      void updateUnits();
      void updateBullets();
      void computeUnitExistence();
      void computePrimaryUnitSets();
      void extractUnitData();
      void augmentUnitData();
      void applyLatencyCompensation();
      void computeSecondaryUnitSets();
      /**
       * Specifies if some order was given, so the loadSelect function will have
       * to be called.
       */
      std::string savedMapHash;
      bool flags[BWAPI::Flag::Max];
      TournamentModule  *tournamentController;
      bool              bTournamentMessageAppeared;
      mutable BWAPI::Error lastError;
      Unitset deadUnits;
      u32 cheatFlags;
      std::string autoMenuLanMode;
      std::string autoMenuRace;
      std::string autoMenuEnemyRace[8];
      unsigned int autoMenuEnemyCount;
      unsigned int autoMenuMinPlayerCount;
      unsigned int autoMenuMaxPlayerCount;
      unsigned int autoMenuWaitPlayerTime;
      std::string autoMenuGameType;
      std::string autoMenuRestartGame;
      std::string autoMenuSaveReplay;
#ifdef _DEBUG
      std::string autoMenuPause;
#endif
      std::string rn_BWAPIName;
      std::string rn_BWAPIRace;
      std::string rn_MapName;
      std::string rn_AlliesNames;
      std::string rn_AlliesRaces;
      std::string rn_EnemiesNames;
      std::string rn_EnemiesRaces;
      DWORD endTick;
      Playerset _allies;
      Playerset _enemies;
      Playerset _observers;

      bool inGame;
      bool actStartedGame;
      bool actRaceSel;

      DWORD   startTickCount;
      DWORD   lastTickCount;
      int     accumulatedFrames;
      int     fps;
      double  averageFPS;
      int     botAPM_noselects;
      int     botAPM_selects;
      double  botAPMCounter_noselects;
      double  botAPMCounter_selects;

      int  textSize;

      bool pathDebug;
      bool unitDebug;
      bool grid;
      bool showfps;

      bool externalModuleConnected;
      bool calledMatchEnd;
      bool tournamentCheck(Tournament::ActionID type, void *parameter = nullptr);
      std::list<UnitCommand> commandOptimizer[UnitCommandTypes::Enum::MAX];

      int lastEventTime;
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
