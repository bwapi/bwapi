#pragma once

#include "BWAPI2Interface.h"
#include "Race.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class UnitType;
  class WeaponType;
  class TechType
  {
    public:
      TechType();
      TechType(int id);
      TechType(const TechType& other);
      TechType& operator=(const TechType& other);
      bool operator==(const TechType& other) const;
      bool operator!=(const TechType& other) const;
      bool operator<(const TechType& other) const;
      int getID() const;
      std::string getName() const;
      Race getRace() const;
      int mineralPrice() const;
      int gasPrice() const;
      int energyUsed() const;
      const UnitType* whatResearches() const;
      const WeaponType* getWeapon() const;
      const std::set<const UnitType*>& whatUses() const;
    private:
      int id;
  };
  namespace TechTypes
  {
    TechType getTechType(std::string& name);
    std::set<TechType>& allTechTypes();
    void init();
    BWAPI2_EXPORT extern const TechType Stim_Packs;
    BWAPI2_EXPORT extern const TechType Lockdown;
    BWAPI2_EXPORT extern const TechType EMP_Shockwave;
    BWAPI2_EXPORT extern const TechType Spider_Mines;
    BWAPI2_EXPORT extern const TechType Scanner_Sweep;
    BWAPI2_EXPORT extern const TechType Tank_Siege_Mode;
    BWAPI2_EXPORT extern const TechType Defensive_Matrix;
    BWAPI2_EXPORT extern const TechType Irradiate;
    BWAPI2_EXPORT extern const TechType Yamato_Gun;
    BWAPI2_EXPORT extern const TechType Cloaking_Field;
    BWAPI2_EXPORT extern const TechType Personnel_Cloaking;
    BWAPI2_EXPORT extern const TechType Burrowing;
    BWAPI2_EXPORT extern const TechType Infestation;
    BWAPI2_EXPORT extern const TechType Spawn_Broodlings;
    BWAPI2_EXPORT extern const TechType Dark_Swarm;
    BWAPI2_EXPORT extern const TechType Plague;
    BWAPI2_EXPORT extern const TechType Consume;
    BWAPI2_EXPORT extern const TechType Ensnare;
    BWAPI2_EXPORT extern const TechType Parasite;
    BWAPI2_EXPORT extern const TechType Psionic_Storm;
    BWAPI2_EXPORT extern const TechType Hallucination;
    BWAPI2_EXPORT extern const TechType Recall;
    BWAPI2_EXPORT extern const TechType Stasis_Field;
    BWAPI2_EXPORT extern const TechType Archon_Warp;
    BWAPI2_EXPORT extern const TechType Restoration;
    BWAPI2_EXPORT extern const TechType Disruption_Web;
    BWAPI2_EXPORT extern const TechType Mind_Control;
    BWAPI2_EXPORT extern const TechType Dark_Archon_Meld;
    BWAPI2_EXPORT extern const TechType Feedback;
    BWAPI2_EXPORT extern const TechType Optical_Flare;
    BWAPI2_EXPORT extern const TechType Maelstrom;
    BWAPI2_EXPORT extern const TechType Lurker_Aspect;
    BWAPI2_EXPORT extern const TechType Healing;
    BWAPI2_EXPORT extern const TechType None;
    BWAPI2_EXPORT extern const TechType Unknown;
    BWAPI2_EXPORT extern const TechType Nuclear_Strike;
  };
}
