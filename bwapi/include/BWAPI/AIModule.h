#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/TournamentAction.h>
#include <BWAPI/Unit.h>
#include <string>

namespace BWAPI
{
  // Forward declarations
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class Color;

  /// AIModule is a virtual class that is intended to be implemented or inherited by a custom AI
  /// class. The Broodwar interface is guaranteed to be initialized if any of these predefined
  /// interface functions are invoked by BWAPI.
  ///
  /// @warning 
  ///   Using BWAPI in any thread other than the thread that invokes these functions can produce
  ///   unexpected behaviour and possibly crash your bot. Multi-threaded AIs are possible so
  ///   long as all BWAPI interaction is limited to the calling thread.
  ///
  /// @note
  ///   Replays are considered games and call all of the same callbacks as a standard game would.
  class AIModule
  {
    public:
      AIModule();
      virtual ~AIModule();

      /// Called only once at the beginning of a game. It is intended that the AI module do any
      /// data initialization in this function.
      ///
      /// @warning
      ///   Using the Broodwar interface before this function is called can produce undefined
      ///   behaviour and crash your bot. (During static initialization of a class for example)
      virtual void onStart();

      /// Called once at the end of a game.
      ///
      /// @param isWinner
      ///   A boolean value to determine if the current player has won the match. This value will
      ///   be true if the current player has won, and false if either the player has lost or the
      ///   game is actually a replay.
      virtual void onEnd(bool isWinner);

      /// Called once for every execution of a logical frame in Broodwar.
      /// Users will generally put most of their code in this function.
      virtual void onFrame();

      /// Called when the user attempts to send a text message. This function can be used to make
      /// the bot execute text commands entered by the user for debugging purposes.
      ///
      /// @param text
      ///   A string containing the exact text message that was sent by the user.
      ///
      /// @note
      ///   If Flag::UserInput is disabled, then this function is not called.
      virtual void onSendText(std::string text);

      /// Called when the client receives a message from another Player. This function can be used
      /// to retrieve information from allies in team games, or just to respond to other players.
      ///
      /// @param player
      ///   The Player interface object representing the owner of the text message.
      /// @param text
      ///   The text message that the \p player sent.
      ///
      /// @note
      ///   Messages sent by the current player will never invoke this function.
      virtual void onReceiveText(Player player, std::string text);

      /// Called when a Player leaves the game. All of their units are automatically given to the
      /// neutral player with their colour and alliance parameters preserved.
      ///
      /// @param player
      ///   The Player interface object representing the player that left the game.
      virtual void onPlayerLeft(Player player);

      /// Called when a @Nuke has been launched somewhere on the map.
      ///
      /// @param target
      ///   A Position object containing the target location of the @Nuke. If the target location
      ///   is not visible and Flag::CompleteMapInformation is disabled, then target will be
      ///   Positions::Unknown.
      virtual void onNukeDetect(Position target);

      /// Called when a Unit becomes accessible.
      ///
      /// @param unit
      ///   The Unit interface object representing the unit that has just become accessible.
      ///
      /// @note
      ///   This function INCLUDES the state of Flag::CompleteMapInformation.
      ///
      /// @see onUnitShow
      virtual void onUnitDiscover(Unit unit);

      /// Called when a Unit becomes inaccessible.
      ///
      /// @param unit
      ///   The Unit interface object representing the unit that has just become inaccessible.
      ///
      /// @note
      ///   This function INCLUDES the state of Flag::CompleteMapInformation.
      ///
      /// @see onUnitHide
      virtual void onUnitEvade(Unit unit);

      /// Called when a previously invisible unit becomes visible.
      ///
      /// @param unit
      ///   The Unit interface object representing the unit that has just become visible.
      ///
      /// @note
      ///   This function EXCLUDES the state of Flag::CompleteMapInformation.
      ///
      /// @see onUnitDiscover
      virtual void onUnitShow(Unit unit);

