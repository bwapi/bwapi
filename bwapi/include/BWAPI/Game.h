#pragma once

#include <list>
#include <map>
#include <set>

#include <BWAPI/Color.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Error.h>
#include <BWAPI/Event.h>
#include <BWAPI/Flag.h>
#include <BWAPI/GameType.h>
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>
#include <BWAPI/Latency.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Input.h>
namespace BWAPI
{
  class Force;
  class Player;
  class Unit;
  class Bullet;

  /** The abstract Game class is implemented by BWAPI and offers many methods for retrieving information
   * about the current Broodwar game, including the set of players, units, map information, as well as
   * information about the user, such as mouse position, screen position, and the current selection of
   * units. */
  class Game
  {
    public :
      /** Returns the set of all forces in the match. */
      virtual std::set< Force* >& getForces() = 0;

      /** Returns the set of all players in the match. Note that this includes the Neutral player, which owns
       * all the neutral units such as minerals, critters, etc. */
      virtual std::set< Player* >& getPlayers() = 0;

      /** Returns all the visible units. If Flag::CompleteMapInformation is enabled, the set of all units
       * is returned, not just visible ones. Note that units inside refineries are not included in this set
       * yet. */
      virtual std::set< Unit* >& getAllUnits() = 0;

      /** Returns the set of all accessible mineral patches. */
      virtual std::set< Unit* >& getMinerals() = 0;

      /** Returns the set of all accessible vespene geysers. */
      virtual std::set< Unit* >& getGeysers() = 0;

      /** Returns the set of all accessible neutral units. */
      virtual std::set< Unit* >& getNeutralUnits() = 0;

      /** Returns the set of all mineral patches (including mined out and other inaccessible ones). */
      virtual std::set< Unit* >& getStaticMinerals() = 0;

      /** Returns the set of all vespene geysers (including mined out and other inaccessible ones). */
      virtual std::set< Unit* >& getStaticGeysers() = 0;

      /** Returns the set of all neutral units (including mined out and other inaccessible ones). */
      virtual std::set< Unit* >& getStaticNeutralUnits() = 0;

      /** Returns all visible bullets. If Flag::CompleteMapInformation is enabled, the set of all bullets is
       * returned, not just visible ones. */

      virtual std::set< Bullet* >& getBullets() = 0;

      /** Returns the list of events */
      virtual std::list< Event >& getEvents() = 0;

      /** Returns a pointer to a Unit given an index. */
      virtual Unit *indexToUnit(int unitIndex) = 0;

      /** Returns the game type */
      virtual GameType getGameType() = 0;

      /** Returns the amount of latency the current game has. Currently only returns Latency::SinglePlayer,
       * Latency::LanLow, Latency::LanMedium, or Latency::LanHigh. */
      virtual int getLatency() = 0;

      /** Returns the number of logical frames since the match started. If the game is paused,
       * Game::getFrameCount will not increase however AIModule::onFrame will still be called while paused.
       * On Fastest, there are about 23.8 - 24 frames per second. */
      virtual int getFrameCount() = 0;

      /** Returns the Frames Per Second (FPS) that the game is currently running at */
      virtual int getFPS() = 0;
      virtual double getAverageFPS() = 0;

      /** Returns the horizontal coordinate of the mouse on the screen. Returns 0 if Flag::UserInput? is
       * disabled. */
      virtual int getMouseX() = 0;

      /** Returns the vertical coordinate of the mouse on the screen. Returns 0 if Flag::UserInput? is
       * disabled. */
      virtual int getMouseY() = 0;

      /** Returns the position of the mouse on the screen. Returns Positions::Unknown if Flag::UserInput is
       * disabled. */
      virtual BWAPI::Position getMousePosition() = 0;

      /** Returns true if the specified mouse button is pressed. Returns false if Flag::UserInput is
       * disabled. */
      virtual bool getMouseState(MouseButton button) = 0;

