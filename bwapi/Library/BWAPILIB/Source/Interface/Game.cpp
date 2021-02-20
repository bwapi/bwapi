#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Region.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Player.h>

#include <BWAPI/UnitSizeType.h>
#include <BWAPI/DamageType.h>
#include <BWAPI/ExplosionType.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/GameType.h>

#include <BWAPI/Client/Client.h>

#include "../Convenience.h"

#include <numeric>
#include <cstdarg>

// Needed by other compilers.
#include <cstring>


namespace BWAPI
{
    using namespace Filter;

    Game::Game(Client& newClient)
      : client(newClient)
      , commandOptimizer(*this)
    {
      initGameData();
    }

    void Game::initGameData()
    {
      gameData = std::make_unique<GameData>();
    }

    //------------------------------------ ETC ------------------------------------------
    Unitset Game::getUnitsOnTile(int tileX, int tileY, const UnitFilter &pred) const {
        return getUnitsOnTile({ tileX, tileY }, pred);
    }
    Unitset Game::getUnitsOnTile(BWAPI::TilePosition tile, const UnitFilter &pred) const {
        if (isValid(tile)) {
            Position p{ tile };
            return getUnitsInRectangle(p.x, p.y, p.x + 32, p.y + 32, pred);
        }
        return Unitset::none;
    }
    //------------------------------------ DAMAGE CALCULATION ------------------------------------------
    int damageRatio[DamageTypes::Enum::MAX][UnitSizeTypes::Enum::MAX] =
    {
        // Ind, Sml, Med, Lrg, Non, Unk
          {  0,   0,   0,   0,   0,   0 }, // Independent
          {  0, 128, 192, 256,   0,   0 }, // Explosive
          {  0, 256, 128,  64,   0,   0 }, // Concussive
          {  0, 256, 256, 256,   0,   0 }, // Normal
          {  0, 256, 256, 256,   0,   0 }, // Ignore_Armor
          {  0,   0,   0,   0,   0,   0 }, // None
          {  0,   0,   0,   0,   0,   0 }  // Unknown
    };
    int getDamageFromImpl(UnitType fromType, UnitType toType, Player fromPlayer, Player toPlayer)
    {
        // Retrieve appropriate weapon
        WeaponType wpn = toType.isFlyer() ? fromType.airWeapon() : fromType.groundWeapon();
        if (wpn == WeaponTypes::None || wpn == WeaponTypes::Unknown)
            return 0;

        // Get initial weapon damage
        int dmg = fromPlayer ? fromPlayer->damage(wpn) : wpn.damageAmount() * wpn.damageFactor();

        // If we need to calculate using armor
        if (wpn.damageType() != DamageTypes::Ignore_Armor && toPlayer != nullptr)
            dmg -= std::min(dmg, toPlayer->armor(toType));

        return dmg * damageRatio[wpn.damageType()][toType.size()] / 256;
    }
    int Game::getDamageFrom(UnitType fromType, UnitType toType, Player fromPlayer, Player toPlayer)
    {
        // Get self if toPlayer not provided
        if (toPlayer == nullptr)
            toPlayer = this->self();

        return getDamageFromImpl(fromType, toType, fromPlayer, toPlayer);
    }
    int Game::getDamageTo(UnitType toType, UnitType fromType, Player toPlayer, Player fromPlayer)
    {
        // Get self if fromPlayer not provided
        if (fromPlayer == nullptr)
            fromPlayer = this->self();

        return getDamageFromImpl(fromType, toType, fromPlayer, toPlayer);
    }
    //-------------------------------------- BUILD LOCATION --------------------------------------------
    // TODO: Redo this garbage and don't try to mimic Starcraft built-in AI
    constexpr int MAX_RANGE = 64;
    class PlacementReserve
    {
    public:
        PlacementReserve(int maxRange) : maxSearch(std::min(std::max(0, maxRange), MAX_RANGE))
        {
            this->reset();
            this->backup();
        };

        void reset()
        {
            memset(data, 0, sizeof(data));
        };

        // Checks if the given x/y value is valid for the Placement position
        static bool isValidPos(int x, int y)
        {
            return x >= 0 && x < MAX_RANGE && y >= 0 && y < MAX_RANGE;
        };
        static bool isValidPos(TilePosition p)
        {
            return isValidPos(p.x, p.y);
        };

        // Sets the value in the placement reserve array
        void setValue(int x, int y, unsigned char value)
        {
            if (isValidPos(x, y))
                data[y][x] = value;
        };
        void setValue(TilePosition p, unsigned char value)
        {
            this->setValue(p.x, p.y, value);
        };
        void setRange(int left, int top, int right, int bottom, unsigned char value)
        {
            for (int y = top; y < bottom; ++y)
                for (int x = left; x < right; ++x)
                    this->setValue(x, y, value);
        };
        void setRange(TilePosition lt, TilePosition rb, unsigned char value)
        {
            this->setRange(lt.x, lt.y, rb.x, rb.y, value);
        };

        // Gets the value from the placement reserve array, 0 if position is invalid
        unsigned char getValue(int x, int y) const
        {
            if (isValidPos(x, y))
                return data[y][x];
            return 0;
        };
        unsigned char getValue(TilePosition p) const
        {
            return this->getValue(p.x, p.y);
        };

        template <typename T>
        void iterate(const T& proc)
        {
            // Get min/max distances
            int min = MAX_RANGE / 2 - maxSearch / 2;
            int max = min + maxSearch;
            for (int y = min; y < max; ++y)
                for (int x = min; x < max; ++x)
                    proc(this, x, y);
        }

        bool hasValidSpace() const
        {
            // Get min/max distances
            int min = MAX_RANGE / 2 - maxSearch / 2;
            int max = min + maxSearch;
            for (int y = min; y < max; ++y)
            {
                for (int x = min; x < max; ++x)
                {
                    if (this->getValue(x, y) == 1)
                        return true;
                }
            }
            return false;
        }

        void backup()
        {
            memcpy(save, data, sizeof(data));
        }
        void restore()
        {
            memcpy(data, save, sizeof(data));
        }
        void restoreIfInvalid(const char * /*const pszDebug*/)
        {
            if (!hasValidSpace())
            {
                //Broodwar << pszDebug << std::endl;
                restore();
            }
        }
        int getMaxRange() const
        {
            return this->maxSearch;
        }
    private:
        unsigned char data[MAX_RANGE][MAX_RANGE];
        unsigned char save[MAX_RANGE][MAX_RANGE];
        int maxSearch;
    };

    void AssignBuildableLocations(Game const &game, PlacementReserve &reserve, UnitType type, TilePosition desiredPosition)
    {
        TilePosition start = desiredPosition - TilePosition(MAX_RANGE, MAX_RANGE) / 2;

        // Reserve space for the addon as well
        bool hasAddon = type.canBuildAddon();

        // Assign 1 to all buildable locations
        reserve.iterate([&](PlacementReserve *pr, int x, int y)
        {
            if ((!hasAddon || game.canBuildHere(start + TilePosition(x + 4, y + 1), UnitTypes::Terran_Missile_Turret)) &&
                game.canBuildHere(start + TilePosition(x, y), type))
            {
                pr->setValue(x, y, 1);
            }
        });
    }

    void RemoveDisconnected(Game const &game, PlacementReserve &reserve, TilePosition desiredPosition)
    {
        TilePosition start = desiredPosition - TilePosition(MAX_RANGE, MAX_RANGE) / 2;

        // Assign 0 to all locations that aren't connected
        reserve.iterate([&](PlacementReserve *pr, int x, int y)
        {
            if (!game.hasPath(Position(desiredPosition), Position(start + TilePosition(x, y))))
                pr->setValue(x, y, 0);
        });
    }

    // @TODO: This interpretation might be incorrect
    void ReserveStructureWithPadding(PlacementReserve &reserve, TilePosition currentPosition, TilePosition sizeExtra, int padding, UnitType type, TilePosition desiredPosition)
    {
        TilePosition paddingSize = sizeExtra + TilePosition(padding, padding) * 2;

        TilePosition topLeft = currentPosition - type.tileSize() - paddingSize / 2 - TilePosition(1, 1);
        TilePosition topLeftRelative = topLeft - desiredPosition + TilePosition(MAX_RANGE, MAX_RANGE) / 2;
        TilePosition maxSize = topLeftRelative + type.tileSize() + paddingSize + TilePosition(1, 1);

        reserve.setRange(topLeftRelative, maxSize, 0);
    }

