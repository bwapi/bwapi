#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class ExplosionType
  {
    public:
      ExplosionType();
      ExplosionType(int id);
      ExplosionType(const ExplosionType& other);
      ExplosionType& operator=(const ExplosionType& other);
      bool operator==(const ExplosionType& other) const;
      bool operator!=(const ExplosionType& other) const;
      bool operator<(const ExplosionType& other) const;
      int getID() const;
      std::string getName() const;
    private:
      int id;
  };
  namespace ExplosionTypes
  {
    ExplosionType getExplosionType(std::string& name);
    std::set<ExplosionType>& allExplosionTypes();
    void init();
    BWAPI2_EXPORT extern const ExplosionType None;
    BWAPI2_EXPORT extern const ExplosionType Normal;
    BWAPI2_EXPORT extern const ExplosionType Radial_Splash;
    BWAPI2_EXPORT extern const ExplosionType Enemy_Splash;
    BWAPI2_EXPORT extern const ExplosionType Lockdown;
    BWAPI2_EXPORT extern const ExplosionType Nuclear_Missile;
    BWAPI2_EXPORT extern const ExplosionType Parasite;
    BWAPI2_EXPORT extern const ExplosionType Broodlings;
    BWAPI2_EXPORT extern const ExplosionType EMP_Shockwave;
    BWAPI2_EXPORT extern const ExplosionType Irradiate;
    BWAPI2_EXPORT extern const ExplosionType Ensnare;
    BWAPI2_EXPORT extern const ExplosionType Plague;
    BWAPI2_EXPORT extern const ExplosionType Stasis_Field;
    BWAPI2_EXPORT extern const ExplosionType Dark_Swarm;
    BWAPI2_EXPORT extern const ExplosionType Consume;
    BWAPI2_EXPORT extern const ExplosionType Yamato_Gun;
    BWAPI2_EXPORT extern const ExplosionType Restoration;
    BWAPI2_EXPORT extern const ExplosionType Disruption_Web;
    BWAPI2_EXPORT extern const ExplosionType Corrosive_Acid;
    BWAPI2_EXPORT extern const ExplosionType Mind_Control;
    BWAPI2_EXPORT extern const ExplosionType Feedback;
    BWAPI2_EXPORT extern const ExplosionType Optical_Flare;
    BWAPI2_EXPORT extern const ExplosionType Maelstrom;
    BWAPI2_EXPORT extern const ExplosionType Air_Splash;
    BWAPI2_EXPORT extern const ExplosionType Unknown;
  }
}
