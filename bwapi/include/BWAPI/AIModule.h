#pragma once
#include <string>
#include <BWAPI/Position.h>
namespace BWAPI
{
  class Unit;
  class Player;

  /** AIModule is a virtual class that is intended to be implemented or inherited by a custom AI class.
   *
   * \note
   * Using BWAPI in a different thread than the default one will produce unexpected results and possibly crash
   * the program. Multi-threaded AIs are possible so long as all BWAPI interaction is limited to the default
   * thread (during one of the call-backs). */
  class AIModule
  {
    public:
      AIModule();
      virtual ~AIModule();

      /** BWAPI calls this at the start of a match. Typically an AI will execute set up code in this method
       * (initialize data structures, load build orders, etc). */
      virtual void onStart();

      /** BWAPI calls this at the end of the match. isWinner will be true if the AIModule won the game. If the
       * game is a replay, isWinner will always be false. */
      virtual void onEnd(bool isWinner);

      /** BWAPI calls this on every logical frame in the game. */
      virtual void onFrame();

      /** If Flag::UserInput is enabled, BWAPI will call this each time a user enters a message into the chat.
       * */
      virtual bool onSendText(std::string text);

      /** BWAPI calls this when a player leaves the game. */
      virtual void onPlayerLeft(Player* player);

      /** BWAPI calls this when another player sends a message. */
      virtual void onReceiveText(Player* player, std::string text);

      /** BWAPI calls this when a nuclear launch has been detected. If the target position is visible, or if
       * Complete Map Information is enabled, the target position will also be provided. If Complete Map
       * Information is disabled and the target position is not visible, target will be set to
       * Positions::Unknown. */
      virtual void onNukeDetect(Position target);

      /** BWAPI calls this when a unit is created. Note that this is NOT called when a unit changes type
       * (such as larva into egg or egg into drone). Building a refinery/assimilator/extractor will not
       * produce an onUnitCreate call since the vespene geyser changes to the unit type of the
       * refinery/assimilator/extractor. If Complete Map Information is enabled, this will also be called for
       * new units that are hidden by the fog of war. If the unit is visible upon creation, onUnitShow will be
       * called shortly after onUnitCreate is called. */
      virtual void onUnitCreate(Unit* unit);

      /** BWAPI calls this when a unit dies or otherwise removed from the game (i.e. a mined out mineral
       * patch). When a zerg drone becomes an extractor, the Vespene geyser changes to the Zerg Extractor type
       * and the drone is removed. If Complete Map Information is enabled, this will also be called for units
       * that are hidden by the fog of war. If a unit that was visible gets destroyed, onUnitHide will be
       * called right before onUnitDestroy is called. */
      virtual void onUnitDestroy(Unit* unit);

      /** BWAPI calls this when a unit changes type, such as from a Zerg Drone to a Zerg Hatchery, or from a
       * Terran Siege Tank Tank Mode to Terran Siege Tank Siege Mode. This is not called when the type changes
       * to or from UnitTypes::Unknown (which happens when a unit becomes visible or invisible). */
      virtual void onUnitMorph(Unit* unit);

      /** BWAPI calls this the instant a previously invisible unit becomes visible. The complete map
       * information flag has no effect on this callback. */
      virtual void onUnitShow(Unit* unit);

      /** BWAPI calls this right before a unit becomes invisible, so if you want your non-cheating AI to
       * remember where it last saw a unit, this callback would be a good place to implement it. The complete
       * map information flag has no effect on this callback. */
      virtual void onUnitHide(Unit* unit);

      /** BWAPI calls this when an accessible unit changes ownership. */
      virtual void onUnitRenegade(Unit* unit);

      // TODO: Add Doxygen documentation
      virtual void onSaveGame(std::string gameName);
  };
}