      /** \copydoc getMouseState(MouseButton) */
      virtual bool getMouseState(int button) = 0;

      /** Returns true if the specified key is pressed. Returns false if Flag::UserInput is disabled.
       * Unfortunately this does not read the raw keyboard input yet - when you hold down a key, the
       * getKeyState function is true for a frame, then false for a few frames, and then alternates between
       * true and false (as if you were holding down the key in a text box). Hopefully this will be fixed in
       * a later version. */
      virtual bool getKeyState(Key key) = 0;

      /** \copydoc getKeyState(Key) */
      virtual bool getKeyState(int key) = 0;

      /** Returns the horizontal coordinate of the screen on the map. Returns 0 if Flag::UserInput is disabled. */
      virtual int getScreenX() = 0;

      /** Returns the vertical coordinate of the screen on the map. Returns 0 if Flag::UserInput is disabled. */
      virtual int getScreenY() = 0;

      /** Returns the position of the top left corner of the screen on the map. Returns Positions::Unknown if
       * Flag::UserInput is disabled. */
      virtual BWAPI::Position getScreenPosition() = 0;

      /** Moves the screen to the given position on the map. The position specified where the top left corner
       * of the screen will be. */
      virtual void setScreenPosition(int x, int y) = 0;

      /** \copydoc setScreenPosition(int, int) */
      virtual void setScreenPosition(BWAPI::Position p) = 0;

      /** Pings the given position on the minimap. */
      virtual void pingMinimap(int x, int y) = 0;

      /** \copydoc pingMinimap(int, int) */
      virtual void pingMinimap(BWAPI::Position p) = 0;

      /** Returns true if the given flag has been enabled. Note that flags can only be enabled at the
       * beginning of a match, during the AIModule::onStart callback. */
      virtual bool isFlagEnabled(int flag) = 0;

      /** Enables the specified flag. Note that flags can only be enabled at the beginning of a match, during
       * the AIModule::onStart callback. */
      virtual void enableFlag(int flag) = 0;

      /** Returns the set of units that are on the given build tile. Only returns accessible units on
       * accessible tiles. */
      virtual std::set<Unit*>& unitsOnTile(int x, int y) = 0;

      /** Returns the last error that was set. If you try to order enemy units around, or morph bunkers into
       * lurkers, BWAPI will set error codes, which can be retrieved using this function. */
      virtual Error getLastError() const = 0;

      /** Returns the width of the current map, in build tile units. To get the width of the current map in
       * walk tile units, multiply by 4. To get the width of the current map in Position units, multiply by
       * TILE_SIZE (which is 32). */
      virtual int mapWidth() = 0;

      /** Returns the height of the current map, in build tile units. To get the height of the current map in
       * walk tile units, multiply by 4. To get the height of the current map in Position units, multiply by
       * TILE_SIZE (which is 32). */
      virtual int mapHeight() = 0;

      /** Returns the file name of the current map. */
      virtual std::string mapFilename() = 0;

      /** Returns the name/title of the current map. */
      virtual std::string mapName() = 0;

      /** Returns a unique identifier for the given map data that does not depend on the file name. */
      virtual int getMapHash() = 0;

      /** Returns the ground height of the given build tile. 0 = normal, 1 = high ground.  2 = very high ground. */
      virtual int  getGroundHeight(int x, int y) = 0;

      /** Returns true if the specified walk tile is walkable. The values of x and y are in walk tile
       * coordinates (different from build tile coordinates). Note that this just uses the static map data.
       * You will also need to make sure no ground units are on the coresponding build tile to see if its
       * currently walkable. To do this, see unitsOnTile. */
      virtual bool isWalkable(int x, int y) = 0;

      /** Returns true if the specified build tile is buildable. Note that this just uses the static map data.
       * You will also need to make sure no ground units on the tile to see if its currently buildable. To do
       * this, see unitsOnTile. */
      virtual bool isBuildable(int x, int y) = 0;

