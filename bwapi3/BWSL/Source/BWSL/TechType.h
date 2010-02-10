#pragma once

#include "BWSLInterface.h"
#include "Race.h"

#include <string>
#include <set>

namespace BWSL
{
  class UnitType;
  class WeaponType;
  class BWSL_EXPORT TechType
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
    BWSL_EXPORT TechType getTechType(std::string& name);
    BWSL_EXPORT std::set<TechType>& allTechTypes();
    void init();
    BWSL_EXPORT extern const TechType Stim_Packs;
    BWSL_EXPORT extern const TechType Lockdown;
    BWSL_EXPORT extern const TechType EMP_Shockwave;
    BWSL_EXPORT extern const TechType Spider_Mines;
    BWSL_EXPORT extern const TechType Scanner_Sweep;
    BWSL_EXPORT extern const TechType Tank_Siege_Mode;
    BWSL_EXPORT extern const TechType Defensive_Matrix;
    BWSL_EXPORT extern const TechType Irradiate;
    BWSL_EXPORT extern const TechType Yamato_Gun;
    BWSL_EXPORT extern const TechType Cloaking_Field;
    BWSL_EXPORT extern const TechType Personnel_Cloaking;
    BWSL_EXPORT extern const TechType Burrowing;
    BWSL_EXPORT extern const TechType Infestation;
    BWSL_EXPORT extern const TechType Spawn_Broodlings;
    BWSL_EXPORT extern const TechType Dark_Swarm;
    BWSL_EXPORT extern const TechType Plague;
    BWSL_EXPORT extern const TechType Consume;
    BWSL_EXPORT extern const TechType Ensnare;
    BWSL_EXPORT extern const TechType Parasite;
    BWSL_EXPORT extern const TechType Psionic_Storm;
    BWSL_EXPORT extern const TechType Hallucination;
    BWSL_EXPORT extern const TechType Recall;
    BWSL_EXPORT extern const TechType Stasis_Field;
    BWSL_EXPORT extern const TechType Archon_Warp;
    BWSL_EXPORT extern const TechType Restoration;
    BWSL_EXPORT extern const TechType Disruption_Web;
    BWSL_EXPORT extern const TechType Mind_Control;
    BWSL_EXPORT extern const TechType Dark_Archon_Meld;
    BWSL_EXPORT extern const TechType Feedback;
    BWSL_EXPORT extern const TechType Optical_Flare;
    BWSL_EXPORT extern const TechType Maelstrom;
    BWSL_EXPORT extern const TechType Lurker_Aspect;
    BWSL_EXPORT extern const TechType Healing;
    BWSL_EXPORT extern const TechType None;
    BWSL_EXPORT extern const TechType Unknown;
    BWSL_EXPORT extern const TechType Nuclear_Strike;
  };
}
