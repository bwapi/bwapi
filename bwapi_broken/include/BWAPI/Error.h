#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class UnitType;

  /** Functions in BWAPI may set an error code. To retrieve the error code, call Game::getLastError. */
  class Error
  {
    public:
      Error();
      Error(int id);
      Error(const Error& other);
      Error& operator=(const Error& other);
      bool operator==(const Error& other) const;
      bool operator!=(const Error& other) const;
      bool operator<(const Error& other) const;

      /** Returns a unique ID for this error. */
      int getID() const;

      /** Returns the name of the error. For example Errors::Insufficient_Minerals?.toString() will return a
       * std::string object containing "Insufficient Minerals". */
      std::string toString() const;
    private:
      int id;
  };
  namespace Errors
  {
    /** Given the name of an error, this function will return the error code. For example:
     * Errors::getError("Unbuildable Location") will return Errors::Unbuildable_Location?. */
    Error getError(std::string name);

    /** The set of all the error codes. */
    std::set<Error>& allErrors();

    void init();

    /** Returned if you try to order a unit or get information from a unit that no longer exists. */
    extern const Error Unit_Does_Not_Exist;

    /** Returned if you try to retrieve information about a unit that is not currently visible or is dead. */
    extern const Error Unit_Not_Visible;

    /** Returned when attempting to order a unit that BWAPI does not own (i.e. can't order enemy army to go
     *  away) */
    extern const Error Unit_Not_Owned;

    /** Returned when trying to order a unit to do something when it is performing another order or is in a
     * state which prevents it from performing the desired order. For example, ordering a Terran Engineering
     * Bay to upgrade something while it is already upgrading something else will return this error.
     * Similarly, trying to train units from a factory that is lifted will return this error. */
    extern const Error Unit_Busy;

    /** Returned if you do something weird like try to build a Pylon with an SCV, or train Vultures in a
     * Barracks, or order a Hydralisk to lay a spider mine. */
    extern const Error Incompatible_UnitType;

    /** Returned if when trying to use a tech type with the wrong Unit::useTech method. */
    extern const Error Incompatible_TechType;

    /** Returned if you try to research something that is already researched. */
    extern const Error Already_Researched;

    /** Returned if you try to upgrade something that is already fully upgraded. */
    extern const Error Fully_Upgraded;

    /** Returned if you try to research something that is already being researched. */
    extern const Error Currently_Researching;

    /** Returned if you try to upgrade something that is already being upgraded. */
    extern const Error Currently_Upgrading;

    /** Returned if you try to train or build something without enough minerals. */
    extern const Error Insufficient_Minerals;

    /** Returned if you try to train or build something without enough vespene gas. */
    extern const Error Insufficient_Gas;

    /** Returned if you try to train something without enough supply. */
    extern const Error Insufficient_Supply;

    /** Returned if you to do something like try to order a Defiler to cast a Dark Swarm without enough
     * energy. */
    extern const Error Insufficient_Energy;

    /** Returned if you do something like try to train Medics when you don't have an Academy, or try to lay
     * Spider Mines before spider mines have been researched. */
    extern const Error Insufficient_Tech;

    /** Returned if you do something like try to lay Spider Mines when your Vulture is out of Spider Mines.
     * Same thing with Reavers and Scarabs. */
    extern const Error Insufficient_Ammo;

    /** Returned if you try to build something on unbuildable terrain (either from the buildability map data
     * or if a unit is in the way). For build tiles that are not visible, we could just use the buildability
     * map data and assume that no units are blocking it (to prevent cheating). */
    extern const Error Insufficient_Space;

    /** Returned if you order an immovable unit, like a Protoss Photon Cannon, to attack a unit that is out
     * of range. */
    extern const Error Unbuildable_Location;

    /** Returned if you order an immovable unit, like a Protoss Photon Cannon, to attack a unit that is out of
     * range.*/
    extern const Error Out_Of_Range;

    /** Returned if you do something like order a Vulture to attack a flying unit. */
    extern const Error Unable_To_Hit;

    /** Returned if you try to get information that is not allowed with the given flag settings. For example,
     * trying to read the enemy's resource counts while the CompleteMapInformation?  flag is not enabled will
     * return this error. Similarly, trying to read the coordinates of the screen or mouse while the UserInput
     *  flag is not enabled will also return this error. */
    extern const Error Access_Denied;

    /** Used when no error has been encountered. */
    extern const Error None;

    /** Used when the error code is not recognized or can not be determined. */
    extern const Error Unknown;
  }
}
