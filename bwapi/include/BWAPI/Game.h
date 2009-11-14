#pragma once

#include <list>
#include <map>
#include <set>

#include <BWAPI/Color.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Error.h>
#include <BWAPI/Flag.h>
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>
#include <BWAPI/Latency.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
namespace BWAPI
{
  class Force;
  class Player;
  class Unit;
  class Game
  {
    public :
      virtual std::set< Force* > __cdecl getForces() = 0;
      virtual std::set< Player* > __cdecl getPlayers() = 0;
      virtual std::set< Unit* > __cdecl getAllUnits() = 0;
      virtual std::set< Unit* > __cdecl getMinerals() = 0;
      virtual std::set< Unit* > __cdecl getGeysers() = 0;
      virtual std::set< Unit* > __cdecl getNeutralUnits() = 0;

      virtual Latency::Enum __cdecl getLatency() = 0;
      virtual int __cdecl getFrameCount() = 0;
      virtual int __cdecl getMouseX() = 0;
      virtual int __cdecl getMouseY() = 0;
      virtual int __cdecl getScreenX() = 0;
      virtual int __cdecl getScreenY() = 0;

      virtual bool __cdecl isFlagEnabled(Flag::Enum flag) = 0;
      virtual void __cdecl enableFlag(Flag::Enum flag) = 0;
      virtual std::set<Unit*> __cdecl unitsOnTile(int x, int y) = 0;
      virtual Error __cdecl getLastError() const = 0;

      virtual int __cdecl mapWidth() = 0;
      virtual int __cdecl mapHeight() = 0;
      virtual std::string __cdecl mapFilename() = 0;
      virtual std::string __cdecl mapName() = 0;
      virtual bool __cdecl buildable(int x, int y) = 0;
      virtual bool __cdecl walkable(int x, int y) = 0;
      virtual bool __cdecl visible(int x, int y) = 0;
      virtual bool __cdecl hasCreep(int x, int y) = 0;
      virtual bool __cdecl hasPower(int x, int y, int tileWidth, int tileHeight) = 0;
      virtual bool __cdecl canBuildHere(Unit* builder, TilePosition position, UnitType type) = 0;
      virtual bool __cdecl canMake(Unit* builder, UnitType type) = 0;
      virtual bool __cdecl canResearch(Unit* unit, TechType type) = 0;
      virtual bool __cdecl canUpgrade(Unit* unit, UpgradeType type) = 0;
      virtual int __cdecl groundHeight(int x, int y)= 0;
      virtual std::set< TilePosition > __cdecl getStartLocations() = 0;
      virtual int __cdecl getMapHash() = 0;

      virtual void __cdecl printf(const char* text, ...) = 0;
      virtual void __cdecl sendText(const char* text, ...) = 0;
      virtual void __cdecl text(CoordinateType::Enum ctype, int x, int y, const char* text, ...) = 0;
      virtual void __cdecl changeRace(Race race) = 0;
      virtual bool __cdecl inReplay() = 0;
      virtual void __cdecl startGame() = 0;
      virtual void __cdecl pauseGame() = 0;
      virtual void __cdecl resumeGame() = 0;
      virtual void __cdecl leaveGame() = 0;
      virtual void __cdecl setLocalSpeed(int speed) = 0;
      virtual std::set<Unit*> __cdecl getSelectedUnits() = 0;
      virtual Player* __cdecl self() = 0;
      virtual Player* __cdecl enemy() = 0;
      virtual void __cdecl drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid) = 0;
      virtual void __cdecl drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid) = 0;
      virtual void __cdecl drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid) = 0;
      virtual void __cdecl drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid) = 0;
      virtual void __cdecl drawDot(CoordinateType::Enum ctype, int x, int y, Color color) = 0;
      virtual void __cdecl drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color) = 0;
  };
  extern Game* Broodwar;
}
