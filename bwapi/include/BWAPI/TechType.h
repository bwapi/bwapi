#pragma once
#include <BWAPI/Type.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class WeaponType;
  class Order;
  class Race;

  namespace TechTypes
  {
    /// Enumeration of Tech Types
    namespace Enum
    {
      /// Enumeration of Tech Types
      enum Enum
      {
        Stim_Packs = 0,
        Lockdown,
        EMP_Shockwave,
        Spider_Mines,
        Scanner_Sweep,
        Tank_Siege_Mode,
        Defensive_Matrix,
        Irradiate,
        Yamato_Gun,
        Cloaking_Field,
        Personnel_Cloaking,
        Burrowing,
        Infestation,
        Spawn_Broodlings,
        Dark_Swarm,
        Plague,
        Consume,
        Ensnare,
        Parasite,
        Psionic_Storm,
        Hallucination,
        Recall,
        Stasis_Field,
        Archon_Warp,
        Restoration,
        Disruption_Web,
        Unused_26,
        Mind_Control,
        Dark_Archon_Meld,
        Feedback,
        Optical_Flare,
        Maelstrom,
        Lurker_Aspect,
        Unused_33,
        Healing,

        None = 44,
        Nuclear_Strike,
        Unknown,
        MAX
      };
    };
  }
  /// The TechType (or Technology Type, also referred to as an Ability) represents a Unit's ability
  /// which can be researched with UnitInterface::research or used with UnitInterface::useTech. In order for a Unit
  /// to use its own specialized ability, it must first be available and researched.
  class TechType : public Type<TechType, TechTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      TechType(int id = TechTypes::Enum::None);

      /// Retrieves the race that is required to research or use the TechType.
      ///
      /// @note There is an exception where @Infested_Kerrigan can use @Psi_Storm. This does not
      /// apply to the behavior of this function.
      ///
      /// @returns Race object indicating which race is designed to use this technology type.
      Race getRace() const;

      /// Retrieves the mineral cost of researching this technology.
      ///
      /// @returns Amount of minerals needed in order to research this technology.
      int mineralPrice() const;

      /// Retrieves the vespene gas cost of researching this technology.
      ///
      /// @returns Amount of vespene gas needed in order to research this technology.
      int gasPrice() const;

      /// Retrieves the number of frames needed to research the tech type.
      ///
      /// @returns The time, in frames, it will take for the research to complete.
      /// @see UnitInterface::getRemainingResearchTime
      int researchTime() const;

      /// Retrieves the amount of energy needed to use this TechType as an ability.
      ///
      /// @returns Energy cost of the ability.
      /// @see UnitInterface::getEnergy
      int energyCost() const;

      /// Retrieves the UnitType that can research this technology.
      ///
      /// @returns UnitType that is able to research the technology in the game.
      /// @retval UnitTypes::None If the technology/ability is either provided for free or never
      /// available.
      UnitType whatResearches() const;

      /// Retrieves the Weapon that is attached to this tech type. A technology's WeaponType
      /// is used to indicate the range and behaviour of the ability when used by a Unit.
      ///
      /// @returns WeaponType containing information about the ability's behavior.
      /// @retval WeaponTypes::None If there is no corresponding WeaponType.
      WeaponType getWeapon() const;

      /// Checks if this ability can be used on other units.
      ///
      /// @returns true if the ability can be used on other units, and false if it can not.
      bool targetsUnit() const;

      /// Checks if this ability can be used on the terrain (ground).
      ///
      /// @returns true if the ability can be used on the terrain.
      bool targetsPosition() const;

      /// Retrieves the set of all UnitTypes that are capable of using this ability.
      ///
      /// @returns Set of UnitTypes that can use this ability when researched.
      const UnitType::const_set& whatUses() const;

      /// Retrieves the Order that a Unit uses when using this ability.
      ///
      /// @returns Order representing the action a Unit uses to perform this ability
      Order getOrder() const;
  };
  /// Namespace containing tech types
  namespace TechTypes
  {
    /// Retrieves the set of all the TechTypes.
    ///
    /// @returns Set of all available TechTypes.
    const TechType::const_set& allTechTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const TechType x
    BWAPI_DECL(Stim_Packs);
    BWAPI_DECL(Lockdown);
    BWAPI_DECL(EMP_Shockwave);
    BWAPI_DECL(Spider_Mines);
    BWAPI_DECL(Scanner_Sweep);
    BWAPI_DECL(Tank_Siege_Mode);
    BWAPI_DECL(Defensive_Matrix);
    BWAPI_DECL(Irradiate);
    BWAPI_DECL(Yamato_Gun);
    BWAPI_DECL(Cloaking_Field);
    BWAPI_DECL(Personnel_Cloaking);
    BWAPI_DECL(Burrowing);
    BWAPI_DECL(Infestation);
    BWAPI_DECL(Spawn_Broodlings);
    BWAPI_DECL(Dark_Swarm);
    BWAPI_DECL(Plague);
    BWAPI_DECL(Consume);
    BWAPI_DECL(Ensnare);
    BWAPI_DECL(Parasite);
    BWAPI_DECL(Psionic_Storm);
    BWAPI_DECL(Hallucination);
    BWAPI_DECL(Recall);
    BWAPI_DECL(Stasis_Field);
    BWAPI_DECL(Archon_Warp);
    BWAPI_DECL(Restoration);
    BWAPI_DECL(Disruption_Web);
    BWAPI_DECL(Mind_Control);
    BWAPI_DECL(Dark_Archon_Meld);
    BWAPI_DECL(Feedback);
    BWAPI_DECL(Optical_Flare);
    BWAPI_DECL(Maelstrom);
    BWAPI_DECL(Lurker_Aspect);
    BWAPI_DECL(Healing);
    BWAPI_DECL(None);
    BWAPI_DECL(Nuclear_Strike);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  };
}
