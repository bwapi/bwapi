#pragma once
#include <cstdarg>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>

#include <BWAPI/Error.h>

#include <BWAPI/Filters.h>
#include <BWAPI/UnaryFilter.h>
#include <BWAPI/Input.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Position.h>
#include <BWAPI/IDs.h>

#include <BWAPI/Client/GameData.h>
#include <BWAPI/Client/UnitData.h>
#include <BWAPI/Client/PlayerData.h>
#include <BWAPI/Client/RegionData.h>
#include <BWAPI/Client/ForceData.h>
#include <BWAPI/Client/BulletData.h>

#include <BWAPI/Unit.h>
#include <BWAPI/Player.h>
#include <BWAPI/Region.h>
#include <BWAPI/Force.h>
#include <BWAPI/Bullet.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Forceset.h>
#include <BWAPI/Playerset.h>
#include <BWAPI/Bulletset.h>
#include <BWAPI/Regionset.h>
#include <BWAPI/Event.h>
#include <BWAPI/CommandOptimizer.h>
#include <BWAPI/UnitFinder.h>

#include <BWAPI/FPSCounter.h>
#include <BWAPI/APMCounter.h>

namespace BWAPI
{
  // Forward Declarations
  class Client;
  class Color;
  class GameType;
  class TechType;
  class UnitCommand;
  class UpgradeType;

  /// <summary>The abstract Game class is implemented by BWAPI and is the primary means of obtaining all
  /// game state information from Starcraft Broodwar.</summary> Game state information includes all units,
  /// resources, players, forces, bullets, terrain, fog of war, regions, etc.
  ///
  /// @ingroup Interface
  class Game {
  public:
    Game &operator =(Game const &other) = delete;
    Game &operator =(Game &&other) = delete;

    Game(Client& newClient);

    UnitData const *getUnitData(UnitID unit) const
    {
      if (auto const it = units.find(unit); it != units.end())
        return &it->getData();
      else return nullptr;
    }

    UnitData const *getInitialData(UnitID unit) const
    {
      if (auto const it = initialUnits.find(unit); it != units.end())
        return &it->getData();
      else return nullptr;
    }

    PlayerData const *getPlayerData(PlayerID player) const
    {
      if (auto const it = players.find(player); it != players.end())
        return &it->getData();
      else return nullptr;
    }

    RegionData const *getRegionData(RegionID region) const
    {
      if (auto const it = regions.find(region); it != regions.end())
        return &it->getData();
      else return nullptr;
    }

    ForceData const *getForceData(ForceID force) const
    {
      if (auto const it = forces.find(force); it != forces.end())
        return &it->getData();
      else return nullptr;
    }

    BulletData const *getBulletData(BulletID bullet) const
    {
      if (auto const it = bullets.find(bullet); it != bullets.end())
        return &it->getData();
      else return nullptr;
    }

    /// <summary>Initializes the members of GameData.</summary> Prepares the Game object for
    /// the start of a new game.
    void initGameData();

    /// <summary>Retrieves the set of all teams/forces.</summary> Forces are commonly seen in @UMS
    /// game types and some others such as @TvB and the team versions of game types.
    ///
    /// @returns Forceset containing all forces in the game.
    const Forceset& getForces() const;
    

    /// <summary>Retrieves the set of all players in the match.</summary> This includes the neutral
    /// player, which owns all the resources and critters by default.
    ///
    /// @returns Playerset containing all players in the game.
    const Playerset& getPlayers() const;

    /// <summary>Retrieves the set of all accessible units.</summary> If
    /// Flag::CompleteMapInformation is enabled, then the set also includes units that are not
    /// visible to the player.
    ///
    /// @note Units that are inside refineries are not included in this set.
    ///
    /// @returns Unitset containing all known units in the game.
    const Unitset& getAllUnits() const;

    /// <summary>Retrieves the set of all accessible @minerals in the game.</summary>
    ///
    /// @returns Unitset containing @minerals
    const Unitset& getMinerals() const;

    /// <summary>Retrieves the set of all accessible @geysers in the game.</summary>
    ///
    /// @returns Unitset containing @geysers
    const Unitset& getGeysers() const;

    /// <summary>Retrieves the set of all accessible neutral units in the game.</summary> This
    /// includes @minerals, @geysers, and @critters.
    ///
    /// @returns Unitset containing all neutral units.
    const Unitset& getNeutralUnits() const;

    /// <summary>Retrieves the set of all @minerals that were available at the beginning of the
    /// game.</summary>
    ///
    /// @note This set includes resources that have been mined out or are inaccessible.
    ///
    /// @returns Unitset containing static @minerals
    const Unitset& getStaticMinerals() const;

    /// <summary>Retrieves the set of all @geysers that were available at the beginning of the
    /// game.</summary>
    ///
    /// @note This set includes resources that are inaccessible.
    ///
    /// @returns Unitset containing static @geysers
    const Unitset& getStaticGeysers() const;

    /// <summary>Retrieves the set of all units owned by the neutral player (resources, critters,
    /// etc.) that were available at the beginning of the game.</summary>
    ///
    /// @note This set includes units that are inaccessible.
    ///
    /// @returns Unitset containing static neutral units
    const Unitset& getStaticNeutralUnits() const;

    /// <summary>Retrieves the set of all accessible bullets.</summary>
    ///
    /// @returns Bulletset containing all accessible Bullet objects.
    const Bulletset& getBullets() const;

    /// <summary>Retrieves the set of all accessible @Nuke dots.</summary>
    ///
    /// @note Nuke dots are the red dots painted by a @Ghost when using the nuclear strike ability.
    ///
    /// @returns Set of Positions giving the coordinates of nuke locations.
    const Position::list& getNukeDots() const;

    /// <summary>Retrieves the list of all unhandled game events.</summary>
    ///
    /// @returns std::list containing Event objects.
    const std::list< Event >& getEvents() const;

    const void addEvent(const Event& e) { events.push_back(e); }

    /// <summary>Retrieves the Force interface object associated with a given identifier.</summary>
    ///
    /// <param name="forceID">
    ///   The identifier for the Force object.
    /// </param>
    ///
    /// @returns Force interface object mapped to the given \p forceID.
    /// @retval nullptr if the given identifier is invalid.
    Force getForce(ForceID forceID) const
    {
      if (auto const fp = getForceData(forceID); fp)
        return *fp;
      else return nullptr;
    }

    /// <summary>Retrieves the Player interface object associated with a given identifier.</summary>
    ///
    /// <param name="playerID">
    ///   The identifier for the Player object.
    /// </param>
    ///
    /// @returns Player interface object mapped to the given \p playerID.
    /// @retval nullptr if the given identifier is invalid.
    Player getPlayer(PlayerID playerID) const
    {
      if (auto const pp = getPlayerData(playerID); pp)
        return *pp;
      else return nullptr;
    };

    /// <summary>Retrieves the Unit interface object associated with a given identifier.</summary>
    ///
    /// <param name="unitID">
    ///   The identifier for the Unit object.
    /// </param>
    ///
    /// @returns Unit interface object mapped to the given \p unitID.
    /// @retval nullptr if the given identifier is invalid.
    Unit getUnit(UnitID unitID) const
    {
      if (auto const up = getUnitData(unitID); up)
        return *up;
      else return nullptr;
    }

    /// <summary>Retrieves the Region interface object associated with a given identifier.</summary>
    ///
    /// <param name="regionID">
    ///   The identifier for the Region object.
    /// </param>
    ///
    /// @returns Region interface object mapped to the given \p regionID.
    /// @retval nullptr if the given ID is invalid.
    Region getRegion(int regionID) const;

    /// <summary>Retrieves the GameType of the current game.</summary>
    ///
    /// @returns GameType indicating the rules of the match.
    /// @see GameType
    GameType getGameType() const;

    /// <summary>Retrieves the number of logical frames since the beginning of the match.</summary>
    /// If the game is paused, then getFrameCount will not increase.
    ///
    /// @returns Number of logical frames that have elapsed since the game started as an integer.
    int getFrameCount() const;

    /// <summary>Retrieves the logical frame rate of the game in frames per second (FPS).</summary>
    ///
    /// Example:
    /// @code{.cpp}
    ///   game.setLocalSpeed(0);
    ///   
    ///   // Log and display the best logical FPS seen in the game
    ///   static int bestFPS = 0;
    ///   bestFPS = std::max(bestFPS, game.getFPS());
    ///   game.drawTextScreen(BWAPI::Positions::Origin, "%cBest: %d GFPS\nCurrent: %d GFPS", BWAPI::Text::White, bestFPS, BWAPI::Broodwar->getFPS());
    /// @endcode
    /// @returns Logical frames per second that the game is currently running at as an integer.
    /// @see getAverageFPS
    int getFPS() const;

