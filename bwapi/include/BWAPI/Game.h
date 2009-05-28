#pragma once

#include <list>
#include <map>
#include <set>

#include <BW/Latency.h>
#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/TilePosition.h>
#include <BW/OrderTypes.h>
#include <BWAPI/Flag.h>
namespace BWAPI
{
  class Force;
  class Player;
  class Unit;
  class Game
  {
  public :
    virtual std::set< Force* > getForces() const=0;
    virtual std::set< Player* > getPlayers() const=0;
    virtual std::set< Unit* > getAllUnits() const=0;
    virtual std::set< Unit* > getMinerals() const=0;
    virtual std::set< Unit* > getGeysers() const=0;
    virtual std::set< Unit* > getNeutralUnits() const=0;

    virtual BW::Latency::Enum getLatency()=0;
    virtual int getFrameCount() const=0;
    virtual int getMouseX() const=0;
    virtual int getMouseY() const=0;
    virtual int getScreenX() const=0;
    virtual int getScreenY() const=0;
    virtual bool isFlagEnabled(BWAPI::Flag::Enum flag) const=0;
    virtual void enableFlag(BWAPI::Flag::Enum flag)=0;
    virtual std::list<Unit*> unitsOnTile(int x, int y) const=0;

    virtual const std::set< BW::UnitType >& allUnitTypes() const=0;
    virtual BW::UnitType getUnitType(std::string &name) const=0;

    virtual BW::UpgradeType getUpgradeType(std::string &name) const=0;

    virtual BW::TechType getTechType(std::string &name) const=0;

    virtual int mapWidth() const=0;
    virtual int mapHeight() const=0;
    virtual std::string mapFilename() const=0;
    virtual std::string mapName() const=0;
    virtual bool buildable(int x, int y) const=0;
    virtual bool walkable(int x, int y) const=0;
    virtual bool visible(int x, int y) const=0;
    virtual int groundHeight(int x, int y) const=0; 
    virtual const std::set< BW::TilePosition >& getStartLocations() const=0;
    virtual int getMapHash() const=0;

    virtual void print(const char *text, ...)=0;
    virtual void printEx(s32 pID, const char *text, ...)=0;
    virtual void printPublic(const char *text, ...)=0;
    virtual bool isOnStartCalled() const=0;
    virtual bool isInGame() const=0;
    virtual void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)=0;
    virtual void changeRace(BW::Race::Enum race, u8 slotID)=0;
    virtual void startGame()=0;
    virtual void pauseGame()=0;
    virtual void resumeGame()=0;
    virtual const std::set<BWAPI::Unit*>& getSelectedUnits() const=0;
    virtual Player* self() const=0;
    virtual Player* enemy() const=0;
  };
  extern Game* Broodwar;
}