#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class ExplosionType
  {
    public:
      ExplosionType();
      ExplosionType(int id);
      ExplosionType(const ExplosionType& other);
      ExplosionType& __cdecl operator=(const ExplosionType& other);
      bool __cdecl operator==(const ExplosionType& other) const;
      bool __cdecl operator!=(const ExplosionType& other) const;
      bool __cdecl operator<(const ExplosionType& other) const;
      int __cdecl getID() const;
      std::string __cdecl getName() const;
    private:
      int id;
  };
  namespace ExplosionTypes
  {
    ExplosionType __cdecl getExplosionType(std::string& name);
    std::set<ExplosionType>& __cdecl allExplosionTypes();
    void __cdecl init();
    extern const ExplosionType None;
    extern const ExplosionType Normal;
    extern const ExplosionType Radial_Splash;
    extern const ExplosionType Enemy_Splash;
    extern const ExplosionType Lockdown;
    extern const ExplosionType Nuclear_Missile;
    extern const ExplosionType Parasite;
    extern const ExplosionType Broodlings;
    extern const ExplosionType EMP_Shockwave;
    extern const ExplosionType Irradiate;
    extern const ExplosionType Ensnare;
    extern const ExplosionType Plague;
    extern const ExplosionType Stasis_Field;
    extern const ExplosionType Dark_Swarm;
    extern const ExplosionType Consume;
    extern const ExplosionType Yamato_Gun;
    extern const ExplosionType Restoration;
    extern const ExplosionType Disruption_Web;
    extern const ExplosionType Corrosive_Acid;
    extern const ExplosionType Mind_Control;
    extern const ExplosionType Feedback;
    extern const ExplosionType Optical_Flare;
    extern const ExplosionType Maelstrom;
    extern const ExplosionType Air_Splash;
    extern const ExplosionType Unknown;
  }
}