    /// <summary>Retrieves the average logical frame rate of the game in frames per second (FPS).</summary>
    ///
    /// @returns Average logical frames per second that the game is currently running at as a
    /// double.
    /// @see getFPS
    double getAverageFPS() const;

    /// <summary>Retrieves the position of the user's mouse on the screen, in Position coordinates.</summary>
    ///
    /// @returns Position indicating the location of the mouse.
    /// @retval Positions::Unknown if Flag::UserInput is disabled.
    Position getMousePosition() const;

    /// <summary>Retrieves the state of the given mouse button.</summary>
    ///
    /// <param name="button">
    ///   A MouseButton enum member indicating which button on the mouse to check.
    /// </param>
    ///
    /// @return A bool indicating the state of the given \p button. true if the button was pressed
    /// and false if it was not.
    /// @retval false always if Flag::UserInput is disabled.
    ///
    /// @see MouseButton
    bool getMouseState(MouseButton button) const;

    /// <summary>Retrieves the state of the given keyboard key.</summary>
    ///
    /// <param name="key">
    ///   A Key enum member indicating which key on the keyboard to check.
    /// </param>
    ///
    /// @return A bool indicating the state of the given \p key. true if the key was pressed
    /// and false if it was not.
    /// @retval false always if Flag::UserInput is disabled.
    ///
    /// @see Key
    bool getKeyState(Key key) const;

    /// <summary>Retrieves the top left position of the viewport from the top left corner of the
    /// map, in pixels.</summary>
    ///
    /// @returns Position containing the coordinates of the top left corner of the game's viewport.
    /// @retval Positions::Unknown always if Flag::UserInput is disabled.
    /// @see setScreenPosition
    BWAPI::Position getScreenPosition() const;

    /// <summary>Moves the top left corner of the viewport to the provided position relative to
    /// the map's origin (top left (0,0)).</summary>
    ///
    /// <param name="x">
    ///   The x coordinate to move the screen to, in pixels.
    /// </param>
    /// <param name="y">
    ///   The y coordinate to move the screen to, in pixels.
    /// </param>
    /// @see getScreenPosition
    void setScreenPosition(int x, int y);
    /// @overload
    void setScreenPosition(BWAPI::Position p);

    /// <summary>Pings the minimap at the given position.</summary> Minimap pings are visible to
    /// allied players.
    ///
    /// <param name="x">
    ///   The x coordinate to ping at, in pixels, from the map's origin (left).
    /// </param>
    /// <param name="y">
    ///   The y coordinate to ping at, in pixels, from the map's origin (top).
    /// </param>
    void pingMinimap(int x, int y);
    /// @overload
    void pingMinimap(BWAPI::Position p);

    /// <summary>Checks if the state of the given flag is enabled or not.</summary>
    ///
    /// @note Flags may only be enabled at the start of the match during the AIModule::onStart
    /// callback.
    ///
    /// <param name="flag">
    ///   The Flag::Enum entry describing the flag's effects on BWAPI.
    /// </param>
    ///
    /// @returns true if the given \p flag is enabled, false if the flag is disabled.
    ///
    /// @see Flag::Enum
    ///
    /// @todo Take Flag::Enum as parameter instead of int
    bool isFlagEnabled(int flag) const;

    /// <summary>Retrieves the set of accessible units that are on a given build tile.</summary>
    ///
    /// <param name="tileX">
    ///   The X position, in tiles.
    /// </param>
    /// <param name="tileY">
    ///   The Y position, in tiles.
    /// </param>
    /// <param name="pred"> (optional)
    ///   A function predicate that indicates which units are included in the returned set.
    /// </param>
    ///
    /// @returns A Unitset object consisting of all the units that have any part of them on the
    /// given build tile.
    Unitset getUnitsOnTile(int tileX, int tileY, const UnitFilter &pred = nullptr) const;

    /// @overload
    Unitset getUnitsOnTile(BWAPI::TilePosition tile, const UnitFilter &pred = nullptr) const;

