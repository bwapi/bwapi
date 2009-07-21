#pragma once
#include <string>
#include <set>
#include <BWAPI/Race.h>
namespace BWAPI
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
    extern const TechType Stim_Packs;
    extern const TechType Lockdown;
    extern const TechType EMP_Shockwave;
    extern const TechType Spider_Mines;
    extern const TechType Scanner_Sweep;
    extern const TechType Tank_Siege_Mode;
    extern const TechType Defensive_Matrix;
    extern const TechType Irradiate;
    extern const TechType Yamato_Gun;
    extern const TechType Cloaking_Field;
    extern const TechType Personnel_Cloaking;
    extern const TechType Burrowing;
    extern const TechType Infestation;
    extern const TechType Spawn_Broodlings;
    extern const TechType Dark_Swarm;
    extern const TechType Plague;
    extern const TechType Consume;
    extern const TechType Ensnare;
    extern const TechType Parasite;
    extern const TechType Psionic_Storm;
    extern const TechType Hallucination;
    extern const TechType Recall;
    extern const TechType Stasis_Field;
    extern const TechType Archon_Warp;
    extern const TechType Restoration;
    extern const TechType Disruption_Web;
    extern const TechType Mind_Control;
    extern const TechType Dark_Archon_Meld;
    extern const TechType Feedback;
    extern const TechType Optical_Flare;
    extern const TechType Maelstrom;
    extern const TechType Lurker_Aspect;
    extern const TechType Healing;
    extern const TechType None;
    extern const TechType Unknown;
    extern const TechType Nuclear_Strike;
  };
}