      /// Called just as a visible unit is becoming invisible.
      ///
      /// @param unit
      ///   The Unit interface object representing the unit that is about to go out of scope.
      ///
      /// @note
      ///   This function EXCLUDES the state of Flag::CompleteMapInformation.
      ///
      /// @see onUnitEvade
      virtual void onUnitHide(Unit unit);

      /// Called when any unit is created.
      ///
      /// @param unit
      ///   The Unit interface object representing the unit that has just been created.
      ///
      /// @note
      ///   Due to the internal workings of Broodwar, this function excludes Zerg morphing and
      ///   the construction of structures over a @Geyser .
      ///
      /// @see onUnitMorph
      virtual void onUnitCreate(Unit unit);

      /// Called when a unit is removed from the game either through death or other means.
      ///
      /// @param unit
      ///   Unit object representing the unit that has just been destroyed or otherwise completely
      ///   removed from the game.
      ///
      /// @note
      ///   When a @Drone morphs into an @Extractor, the @Drone is removed from the game and the
      ///   @Geyser morphs into an @Extractor.
      ///
      /// @note
      ///   If a unit is visible and destroyed, then onUnitHide is called just before this.
      virtual void onUnitDestroy(Unit unit);

      /// Called when a unit changes its UnitType. For example, when a @Drone transforms into a
      /// @Hatchery, a @SiegeTank uses @SiegeMode , or a @Geyser receives a @Refinery .
      ///
      /// @param unit
      ///   Unit object representing the unit that had its UnitType change.
      ///
      /// @note
      ///   This is NOT called if the unit type changes to or from UnitTypes::Unknown.
      virtual void onUnitMorph(Unit unit);

      /// Called when a unit changes ownership. This occurs when the @Protoss ability @MindControl
      /// is used, or if a unit changes ownership in @UseMapSettings .
      ///
      /// @param unit
      ///   Unit interface object pertaining to the unit that has just changed ownership.
      virtual void onUnitRenegade(Unit unit);

      /// Called when the state of the Broodwar game is saved to file.
      ///
      /// @param gameName
      ///   A String object containing the file name that the game was saved as.
      virtual void onSaveGame(std::string gameName);

      /// Called when the state of a unit changes from incomplete to complete.
      ///
      /// @param unit
      ///   The Unit object representing the Unit that has just finished training or constructing.
      virtual void onUnitComplete(Unit unit);
  };

  /// TournamentModule is a virtual class that is intended to be implemented or inherited by a
  /// custom Tournament class. Like AIModule, the Broodwar interface is guaranteed to be
  /// initialized if any of these predefined interface functions are invoked by BWAPI.
  ///
  /// @note
  ///   The TournamentModule is to be implemented by Tournament Modules ONLY. A standard AI module
  ///   should never implement it. The Tournament Module is invoked only if it is explicitly
  ///   defined in the configuration file. Tournament Modules also contain an AI Module interface
  ///   so that it can monitor the time an AI module spent during its calls using
  ///   Game::getLastEventTime.
  class TournamentModule
  {
    public:
      TournamentModule();
      virtual ~TournamentModule();

      /// This function regulates the functions involving game settings that an AI module is
      /// allowed to execute. For example, if the tournament forbids the enabling of
      /// Flag::CompleteMapInformation, then this function can deny the request.
      ///
      /// @param actionType
      ///   An ActionID containing the action that the AI module is requesting.
      ///
      /// @param parameter
      ///   An optional parameter that pertains to certain action requests. For example, if 
      ///   \p actionType is Tournament::ActionID::SendText, then \p parameter is a pointer to a
      ///   null-terminated character array containing the message. If \p actionType is
      ///   Tournament::ActionID::SetLocalSpeed, then parameter is a pointer to an integer.
      virtual bool onAction(BWAPI::Tournament::ActionID actionType, void *parameter = nullptr);

      /// This function is called if the current player is chosen to advertise the BWAPI revision.
      /// When tournament mode is enabled, to reduce spam, only one bot sends the revision message.
      /// The bot that is chosen depends on its player ID. Only one of the bots can call this
      /// function during a tournament game.
      virtual void onFirstAdvertisement();
  };
}