      /** Returns true if the specified build tile is visible. If the tile is concealed by fog of war, the
       * function will return false. */
      virtual bool isVisible(int x, int y) = 0;

      /** Returns true if the specified build tile has been explored (i.e. was visible at some point in the
       * match). */
      virtual bool isExplored(int x, int y) = 0;

      /** Returns true if the specified build tile has zerg creep on it. If the tile is concealed by fog of
       * war, the function will return false. */
      virtual bool hasCreep(int x, int y) = 0;

      /** Returns true if the given build location is powered by a nearby friendly pylon. */
      virtual bool hasPower(int x, int y, int tileWidth, int tileHeight) = 0;

      /** Returns the ground height of the given build tile. 0 = normal, 1 = high ground. 2 = very high ground. */
      virtual int  getGroundHeight(TilePosition position) = 0;

      /** \copydoc isBuildable(int, int) */
      virtual bool isBuildable(TilePosition position) = 0;
      /** \copydoc isVisible(int, int) */
      virtual bool isVisible(TilePosition position) = 0;
      /** \copydoc isExplored(int, int) */
      virtual bool isExplored(TilePosition position) = 0;
      /** \copydoc hasCreep(int, int) */
      virtual bool hasCreep(TilePosition position) = 0;
      /** \copydoc hasPower(int, int, int, int) */
      virtual bool hasPower(TilePosition position, int tileWidth, int tileHeight) = 0;

      /** Returns true if the given unit type can be built at the given build tile position. Note the tile
       * position specifies the top left tile of the building. If builder is not null, the unit will be
       * discarded when determining whether or not any ground units are blocking the build location. */
      virtual bool canBuildHere(Unit *builder, TilePosition position, UnitType type) = 0;

      /** Returns true if the AI player has enough resources, supply, tech, and required units in order to
       * make the given unit type. If builder is not null, canMake will return true only if the builder unit
       * can build the given unit type. */
      virtual bool canMake(Unit *builder, UnitType type) = 0;

      /** Returns true if the AI player has enough resources required to research the given tech type. If unit
       * is not null, canResearch will return true only if the given unit can research the given tech type. */
      virtual bool canResearch(Unit *unit, TechType type) = 0;

      /** Returns true if the AI player has enough resources required to upgrade the given upgrade type. If
       * unit is not null, canUpgrade will return true only if the given unit can upgrade the given upgrade
       * type. */
      virtual bool canUpgrade(Unit *unit, UpgradeType type) = 0;

      /** Returns the set of starting locations for the given map. To determine the starting location for the
       * players in the current match, see Player::getStartLocation. */
      virtual std::set< TilePosition >& getStartLocations() = 0;

      /** Prints text on the screen. Text is not sent to other players in multiplayer games. */
      virtual void printf(const char *text, ...) = 0;

      /** Sends text to other players - as if it were entered in chat. In single player games and replays,
       * this will just print the text on the screen. If the game is a single player match and not a replay,
       * then this function can be used to execute cheat codes, i.e. Broodwar->sendText("show me the money"). */
      virtual void sendText(const char *text, ...) = 0;

      /** Used to change the race while in a lobby. Note that there is no onLobbyEnter callback yet, so this
       * function cannot be used at this time. */
      virtual void changeRace(Race race) = 0;

      /** Returns true if Broodwar is in a game. Returns false for lobby and menu screens */
      virtual bool isInGame() = 0;

      /** Returns true if Broodwar is in a multiplayer game. Returns false for single player games and
       * replays. */
      virtual bool isMultiplayer() = 0;

      /** Returns true if Broodwar is in a BNet multiplayer game.
      */
      virtual bool isBattleNet() = 0;

      /** Returns true if Broodwar is paused. If the game is paused, Game::getFrameCount will continue to
       * increase and AIModule::onFrame will still be called while paused. */
      virtual bool isPaused() = 0;

