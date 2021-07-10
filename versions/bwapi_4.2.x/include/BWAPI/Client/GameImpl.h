#pragma once
#include <BWAPI.h>

#include "GameData.h"
#include "Client.h"
#include "Shape.h"
#include "Command.h"
#include "UnitCommand.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "RegionImpl.h"
#include "UnitImpl.h"
#include "BulletImpl.h"

#include <list>
#include <vector>
#include <array>

namespace BWAPI
{
  class ForceInterface;
  typedef ForceInterface *Force;
  class PlayerInterface;
  typedef PlayerInterface *Player;

  class GameImpl : public Game
  {
    private :
      int addShape(const BWAPIC::Shape &s);
      int addString(const char* text);
      int addText(BWAPIC::Shape &s, const char* text);
      int addCommand(const BWAPIC::Command &c);
      void processInterfaceEvents();
      void clearAll();

      GameData* data;
      std::vector<ForceImpl>  forceVector;
      std::vector<PlayerImpl> playerVector;
      std::vector<UnitImpl>   unitVector;
      std::vector<BulletImpl> bulletVector;
      std::array<RegionImpl*,5000> regionArray;

      Forceset forces;
      Playerset playerSet;
      Unitset accessibleUnits;//all units that are accessible (and definitely alive)
      //notDestroyedUnits - accessibleUnits = all units that may or may not be alive (status unknown)
      Unitset minerals;
      Unitset geysers;
      Unitset neutralUnits;
      Unitset staticMinerals;
      Unitset staticGeysers;
      Unitset staticNeutralUnits;
      Bulletset bullets;
      Position::list nukeDots;
      Unitset selectedUnits;
      Unitset pylons;
      Regionset regionsList;

      TilePosition::list startLocations;
      std::list< Event > events;
      Player thePlayer;
      Player theEnemy;
      Player theNeutral;
      Playerset _allies;
      Playerset _enemies;
      Playerset _observers;
      mutable Error lastError;
      Text::Size::Enum textSize = Text::Size::Default;

    public :
      Event makeEvent(BWAPIC::Event e);
      int addUnitCommand(BWAPIC::UnitCommand& c);
      bool inGame;
      GameImpl(GameData* data);
      void onMatchStart();
      void onMatchEnd();
      void onMatchFrame();
      const GameData* getGameData() const;
      Unit _unitFromIndex(int index);

      virtual const Forceset& getForces() const override;
      virtual const Playerset& getPlayers() const override;
      virtual const Unitset& getAllUnits() const override;
      virtual const Unitset& getMinerals() const override;
      virtual const Unitset& getGeysers() const override;
      virtual const Unitset& getNeutralUnits() const override;

      virtual const Unitset& getStaticMinerals() const override;
      virtual const Unitset& getStaticGeysers() const override;
      virtual const Unitset& getStaticNeutralUnits() const override;

      virtual const Bulletset& getBullets() const override;
      virtual const Position::list& getNukeDots() const override;
      virtual const std::list< Event>& getEvents() const override;

      virtual Force   getForce(int forceID) const override;
      virtual Player  getPlayer(int playerID) const override;
      virtual Unit    getUnit(int unitID) const override;
      virtual Unit    indexToUnit(int unitIndex) const override;
      virtual Region  getRegion(int regionID) const override;

      virtual GameType getGameType() const override;
      virtual int getFrameCount() const override;
      virtual int getReplayFrameCount() const override;
      virtual int getFPS() const override;
      virtual double getAverageFPS() const override;
      virtual BWAPI::Position getMousePosition() const override;
      virtual bool getMouseState(MouseButton button) const override;
      virtual bool getKeyState(Key key) const override;
      virtual BWAPI::Position getScreenPosition() const override;
      virtual void setScreenPosition(int x, int y) override;
      virtual void pingMinimap(int x, int y) override;