    /*
    void ReserveUnbuildable(PlacementReserve &reserve, UnitType type, TilePosition desiredPosition)
    {
      if ( type == UnitTypes::Special_Start_Location )
        return;

      TilePosition start = desiredPosition - TilePosition(MAX_RANGE,MAX_RANGE)/2;

      // Exclude locations with unbuildable locations, but restore a backup in case there are no more build locations
      reserve.backup();
      reserve.iterate( [&](PlacementReserve *, int x, int y)
                        {
                          if ( !Broodwar->isBuildable( start + TilePosition(x,y), type != UnitTypes::Special_Start_Location ) )
                            ReserveStructureWithPadding(reserve, start + TilePosition(x,y), TilePosition(1,1), 1, type, desiredPosition);
                        });

      // Restore original if there is nothing left
      reserve.restoreIfInvalid(__FUNCTION__);
    }*/

    void ReserveGroundHeight(Game const &game, PlacementReserve &reserve, TilePosition desiredPosition)
    {
        TilePosition start = desiredPosition - TilePosition(MAX_RANGE, MAX_RANGE) / 2;

        // Exclude locations with a different ground height, but restore a backup in case there are no more build locations
        reserve.backup();
        int targetHeight = game.getGroundHeight(desiredPosition);
        reserve.iterate([&](PlacementReserve *pr, int x, int y)
        {
            if (game.getGroundHeight(start + TilePosition(x, y)) != targetHeight)
                pr->setValue(x, y, 0);
        });

        // Restore original if there is nothing left
        reserve.restoreIfInvalid(__FUNCTION__);
    }

    void ReserveExistingAddonPlacement(Game const &game, PlacementReserve &reserve, TilePosition desiredPosition)
    {
        TilePosition start = desiredPosition - TilePosition(MAX_RANGE, MAX_RANGE) / 2;

        //Exclude addon placement locations
        reserve.backup();
        Unitset myUnits = game.self()->getUnits();
        myUnits.erase_if(!(Exists && CanBuildAddon));
        for (auto &u : myUnits)
        {
            TilePosition addonPos = (u->getTilePosition() + TilePosition(4, 1)) - start;
            reserve.setRange(addonPos, addonPos + TilePosition(2, 2), 0);
        }

        // Restore if this gave us no build locations
        reserve.restoreIfInvalid(__FUNCTION__);
    }

    void ReserveStructure(PlacementReserve &reserve, Unit pUnit, int padding, UnitType type, TilePosition desiredPosition)
    {
        ReserveStructureWithPadding(reserve, TilePosition(pUnit->getPosition()), pUnit->getType().tileSize(), padding, type, desiredPosition);
    }

    void ReserveAllStructures(Game const &game, PlacementReserve &reserve, UnitType type, TilePosition desiredPosition)
    {
        if (type.isAddon())
            return;
        reserve.backup();

        // Reserve space around owned resource depots and resource containers
        Unitset myUnits = game.self()->getUnits();
        myUnits.erase_if(!(Exists && (IsCompleted || (ProducesLarva && IsMorphing)) && IsBuilding && (IsResourceDepot || IsRefinery)));
        for (auto &u : myUnits)
            ReserveStructure(reserve, u, 2, type, desiredPosition);

        // Reserve space around neutral resources
        if (type != UnitTypes::Terran_Bunker)
        {
            Unitset resources = game.getNeutralUnits();
            resources.erase_if(!(Exists && IsResourceContainer));
            for (auto &u : resources)
                ReserveStructure(reserve, u, 2, type, desiredPosition);
        }
        reserve.restoreIfInvalid(__FUNCTION__);
    }

    TilePosition gDirections[] = {
      TilePosition(1, 1),
      TilePosition(0, 1),
      TilePosition(-1, 1),
      TilePosition(1, 0),
      TilePosition(-1, 0),
      TilePosition(1,-1),
      TilePosition(0,-1),
      TilePosition(-1,-1)
    };
    void ReserveDefault(Game const &game, PlacementReserve &reserve, UnitType type, TilePosition desiredPosition)
    {
        reserve.backup();
        auto original = reserve;

        // Reserve some space around some specific units
        for (auto &it : game.self()->getUnits())
        {
            if (!it->exists())
                continue;

            switch (it->getType())
            {
            case UnitTypes::Enum::Terran_Factory:
            case UnitTypes::Enum::Terran_Missile_Turret:
            case UnitTypes::Enum::Protoss_Robotics_Facility:
            case UnitTypes::Enum::Protoss_Gateway:
            case UnitTypes::Enum::Protoss_Photon_Cannon:
            default:
                ReserveStructure(reserve, it, 2, type, desiredPosition);
                break;
            case UnitTypes::Enum::Terran_Barracks:
            case UnitTypes::Enum::Terran_Bunker:
            case UnitTypes::Enum::Zerg_Creep_Colony:
                ReserveStructure(reserve, it, 1, type, desiredPosition);
                break;
            }
        }

        switch (type)
        {
        case UnitTypes::Enum::Terran_Barracks:
        case UnitTypes::Enum::Terran_Factory:
        case UnitTypes::Enum::Terran_Missile_Turret:
        case UnitTypes::Enum::Terran_Bunker:
        case UnitTypes::Enum::Protoss_Robotics_Facility:
        case UnitTypes::Enum::Protoss_Gateway:
        case UnitTypes::Enum::Protoss_Photon_Cannon:
            for (int y = 0; y < 64; ++y)
            {
                for (int x = 0; x < 64; ++x)
                {
                    for (int dir = 0; dir < 8; ++dir)
                    {
                        TilePosition p = TilePosition(x, y) + gDirections[dir];
                        if (!PlacementReserve::isValidPos(p) || original.getValue(p) == 0)
                            reserve.setValue(p, 0);
                    }

                }
            }
            break;
        }
        reserve.restoreIfInvalid(__FUNCTION__);
    }

    void ReservePlacement(Game const &game, PlacementReserve &reserve, UnitType type, TilePosition desiredPosition, bool /*creep*/)
    {
        // Reset the array
        reserve.reset();

        AssignBuildableLocations(game, reserve, type, desiredPosition);
        RemoveDisconnected(game, reserve, desiredPosition);

        // @TODO: Assign 0 to all locations that have a ground distance > maxRange

        // exclude positions off the map
        TilePosition start = desiredPosition - TilePosition(MAX_RANGE, MAX_RANGE) / 2;
        reserve.iterate([&](PlacementReserve *pr, int x, int y)
        {
            if (!game.isValid(start + TilePosition(x, y)))
                pr->setValue(x, y, 0);
        });

        // Return if can't find a valid space
        if (!reserve.hasValidSpace())
            return;

        ReserveGroundHeight(game, reserve, desiredPosition);
        //ReserveUnbuildable(reserve, type, desiredPosition); // NOTE: canBuildHere already includes this!

        if (!type.isResourceDepot())
        {
            ReserveAllStructures(game, reserve, type, desiredPosition);
            ReserveExistingAddonPlacement(game, reserve, desiredPosition);
        }

        // Unit-specific reservations
        switch (type)
        {
        case UnitTypes::Enum::Protoss_Pylon:  // @TODO
          //reservePylonPlacement();
            break;
        case UnitTypes::Enum::Terran_Bunker:  // @TODO
          //if ( !GetBunkerPlacement() )
        {
            //reserveTurretPlacement();
        }
        break;
        case UnitTypes::Enum::Terran_Missile_Turret:  // @TODO
        case UnitTypes::Enum::Protoss_Photon_Cannon:
            //reserveTurretPlacement();
            break;
        case UnitTypes::Enum::Zerg_Creep_Colony:  // @TODO
          //if ( creep || !GetBunkerPlacement() )
        {
            //reserveTurretPlacement();
        }
        break;
        default:
            if (!type.isResourceDepot())
                ReserveDefault(game, reserve, type, desiredPosition);
            break;
        }
    }

