#pragma once
#include <BWAPI.h>
#include <SharedStructures\GameData.h>
#include "BWAPIClient.h"
#include <SharedStructures\Shape.h>
#include <SharedStructures\Command.h>
#include <SharedStructures\UnitCommand.h>
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include <list>
#include <map>
#include <set>
#include <vector>

namespace BWAPI
{
  class Force;
  class Player;
  class Unit;
  class GameImpl : public Game
  {
    private :
      int addShape(BWAPIC::Shape &s);
      int addString(const char* text);
      int addText(BWAPIC::Shape &s, const char* text);
      int addCommand(BWAPIC::Command &c);
      void clearAll();

      BWAPIC::GameData* data;
      std::vector<ForceImpl> forceVector;
      std::vector<PlayerImpl> playerVector;
      std::vector<UnitImpl> unitVector;

      std::set<Force*> forces;
      std::set<Player*> players;
      std::set<Unit*> notDestroyedUnits;//allUnits that may still be alive
      std::set<Unit*> accessibleUnits;//all units that are accessible (and definitely alive)
      //notDestroyedUnits - accessibleUnits = all units that may or may not be alive (status unknown)
      std::set<Unit*> minerals;
      std::set<Unit*> geysers;
      std::set<Unit*> neutralUnits;
      std::set<Unit*> staticMinerals;
      std::set<Unit*> staticGeysers;
      std::set<Unit*> staticNeutralUnits;
      std::set<Unit*> selectedUnits;
      std::set<Unit*> pylons;
      std::set<Unit*> unitsOnTileData[256][256];

      std::set< TilePosition > startLocations;
      std::list< Event > events;
      bool flagEnabled[2];
      Player* thePlayer;
      Player* theEnemy;
      Error lastError;

    public :
      Event makeEvent(BWAPIC::Event e);
      int addUnitCommand(BWAPIC::UnitCommand& c);
      GameImpl(BWAPIC::GameData* data);
      void onMatchStart();
      void onMatchEnd();
      void onMatchFrame();
      std::set<Unit*>& getPlayerUnits(const Player* player);

      Force* getForce(int forceID);
      Player* getPlayer(int playerID);
      Unit* getUnit(int unitID);

      virtual std::set< Force* >& getForces();
      virtual std::set< Player* >& getPlayers();
      virtual std::set< Unit* >& getAllUnits();
      virtual std::set< Unit* >& getMinerals();
      virtual std::set< Unit* >& getGeysers();
      virtual std::set< Unit* >& getNeutralUnits();

      virtual std::set< Unit* >& getStaticMinerals();
      virtual std::set< Unit* >& getStaticGeysers();
      virtual std::set< Unit* >& getStaticNeutralUnits();

      virtual std::list< Event>& getEvents();

      virtual int getLatency();
      virtual int getFrameCount();
      virtual int getMouseX();
      virtual int getMouseY();
      virtual BWAPI::Position getMousePosition();
      virtual bool getMouseState(MouseButton button);
      virtual bool getMouseState(int button);
      virtual bool getKeyState(Key key);
      virtual bool getKeyState(int key);
      virtual int getScreenX();
      virtual int getScreenY();
      virtual BWAPI::Position getScreenPosition();
      virtual void setScreenPosition(int x, int y);
      virtual void setScreenPosition(BWAPI::Position p);
      virtual void pingMinimap(int x, int y);
      virtual void pingMinimap(BWAPI::Position p);

      virtual bool  isFlagEnabled(int flag);
      virtual void  enableFlag(int flag);
      virtual std::set<Unit*>& unitsOnTile(int x, int y);
      virtual Error  getLastError() const;

      virtual int  mapWidth();
      virtual int  mapHeight();
      virtual std::string  mapFilename();
      virtual std::string  mapName();
      virtual int getMapHash();

      virtual int  getGroundHeight(int x, int y);
      virtual bool isWalkable(int x, int y);
      virtual bool isBuildable(int x, int y);
      virtual bool isVisible(int x, int y);
      virtual bool isExplored(int x, int y);
      virtual bool hasCreep(int x, int y);
      virtual bool hasPower(int x, int y, int tileWidth, int tileHeight);

      virtual bool isBuildable(TilePosition position);
      virtual bool isVisible(TilePosition position);
      virtual bool isExplored(TilePosition position);
      virtual bool hasCreep(TilePosition position);
      virtual bool hasPower(TilePosition position, int tileWidth, int tileHeight);

      virtual bool canBuildHere(Unit* builder, TilePosition position, UnitType type);
      virtual bool canMake(Unit* builder, UnitType type);
      virtual bool canResearch(Unit* unit, TechType type);
      virtual bool canUpgrade(Unit* unit, UpgradeType type);
      virtual std::set< TilePosition >& getStartLocations();

      virtual void printf(const char* text, ...);
      virtual void sendText(const char* text, ...);

      virtual void changeRace(BWAPI::Race race);
      virtual bool isInGame();
      virtual bool isMultiplayer();
      virtual bool isPaused();
      virtual bool isReplay();

      virtual void  startGame();
      virtual void  pauseGame();
      virtual void  resumeGame();
      virtual void  leaveGame();
      virtual void  restartGame();
      virtual void  setLocalSpeed(int speed = -1);
      virtual std::set<BWAPI::Unit*>& getSelectedUnits();
      virtual Player*  self();
      virtual Player*  enemy();

      virtual void drawText(int ctype, int x, int y, const char* text, ...);
      virtual void drawTextMap(int x, int y, const char* text, ...);
      virtual void drawTextMouse(int x, int y, const char* text, ...);
      virtual void drawTextScreen(int x, int y, const char* text, ...);

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
  };
}