      virtual bool      isFlagEnabled(int flag) const override;
      virtual void      enableFlag(int flag) override;
      virtual Unitset   getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred = nullptr) const override;
      virtual Unit getClosestUnitInRectangle(Position center, const UnitFilter &pred = nullptr, int left = 0, int top = 0, int right = 999999, int bottom = 999999) const override;
      virtual Unit getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center = Positions::None, int radius = 999999) const override;
      virtual Error     getLastError() const override;
      virtual bool      setLastError(BWAPI::Error e = Errors::None) const override;

      virtual int         mapWidth() const override;
      virtual int         mapHeight() const override;
      virtual std::string mapFileName() const override;
      virtual std::string mapPathName() const override;
      virtual std::string mapName() const override;
      virtual std::string mapHash() const override;

      virtual bool isWalkable(int x, int y) const override;
      virtual int  getGroundHeight(int x, int y) const override;
      virtual bool isBuildable(int x, int y, bool includeBuildings = false) const override;
      virtual bool isVisible(int x, int y) const override;
      virtual bool isExplored(int x, int y) const override;
      virtual bool hasCreep(int x, int y) const override;
      virtual bool hasPowerPrecise(int x, int y, UnitType unitType = UnitTypes::None ) const override;

      virtual bool canBuildHere(TilePosition position, UnitType type, Unit builder = nullptr, bool checkExplored = false) override;
      virtual bool canMake(UnitType type, Unit builder = nullptr) const override;
      virtual bool canResearch(TechType type, Unit unit = nullptr, bool checkCanIssueCommandType = true) override;
      virtual bool canUpgrade(UpgradeType type, Unit unit = nullptr, bool checkCanIssueCommandType = true) override;
      virtual const TilePosition::list& getStartLocations() const override;

      virtual void vPrintf(const char* format, va_list arg) override;
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
      virtual void setLocalSpeed(int speed = -1) override;
      virtual bool issueCommand(const Unitset& units, UnitCommand command) override;
      virtual const Unitset& getSelectedUnits() const override;
      virtual Player self() const override;
      virtual Player enemy() const override;
      virtual Player neutral() const override;
      virtual Playerset& allies() override;
      virtual Playerset& enemies() override;
      virtual Playerset& observers() override;

      virtual void setTextSize(Text::Size::Enum size = Text::Size::Default) override;
      virtual void vDrawText(CoordinateType::Enum ctype, int x, int y, const char *format, va_list arg) override;
      virtual void drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false) override;
      virtual void drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) override;
      virtual void drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid = false) override;
      virtual void drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false) override;
      virtual void drawDot(CoordinateType::Enum ctype, int x, int y, Color color) override;
      virtual void drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color) override;

      virtual int  getLatencyFrames() const override;
      virtual int  getLatencyTime() const override;
      virtual int  getRemainingLatencyFrames() const override;
      virtual int  getRemainingLatencyTime() const override;
      virtual int  getRevision() const override;
      virtual int  getClientVersion() const override;
      virtual bool isDebug() const override;
      virtual bool isLatComEnabled() const override;
      virtual void setLatCom(bool isEnabled) override;
      virtual bool isGUIEnabled() const override;
      virtual void setGUI(bool enabled) override;
      virtual int  getInstanceNumber() const override;
      virtual int  getAPM(bool includeSelects = false) const override;
      virtual bool setMap(const char *mapFileName) override;
      virtual void setFrameSkip(int frameSkip) override;
      virtual bool setAlliance(BWAPI::Player player, bool allied = true, bool alliedVictory = true) override;
      virtual bool setVision(BWAPI::Player player, bool enabled = true) override;
      virtual int  elapsedTime() const override;
      virtual void setCommandOptimizationLevel(int level) override;
      virtual int  countdownTimer() const override;
      virtual const Regionset &getAllRegions() const override;
      virtual BWAPI::Region getRegionAt(int x, int y) const override;
      virtual int getLastEventTime() const override;
      virtual bool setRevealAll(bool reveal = true) override;
      virtual unsigned getRandomSeed() const override;
  };
}
