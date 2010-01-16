#pragma once

#include "BWSLInterface.h"

#include <string>
#include <set>

namespace BWSL
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
    BWSL_EXPORT extern const ExplosionType None;
    BWSL_EXPORT extern const ExplosionType Normal;
    BWSL_EXPORT extern const ExplosionType Radial_Splash;
    BWSL_EXPORT extern const ExplosionType Enemy_Splash;
    BWSL_EXPORT extern const ExplosionType Lockdown;
    BWSL_EXPORT extern const ExplosionType Nuclear_Missile;
    BWSL_EXPORT extern const ExplosionType Parasite;
    BWSL_EXPORT extern const ExplosionType Broodlings;
    BWSL_EXPORT extern const ExplosionType EMP_Shockwave;
    BWSL_EXPORT extern const ExplosionType Irradiate;
    BWSL_EXPORT extern const ExplosionType Ensnare;
    BWSL_EXPORT extern const ExplosionType Plague;
    BWSL_EXPORT extern const ExplosionType Stasis_Field;
    BWSL_EXPORT extern const ExplosionType Dark_Swarm;
    BWSL_EXPORT extern const ExplosionType Consume;
    BWSL_EXPORT extern const ExplosionType Yamato_Gun;
    BWSL_EXPORT extern const ExplosionType Restoration;
    BWSL_EXPORT extern const ExplosionType Disruption_Web;
    BWSL_EXPORT extern const ExplosionType Corrosive_Acid;
    BWSL_EXPORT extern const ExplosionType Mind_Control;
    BWSL_EXPORT extern const ExplosionType Feedback;
    BWSL_EXPORT extern const ExplosionType Optical_Flare;
    BWSL_EXPORT extern const ExplosionType Maelstrom;
    BWSL_EXPORT extern const ExplosionType Air_Splash;
    BWSL_EXPORT extern const ExplosionType Unknown;
  }
}
