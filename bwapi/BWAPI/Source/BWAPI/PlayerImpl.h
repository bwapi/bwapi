#pragma once

namespace BW { class Unit; };

#include <BWAPI/Race.h>
#include <BWAPI/Player.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include "UnitImpl.h"
#include "ForceImpl.h"
#include <BWAPI/Client/PlayerData.h>
#include <list>
#include <set>
#include <string>
#include <Util/Types.h>

#include <BW/Offsets.h>
#include <BW/Race.h>
#include <BW/TilePosition.h>

namespace BWAPI
{
  class Unit;
  class Force;
  /** Represents one player in game. Note that there is always player 12 who owns resources. */
  class PlayerImpl : public Player
  {
    public :

      /**
       * Gets the name of the player
       * @return Pointer directly to bw memory where the name is represented.
       */
      virtual int         getID() const;
      virtual std::string getName() const;
      virtual Race        getRace() const;
      virtual PlayerType  getType() const;
      virtual Force*      getForce() const;

      virtual bool isAlly(Player* player) const;
      virtual bool isEnemy(Player* player) const;
      virtual bool isNeutral() const;

      virtual bool isVictorious() const;
      virtual bool isDefeated() const;
      virtual bool leftGame() const;

      virtual const std::set<Unit*>& getUnits() const;
      virtual TilePosition  getStartLocation() const;

      virtual int  minerals() const;
      virtual int  gas() const;

      virtual int  cumulativeMinerals() const;
      virtual int  cumulativeGas() const;

      virtual int  supplyTotal() const;
      virtual int  supplyUsed() const;
      virtual int  supplyTotal(Race race) const;
      virtual int  supplyUsed(Race race) const;

      virtual int  allUnitCount(UnitType unit) const;
      virtual int  visibleUnitCount(UnitType unit) const;
      virtual int  completedUnitCount(UnitType unit) const;
      virtual int  incompleteUnitCount(UnitType unit) const;
      virtual int  deadUnitCount(UnitType unit) const;
      virtual int  killedUnitCount(UnitType unit) const;

      virtual int  getUpgradeLevel(UpgradeType upgrade) const;
      virtual bool hasResearched(TechType tech) const;
      virtual bool isResearching(TechType tech) const;
      virtual bool isUpgrading(UpgradeType upgrade) const;

      virtual BWAPI::Color getColor() const;
      virtual int getTextColor() const;

      virtual int maxEnergy(UnitType unit) const;
      virtual double topSpeed(UnitType unit) const;
      virtual int groundWeaponMaxRange(UnitType unit) const;
      virtual int airWeaponMaxRange(UnitType unit) const;
      virtual int sightRange(UnitType unit) const;
      virtual int groundWeaponDamageCooldown(UnitType unit) const;
      virtual int armor(UnitType unit) const;

      virtual int getUnitScore() const;
      virtual int getKillScore() const;
      virtual int getBuildingScore() const;
      virtual int getRazingScore() const;
      virtual int getCustomScore() const;

      //Internal BWAPI commands:
      /**
       * Constructs new player
       * @param id 0-based Index of the player (11 for resources as it is player 12)
       */
      PlayerImpl(u8 index);
      ~PlayerImpl();
      int getIndex() const;
      /** Gets 0-based index of the player. (11 for neutral) */
      char* getForceName() const;
      void setID(int newID);
      ForceImpl* force;

      /** Gets the bw mapping of synchronised version of selected unit for current player */
      BW::Unit** selectedUnit();
      /**
       * Updates localData according to bw values. (Should be called every frame before local data updates are
       * performed
       */
      void updateData();
      PlayerData data;
      PlayerData* self;
      void onGameEnd();
      std::set<Unit*> units;
      bool leftTheGame;

      bool isParticipating();

    private :
      int id;
      u8 index;  /**< Order of the player, is used to load player's information from the memory */
  };
};