    /// <summary>Retrieves the set of accessible units that are in a given rectangle.</summary>
    ///
    /// <param name="left">
    ///   The X coordinate of the left position of the bounding box, in pixels.
    /// </param>
    /// <param name="top">
    ///   The Y coordinate of the top position of the bounding box, in pixels.
    /// </param>
    /// <param name="right">
    ///   The X coordinate of the right position of the bounding box, in pixels.
    /// </param>
    /// <param name="bottom">
    ///   The Y coordinate of the bottom position of the bounding box, in pixels.
    /// </param>
    /// <param name="pred"> (optional)
    ///   A function predicate that indicates which units are included in the returned set.
    /// </param>
    ///
    /// @returns A Unitset object consisting of all the units that have any part of them within the
    /// given rectangle bounds.
    Unitset getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred = nullptr) const;
    /// @overload
    Unitset getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight, const UnitFilter &pred = nullptr) const;

    /// <summary>Retrieves the set of accessible units that are within a given radius of a
    /// position.</summary>
    ///
    /// <param name="x">
    ///   The x coordinate of the center, in pixels.
    /// </param>
    /// <param name="y">
    ///   The y coordinate of the center, in pixels.
    /// </param>
    /// <param name="radius">
    ///   The radius from the center, in pixels, to include units.
    /// </param>
    /// <param name="pred"> (optional)
    ///   A function predicate that indicates which units are included in the returned set.
    /// </param>
    ///
    /// @returns A Unitset object consisting of all the units that have any part of them within the
    /// given radius from the center position.
    Unitset getUnitsInRadius(int x, int y, int radius, const UnitFilter &pred = nullptr) const;
    /// @overload
    Unitset getUnitsInRadius(BWAPI::Position center, int radius, const UnitFilter &pred = nullptr) const;

    /// <summary>Retrieves the closest unit to center that matches the criteria of the callback
    /// pred within an optional radius.</summary>
    ///
    /// <param name="center">
    ///   The position to start searching for the closest unit.
    /// </param>
    /// <param name="pred"> (optional)
    ///   The UnitFilter predicate to determine which units should be included. This includes
    ///   all units by default.
    /// </param>
    /// <param name="radius"> (optional)
    ///   The radius to search in. If omitted, the entire map will be searched.
    /// </param>
    ///
    /// @returns The desired unit that is closest to center.
    /// @retval nullptr If a suitable unit was not found.
    ///
    /// @see getBestUnit, UnitFilter
    Unit getClosestUnit(Position center, const UnitFilter &pred = nullptr, int radius = 999999) const;

    /// <summary>Retrieves the closest unit to center that matches the criteria of the callback
    /// pred within an optional rectangle.</summary>
    ///
    /// <param name="center">
    ///   The position to start searching for the closest unit.
    /// </param>
    /// <param name="pred"> (optional)
    ///   The UnitFilter predicate to determine which units should be included. This includes
    ///   all units by default.
    /// </param>
    /// <param name="left"> (optional)
    ///   The left position of the rectangle. This value is 0 by default.
    /// </param>
    /// <param name="top"> (optional)
    ///   The top position of the rectangle. This value is 0 by default.
    /// </param>
    /// <param name="right"> (optional)
    ///   The right position of the rectangle. This value includes the entire map width by default.
    /// </param>
    /// <param name="bottom"> (optional)
    ///   The bottom position of the rectangle. This value includes the entire map height by default.
    /// </param>
    ///
    /// @see UnitFilter
    Unit getClosestUnitInRectangle(Position center, const UnitFilter &pred = nullptr, int left = 0, int top = 0, int right = 999999, int bottom = 999999) const;

    /// <summary>Compares all units with pred to determine which of them is the best.</summary>
    /// All units are checked. If center and radius are specified, then it will check all units
    /// that are within the radius of the position.
    ///
    /// <param name="best">
    ///   A BestUnitFilter that determines which parameters should be considered when calculating
    ///   which units are better than others.
    /// </param>
    /// <param name="pred">
    ///   A UnitFilter that determines which units to include in calculations.
    /// </param>
    /// <param name="center"> (optional)
    ///   The position to use in the search. If omitted, then the entire map is searched.
    /// </param>
    /// <param name="radius"> (optional)
    ///   The distance from \p center to search for units. If omitted, then the entire map is
    ///   searched.
    /// </param>
    /// 
    /// @returns The desired unit that best matches the given criteria.
    /// @retval nullptr if a suitable unit was not found.
    ///
    /// @see getClosestUnit, BestUnitFilter, UnitFilter
    Unit getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center = Positions::Origin, int radius = 999999) const;

    /// <summary>Returns the last error that was set using setLastError.</summary> If a function
    /// call in BWAPI has failed, you can use this function to retrieve the reason it failed.
    ///
    /// @returns Error type containing the reason for failure.
    ///
    /// @see setLastError, Errors
    Error getLastError() const;

    /// <summary>Sets the last error so that future calls to getLastError will return the value
    /// that was set.</summary>
    ///
    /// <param name="e"> (optional)
    ///   The error code to set. If omitted, then the last error will be cleared.
    /// </param>
    ///
    /// @retval true If the type passed was Errors::None, clearing the last error.
    /// @retval false If any other error type was passed.
    /// @see getLastError, Errors
    bool setLastError(BWAPI::Error e = Errors::None) const;

    /// <summary>Retrieves the width of the map in build tile units.</summary>
    ///
    /// @returns Width of the map in tiles.
    /// @see mapHeight
    int mapWidth() const;

    /// <summary>Retrieves the height of the map in build tile units.</summary>
    ///
    /// @returns Height of the map in tiles.
    /// @see mapHeight
    int mapHeight() const;

    /// <summary>Retrieves the file name of the currently loaded map.</summary>
    ///
    /// @returns Map file name as std::string object.
    ///
    /// @see mapPathName, mapName
    ///
    /// @TODO: Note on campaign files.
    std::string mapFileName() const;

    /// <summary>Retrieves the full path name of the currently loaded map.</summary>
    ///
    /// @returns Map file name as std::string object.
    ///
    /// @see mapFileName, mapName
    ///
    /// @TODO: Note on campaign files.
    std::string mapPathName() const;

    /// <summary>Retrieves the title of the currently loaded map.</summary>
    ///
    /// @returns Map title as std::string object.
    ///
    /// @see mapFileName, mapPathName
    std::string mapName() const;

    /// <summary>Calculates the SHA-1 hash of the currently loaded map file.</summary>
    ///
    /// @returns std::string object containing SHA-1 hash.
    ///
    /// @note Campaign maps will return a hash of their internal map chunk components(.chk), while
    /// standard maps will return a hash of their entire map archive (.scm,.scx).
    ///
    /// @TODO: Note on replays.
    std::string mapHash() const;

    /// <summary>Checks if the given mini-tile position is walkable.</summary>
    ///
    /// @note This function only checks if the static terrain is walkable. Its current occupied
    /// state is excluded from this check. To see if the space is currently occupied or not, then
    /// see #getUnitsInRectangle .
    ///
    /// <param name="walkX">
    ///   The x coordinate of the mini-tile, in mini-tile units (8 pixels).
    /// </param>
    /// <param name="walkY">
    ///   The y coordinate of the mini-tile, in mini-tile units (8 pixels).
    /// </param>
    ///
    /// @returns true if the mini-tile is walkable and false if it is impassable for ground units.
    bool isWalkable(int walkX, int walkY) const;
    /// @overload
    bool isWalkable(BWAPI::WalkPosition position) const;

    /// <summary>Returns the ground height at the given tile position.</summary>
    ///
    /// <param name="tileX">
    ///   X position to query, in tiles
    /// </param>
    /// <param name="tileY">
    ///   Y position to query, in tiles
    /// </param>
    ///
    /// @returns The tile height as an integer. Possible values are:
    ///     - 0: Low ground
    ///     - 1: Low ground doodad
    ///     - 2: High ground
    ///     - 3: High ground doodad
    ///     - 4: Very high ground
    ///     - 5: Very high ground doodad
    int  getGroundHeight(int tileX, int tileY) const;
    /// @overload
    int  getGroundHeight(TilePosition position) const;

    /// <summary>Checks if a given tile position is buildable.</summary> This means that, if all
    /// other requirements are met, a structure can be placed on this tile. This function uses
    /// static map data.
    ///
    /// <param name="tileX">
    ///   The x value of the tile to check.
    /// </param>
    /// <param name="tileY">
    ///   The y value of the tile to check.
    /// </param>
    /// <param name="includeBuildings"> (optional)
    ///   If this is true, then this function will also check if any visible structures are
    ///   occupying the space. If this value is false, then it only checks the static map data
    ///   for tile buildability. This value is false by default.
    /// </param>
    ///
    /// @returns boolean identifying if the given tile position is buildable (true) or not (false).
    /// If \p includeBuildings was provided, then it will return false if a structure is currently
    /// occupying the tile.
    bool isBuildable(int tileX, int tileY, bool includeBuildings = false) const {
      return isBuildable({ tileX, tileY }, includeBuildings);
    }
    /// @overload
    bool isBuildable(TilePosition position, bool includeBuildings = false) const {
      return isValid(position) &&
             gameData->map.isBuildable[position.x][position.y] &&
             (includeBuildings
                  ? !(isVisible(position) &&
                      gameData->map.isOccupied[position.x][position.y])
                  : true);
    }

    /// <summary>Checks if a given tile position is visible to the current player.</summary>
    ///
    /// <param name="tileX">
    ///   The x value of the tile to check.
    /// </param>
    /// <param name="tileY">
    ///   The y value of the tile to check.
    /// </param>
    /// 
    /// @returns boolean identifying the visibility of the tile. If the given tile is visible, then
    /// the value is true. If the given tile is concealed by the fog of war, then this value will
    /// be false.
    bool isVisible(int tileX, int tileY) const {
      return isVisible({tileX, tileY});
    }
    /// @overload
    bool isVisible(TilePosition position) const {
      return isValid(position) ? gameData->map.isVisible[position.x][position.y]
                               : false;
    }

    /// <summary>Checks if a given tile position has been explored by the player.</summary> An
    /// explored tile position indicates that the player has seen the location at some point in the
    /// match, partially revealing the fog of war for the remainder of the match.
    ///
    /// <param name="tileX">
    ///   The x tile coordinate to check.
    /// </param>
    /// <param name="tileY">
    ///   The y tile coordinate to check.
    /// </param>
    /// 
    /// @retval true If the player has explored the given tile position (partially revealed fog).
    /// @retval false If the tile position was never explored (completely black fog).
    ///
    /// @see isVisible
    bool isExplored(int tileX, int tileY) const {
      return isExplored({tileX, tileY});
    }
    /// @overload
    bool isExplored(TilePosition position) const {
      return isValid(position) ? gameData->map.isExplored[position.x][position.y]
                               : false;
    }

    /// <summary>Checks if the given tile position has @Zerg creep on it.</summary>
    ///
    /// <param name="tileX">
    ///   The x tile coordinate to check.
    /// </param>
    /// <param name="tileY">
    ///   The y tile coordinate to check.
    /// </param>
    ///
    /// @retval true If the given tile has creep on it.
    /// @retval false If the given tile does not have creep, or if it is concealed by the fog of war.
    bool hasCreep(int tileX, int tileY) const {
      return hasCreep({tileX, tileY});
    }
    /// @overload
    bool hasCreep(TilePosition position) const {
      return isValid(position) ? gameData->map.hasCreep[position.x][position.y]
                               : false;
    }

    /// <summary>Checks if the given pixel position is powered by an owned @Protoss_Pylon for an
    /// optional unit type.</summary>
    ///
    /// <param name="x">
    ///   The x pixel coordinate to check.
    /// </param>
    /// <param name="y">
    ///   The y pixel coordinate to check.
    /// </param>
    /// <param name="unitType"> (optional)
    ///   Checks if the given UnitType requires power or not. If ommitted, then it will assume
    ///   that the position requires power for any unit type.
    /// </param>
    ///
    /// @retval true if the type at the given position will have power.
    /// @retval false if the type at the given position will be unpowered.
    bool hasPowerPrecise(int x, int y, UnitType unitType = UnitTypes::None ) const;
    /// @overload
    bool hasPowerPrecise(Position position, UnitType unitType = UnitTypes::None) const;

    /// <summary>Checks if the given tile position if powered by an owned @Protoss_Pylon for an
    /// optional unit type.</summary>
    ///
    /// <param name="tileX">
    ///   The x tile coordinate to check.
    /// </param>
    /// <param name="tileY">
    ///   The y tile coordinate to check.
    /// </param>
    /// <param name="unitType"> (optional)
    ///   Checks if the given UnitType will be powered if placed at the given tile position. If
    ///   omitted, then only the immediate tile position is checked for power, and the function
    ///   will assume that the location requires power for any unit type.
    /// </param>
    ///   
    /// @retval true if the type at the given tile position will receive power.
    /// @retval false if the type will be unpowered at the given tile position.
    bool hasPower(int tileX, int tileY, UnitType unitType = UnitTypes::None) const;
    /// @overload
    bool hasPower(TilePosition position, UnitType unitType = UnitTypes::None) const;
    /// @overload
    bool hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;
    /// @overload
    bool hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;

    /// <summary>Checks if the given unit type can be built at the given build tile position.</summary>
    /// This function checks for creep, power, and resource distance requirements in addition to
    /// the tiles' buildability and possible units obstructing the build location.
    ///
    /// @note If the type is an addon and a builer is provided, then the location of the addon will
    /// be placed 4 tiles to the right and 1 tile down from the given \p position. If the builder
    /// is not given, then the check for the addon will be conducted at position.
    ///
    /// @note If \p type is UnitTypes::Special_Start_Location, then the area for a resource depot
    /// (@Command_Center, @Hatchery, @Nexus) is checked as normal, but any potential obstructions
    /// (existing structures, creep, units, etc.) are ignored.
    ///
    /// <param name="position">
    ///   Indicates the tile position that the top left corner of the structure is intended to go.
    /// </param>
    /// <param name="type">
    ///   The UnitType to check for.
    /// </param>
    /// <param name="builder"> (optional)
    ///   The intended unit that will build the structure. If specified, then this function will
    ///   also check if there is a path to the build site and exclude the builder from the set of
    ///   units that may be blocking the build site.
    /// </param>
    /// <param name="checkExplored"> (optional)
    ///   If this parameter is true, it will also check if the target position has been explored
    ///   by the current player. This value is false by default, ignoring the explored state of
    ///   the build site.
    /// </param>
    ///
    /// @returns true indicating that the structure can be placed at the given tile position, and
    /// false if something may be obstructing the build location.
    bool canBuildHere(TilePosition position, UnitType type, Unit builder = nullptr, bool checkExplored = false) const;

    /// <summary>Checks all the requirements in order to make a given unit type for the current
    /// player.</summary> These include resources, supply, technology tree, availability, and
    /// required units.
    ///
    /// <param name="type">
    ///   The UnitType to check.
    /// </param>
    /// <param name="builder"> (optional)
    ///   The Unit that will be used to build/train the provided unit \p type. If this value is
    ///   nullptr or excluded, then the builder will be excluded in the check.
    /// </param>
    ///
    /// @returns true indicating that the type can be made. If \p builder is provided, then it is
    /// only true if \p builder can make the \p type. Otherwise it will return false, indicating
    /// that the unit type can not be made.
    bool canMake(UnitType type, Unit builder = nullptr) const;

    /// <summary>Checks all the requirements in order to research a given technology type for the
    /// current player.</summary> These include resources, technology tree, availability, and
    /// required units.
    ///
    /// <param name="type">
    ///   The TechType to check.
    /// </param>
    /// <param name="unit"> (optional)
    ///   The Unit that will be used to research the provided technology \p type. If this value is
    ///   nullptr or excluded, then the unit will be excluded in the check.
    /// </param>
    /// <param name="checkCanIssueCommandType"> (optional)
    ///   TODO fill this in
    /// </param>
    ///
    /// @returns true indicating that the type can be researched. If \p unit is provided, then it is
    /// only true if \p unit can research the \p type. Otherwise it will return false, indicating
    /// that the technology can not be researched.
    bool canResearch(TechType type, Unit unit = nullptr, bool checkCanIssueCommandType = true) const;

    /// <summary>Checks all the requirements in order to upgrade a given upgrade type for the
    /// current player.</summary> These include resources, technology tree, availability, and
    /// required units.
    ///
    /// <param name="type">
    ///   The UpgradeType to check.
    /// </param>
    /// <param name="unit"> (optional)
    ///   The Unit that will be used to upgrade the provided upgrade \p type. If this value is
    ///   nullptr or excluded, then the unit will be excluded in the check.
    /// </param>
    /// <param name="checkCanIssueCommandType"> (optional)
    ///   TODO fill this in
    /// </param>
    ///
    /// @returns true indicating that the type can be upgraded. If \p unit is provided, then it is
    /// only true if \p unit can upgrade the \p type. Otherwise it will return false, indicating
    /// that the upgrade can not be upgraded.
    bool canUpgrade(UpgradeType type, Unit unit = nullptr, bool checkCanIssueCommandType = true) const;

    /// <summary>Retrieves the set of all starting locations for the current map.</summary> A
    /// starting location is essentially a candidate for a player's spawn point.
    ///
    /// @returns A TilePosition::list containing all the TilePosition objects that indicate a start
    /// location.
    /// @see Player::getStartLocation
    const TilePosition::list& getStartLocations() const;

    /// <summary>Prints text to the screen as a notification.</summary> This function allows text
    /// formatting using Text::Enum members. The behaviour of this function is the same as printf,
    /// located in header cstdio.
    /// 
    /// @note That text printed through this function is not seen by other players or in replays.
    /// 
    /// <param name="format">
    ///   Text formatting. See std::printf for more information. Refrain from passing non-constant
    ///   strings directly in this parameter.
    /// </param>
    /// <param name="...">
    ///   The arguments that will be formatted using the given text formatting.
    /// </param>
    ///
    /// @see Text::Enum, std::printf
    void printf(const char *format, ...);

    /// @copydoc printf
    ///
    /// This function is intended to forward an already-existing argument list.
    ///
    /// <param name="args">
    ///   The argument list that will be formatted.
    /// </param>
    ///
    /// @see printf
    void vPrintf(const char *format, va_list args);
    
    /// <summary>Sends a text message to all other players in the game.</summary> The behaviour of
    /// this function is the same as std::printf, located in header cstdio.
    ///
    /// @note In a single player game this function can be used to execute cheat codes.
    ///
    /// <param name="format">
    ///   Text formatting. See std::printf for more information. Refrain from passing non-constant
    ///   strings directly in this parameter.
    /// </param>
    ///
    /// @see sendTextEx, std::printf
    void sendText(const char *format, ...);
    
    /// @copydoc sendText
    ///
    /// This function is intended to forward an already-existing argument list.
    ///
    /// <param name="args">
    ///   The argument list that will be formatted.
    /// </param>
    ///
    /// @see sendText
    void vSendText(const char *format, va_list args);

    /// <summary>An extended version of Game::sendText which allows messages to be forwarded to
    /// allies.</summary> The behaviour of this function is the same as std::printf, located in
    /// header cstdio.
    ///
    /// <param name="toAllies">
    ///   If this parameter is set to true, then the message is only sent to allied players,
    ///   otherwise it will be sent to all players.
    /// </param>
    /// <param name="format">
    ///   Text formatting. See std::printf for more information. Refrain from passing non-constant
    ///   strings directly in this parameter.
    /// </param>
    ///
    /// @see sendText, std::printf
    void sendTextEx(bool toAllies, const char *format, ...);
    
    /// @copydoc sendTextEx
    ///
    /// This function is intended to forward an already-existing argument list.
    ///
    /// <param name="args">
    ///   The argument list that will be formatted.
    /// </param>
    ///
    /// @see sendTextEx
    void vSendTextEx(bool toAllies, const char *format, va_list args);

    /// <summary>Checks if the current client is inside a game.</summary>
    ///
    /// @returns true if the client is in a game, and false if it is not.
    bool isInGame() const { return gameData->isInGame; }

    /// <summary>Checks if the current client is inside a multiplayer game.</summary>
    ///
    /// @returns true if the client is in a multiplayer game, and false if it is a single player
    /// game, a replay, or some other state.
    bool isMultiplayer() const { return gameData->isMultiplayer; }

    /// <summary>Checks if the client is in a game that was created through the Battle.net
    /// multiplayer gaming service.</summary>
    ///
    /// @returns true if the client is in a multiplayer Battle.net game and false if it is not.
    bool isBattleNet() const { return gameData->isBattleNet; }

    /// <summary>Checks if the current game is paused.</summary> While paused, AIModule::onFrame
    /// will still be called.
    ///
    /// @returns true if the game is paused and false otherwise
    /// @see pauseGame, resumeGame
    bool isPaused() const { return gameData->isPaused; }

    /// <summary>Checks if the client is watching a replay.</summary>
    ///
    /// @returns true if the client is watching a replay and false otherwise
    bool isReplay() const { return gameData->isReplay; }

    /// <summary>Pauses the game.</summary> While paused, AIModule::onFrame will still be called.
    /// @see resumeGame
    void pauseGame();

    /// <summary>Resumes the game from a paused state.</summary>
    /// @see pauseGame
    void resumeGame();

    /// <summary>Leaves the current game by surrendering and enters the post-game statistics/score
    /// screen.</summary>
    void leaveGame();

    /// <summary>Restarts the match.</summary> Works the same as if the match was restarted from
    /// the in-game menu (F10). This option is only available in single player games.
    ///
    /// @returns false if not in game, or if isMultiPlayer is true.
    bool restartGame();

    /// <summary>Sets the number of milliseconds Broodwar spends in each frame.</summary> The
    /// default values are as follows:
    ///   - Fastest: 42ms/frame
    ///   - Faster: 48ms/frame
    ///   - Fast: 56ms/frame
    ///   - Normal: 67ms/frame
    ///   - Slow: 83ms/frame
    ///   - Slower: 111ms/frame
    ///   - Slowest: 167ms/frame
    ///
    /// @note Specifying a value of 0 will not guarantee that logical frames are executed as fast
    /// as possible. If that is the intention, use this in combination with #setFrameSkip.
    ///
    /// @bug Changing this value will cause the execution of @UMS scenario triggers to glitch.
    /// This will only happen in campaign maps and custom scenarios (non-melee).
    ///
    /// <param name="speed">
    ///   The time spent per frame, in milliseconds. A value of 0 indicates that frames are
    ///   executed immediately with no delay. Negative values will restore the default value
    ///   as listed above.
    /// </param>
    ///
    /// @see setFrameSkip, getFPS
    void setLocalSpeed(int speed);

    /// <summary>Issues a given command to a set of units.</summary> This function automatically
    /// splits the set into groups of 12 and issues the same command to each of them. If a unit
    /// is not capable of executing the command, then it is simply ignored.
    ///
    /// <param name="units">
    ///   A Unitset containing all the units to issue the command for.
    /// </param>
    /// <param name="command">
    ///   A UnitCommand object containing relevant information about the command to be issued.
    ///   The Unit interface object associated with the command will be ignored.
    /// </param>
    ///
    /// @returns true if any one of the units in the Unitset were capable of executing the
    /// command, and false if none of the units were capable of executing the command.
    bool issueCommand(const Unitset& units, UnitCommand command);

    /// <summary>Retrieves the set of units that are currently selected by the user outside of
    /// BWAPI.</summary> This function requires that Flag::UserInput be enabled.
    ///
    /// @returns A Unitset containing the user's selected units. If Flag::UserInput is disabled, 
    /// then this set is always empty.
    const Unitset& getSelectedUnits() const;

    /// <summary>Retrieves the player object that BWAPI is controlling.</summary> 
    ///
    /// @returns Pointer to Player interface object representing the current player.
    /// @retval nullptr if the current game is a replay.
    ///
    /// Example usage
    /// @code{.cpp}
    ///   if ( game.self() )
    ///     BWAPI::Broodwar->sendText("Hello, my name is %s.", game.self().getName().c_str());
    /// @endcode
    Player self() const { return *getPlayerData(gameData->player); }

    /// <summary>Retrieves the Player interface that represents the enemy player.</summary> If
    /// there is more than one enemy, and that enemy is destroyed, then this function will still
    /// retrieve the same, defeated enemy. If you wish to handle multiple opponents, see the
    /// Game::enemies function.
    ///
    /// @returns Player interface representing an enemy player.
    /// @retval nullptr If there is no enemy or the current game is a replay.
    /// @see enemies
    Player enemy() const;

    /// <summary>Retrieves the Player interface object representing the neutral player.</summary>
    /// The neutral player owns all the resources and critters on the map by default.
    ///
    /// @returns Player interface indicating the neutral player.
    Player neutral() const;

    /// <summary>Retrieves a set of all the current player's remaining allies.</summary>
    ///
    /// @returns Playerset containing all allied players.
    Playerset& allies();

    /// <summary>Retrieves a set of all the current player's remaining enemies.</summary>
    ///
    /// @returns Playerset containing all enemy players.
    Playerset& enemies();

    /// <summary>Retrieves a set of all players currently observing the game.</summary> An observer
    /// is defined typically in a @UMS game type as not having any impact on the game. This means
    /// an observer cannot start with any units, and cannot have any active trigger actions that
    /// create units for it.
    ///
    /// @returns Playerset containing all currently active observer players
    Playerset& observers();

    /// @name Debugging Members
    /// @{

    /// <summary>Sets the size of the text for all calls to drawText following this one.</summary>
    ///
    /// <param name="size"> (optional)
    ///   The size of the text. This value is one of Text::Size::Enum. If this value is omitted,
    ///   then a default value of Text::Size::Default is used.
    /// </param>
    ///
    /// Example usage
    /// @code{.cpp}
    ///   void ExampleAIModule::onFrame()
    ///   {
    ///     // Centers the name of the player in the upper middle of the screen
    ///     BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Large);
    ///     BWAPI::Broodwar->drawTextScreen(BWAPI::Positions::Origin, "%c%c%s",
    ///                                     BWAPI::Text::Align_Center,
    ///                                     BWAPI::Text::Green,
    ///                                     BWAPI::Broodwar->self()->getName().c_str() );
    ///     BWAPI::Broodwar->setTextSize();   // Set text size back to default
    ///   }
    /// @endcode
    /// @see Text::Size::Enum
    void setTextSize(Text::Size::Enum size = Text::Size::Default);

    /// <summary>Draws text on the screen at the given coordinates.</summary> Text can be drawn in
    /// different colors or formatted using the Text::Enum members.
    ///
    /// <param name="ctype">
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// </param>
    /// <param name="x">
    ///   The x coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="y">
    ///   The y coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="format">
    ///   The string formatting portion. This is the same as printf style formatting.
    /// </param>
    /// <param name="arg">
    ///   Arglist containing the intermediate list of arguments to format before finally sending
    ///   the string to Broodwar.
    /// </param>
    void vDrawText(CoordinateType::Enum ctype, int x, int y, const char *format, va_list arg);
    /// @overload
    void drawText(CoordinateType::Enum ctype, int x, int y, const char *format, ...);
    /// @overload
    void drawTextMap(int x, int y, const char *format, ...);
    /// @overload
    void drawTextMap(Position p, const char *format, ...);
    /// @overload
    void drawTextMouse(int x, int y, const char *format, ...);
    /// @overload
    void drawTextMouse(Position p, const char *format, ...);
    /// @overload
    void drawTextScreen(int x, int y, const char *format, ...);
    /// @overload
    void drawTextScreen(Position p, const char *format, ...);

    /// <summary>Draws a rectangle on the screen with the given color.</summary>
    ///
    /// <param name="ctype">
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// </param>
    /// <param name="left">
    ///   The x coordinate, in pixels, relative to ctype, of the left edge of the rectangle.
    /// </param>
    /// <param name="top">
    ///   The y coordinate, in pixels, relative to ctype, of the top edge of the rectangle.
    /// </param>
    /// <param name="right">
    ///   The x coordinate, in pixels, relative to ctype, of the right edge of the rectangle.
    /// </param>
    /// <param name="bottom">
    ///   The y coordinate, in pixels, relative to ctype, of the bottom edge of the rectangle.
    /// </param>
    /// <param name="color">
    ///   The color of the rectangle.
    /// </param>
    /// <param name="isSolid"> (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    /// </param>
    void drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxMap(Position leftTop, Position rightBottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxMouse(Position leftTop, Position rightBottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxScreen(Position leftTop, Position rightBottom, Color color, bool isSolid = false);

    /// <summary>Draws a triangle on the screen with the given color.</summary>
    ///
    /// <param name="ctype">
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// </param>
    /// <param name="ax">
    ///   The x coordinate, in pixels, relative to ctype, of the first point.
    /// </param>
    /// <param name="ay">
    ///   The y coordinate, in pixels, relative to ctype, of the first point.
    /// </param>
    /// <param name="bx">
    ///   The x coordinate, in pixels, relative to ctype, of the second point.
    /// </param>
    /// <param name="by">
    ///   The y coordinate, in pixels, relative to ctype, of the second point.
    /// </param>
    /// <param name="cx">
    ///   The x coordinate, in pixels, relative to ctype, of the third point.
    /// </param>
    /// <param name="cy">
    ///   The y coordinate, in pixels, relative to ctype, of the third point.
    /// </param>
    /// <param name="color">
    ///   The color of the triangle.
    /// </param>
    /// <param name="isSolid"> (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    /// </param>
    void drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleMap(Position a, Position b, Position c, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleMouse(Position a, Position b, Position c, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleScreen(Position a, Position b, Position c, Color color, bool isSolid = false);

    /// <summary>Draws a circle on the screen with the given color.</summary>
    ///
    /// <param name="ctype">
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// </param>
    /// <param name="x">
    ///   The x coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="y">
    ///   The y coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="radius">
    ///   The radius of the circle, in pixels.
    /// </param>
    /// <param name="color">
    ///   The color of the circle.
    /// </param>
    /// <param name="isSolid"> (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    /// </param>
    void drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleMap(Position p, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleMouse(Position p, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleScreen(Position p, int radius, Color color, bool isSolid = false);

    /// <summary>Draws an ellipse on the screen with the given color.</summary>
    ///
    /// <param name="ctype">
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// </param>
    /// <param name="x">
    ///   The x coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="y">
    ///   The y coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="xrad">
    ///   The x radius of the ellipse, in pixels.
    /// </param>
    /// <param name="yrad">
    ///   The y radius of the ellipse, in pixels.
    /// </param>
    /// <param name="color">
    ///   The color of the ellipse.
    /// </param>
    /// <param name="isSolid"> (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    /// </param>
    void drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseMap(Position p, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseMouse(Position p, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseScreen(Position p, int xrad, int yrad, Color color, bool isSolid = false);

    /// <summary>Draws a dot on the map or screen with a given color.</summary>
    ///
    /// <param name="ctype">
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// </param>
    /// <param name="x">
    ///   The x coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="y">
    ///   The y coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="color">
    ///   The color of the dot.
    /// </param>
    void drawDot(CoordinateType::Enum ctype, int x, int y, Color color);
    /// @overload
    void drawDotMap(int x, int y, Color color);
    /// @overload
    void drawDotMap(Position p, Color color);
    /// @overload
    void drawDotMouse(int x, int y, Color color);
    /// @overload
    void drawDotMouse(Position p, Color color);
    /// @overload
    void drawDotScreen(int x, int y, Color color);
    /// @overload
    void drawDotScreen(Position p, Color color);

    /// <summary>Draws a line on the map or screen with a given color.</summary>
    ///
    /// <param name="ctype">
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// </param>
    /// <param name="x1">
    ///   The starting x coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="y1">
    ///   The starting y coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="x2">
    ///   The ending x coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="y2">
    ///   The ending y coordinate, in pixels, relative to ctype.
    /// </param>
    /// <param name="color">
    ///   The color of the line.
    /// </param>
    void drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color);
    /// @overload
    void drawLineMap(int x1, int y1, int x2, int y2, Color color);
    /// @overload
    void drawLineMap(Position a, Position b, Color color);
    /// @overload
    void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
    /// @overload
    void drawLineMouse(Position a, Position b, Color color);
    /// @overload
    void drawLineScreen(int x1, int y1, int x2, int y2, Color color);
    /// @overload
    void drawLineScreen(Position a, Position b, Color color);

    /// <summary>(Single player only) Creates a unit instantly for the given
    /// player at any location on the map.</summary>
    ///
    /// <param name="player">
    ///   The player that will own the new unit.
    /// </param>
    /// <param name="unitType">
    ///   The type of unit to create.
    /// </param>
    /// <param name="x">
    ///   The x position to spawn the unit at.
    /// </param>
    /// <param name="y">
    ///   The y position to spawn the unit at.
    /// </param>
    /// <param name="count">
    ///   (Optional) The number of units to spawn. Default: 1
    /// </param>
    void createUnit(Player player, UnitType unitType, int x, int y, int count = 1);
    /// @overload
    void createUnit(Player player, UnitType unitType, Position position, int count = 1);
    
    /// <summary>(Single player only) Kills the given units immediately.</summary>
    ///
    /// <param name="units">
    ///   The set of units that will instantly die. Can be owned by any player.
    /// </param>
    void killUnits(const Unitset& units);

    /// <summary>(Single player only) Kills the given unit immediately.</summary>
    ///
    /// <param name="unit">
    ///   The unit that will instantly die. Can be owned by any player.
    /// </param>
    void killUnit(Unit unit);

    /// <summary>(Single player only) Removes the given units from the game.</summary>
    ///
    /// <param name="units">
    ///   The set of units that will instantly be erased from the game. Can be owned by any player.
    /// </param>
    void removeUnits(const Unitset& units);
    
    /// <summary>(Single player only) Removes the given unit from the game.</summary>
    ///
    /// <param name="unit">
    ///   The unit that will instantly be erased from the game. Can be owned by any player.
    /// </param>
    void removeUnit(Unit unit);

    /// @}

    /// <summary>Retrieves the maximum delay, in number of frames, between a command being issued
    /// and the command being executed by Broodwar.</summary>
    ///
    /// @note In Broodwar, latency is used to keep the game synchronized between players without
    /// introducing lag.
    ///
    /// @returns Difference in frames between commands being sent and executed.
    /// @see getLatencyTime, getRemainingLatencyFrames
    int getLatencyFrames() const;

    /// <summary>Retrieves the maximum delay, in milliseconds, between a command being issued and
    /// the command being executed by Broodwar.</summary>
    ///
    /// @returns Difference in milliseconds between commands being sent and executed.
    /// @see getLatencyFrames, getRemainingLatencyTime
    int getLatencyTime() const;

    /// <summary>Retrieves the number of frames it will take before a command sent in the current
    /// frame will be executed by the game.</summary>
    ///
    /// @returns Number of frames until a command is executed if it were sent in the current
    /// frame.
    /// @see getRemainingLatencyTime, getLatencyFrames
    int getRemainingLatencyFrames() const;
    
    /// <summary>Retrieves the number of milliseconds it will take before a command sent in the
    /// current frame will be executed by Broodwar.</summary>
    ///
    /// @returns Amount of time, in milliseconds, until a command is executed if it were sent in
    /// the current frame.
    /// @see getRemainingLatencyFrames, getLatencyTime
    int getRemainingLatencyTime() const;

    /// <summary>Retrieves the version that the BWAPI client is using for compatibility checks.</summary>
    ///
    /// @returns The version number for the BWAPI client.
    ///
    /// @threadsafe
    /// @m_since{4,2,0}
    int getClientVersion() const;

    /// <summary>Retrieves the debug state of the BWAPI build.</summary>
    ///
    /// @returns true if the BWAPI module is a DEBUG build, and false if it is a RELEASE build.
    ///
    /// @threadsafe
    bool isDebug() const;

    /// <summary>Checks the state of latency compensation.</summary>
    ///
    /// @returns true if latency compensation is enabled, false if it is disabled.
    /// @see setLatCom
    bool isLatComEnabled() const;

    /// <summary>Changes the state of latency compensation.</summary> Latency compensation
    /// modifies the state of BWAPI's representation of units to reflect the implications of
    /// issuing a command immediately after the command was performed, instead of waiting
    /// consecutive frames for the results. Latency compensation is enabled by default.
    ///
    /// <param name="isEnabled">
    ///   Set whether the latency compensation feature will be enabled (true) or disabled (false).
    /// </param>
    ///
    /// @see isLatComEnabled.
    //void setLatCom(bool isEnabled);

    /// <summary>Checks if the GUI is enabled.</summary> The GUI includes all drawing functions of
    /// BWAPI, as well as screen updates from Broodwar.
    ///
    /// @retval true If the GUI is enabled, and everything is visible
    /// @retval false If the GUI is disabled and drawing functions are rejected
    ///
    /// @see setGUI
    bool isGUIEnabled() const;

    /// <summary>Sets the rendering state of the Starcraft GUI.</summary> This typically gives
    /// Starcraft a very low graphical frame rate and disables all drawing functionality in BWAPI.
    ///
    /// <param name="enabled">
    ///   A boolean value that determines the state of the GUI. Passing false to this function
    ///   will disable the GUI, and true will enable it.
    /// </param>
    ///
    /// Example Usage:
    /// @code{.cpp}
    ///   // Make our bot run thousands of games as fast as possible!
    ///   game.setLocalSpeed(0);
    ///   game.setGUI(false);
    /// @endcode
    ///
    /// @see isGUIEnabled
    void setGUI(bool enabled);

    /// <summary>Retrieves the Starcraft instance number recorded by BWAPI to identify which
    /// Starcraft instance an AI module belongs to.</summary> The very first instance should
    /// return 0.
    ///
    /// @returns
    ///   An integer value representing the instance number.
    ///
    /// @threadsafe
    int getInstanceNumber() const;

    /// <summary>Retrieves the Actions Per Minute (APM) that the bot is producing.</summary>
    ///
    /// <param name="includeSelects"> (optional)
    ///   If true, the return value will include selections as individual commands, otherwise
    ///   it will exclude selections. This value is false by default.
    /// </param>
    ///
    /// @returns The number of actions that the bot has executed per minute, on average.
    int getAPM(bool includeSelects = false) const;

    /// <summary>Changes the map to the one specified.</summary> Once restarted, the game will
    /// load the map that was provided. Changes do not take effect unless the game is restarted.
    ///
    /// <param name="mapFileName">
    ///   A string containing the path and file name to the desired map.
    /// </param>
    ///
    /// @retval true if the function succeeded and has changed the map.
    /// @retval false if the function failed, does not have permission from the tournament module,
    ///               failed to find the map specified, or received an invalid parameter.
    bool setMap(const std::string &mapFileName);

    /// <summary>Sets the number of graphical frames for every logical frame.</summary> This
    /// allows the game to run more logical frames per graphical frame, increasing the speed at
    /// which the game runs.
    ///
    /// <param name="frameSkip">
    ///   Number of graphical frames per logical frame. If this value is 0 or less, then it will
    ///   default to 1.
    /// </param>
    ///
    /// @see setLocalSpeed
    void setFrameSkip(int frameSkip);

    /// <summary>Checks if there is a path from source to destination.</summary> This only checks
    /// if the source position is connected to the destination position. This function does not
    /// check if all units can actually travel from source to destination. Because of this
    /// limitation, it has an O(1) complexity, and cases where this limitation hinders gameplay is
    /// uncommon at best.
    ///
    /// @note If making queries on a unit, it's better to call Unit::hasPath, since it is
    /// a more lenient version of this function that accounts for some edge cases.
    /// 
    /// <param name="source">
    ///   The source position.
    /// </param>
    /// <param name="destination">
    ///   The destination position.
    /// </param>
    ///
    /// @returns true if there is a path between the two positions, and false if there is not.
    /// @see Unit::hasPath
    bool hasPath(Position source, Position destination) const;

    /// <summary>Sets the alliance state of the current player with the target player.</summary>
    ///
    /// <param name="player">
    ///   The target player to set alliance with.
    /// </param>
    /// <param name="allied"> (optional)
    ///   If true, the current player will ally the target player. If false, the current player
    ///   will make the target player an enemy. This value is true by default.
    /// </param>
    /// <param name="alliedVictory"> (optional)
    ///   Sets the state of "allied victory". If true, the game will end in a victory if all
    ///   allied players have eliminated their opponents. Otherwise, the game will only end if
    ///   no other players are remaining in the game. This value is true by default.
    /// </param>
    bool setAlliance(BWAPI::Player player, bool allied = true, bool alliedVictory = true);

    /// <summary>In a game, this function sets the vision of the current BWAPI player with the
    /// target player.</summary> In a replay, this function toggles the visibility of the target
    /// player.
    ///
    /// <param name="player">
    ///   The target player to toggle vision.
    /// </param>
    /// <param name="enabled"> (optional)
    ///   The vision state. If true, and in a game, the current player will enable shared vision
    ///   with the target player, otherwise it will unshare vision. If in a replay, the vision
    ///   of the target player will be shown, otherwise the target player will be hidden. This
    ///   value is true by default.
    /// </param>
    bool setVision(BWAPI::Player player, bool enabled = true);

    /// <summary>Retrieves current amount of time in seconds that the game has elapsed.</summary>
    ///
    /// @returns Time, in seconds, that the game has elapsed as an integer.
    int elapsedTime() const;

    /// <summary>Sets the command optimization level.</summary> Command optimization is a feature
    /// in BWAPI that tries to reduce the APM of the bot by grouping or eliminating unnecessary
    /// game actions. For example, suppose the bot told 24 @Zerglings to @Burrow. At command
    /// optimization level 0, BWAPI is designed to select each Zergling to burrow individually,
    /// which costs 48 actions. With command optimization level 1, it can perform the same
    /// behaviour using only 4 actions. The command optimizer also reduces the amount of bytes used
    /// for each action if it can express the same action using a different command. For example,
    /// Right_Click uses less bytes than Move.
    ///
    /// <param name="level">
    ///   An integer representation of the aggressiveness for which commands are optimized. A
    ///   lower level means less optimization, and a higher level means more optimization.
    /// </param>
    ///
    /// The values for \p level are as follows:
    ///     - 0: No optimization.
    ///     - 1: Some optimization.
    ///       - Is not detected as a hack.
    ///       - Does not alter behaviour.
    ///       - Units performing the following actions are grouped and ordered 12 at a time:
    ///         - Attack_Unit
    ///         - Morph (@Larva only)
    ///         - Hold_Position
    ///         - Stop
    ///         - Follow
    ///         - Gather
    ///         - Return_Cargo
    ///         - Repair
    ///         - Burrow
    ///         - Unburrow
    ///         - Cloak
    ///         - Decloak
    ///         - Siege
    ///         - Unsiege
    ///         - Right_Click_Unit
    ///         - Halt_Construction
    ///         - Cancel_Train (@Carrier and @Reaver only)
    ///         - Cancel_Train_Slot (@Carrier and @Reaver only)
    ///         - Cancel_Morph (for non-buildings only)
    ///         - Use_Tech
    ///         - Use_Tech_Unit
    ///       - The following order transformations are applied to allow better grouping:
    ///         - Attack_Unit becomes Right_Click_Unit if the target is an enemy
    ///         - Move becomes Right_Click_Position
    ///         - Gather becomes Right_Click_Unit if the target contains resources
    ///         - Set_Rally_Position becomes Right_Click_Position for buildings
    ///         - Set_Rally_Unit becomes Right_Click_Unit for buildings
    ///         - Use_Tech_Unit with Infestation becomes Right_Click_Unit if the target is valid
    ///     - 2: More optimization by grouping structures.
    ///       - Includes the optimizations made by all previous levels.
    ///       - May be detected as a hack by some replay utilities.
    ///       - Does not alter behaviour.
    ///       - Units performing the following actions are grouped and ordered 12 at a time:
    ///         - Attack_Unit (@Turrets, @Photon_Cannons, @Sunkens, @Spores)
    ///         - Train
    ///         - Morph
    ///         - Set_Rally_Unit
    ///         - Lift
    ///         - Cancel_Construction
    ///         - Cancel_Addon
    ///         - Cancel_Train
    ///         - Cancel_Train_Slot
    ///         - Cancel_Morph
    ///         - Cancel_Research
    ///         - Cancel_Upgrade
    ///     - 3: Extensive optimization 
    ///       - Includes the optimizations made by all previous levels.
    ///       - Units may behave or move differently than expected.
    ///       - Units performing the following actions are grouped and ordered 12 at a time:
    ///         - Attack_Move
    ///         - Set_Rally_Position
    ///         - Move
    ///         - Patrol
    ///         - Unload_All
    ///         - Unload_All_Position
    ///         - Right_Click_Position
    ///         - Use_Tech_Position
    ///     - 4: Aggressive optimization
    ///       - Includes the optimizations made by all previous levels.
    ///       - Positions used in commands will be rounded to multiples of 32.
    ///       - @High_Templar and @Dark_Templar that merge into @Archons will be grouped and may
    ///         choose a different target to merge with. It will not merge with a target that
    ///         wasn't included.
    void setCommandOptimizationLevel(int level);

    /// <summary>Returns the remaining countdown time.</summary> The countdown timer is used in
    /// @CTF and @UMS game types.
    ///
    /// Example usage:
    /// @code{.cpp}
    ///   if ( (game.getGameType() == BWAPI::GameTypes::Capture_The_Flag ||
    ///         game.getGameType() == BWAPI::GameTypes::Team_Capture_The_Flag) &&
    ///        game.countdownTimer() == 1 )
    ///   {
    ///     game.sendText("Try to find my flag!");
    ///   }
    /// @endcode
    ///
    /// @returns Integer containing the time (in game seconds) on the countdown timer.
    int countdownTimer() const;

    /// <summary>Retrieves the set of all regions on the map.</summary>
    ///
    /// @returns Regionset containing all map regions.
    const Regionset& getAllRegions() const;

    /// <summary>Retrieves the region at a given position.</summary>
    ///
    /// <param name="x">
    ///   The x coordinate, in pixels.
    /// </param>
    /// <param name="y">
    ///   The y coordinate, in pixels.
    /// </param>
    ///
    /// @returns Pointer to the Region interface at the given position.
    /// @retval nullptr if the provided position is not valid (i.e. not within the map bounds).
    ///
    /// @note If the provided position is invalid, the error Errors::Invalid_Parameter is set.
    /// @see getAllRegions, getRegion
    BWAPI::Region getRegionAt(int x, int y) const;
    /// @overload
    BWAPI::Region getRegionAt(BWAPI::Position position) const;

    /// <summary>Retrieves the amount of time (in milliseconds) that has elapsed when running the last AI
    /// module callback.</summary> This is used by tournament modules to penalize AI modules that use too
    /// much processing time.
    ///
    /// @retval 0 When called from an AI module.
    /// @returns Time in milliseconds spent in last AI module call.
    int getLastEventTime() const;

    /// <summary>Sets the state of the fog of war when watching a replay.</summary>
    ///
    /// <param name="reveal"> (optional)
    ///   The state of the reveal all flag. If false, all fog of war will be enabled. If true,
    ///   then the fog of war will be revealed. It is true by default.
    /// </param>
    bool setRevealAll(bool reveal = true);

    /// <summary>Retrieves a basic build position just as the default Computer AI would.</summary>
    /// This allows users to find simple build locations without relying on external libraries.
    ///
    /// <param name="type">
    ///   A valid UnitType representing the unit type to accomodate space for.
    /// </param>
    /// <param name="desiredPosition">
    ///   A valid TilePosition containing the desired placement position.
    /// </param>
    /// <param name="maxRange"> (optional)
    ///   The maximum distance (in tiles) to build from \p desiredPosition.
    /// </param>
    /// <param name="creep"> (optional)
    ///   A special boolean value that changes the behaviour of @Creep_Colony placement.
    /// </param>
    ///
    /// @retval TilePositions::Invalid If a build location could not be found within \p maxRange.
    /// @returns
    ///   A TilePosition containing the location that the structure should be constructed at.
    TilePosition getBuildLocation(UnitType type, TilePosition desiredPosition, int maxRange = 64, bool creep = false) const;

    /// <summary>Calculates the damage received for a given player.</summary> It can be understood
    /// as the damage from \p fromType to \p toType. Does not include shields in calculation.
    /// Includes upgrades if players are provided.
    ///
    /// <param name="fromType">
    ///   The unit type that will be dealing the damage.
    /// </param>
    /// <param name="toType">
    ///   The unit type that will be receiving the damage.
    /// </param>
    /// <param name="fromPlayer"> (optional)
    ///   The player owner of the given type that will be dealing the damage. If omitted, then
    ///   no player will be used to calculate the upgrades for \p fromType.
    /// </param>
    /// <param name="toPlayer"> (optional)
    ///   The player owner of the type that will be receiving the damage. If omitted, then this
    ///   parameter will default to Broodwar->self().
    /// </param>
    ///
    /// @returns The amount of damage that fromType would deal to toType.
    /// @see getDamageTo
    int getDamageFrom(UnitType fromType, UnitType toType, Player fromPlayer = nullptr, Player toPlayer = nullptr);

    /// <summary>Calculates the damage dealt for a given player.</summary> It can be understood as
    /// the damage to \p toType from \p fromType. Does not include shields in calculation.
    /// Includes upgrades if players are provided.
    ///
    /// @note This function is nearly the same as #getDamageFrom. The only difference is that
    /// the last parameter is intended to default to Broodwar->self().
    ///
    /// <param name="toType">
    ///   The unit type that will be receiving the damage.
    /// </param>
    /// <param name="fromType">
    ///   The unit type that will be dealing the damage.
    /// </param>
    /// <param name="toPlayer"> (optional)
    ///   The player owner of the type that will be receiving the damage. If omitted, then
    ///   no player will be used to calculate the upgrades for \p toType.
    /// </param>
    /// <param name="fromPlayer"> (optional)
    ///   The player owner of the given type that will be dealing the damage. If omitted, then
    ///   this parameter will default to Broodwar->self().
    /// </param>
    ///
    /// @returns The amount of damage that fromType would deal to toType.
    /// @see getDamageFrom
    int getDamageTo(UnitType toType, UnitType fromType, Player toPlayer = nullptr, Player fromPlayer = nullptr);

    /// <summary>Retrieves the initial random seed that was used in this game's creation.</summary>
    /// This is used to identify the seed that started this game, in case an error occurred, so
    /// that developers can deterministically reproduce the error. Works in both games and replays.
    ///
    /// @returns This game's random seed.
    /// @m_since{4,2,0}
    unsigned getRandomSeed() const;

    /// <summary>Checks if this point is within the game's map bounds.</summary>
    ///
    /// @retval true If it is a valid position and on the map/playing field.
    /// @retval false If this is not a valid position.
    ///
    /// @see makeValid
    template<typename T, int Scale>
    bool isValid(BWAPI::Point<T, Scale> pos) const {
      BWAPI::Position temp{ pos };

      return temp.x >= 0 &&
        temp.y >= 0 &&
        temp.x < mapWidth() * 32 &&
        temp.y < mapHeight() * 32;
    }

    /// <summary>Checks if this point is within the game's map bounds, if not, then it will set
    /// the x and y values to be within map bounds.</summary> For example, if x is less than 0,
    /// then x is set to 0.
    ///
    /// @returns A reference to pos for convenience
    /// @see isValid
    template<typename T, int Scale>
    auto makeValid(BWAPI::Point<T, Scale> &pos) const -> BWAPI::Point<T, Scale>& {
      pos.setMin(0, 0);
      pos.setMax(mapWidth() * 32 / Scale - 1, mapHeight() * 32 / Scale - 1);
      return pos;
    }

    /// <summary>Checks if this point is within the game's map bounds, if not, then it will set
    /// the x and y values to be within map bounds.</summary> For example, if x is less than 0,
    /// then x is set to 0.
    ///
    /// @returns A copy of the Position for convenience
    /// @see isValid
    template<typename T, int Scale>
    auto makeValid(BWAPI::Point<T, Scale> &&pos) const -> BWAPI::Point<T, Scale> {
      pos.setMin(0, 0);
      pos.setMax(mapWidth() * 32 / Scale - 1, mapHeight() * 32 / Scale - 1);
      return std::move(pos);
    }

    constexpr operator bool() const { return true; }

    /// <summary>Member access operator to retain the original Broodwar-> behaviour.</summary>
    constexpr Game *operator ->() { return this; }

    /// <summary>Member access operator to retain the original Broodwar-> behaviour.</summary>
    constexpr Game const *operator ->() const { return this; }

  private:
    mutable std::stringstream ss;
  public:
    /// <summary>Definition of ostream_manipulator type for convenience.</summary>
    typedef std::ostream& (*ostream_manipulator)(std::ostream&);

    /// <summary>Output stream operator for printing text to Broodwar.</summary> Using this
    /// operator invokes Game::printf when a newline character is encountered.
    template<typename T>
    Game& operator <<(T const &in) {
      ss << in;
      return *this;
    }

    /// @overload
    Game& operator <<(ostream_manipulator fn);

    /// <summary>Flushes the Broodwar stream, printing all text in the stream to the screen.</summary>
    void flush() {
      if (ss.str().empty()) return;
      this->printf("%s", ss.str().c_str());
      ss.str("");
    }

    std::unique_ptr<GameData> gameData;

    Client& client;
    
    Unitset const &getUnits(Player player) const;

    void addUnit(const UnitData& unitdata);
    void updatePlayer(const PlayerData& playerdata);
    void addForce(const ForceData& forceData);
    void addRegion(const RegionData& regionData);
    void addBullet(const BulletData& bulletData);
    void update();
    void clearEvents();
    void flushCommandOptimizer();

    private:
      FPSCounter fpsCounter;
      APMCounter apmCounter;
      CommandOptimizer commandOptimizer;
      UnitFinder unitFinder;

      Unitset accessibleUnits;//all units that are accessible (and definitely alive)
      Unitset pylons;
      Unitset minerals;
      Unitset geysers;
      Regionset regionsList;
      Bulletset bulletsset;
      Unitset selectedUnits;
      Unitset staticMinerals;
      Unitset staticGeysers;
      Unitset staticNeutralUnits;
      Unitset neutralUnits;
      Playerset _allies;
      Playerset _enemies;
      Playerset _observers;
      Playerset playerSet;
      Forceset forcesset;
      Position::list nukeDots;
      std::list< Event > events;
      Text::Size::Enum textSize = Text::Size::Default;

      void computePrimaryUnitSets();

      // We can't include bwapi sets here because they depend on Game
      std::set<Player, IDCompare> players;
      std::set<Unit, IDCompare> units;
      decltype(units)             initialUnits;
      std::set<Region, IDCompare> regions;
      std::set<Bullet, IDCompare> bullets;
      std::set<Force, IDCompare> forces;

      std::map<Player, Unitset> playerUnits;
  };
}

