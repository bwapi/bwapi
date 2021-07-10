#pragma once
#include <BWAPI/Type.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class UnitType;
  class WeaponType;
  class Order;
  class Race;

  /// <summary>Namespace containing tech types.</summary>
  /// @see TechType
  namespace TechTypes
  {
    /// <summary>Enumeration of Tech Types.</summary>
    /// @see TechType
    namespace Enum
    {
      /// <summary>Enumeration of Tech Types.</summary>
      /// @see TechType
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
  /// <summary>The TechType (or Technology Type, also referred to as an Ability) represents a Unit's ability
  /// which can be researched with UnitInterface::research or used with UnitInterface::useTech.</summary>
  /// In order for a Unit to use its own specialized ability, it must first be available and researched.
  ///
  /// @see TechTypes
  /// @ingroup TypeClasses
  class TechType : public Type<TechType, TechTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      constexpr TechType(int id = TechTypes::Enum::None) : Type(id) {}

      /// <summary>Retrieves the race that is required to research or use the TechType.</summary>
      ///
      /// @note There is an exception where @Infested_Kerrigan can use @Psi_Storm. This does not
      /// apply to the behavior of this function.
      ///
      /// @returns Race object indicating which race is designed to use this technology type.
      Race getRace() const;

      /// <summary>Retrieves the mineral cost of researching this technology.</summary>
      ///
      /// @returns Amount of minerals needed in order to research this technology.
      int mineralPrice() const;

      /// <summary>Retrieves the vespene gas cost of researching this technology.</summary>
      ///
      /// @returns Amount of vespene gas needed in order to research this technology.
      int gasPrice() const;

      /// <summary>Retrieves the number of frames needed to research the tech type.</summary>
      ///
      /// @returns The time, in frames, it will take for the research to complete.
      /// @see UnitInterface::getRemainingResearchTime
      int researchTime() const;

      /// <summary>Retrieves the amount of energy needed to use this TechType as an ability.</summary>
      ///
      /// @returns Energy cost of the ability.
      /// @see UnitInterface::getEnergy
      int energyCost() const;

      /// <summary>Retrieves the UnitType that can research this technology.</summary>
      ///
      /// @returns UnitType that is able to research the technology in the game.
      /// @retval UnitTypes::None If the technology/ability is either provided for free or never
      /// available.
      UnitType whatResearches() const;

      /// <summary>Retrieves the Weapon that is attached to this tech type.</summary>
      /// A technology's WeaponType is used to indicate the range and behaviour of the ability
      /// when used by a Unit.
      ///
      /// @returns WeaponType containing information about the ability's behavior.
      /// @retval WeaponTypes::None If there is no corresponding WeaponType.
      WeaponType getWeapon() const;

      /// <summary>Checks if this ability can be used on other units.</summary>
      ///
      /// @returns true if the ability can be used on other units, and false if it can not.
      bool targetsUnit() const;

      /// <summary>Checks if this ability can be used on the terrain (ground).</summary>
      ///
      /// @returns true if the ability can be used on the terrain.
      bool targetsPosition() const;

      /// <summary>Retrieves the set of all UnitTypes that are capable of using this ability.</summary>
      ///
      /// @returns Set of UnitTypes that can use this ability when researched.
      const UnitType::set& whatUses() const;

      /// <summary>Retrieves the Order that a Unit uses when using this ability.</summary>
      ///
      /// @returns Order representing the action a Unit uses to perform this ability
      Order getOrder() const;

      /// <summary>Retrieves the UnitType required to research this technology.</summary>
      /// The required unit type must be a completed unit owned by the player researching the
      /// technology.
      ///
      /// @returns UnitType that is needed to research this tech type.
      /// @retval UnitTypes::None if no unit is required to research this tech type.
      ///
      /// @see PlayerInterface::completedUnitCount
      ///
      /// @since 4.1.2
      UnitType requiredUnit() const;
  };

  /// @ingroup Types
  namespace TechTypes
  {
    /// <summary>Retrieves the set of all the TechTypes.</summary>
    ///
    /// @returns Set of all available TechTypes.
    const TechType::set& allTechTypes();

    /// @name Terran Abilities
    /// @{
    constexpr TechType Stim_Packs{Enum::Stim_Packs};
    constexpr TechType Lockdown{Enum::Lockdown};
    constexpr TechType EMP_Shockwave{Enum::EMP_Shockwave};
    constexpr TechType Spider_Mines{Enum::Spider_Mines};
    constexpr TechType Scanner_Sweep{Enum::Scanner_Sweep};
    constexpr TechType Tank_Siege_Mode{Enum::Tank_Siege_Mode};
    constexpr TechType Defensive_Matrix{Enum::Defensive_Matrix};
    constexpr TechType Irradiate{Enum::Irradiate};
    constexpr TechType Yamato_Gun{Enum::Yamato_Gun};
    constexpr TechType Cloaking_Field{Enum::Cloaking_Field};
    constexpr TechType Personnel_Cloaking{Enum::Personnel_Cloaking};
    constexpr TechType Restoration{Enum::Restoration};
    constexpr TechType Optical_Flare{Enum::Optical_Flare};
    constexpr TechType Healing{Enum::Healing};
    constexpr TechType Nuclear_Strike{Enum::Nuclear_Strike};
    /// @}
    /// @name Zerg Abilities
    /// @{
    constexpr TechType Burrowing{Enum::Burrowing};
    constexpr TechType Infestation{Enum::Infestation};
    constexpr TechType Spawn_Broodlings{Enum::Spawn_Broodlings};
    constexpr TechType Dark_Swarm{Enum::Dark_Swarm};
    constexpr TechType Plague{Enum::Plague};
    constexpr TechType Consume{Enum::Consume};
    constexpr TechType Ensnare{Enum::Ensnare};
    constexpr TechType Parasite{Enum::Parasite};
    constexpr TechType Lurker_Aspect{Enum::Lurker_Aspect};
    /// @}
    /// @name Protoss Abilities
    /// @{
    constexpr TechType Psionic_Storm{Enum::Psionic_Storm};
    constexpr TechType Hallucination{Enum::Hallucination};
    constexpr TechType Recall{Enum::Recall};
    constexpr TechType Stasis_Field{Enum::Stasis_Field};
    constexpr TechType Archon_Warp{Enum::Archon_Warp};
    constexpr TechType Disruption_Web{Enum::Disruption_Web};
    constexpr TechType Mind_Control{Enum::Mind_Control};
    constexpr TechType Dark_Archon_Meld{Enum::Dark_Archon_Meld};
    constexpr TechType Feedback{Enum::Feedback};
    constexpr TechType Maelstrom{Enum::Maelstrom};
    /// @}

    constexpr TechType None{Enum::None};
    constexpr TechType Unknown{Enum::Unknown};
  };

  static_assert(sizeof(TechType) == sizeof(int), "Expected type to resolve to primitive size.");
}