      /** Returns true if Broodwar is in a replay. */
      virtual bool isReplay() = 0;

      /** Used to start the game while in a lobby. Note that there is no onLobbyEnter callback yet, so this
       * function cannot be used at this time. */
      virtual void startGame() = 0;

      /** Pauses the game. If the game is paused, Game::getFrameCount will not increase however
       * AIModule::onFrame will still be called while paused. */
      virtual void pauseGame() = 0;

      /** Resumes the game. */
      virtual void resumeGame() = 0;

      /** Leaves the current match and goes to the after-game stats screen. */
      virtual void leaveGame() = 0;

      /** Restarts the match. Works the same way as if you restarted the match from the menu screen. Only
       * available in single player mode. */
      virtual void restartGame() = 0;

      /** Sets the speed of the game to the given number. Lower numbers are faster. 0 is the fastest speed
       * StarCraft can handle (which is about as fast as the fastest speed you can view a replay at). Any
       * negative value will reset the speed to the StarCraft default. */
      virtual void setLocalSpeed(int speed = -1) = 0;

      /** Returns the set of units currently selected by the user in the GUI. If Flag?::UserInput? was not
       * enabled during the AIModule::onStart callback, this function will always return an empty set. */
      virtual std::set<Unit*>& getSelectedUnits() = 0;

      /** Returns a pointer to the player that BWAPI controls. In replays this will return null. */
      virtual Player* self() = 0;

      /** Returns a pointer to the enemy player. If there is more than one enemy, this returns a pointer to
       * just one enemy (see getPlayers  and Player::isEnemy to get the other enemies). In replays this will
       * return NULL. */
      virtual Player* enemy() = 0;

      virtual void setTextSize(int size = 1) = 0;
      /** Draws text on the screen at the given position. Text can be drawn in different colors by using the
       * following control characters: TODO: add image from wiki.*/
      virtual void drawText(int ctype, int x, int y, const char* text, ...) = 0;
      virtual void drawTextMap(int x, int y, const char* text, ...) = 0;
      virtual void drawTextMouse(int x, int y, const char* text, ...) = 0;
      virtual void drawTextScreen(int x, int y, const char* text, ...) = 0;

      /** Draws a box on the screen, with the given color. If isSolid is true, the entire box will be
       * rendered, otherwise just the outline will be drawn. */
      virtual void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;
      virtual void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;
      virtual void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;
      virtual void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;

      /** Draws a triangle on the screen. If isSolid is true, a solid triangle is drawn, otherwise just the
       * outline of the triangle will be drawn. */
      virtual void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;
      virtual void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;
      virtual void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;
      virtual void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;

      /** Draws a circle on the screen, with the given color. If isSolid is true, a solid circle is drawn,
       * otherwise just the outline of a circle will be drawn. */
      virtual void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false) = 0;
      virtual void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false) = 0;
      virtual void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false) = 0;
      virtual void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false) = 0;

      /** Draws an ellipse on the screen, with the given color. If isSolid is true, a solid ellipse is drawn,
       * otherwise just the outline of an ellipse will be drawn. */
      virtual void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;
      virtual void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;
      virtual void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;
      virtual void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;

      /** Draws a dot on the screen at the given position with the given color. */
      virtual void drawDot(int ctype, int x, int y, Color color) = 0;
      virtual void drawDotMap(int x, int y, Color color) = 0;
      virtual void drawDotMouse(int x, int y, Color color) = 0;
      virtual void drawDotScreen(int x, int y, Color color) = 0;

      /** Draws a line on the screen from (x1,y1) to (x2,y2) with the given color. */
      virtual void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color) = 0;
      virtual void drawLineMap(int x1, int y1, int x2, int y2, Color color) = 0;
      virtual void drawLineMouse(int x1, int y1, int x2, int y2, Color color) = 0;
      virtual void drawLineScreen(int x1, int y1, int x2, int y2, Color color) = 0;
  };
  extern Game* Broodwar;
}