    struct buildTemplate
    {
        int startX, startY, stepX, stepY;
    } buildTemplates[] = // [13 + 1]
    {
      { 32, 0, 0, 1 },
      { 0, 32, 1, 0 },
      { 31, 0, 0, 1 },
      { 0, 31, 1, 0 },
      { 33, 0, 0, 1 },
      { 0, 33, 1, 0 },
      { 30, 0, 0, 1 },
      { 29, 0, 0, 1 },
      { 0, 30, 1, 0 },
      { 28, 0, 0, 1 },
      { 0, 29, 1, 0 },
      { 27, 0, 0, 1 },
      { 0, 28, 1, 0 },
      { -1, 0, 0, 0 } // last
    };
    void reserveTemplateSpacing(PlacementReserve &reserve)
    {
        reserve.backup();

        for (buildTemplate *t = buildTemplates; t->startX != -1; ++t)
        {
            int x = t->startX, y = t->startY;
            for (int i = 0; i < 64; ++i)
            {
                reserve.setValue(x, y, 0);
                x += t->stepX;
                y += t->stepY;
            }
        }

        reserve.restoreIfInvalid(__FUNCTION__);
    }

    // ----- GET BUILD LOCATION
    // @TODO: If self() is nullptr, this will crash
    TilePosition Game::getBuildLocation(UnitType type, TilePosition desiredPosition, int maxRange, bool creep) const
    {
        this->setLastError(); // Reset last error

        if (!self())
        {
          this->setLastError(Errors::Incompatible_State);
          return TilePositions::Invalid;
        }

        // Make sure the type is compatible
        if (!type.isBuilding())
        {
            this->setLastError(Errors::Incompatible_UnitType);
            return TilePositions::Invalid;
        }
        // @TODO: Addon

        // Do type-specific checks
        bool trimPlacement = true;
        Region pTargRegion = nullptr;
        Unit pSpecialUnitTarget = nullptr;
        switch (type)
        {
        case UnitTypes::Enum::Protoss_Pylon:
            pSpecialUnitTarget = this->getClosestUnit(Position(desiredPosition), IsOwned && !IsPowered);
            if (pSpecialUnitTarget)
            {
                desiredPosition = TilePosition(pSpecialUnitTarget->getPosition());
                trimPlacement = false;
            }
            break;
        case UnitTypes::Enum::Terran_Command_Center:
        case UnitTypes::Enum::Protoss_Nexus:
        case UnitTypes::Enum::Zerg_Hatchery:
        case UnitTypes::Enum::Special_Start_Location:
            trimPlacement = false;
            break;
        case UnitTypes::Enum::Zerg_Creep_Colony:
        case UnitTypes::Enum::Terran_Bunker:
            //if ( Get bunker placement region )
            //  trimPlacement = false;
            break;
        }

        PlacementReserve reserve(maxRange);
        ReservePlacement(*this, reserve, type, desiredPosition, creep);

        if (trimPlacement)
            reserveTemplateSpacing(reserve);

        TilePosition centerPosition = desiredPosition - TilePosition(MAX_RANGE, MAX_RANGE) / 2;
        if (pTargRegion != nullptr)
            desiredPosition = TilePosition(pTargRegion->getCenter());

        // Find the best position
        int bestDistance, fallbackDistance;
        TilePosition bestPosition, fallbackPosition;

        bestDistance = fallbackDistance = 999999;
        bestPosition = fallbackPosition = TilePositions::None;
        for (int passCount = 0; passCount < (pTargRegion ? 2 : 1); ++passCount)
        {
            for (int y = 0; y < MAX_RANGE; ++y)
                for (int x = 0; x < MAX_RANGE; ++x)
                {
                    // Ignore if space is reserved
                    if (reserve.getValue(x, y) == 0)
                        continue;

                    TilePosition currentPosition(TilePosition(x, y) + centerPosition);
                    int currentDistance = desiredPosition.getApproxDistance(currentPosition);//Broodwar->getGroundDistance( desiredPosition, currentPosition );
                    if (currentDistance < bestDistance)
                    {
                        if (currentDistance <= maxRange)
                        {
                            bestDistance = currentDistance;
                            bestPosition = currentPosition;
                        }
                        else if (currentDistance < fallbackDistance)
                        {
                            fallbackDistance = currentDistance;
                            fallbackPosition = currentPosition;
                        }
                    }
                }
            // Break pass if position is found
            if (bestPosition != TilePositions::None)
                break;

            // Break if an alternative position was found
            if (fallbackPosition != TilePositions::None)
            {
                bestPosition = fallbackPosition;
                break;
            }

            // If we were really targetting a region, and couldn't find a position above
            if (pTargRegion != nullptr) // Then fallback to the default build position
                desiredPosition = centerPosition;
        }

        return bestPosition;
    }
    //------------------------------------------- FLAGS ------------------------------------------------
    bool Game::isFlagEnabled(int flag) const
    {
      // TODO: Implement
      return false;
    }
    //------------------------------------------ ACTIONS -----------------------------------------------
    void Game::setScreenPosition(int x, int y)
    {
      client.setScreenPosition(x, y);
    }
    void Game::setScreenPosition(BWAPI::Position p)
    {
      setScreenPosition(p.x, p.y);
    }
    void Game::pingMinimap(int x, int y)
    {
      client.pingMinimap(x, y);
    }
    void Game::pingMinimap(BWAPI::Position p)
    {
      pingMinimap(p.x, p.y);
    }
    void Game::vSendText(const char *format, va_list arg)
    {
        vSendTextEx(false, format, arg);
    }
    void Game::sendText(const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vSendTextEx(false, format, ap);
        va_end(ap);
    }
    void Game::sendTextEx(bool toAllies, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vSendTextEx(toAllies, format, ap);
        va_end(ap);
    }
    void Game::printf(const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vPrintf(format, ap);
        va_end(ap);
    }
    //--------------------------------------------- HAS POWER --------------------------------------------------
    bool Game::hasPower(int tileX, int tileY, UnitType unitType) const
    {
        if (unitType.isValid() && unitType < UnitTypes::None)
            return hasPowerPrecise(tileX * 32 + unitType.tileWidth() * 16, tileY * 32 + unitType.tileHeight() * 16, unitType);
        return hasPowerPrecise(tileX * 32, tileY * 32, UnitTypes::None);
    }
    bool Game::hasPower(TilePosition position, UnitType unitType) const
    {
        return hasPower(position.x, position.y, unitType);
    }
    bool Game::hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType) const
    {
        return hasPowerPrecise(tileX * 32 + tileWidth * 16, tileY * 32 + tileHeight * 16, unitType);
    }
    bool Game::hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType) const
    {
        return hasPower(position.x, position.y, tileWidth, tileHeight, unitType);
    }
    const bool bPsiFieldMask[10][16] = {
      { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
      { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
      { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
      { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }
        };
    bool Game::hasPowerPrecise(int x, int y, UnitType unitType) const
    {
        if (unitType >= 0 && unitType < UnitTypes::None && (!unitType.requiresPsi() || !unitType.isBuilding()))
            return true;

        // Loop through all pylons for the current player
        for (Unit i : pylons)
        {
            if (!i->exists() || !i->isCompleted())
                continue;

            Position p = i->getPosition();
            if (abs(p.x - x) >= 256)
                continue;

            if (abs(p.y - y) >= 160)
                continue;

            if (bPsiFieldMask[(y - p.y + 160) / 32][(x - p.x + 256) / 32])
                return true;
        }
        return false;
    }
    bool Game::hasPowerPrecise(Position position, UnitType unitType) const
    {
        return hasPowerPrecise(position.x, position.y, unitType);
    }
    //------------------------------------------ MAP DATA -----------------------------------------------
    bool Game::isWalkable(int x, int y) const
    {
        return isWalkable({ x, y });
    }
    bool Game::isWalkable(BWAPI::WalkPosition position) const
    {
        return isValid(position) ? gameData->map.isWalkable[position.x][position.y]
            : false;
    }
    int Game::getGroundHeight(int tileX, int tileY) const
    {
        if (isValid(TilePosition{ tileX, tileY }))
            return gameData->map.groundHeight[tileX][tileY];
        else
            return -1; //What should we return for invalid tiles?
    }
    int Game::getGroundHeight(TilePosition position) const
    {
        return getGroundHeight(position.x, position.y);
    }
    Unitset Game::getUnitsInRadius(int x, int y, int radius, const UnitFilter &pred) const
    {
        return getUnitsInRectangle(x - radius,
            y - radius,
            x + radius,
            y + radius,
            [&](Unit u) { return u->getDistance(Position(x, y)) <= radius && (!pred.isValid() || pred(u)); });
    }
    Unitset Game::getUnitsInRadius(Position center, int radius, const UnitFilter &pred) const
    {
        return getUnitsInRadius(center.x, center.y, radius, pred);
    }
    Unitset Game::getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight, const UnitFilter &pred) const
    {
        return getUnitsInRectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, pred);
    }
    Unit Game::getClosestUnit(Position center, const UnitFilter &pred, int radius) const
    {
        return getClosestUnitInRectangle(center,
            [&](Unit u) { return u->getDistance(center) <= radius && (!pred.isValid() || pred(u)); },
            center.x - radius,
            center.y - radius,
            center.x + radius,
            center.y + radius);
    }
    //------------------------------------------ REGIONS -----------------------------------------------
    BWAPI::Region Game::getRegionAt(int x, int y) const
    {
        if (!isValid(Position(x, y)))
        {
            this->setLastError(BWAPI::Errors::Invalid_Parameter);
            return nullptr;
        }
        unsigned short rawId = gameData->map.mapTileRegionId[x / 32][y / 32];
        if (rawId & 0x2000)
        {
            const int minitilePosX = (x & 0x1F) / 8;
            const int minitilePosY = (y & 0x1F) / 8;
            const int minitileShift = minitilePosX + minitilePosY * 4;
            const int index = rawId & 0x1FFF;
            if (index >= std::extent<decltype(gameData->map.mapSplitTilesMiniTileMask)>::value)
                return nullptr;

            unsigned short miniTileMask = gameData->map.mapSplitTilesMiniTileMask[index];

            if (index >= std::extent<decltype(gameData->map.mapSplitTilesRegion1)>::value)
                return nullptr;

            if ((miniTileMask >> minitileShift) & 1)
            {
                unsigned short rgn2 = gameData->map.mapSplitTilesRegion2[index];
                return this->getRegion(rgn2);
            }
            else
            {
                unsigned short rgn1 = gameData->map.mapSplitTilesRegion1[index];
                return this->getRegion(rgn1);
            }
        }
        else
        {
          return this->getRegion(rawId);
        }
    }
    BWAPI::Region Game::getRegionAt(BWAPI::Position position) const
    {
        return getRegionAt(position.x, position.y);
    }
    bool Game::hasPath(Position source, Position destination) const
    {
        if (isValid(source) && isValid(destination))
        {
            Region rgnA = getRegionAt(source);
            Region rgnB = getRegionAt(destination);
            if (rgnA && rgnB && rgnA->getRegionGroupID() == rgnB->getRegionGroupID())
                return setLastError();
        }
        return setLastError(Errors::Unreachable_Location);
    }
    //------------------------------------------ DRAW TEXT ----------------------------------------------
    void Game::drawText(CoordinateType::Enum ctype, int x, int y, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vDrawText(ctype, x, y, format, ap);
        va_end(ap);
    }
    void Game::drawTextMap(int x, int y, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vDrawText(BWAPI::CoordinateType::Map, x, y, format, ap);
        va_end(ap);
    }
    void Game::drawTextMouse(int x, int y, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vDrawText(BWAPI::CoordinateType::Mouse, x, y, format, ap);
        va_end(ap);
    }
    void Game::drawTextScreen(int x, int y, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vDrawText(BWAPI::CoordinateType::Screen, x, y, format, ap);
        va_end(ap);
    }
    void Game::drawTextMap(Position p, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vDrawText(BWAPI::CoordinateType::Map, p.x, p.y, format, ap);
        va_end(ap);
    }
    void Game::drawTextMouse(Position p, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vDrawText(BWAPI::CoordinateType::Mouse, p.x, p.y, format, ap);
        va_end(ap);
    }
    void Game::drawTextScreen(Position p, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vDrawText(BWAPI::CoordinateType::Screen, p.x, p.y, format, ap);
        va_end(ap);
    }
    //------------------------------------------ DRAW BOX -----------------------------------------------
    void Game::drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
    {
        if (!gameData->hasGUI) return;

        client.drawShape(ShapeType::Box, ctype, left, top, right, bottom, 0, 0, color, isSolid);
    }
    void Game::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
        drawBox(CoordinateType::Map, left, top, right, bottom, color, isSolid);
    }
    void Game::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
        drawBox(CoordinateType::Mouse, left, top, right, bottom, color, isSolid);
    }
    void Game::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
        drawBox(CoordinateType::Screen, left, top, right, bottom, color, isSolid);
    }
    void Game::drawBoxMap(Position leftTop, Position rightBottom, Color color, bool isSolid)
    {
        drawBoxMap(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y, color, isSolid);
    }
    void Game::drawBoxMouse(Position leftTop, Position rightBottom, Color color, bool isSolid)
    {
        drawBoxMouse(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y, color, isSolid);
    }
    void Game::drawBoxScreen(Position leftTop, Position rightBottom, Color color, bool isSolid)
    {
        drawBoxScreen(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y, color, isSolid);
    }
    //------------------------------------------ DRAW TRIANGLE -----------------------------------------------
    void Game::drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
        if (!gameData->hasGUI) return;

        client.drawShape(ShapeType::Triangle, ctype, ax, ay, bx, by, cx, cy, color, isSolid);
    }
    void Game::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
        drawTriangle(CoordinateType::Map, ax, ay, bx, by, cx, cy, color, isSolid);
    }
    void Game::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
        drawTriangle(CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color, isSolid);
    }
    void Game::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
        drawTriangle(CoordinateType::Screen, ax, ay, bx, by, cx, cy, color, isSolid);
    }
    void Game::drawTriangleMap(Position a, Position b, Position c, Color color, bool isSolid)
    {
        drawTriangleMap(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
    }
    void Game::drawTriangleMouse(Position a, Position b, Position c, Color color, bool isSolid)
    {
        drawTriangleMouse(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
    }
    void Game::drawTriangleScreen(Position a, Position b, Position c, Color color, bool isSolid)
    {
        drawTriangleScreen(a.x, a.y, b.x, b.y, c.x, c.y, color, isSolid);
    }
    //------------------------------------------ DRAW CIRCLE -----------------------------------------------
    void Game::drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid)
    {
      if (!gameData->hasGUI) return;

      client.drawShape(ShapeType::Circle, ctype, x, y, 0, 0, radius, 0, color, isSolid);
    }
    void Game::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
    {
        drawCircle(CoordinateType::Map, x, y, radius, color, isSolid);
    }
    void Game::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
    {
        drawCircle(CoordinateType::Mouse, x, y, radius, color, isSolid);
    }
    void Game::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
    {
        drawCircle(CoordinateType::Screen, x, y, radius, color, isSolid);
    }
    void Game::drawCircleMap(Position p, int radius, Color color, bool isSolid)
    {
        drawCircleMap(p.x, p.y, radius, color, isSolid);
    }
    void Game::drawCircleMouse(Position p, int radius, Color color, bool isSolid)
    {
        drawCircleMouse(p.x, p.y, radius, color, isSolid);
    }
    void Game::drawCircleScreen(Position p, int radius, Color color, bool isSolid)
    {
        drawCircleScreen(p.x, p.y, radius, color, isSolid);
    }
    //------------------------------------------ DRAW ELLIPSE -----------------------------------------------
    void Game::drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
        if (!gameData->hasGUI) return;

        client.drawShape(ShapeType::Ellipse, ctype, x, y, 0, 0, xrad, yrad, color, isSolid);
    }
    void Game::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
        drawEllipse(CoordinateType::Map, x, y, xrad, yrad, color, isSolid);
    }
    void Game::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
        drawEllipse(CoordinateType::Mouse, x, y, xrad, yrad, color, isSolid);
    }
    void Game::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
        drawEllipse(CoordinateType::Screen, x, y, xrad, yrad, color, isSolid);
    }
    void Game::drawEllipseMap(Position p, int xrad, int yrad, Color color, bool isSolid)
    {
        drawEllipseMap(p.x, p.y, xrad, yrad, color, isSolid);
    }
    void Game::drawEllipseMouse(Position p, int xrad, int yrad, Color color, bool isSolid)
    {
        drawEllipseMouse(p.x, p.y, xrad, yrad, color, isSolid);
    }
    void Game::drawEllipseScreen(Position p, int xrad, int yrad, Color color, bool isSolid)
    {
        drawEllipseScreen(p.x, p.y, xrad, yrad, color, isSolid);
    }
    //------------------------------------------ DRAW DOT -----------------------------------------------
    void Game::drawDot(CoordinateType::Enum ctype, int x, int y, Color color)
    {
        if (!gameData->hasGUI) return;

        client.drawShape(ShapeType::Dot, ctype, x, y, 0, 0, 0, 0, color, false);
    }
    void Game::drawDotMap(int x, int y, Color color)
    {
        drawDot(CoordinateType::Map, x, y, color);
    }
    void Game::drawDotMouse(int x, int y, Color color)
    {
        drawDot(CoordinateType::Mouse, x, y, color);
    }
    void Game::drawDotScreen(int x, int y, Color color)
    {
        drawDot(CoordinateType::Screen, x, y, color);
    }
    void Game::drawDotMap(Position p, Color color)
    {
        drawDotMap(p.x, p.y, color);
    }
    void Game::drawDotMouse(Position p, Color color)
    {
        drawDotMouse(p.x, p.y, color);
    }
    void Game::drawDotScreen(Position p, Color color)
    {
        drawDotScreen(p.x, p.y, color);
    }
    //------------------------------------------ DRAW LINE -----------------------------------------------
    void Game::drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color)
    {
      if (!gameData->hasGUI) return;
      
      client.drawShape(ShapeType::Line, ctype, x1, y1, x2, y2, 0, 0, color, false);
    }
    void Game::drawLineMap(int x1, int y1, int x2, int y2, Color color)
    {
        drawLine(CoordinateType::Map, x1, y1, x2, y2, color);
    }
    void Game::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
    {
        drawLine(CoordinateType::Mouse, x1, y1, x2, y2, color);
    }
    void Game::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
    {
        drawLine(CoordinateType::Screen, x1, y1, x2, y2, color);
    }
    void Game::drawLineMap(Position a, Position b, Color color)
    {
        drawLineMap(a.x, a.y, b.x, b.y, color);
    }
    void Game::drawLineMouse(Position a, Position b, Color color)
    {
        drawLineMouse(a.x, a.y, b.x, b.y, color);
    }
    void Game::drawLineScreen(Position a, Position b, Color color)
    {
        drawLineScreen(a.x, a.y, b.x, b.y, color);
    }
    //----------------------------------------------- CREATE UNIT -----------------------------------------------
    void Game::createUnit(Player player, UnitType unitType, int x, int y, int count)
    {
      client.createUnit(player.getID().id, unitType, x, y, count);
    }
    void Game::createUnit(Player player, UnitType unitType, Position position, int count)
    {
      createUnit(player, unitType, position.x, position.y, count);
    }
    //----------------------------------------------- KILL UNITS -----------------------------------------------
    void Game::killUnits(const Unitset& units)
    {
      client.killUnits(units, false);
    }
    void Game::killUnit(Unit unit)
    {
      killUnits(Unitset{ unit });
    }
    //---------------------------------------------- REMOVE UNITS ----------------------------------------------
    void Game::removeUnits(const Unitset& units)
    {
      client.killUnits(units, true);
    }
    void Game::removeUnit(Unit unit)
    {
      removeUnits(Unitset{ unit });
    }
    //----------------------------------------------- GET FORCES -----------------------------------------------
    const Forceset& Game::getForces() const
    {
        return forcesset;
    }
    //----------------------------------------------- GET PLAYERS ----------------------------------------------
    const Playerset& Game::getPlayers() const
    {
        return playerSet;
    }
    //------------------------------------------------- GET UNITS ----------------------------------------------
    const Unitset& Game::getAllUnits() const
    {
        return accessibleUnits;
    }
    //------------------------------------------------- GET MINERALS -------------------------------------------
    const Unitset& Game::getMinerals() const
    {
        return minerals;
    }
    //------------------------------------------------- GET GEYSERS --------------------------------------------
    const Unitset& Game::getGeysers() const
    {
        return geysers;
    }
    //------------------------------------------------- GET NEUTRAL UNITS --------------------------------------
    const Unitset& Game::getNeutralUnits() const
    {
        return neutralUnits;
    }
    //------------------------------------------------- GET STATIC MINERALS ------------------------------------
    const Unitset& Game::getStaticMinerals() const
    {
        return staticMinerals;
    }
    //------------------------------------------------- GET STATIC GEYSERS -------------------------------------
    const Unitset& Game::getStaticGeysers() const
    {
        return staticGeysers;
    }
    //------------------------------------------------- GET STATIC NEUTRAL UNITS -------------------------------
    const Unitset& Game::getStaticNeutralUnits() const
    {
        return staticNeutralUnits;
    }
    //------------------------------------------------ GET BULLETS ---------------------------------------------
    const Bulletset& Game::getBullets() const
    {
        return bulletsset;
    }
    //------------------------------------------------ GET NUKE DOTS -------------------------------------------
    const Position::list& Game::getNukeDots() const
    {
        return nukeDots;
    }
    //------------------------------------------------ GET EVENTS ----------------------------------------------
    const std::list< Event >& Game::getEvents() const
    {
        return events;
    }
    //----------------------------------------------------------------------------------------------------------
    Region Game::getRegion(int regionID) const
    {
        return *regions.find(RegionID(regionID));
    }
    //--------------------------------------------- GET GAME TYPE ----------------------------------------------
    GameType Game::getGameType() const
    {
        return GameType(gameData->gameType);
    }
    //--------------------------------------------- GET FRAME COUNT --------------------------------------------
    int Game::getFrameCount() const
    {
        return gameData->frameCount;
    }
    //------------------------------------------------ GET FPS -------------------------------------------------
    int Game::getFPS() const
    {
        return fpsCounter.getFps();
    }
    //-------------------------------------------- GET Average FPS ---------------------------------------------
    double Game::getAverageFPS() const
    {
      return gameData->averageFPS;
    }
    //-------------------------------------------------- MOUSE -------------------------------------------------
    Position Game::getMousePosition() const
    {
      // TODO: Impelement
      return Positions::Origin;
    }
    bool Game::getMouseState(MouseButton button) const
    {
      // TODO: Impelement
      return false;
    }
    bool Game::getKeyState(Key key) const
    {
      // TODO: Impelement
      return false;
    }
    //------------------------------------------- GET SCREEN POSITION ------------------------------------------
    BWAPI::Position Game::getScreenPosition() const
    {
        return gameData->screenPosition;
    }
    //--------------------------------------------- SET LAST ERROR ---------------------------------------------
    Error lastError;
    bool Game::setLastError(BWAPI::Error e) const
    {
      // TODO: Implement - lastError is global right now, we need to fix it
        // implies that an error has occured
        lastError = e;
        return e == Errors::None;
    }
    //----------------------------------------------- GET LAST ERROR -------------------------------------------
    Error Game::getLastError() const
    {
        // returns the last error encountered in BWAPI
        return lastError;
    }
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int Game::mapWidth() const
    {
        return gameData->map.size.x;
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int Game::mapHeight() const
    {
        return gameData->map.size.y;
    }
    //----------------------------------------------- MAP FILE NAME --------------------------------------------
    std::string Game::mapFileName() const
    {
        return gameData->mapFileName;
    }
    //----------------------------------------------- MAP PATH NAME --------------------------------------------
    std::string Game::mapPathName() const
    {
        return gameData->mapPath;
    }
    //----------------------------------------------- MAP NAME -------------------------------------------------
    std::string Game::mapName() const
    {
        return gameData->mapName;
    }
    //----------------------------------------------- LATENCY FRAMES -------------------------------------------
    int Game::getLatencyFrames() const
    {
        return gameData->latencyFrames;
    }
    //----------------------------------------------- LATENCY TIME ---------------------------------------------
    int Game::getLatencyTime() const
    {
        return gameData->latencyTime;
    }
    //----------------------------------------------- LATENCY FRAMES REMAINING ---------------------------------
    int Game::getRemainingLatencyFrames() const
    {
        return gameData->remainingLatencyFrames;
    }
    //----------------------------------------------- LATENCY TIME REMAINING -----------------------------------
    int Game::getRemainingLatencyTime() const
    {
        return gameData->remainingLatencyTime;
    }
    //----------------------------------------------- VERSION --------------------------------------------------
    int Game::getClientVersion() const
    {
        return gameData->apiVersion;
    }
    //----------------------------------------------- DEBUG ----------------------------------------------------
    bool Game::isDebug() const
    {
        #ifdef _DEBUG
        return true;
        #else
        return false;
        #endif
    }
    //----------------------------------------------- LATCOM ENABLED -------------------------------------------
    bool Game::isLatComEnabled() const
    {
      // TODO: Implement
        //return gameData->???
      return true;
    }
    //----------------------------------------------- GUI ENABLED ----------------------------------------------
    bool Game::isGUIEnabled() const
    {
        return gameData->hasGUI;
    }
    void Game::setGUI(bool enabled)
    {
      // TODO: Implement
    }
    //----------------------------------------------- INSTANCE NUMBER ------------------------------------------
    int Game::getInstanceNumber() const
    {
      // TODO: Implement - or not? Maybe remove?
        //return gameData->in
        return 0;
    }
    //----------------------------------------------- ENEMY ----------------------------------------------------
    Player Game::enemy() const
    {
      if (!self()) return nullptr;

      for (auto p : players)
      {
        if (self().isEnemy(p))
          return p;
      }
      return nullptr;
    }
    //----------------------------------------------- NEUTRAL --------------------------------------------------
    Player Game::neutral() const
    {
      // The server will always set player 12 as neutral and others as not.
      for (auto p : players)
      {
        if (p.isNeutral())
          return p;
      }
      return nullptr;
    }
    //----------------------------------------------- ALLIES ---------------------------------------------------
    Playerset& Game::allies()
    {
        return _allies;
    }
    //----------------------------------------------- ENEMIES --------------------------------------------------
    Playerset& Game::enemies()
    {
        return _enemies;
    }
    //----------------------------------------------- OBSERVERS ------------------------------------------------
    Playerset& Game::observers()
    {
        return _observers;
    }
    //----------------------------------------------- APM ------------------------------------------------------
    int Game::getAPM(bool includeSelects) const
    {
        return apmCounter.apm(includeSelects);
    }
    //----------------------------------------------- MAP ------------------------------------------------------
    bool Game::setMap(const std::string &mapFileName)
    {
      // TODO: Implement
      return false;
    }
    //--------------------------------------------- FRAME SKIP -------------------------------------------------
    void Game::setFrameSkip(int frameSkip)
    {
      //TODO: Implement
    }
    //----------------------------------------------- ELAPSED TIME ---------------------------------------------
    int Game::elapsedTime() const
    {
        return gameData->elapsedTime;
    }
    //----------------------------------------------- COUNTDOWN TIMER ------------------------------------------
    int Game::countdownTimer() const
    {
        return gameData->countdownTimer;
    }
    //----------------------------------------------- GET ALL REGIONS ------------------------------------------
    const Regionset& Game::getAllRegions() const
    {
        return regionsList;
    }
    //----------------------------------------------- LAST EVENT TIME ------------------------------------------
    int Game::getLastEventTime() const
    {
        return gameData->lastEventTime;
    }
    //------------------------------------------- REPLAY REVEAL ALL --------------------------------------------
    bool Game::setRevealAll(bool reveal)
    {
      // TODO: Implement
      return false;
    }
    //----------------------------------------------- RANDOM SEED-----------------------------------------------
    unsigned Game::getRandomSeed() const
    {
        return std::stoi(gameData->randomSeed);
    }
    //----------------------------------------------- MAP HASH -------------------------------------------------
    std::string Game::mapHash() const
    {
        return gameData->map.mapHash;
    }
    //----------------------------------------------- CAN RESEARCH ---------------------------------------------
    bool Game::canResearch(TechType type, Unit unit, bool checkCanIssueCommandType) const
    {
        auto player = getPlayer(gameData->player);
        // Error checking
        if (!player)
            return setLastError(Errors::Unit_Not_Owned);

        if (unit)
        {
            if (unit->getPlayer() != player)
                return setLastError(Errors::Unit_Not_Owned);

            if (!unit->getType().isSuccessorOf(type.whatResearches()))
                return setLastError(Errors::Incompatible_UnitType);

            if (checkCanIssueCommandType && (unit->isLifted() || !unit->isIdle() || !unit->isCompleted()))
                return setLastError(Errors::Unit_Busy);
        }
        if (player->isResearching(type))
            return setLastError(Errors::Currently_Researching);

        if (player->hasResearched(type))
            return setLastError(Errors::Already_Researched);

        if (!player->isResearchAvailable(type))
            return setLastError(Errors::Access_Denied);

        if (player->minerals() < type.mineralPrice())
            return setLastError(Errors::Insufficient_Minerals);

        if (player->gas() < type.gasPrice())
            return setLastError(Errors::Insufficient_Gas);

        if (!player->hasUnitTypeRequirement(type.requiredUnit()))
            return setLastError(Errors::Insufficient_Tech);

        return setLastError();
    }
    //----------------------------------------------- CAN UPGRADE ----------------------------------------------
    bool Game::canUpgrade(UpgradeType type, Unit unit, bool checkCanIssueCommandType) const
    {
        Player player = getPlayer(gameData->player);
        if (!player)
            return setLastError(Errors::Unit_Not_Owned);

        if (unit)
        {
            if (unit->getPlayer() != player)
                return setLastError(Errors::Unit_Not_Owned);

            if (!unit->getType().isSuccessorOf(type.whatUpgrades()))
                return setLastError(Errors::Incompatible_UnitType);

            if (checkCanIssueCommandType && (unit->isLifted() || !unit->isIdle() || !unit->isCompleted()))
                return setLastError(Errors::Unit_Busy);
        }
        int nextLvl = player->getUpgradeLevel(type) + 1;

        if (!player->hasUnitTypeRequirement(type.whatUpgrades()))
            return setLastError(Errors::Unit_Does_Not_Exist);

        if (!player->hasUnitTypeRequirement(type.whatsRequired(nextLvl)))
            return setLastError(Errors::Insufficient_Tech);

        if (player->isUpgrading(type))
            return setLastError(Errors::Currently_Upgrading);

        if (player->getUpgradeLevel(type) >= player->getMaxUpgradeLevel(type))
            return setLastError(Errors::Fully_Upgraded);

        if (player->minerals() < type.mineralPrice(nextLvl))
            return setLastError(Errors::Insufficient_Minerals);

        if (player->gas() < type.gasPrice(nextLvl))
            return setLastError(Errors::Insufficient_Gas);

        return setLastError();
    }
    //----------------------------------------------- START LOCATIONS ------------------------------------------
    const TilePosition::list& Game::getStartLocations() const
    {
        return gameData->startPositions;
    }
    //----------------------------------------------- CAN BUILD HERE -------------------------------------------
    bool Game::canBuildHere(TilePosition position, UnitType type, Unit builder, bool checkExplored) const
    {
        setLastError(Errors::Unbuildable_Location);

        if (builder && type.isAddon())
        {
            position += TilePosition(4, 1); // addon build offset
        }

        // lt = left top, rb = right bottom
        TilePosition lt = position;
        TilePosition rb = lt + type.tileSize();

        // Map limit check
        if (!isValid(lt) || !isValid(Position(rb) - Position(1, 1)))
            return false;

        //if the unit is a refinery, we just need to check the set of geysers to see if the position
        //matches one of them (and the type is still vespene geyser)
        if (type.isRefinery())
        {
            for (Unit g : getGeysers())
            {
                if (g->getTilePosition() == position)
                {
                    if (g->isVisible() && g->getType() != UnitTypes::Resource_Vespene_Geyser)
                        return false;
                    return setLastError();
                }
            }
            return false;
        }

        // Tile buildability check
        for (int x = lt.x; x < rb.x; ++x)
        {
            for (int y = lt.y; y < rb.y; ++y)
            {
                // Check if tile is buildable/unoccupied and explored.
                if (!isBuildable(x, y) || (checkExplored && !isExplored(x, y)))
                    return false; // @TODO: Error code for !isExplored ??
            }
        }

        // Check if builder is capable of reaching the building site
        if (builder)
        {
            if (!builder->getType().isBuilding())
            {
                if (!builder->hasPath(Position(lt) + Position(type.tileSize()) / 2))
                    return false;
            }
            else if (!builder->getType().isFlyingBuilding() && type != UnitTypes::Zerg_Nydus_Canal && !type.isFlagBeacon())
            {
                return false;
            }
        }

        // Ground unit dimension check
        if (type != UnitTypes::Special_Start_Location)
        {
            Position targPos = Position(lt) + Position(type.tileSize()) / 2;
            Unitset unitsInRect(getUnitsInRectangle(Position(lt), Position(rb), !IsFlying &&
                !IsLoaded   &&
                [&builder, &type](Unit u) { return u != builder || type == UnitTypes::Zerg_Nydus_Canal; } &&
                GetLeft <= targPos.x + type.dimensionRight() &&
                GetTop <= targPos.y + type.dimensionDown() &&
                GetRight >= targPos.x - type.dimensionLeft() &&
                GetBottom >= targPos.y - type.dimensionUp()));
            for (Unit u : unitsInRect)
            {
                BWAPI::UnitType iterType = u->getType();
                // Addons can be placed over units that can move, pushing them out of the way
                if (!(type.isAddon() && iterType.canMove()))
                    return false;
            }

            // Creep Check
            // Note: Zerg structures that don't require creep can still be placed on creep
            bool needsCreep = type.requiresCreep();
            if (type.getRace() != Races::Zerg || needsCreep)
            {
                for (int x = lt.x; x < rb.x; ++x)
                {
                    for (int y = lt.y; y < rb.y; ++y)
                    {
                        if (needsCreep != hasCreep(x, y))
                            return false;
                    }
                }
            }

            // Power Check
            if (type.requiresPsi() && !hasPower(lt, type))
                return false;

        } //don't ignore units

        // Resource Check (CC, Nex, Hatch)
        if (type.isResourceDepot())
        {
            for (BWAPI::Unit m : getStaticMinerals())
            {
                TilePosition tp = m->getInitialTilePosition();
                if ((isVisible(tp) || isVisible(tp.x + 1, tp.y)) && !m->exists())
                    continue; // tile position is visible, but mineral is not => mineral does not exist
                if (tp.x > lt.x - 5 &&
                    tp.y > lt.y - 4 &&
                    tp.x < lt.x + 7 &&
                    tp.y < lt.y + 6)
                    return false;
            }
            for (BWAPI::Unit g : getStaticGeysers())
            {
                TilePosition tp = g->getInitialTilePosition();
                if (tp.x > lt.x - 7 &&
                    tp.y > lt.y - 5 &&
                    tp.x < lt.x + 7 &&
                    tp.y < lt.y + 6)
                    return false;
            }
        }

        // A building can build an addon at a different location (i.e. automatically lifts (if not already lifted)
        // then lands at the new location before building the addon), so we need to do similar checks for the
        // location that the building will be when it builds the addon.
        if (builder && !builder->getType().isAddon() && type.isAddon())
        {
            if (!canBuildHere(lt - TilePosition(4, 1), builder->getType(), builder, checkExplored))
                return false;
        }

        //if the build site passes all these tests, return true.
        return setLastError();
    }
    //----------------------------------------------- CAN MAKE -------------------------------------------
    bool Game::canMake(UnitType type, Unit builder) const
    {
        Player player = getPlayer(gameData->player);
        // Error checking
        setLastError();
        if (!player)
            return setLastError(Errors::Unit_Not_Owned);

        // Check if the unit type is available (UMS game)
        if (!player->isUnitAvailable(type))
            return setLastError(Errors::Access_Denied);

        // Get the required UnitType
        BWAPI::UnitType requiredType = type.whatBuilds().first;

        Player pSelf = player;
        if (builder != nullptr) // do checks if a builder is provided
        {
            // Check if the owner of the unit is you
            if (builder->getPlayer() != pSelf)
                return setLastError(Errors::Unit_Not_Owned);

            BWAPI::UnitType builderType = builder->getType();
            if (type == UnitTypes::Zerg_Nydus_Canal && builderType == UnitTypes::Zerg_Nydus_Canal)
            {
                if (!builder->isCompleted())
                    return setLastError(Errors::Unit_Busy);

                if (builder->getNydusExit())
                    return setLastError(Errors::Unknown);

                return true;
            }

            // Check if this unit can actually build the unit type
            if (requiredType == UnitTypes::Zerg_Larva && builderType.producesLarva())
            {
                if (builder->getLarva().size() == 0)
                    return setLastError(Errors::Unit_Does_Not_Exist);
            }
            else if (builderType != requiredType)
            {
                return setLastError(Errors::Incompatible_UnitType);
            }

            // Carrier/Reaver space checking
            int max_amt;
            switch (builderType)
            {
            case UnitTypes::Enum::Protoss_Carrier:
            case UnitTypes::Enum::Hero_Gantrithor:
                // Get max interceptors
                max_amt = 4;
                if (pSelf->getUpgradeLevel(UpgradeTypes::Carrier_Capacity) > 0 || builderType == UnitTypes::Hero_Gantrithor)
                    max_amt += 4;

                // Check if there is room
                if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt)
                    return setLastError(Errors::Insufficient_Space);
                break;
            case UnitTypes::Enum::Protoss_Reaver:
            case UnitTypes::Enum::Hero_Warbringer:
                // Get max scarabs
                max_amt = 5;
                if (pSelf->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0 || builderType == UnitTypes::Hero_Warbringer)
                    max_amt += 5;

                // check if there is room
                if (builder->getScarabCount() + static_cast<int>(builder->getTrainingQueue().size()) >= max_amt)
                    return setLastError(Errors::Insufficient_Space);
                break;
            }
        } // if builder != nullptr

        // Check if player has enough minerals
        if (pSelf->minerals() < type.mineralPrice())
            return setLastError(Errors::Insufficient_Minerals);

        // Check if player has enough gas
        if (pSelf->gas() < type.gasPrice())
            return setLastError(Errors::Insufficient_Gas);

        // Check if player has enough supplies
        BWAPI::Race typeRace = type.getRace();
        const int supplyRequired = type.supplyRequired() * (type.isTwoUnitsInOneEgg() ? 2 : 1);
        if (supplyRequired > 0 && pSelf->supplyTotal(typeRace) < pSelf->supplyUsed(typeRace) + supplyRequired - (requiredType.getRace() == typeRace ? requiredType.supplyRequired() : 0))
            return setLastError(Errors::Insufficient_Supply);

        UnitType addon = UnitTypes::None;
        for (auto &it : type.requiredUnits())
        {
            if (it.first.isAddon())
                addon = it.first;

            if (!pSelf->hasUnitTypeRequirement(it.first, it.second))
                return setLastError(Errors::Insufficient_Tech);
        }

        if (type.requiredTech() != TechTypes::None && !pSelf->hasResearched(type.requiredTech()))
            return setLastError(Errors::Insufficient_Tech);

        if (builder &&
            addon != UnitTypes::None &&
            addon.whatBuilds().first == type.whatBuilds().first &&
            (!builder->getAddon() || builder->getAddon()->getType() != addon))
            return setLastError(Errors::Insufficient_Tech);

        return true;
    }

    //--------------------------------------------- SEND TEXT EX -----------------------------------------------
    void Game::vSendTextEx(bool toAllies, const char *format, va_list arg)
    {
        char buffer[256];
        VSNPrintf(buffer, format, arg);
        client.sendText(buffer, toAllies);
    }

    //------------------------------------------------ PRINTF --------------------------------------------------
    void Game::vPrintf(const char *format, va_list arg)
    {
        char buffer[256];
        VSNPrintf(buffer, format, arg);
        client.printText(buffer);
    }

    //----------------------------------------------- GET UNITS IN RECTANGLE -----------------------------------
    Unitset Game::getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred) const
    {
      Unitset result = unitFinder.find(left, top, right, bottom);
      if (pred.isValid())
        result.erase_if(!pred);
      return result;
    }

    Unit Game::getClosestUnitInRectangle(Position center, const UnitFilter &pred, int left, int top, int right, int bottom) const
    {
      Unitset units = getUnitsInRectangle(left, top, right, bottom, pred);
      auto minCompare = [&](Unit closest, Unit u) { return u->getDistance(center) < closest->getDistance(center); };
      auto result = std::min_element(std::begin(units), std::end(units), minCompare);
      return result == units.end() ? nullptr : *result;
    }

    Unit Game::getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center, int radius) const
    {
      Position rad(radius, radius);

      Position topLeft = makeValid(center - rad);
      Position botRight = makeValid(center + rad);

      Unitset units = getUnitsInRectangle(topLeft.x, topLeft.y, botRight.x, botRight.y, pred);
      
      if (units.empty()) return nullptr;
      
      auto iterator = units.begin();
      Unit init = *iterator;
      ++iterator;
      return std::accumulate(std::begin(units), std::end(units), init, best);
    }
    //-------------------------------------------------- DRAW TEXT ---------------------------------------------
    void Game::vDrawText(CoordinateType::Enum ctype, int x, int y, const char *format, va_list arg)
    {
        if (!gameData->hasGUI) return;
        char buffer[2048];
        VSNPrintf(buffer, format, arg);

        client.drawText(ctype, buffer, x, y, textSize);
    }
    //----------------------------------------------- PAUSE GAME -----------------------------------------------
    void Game::pauseGame()
    {
      client.pauseGame();
    }
    //---------------------------------------------- RESUME GAME -----------------------------------------------
    void Game::resumeGame()
    {
      client.resumeGame();
    }
    //---------------------------------------------- LEAVE GAME ------------------------------------------------
    void Game::leaveGame()
    {
      client.leaveGame();
    }
    //--------------------------------------------- RESTART GAME -----------------------------------------------
    bool Game::restartGame()
    {
      if (gameData->isMultiplayer || !gameData->isInGame)
        return false;

      client.restartGame();
      return true;
    }
    //---------------------------------------------- SET GAME SPEED --------------------------------------------
    void Game::setLocalSpeed(int speed)
    {
      client.setLocalSpeed(speed);
    }
    //------------------------------------------- ISSUE COMMAND ------------------------------------------------
    bool Game::issueCommand(const Unitset& units, UnitCommand command)
    {
      if (units.empty()) return false;

      if (units.size() == 1) {
        command.unit = units.begin()->getID();
        if (commandOptimizer.add(command))
        {
          return true;
        }
      }

      client.issueCommand(units, command);
      return true;
    }
    //------------------------------------------ GET SELECTED UNITS --------------------------------------------
    const Unitset& Game::getSelectedUnits() const
    {
        lastError = Errors::None;
        return selectedUnits;
    }
    //---------------------------------------------- SET TEXT SIZE ---------------------------------------------
    void Game::setTextSize(Text::Size::Enum size)
    {
        if (size < Text::Size::Small) size = Text::Size::Small;
        if (size > Text::Size::Huge) size = Text::Size::Huge;
        textSize = size;
    }
    //--------------------------------------------- SET ALLIANCE -----------------------------------------------
    bool Game::setAlliance(BWAPI::Player player, bool allied, bool alliedVictory)
    {
        /* Set the current player's alliance status */
        if (!self() || isReplay() || !player || player == self())
          return setLastError(Errors::Invalid_Parameter);
        
        client.setAlliance(player.getID().id, allied ? (alliedVictory ? 2 : 1) : 0);
        return setLastError();
    }
    //----------------------------------------------- SET VISION -----------------------------------------------
    bool Game::setVision(BWAPI::Player player, bool enabled)
    {
        // Param check
        if (!player)
            return setLastError(Errors::Invalid_Parameter);

        if (!isReplay() && (!self() || player == self()))
            return setLastError(Errors::Invalid_Parameter);

        client.setVision(player.getID().id, enabled);
        return setLastError();
    }
    //----------------------------------------------- SET COMMAND OPTIMIZATION LEVEL ---------------------------
    void Game::setCommandOptimizationLevel(int level)
    {
      commandOptimizer.level = level;
    }
    namespace
    {
      Unitset const emptyUnitset;
    }
    Unitset const &Game::getUnits(Player player) const
    {
      if (auto it = playerUnits.find(player); it != playerUnits.end()) {
        return it->second;
      }
      else {
        return emptyUnitset;
      }
    }
    void Game::addUnit(const UnitData& unitdata)
    {
      auto itr = units.find(unitdata.id);
      if (itr == units.end())
      {
        units.emplace(unitdata);
      }
    }
    void Game::updatePlayer(const PlayerData& playerdata)
    {
      auto itr = players.find(playerdata.id);
      if (itr == players.end())
      {
        Player player = *players.emplace(playerdata).first;
        playerUnits[player]; // creates Unitset for the player
      }
    }
    void Game::addForce(const ForceData& forceData)
    {
      auto itr = forces.find(forceData.id);
      if (itr == forces.end())
        forces.emplace(forceData);
    }
    void Game::addRegion(const RegionData& regionData)
    {
      auto itr = regions.find(regionData.id);
      if (itr == regions.end())
        regions.emplace(regionData);
    }
    void Game::addBullet(const BulletData& bulletData)
    {
      auto itr = bullets.find(bulletData.id);
      if (itr == bullets.end())
        bullets.emplace(bulletData);
    }
    void Game::computePrimaryUnitSets()
    {
      //this frame computes the set of accessible units.
      accessibleUnits.clear();
      pylons.clear();
      minerals.clear();
      geysers.clear();
      neutralUnits.clear();
      for (auto itr = playerUnits.begin(); itr != playerUnits.end(); itr++)
        itr->second.clear();
      bulletsset.clear();
      selectedUnits.clear();
      //computes sets
      for (Unit u : units)
      {
        if (u->exists())
        {
          accessibleUnits.insert(u);
          auto player = u->getPlayer();
          if (u->getType() == UnitTypes::Protoss_Pylon && player == self())
            pylons.insert(u);
          playerUnits[player].emplace(u);
          if (u->getType().isMineralField())
            minerals.emplace(u);
          else if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
            geysers.emplace(u);
          if (u->isSelected())
            selectedUnits.insert(u);
          if (player->isNeutral())
            neutralUnits.insert(u);
        }
      }
      if (this->staticNeutralUnits.empty()) //if we haven't saved the set of static units, save them now
      {
        for (Unit u : units)
        {
          Unit ui = u;
          if (ui->getPlayer()->isNeutral())
          {
            this->staticNeutralUnits.insert(ui);
            if (ui->getType().isMineralField())
              this->staticMinerals.insert(ui);
            else if (ui->getType() == UnitTypes::Resource_Vespene_Geyser)
              this->staticGeysers.insert(ui);
          }
        }
      }
      for (auto b : bullets)
      {
        if (b.exists())
          bulletsset.emplace(b);
      }
      nukeDots.clear();
      for (auto &nd : gameData->nukeDots)
        nukeDots.emplace_back(Position{ nd.x, nd.y });
      if (this->playerSet.empty()) //if we haven't setup the playerset yet, do so now
      {
        for (Player p : players)
        {
          playerSet.insert(p);
        }
      }
      if (this->forcesset.empty()) //if we haven't setup the forcesset yet, do so now
      {
        for (Force f : forces)
          forcesset.insert(f);
      }
    }
    void Game::update()
    {
      if (gameData->isInGame)
      {
        if (regionsList.empty())
        {
          for (auto r : regions)
            regionsList.emplace(r);
        }
        if (!_allies.size())
        {
          for (auto p : players)
          {
            if (self().isAlly(p))
              _allies.insert(p);
            else if (self().isEnemy(p))
              _enemies.insert(p);
            else if (p.isObserver())
              _observers.insert(p);
          }
        }
        computePrimaryUnitSets();
        unitFinder.update(getAllUnits());
      }
      else
      {
        accessibleUnits.clear();
        pylons.clear();
        minerals.clear();
        geysers.clear();
        regionsList.clear();
        bulletsset.clear();
        selectedUnits.clear();
        staticMinerals.clear();
        staticGeysers.clear();
        staticNeutralUnits.clear();
        neutralUnits.clear();
        _allies.clear();
        _enemies.clear();
        _observers.clear();
        playerSet.clear();
        forcesset.clear();
        nukeDots.clear();
        events.clear();
        //players.clear();
        units.clear();
        initialUnits.clear();
        regions.clear();
        bullets.clear();
        forces.clear();
        playerUnits.clear();
        gameData = std::make_unique<GameData>();
      }
    }
    void Game::clearEvents()
    {
      events.clear();
    }
    void Game::flushCommandOptimizer()
    {
      commandOptimizer.flush();
    }

    Game& Game::operator <<(ostream_manipulator fn)
    {
      // Pass manipulator into the stream
      ss << fn;

      // Flush to Broodwar's printf if we see endl or ends
      if (fn == &std::endl<char, std::char_traits<char>> || fn == &std::ends<char, std::char_traits<char>>)
      {
        this->flush();
      }
      return *this;
    }
}

