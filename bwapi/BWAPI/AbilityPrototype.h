#include <string>
#include "Types.h"
class AbilityPrototype
{
  public :
    AbilityPrototype(const u8           abilityIndex,
                     const std::string& name,
                     const u16          manaCost,
                     const u16          targetFlags,
                     const u16          gasCost,
                     const u16          mineralCost);
    ~AbilityPrototype();
    const std::string& getName() const;
          u16          getManaCost() const;
          u16          getTargetFlags() const;
          s32          getAbilityFlag() const;
  private :
    std::string name;
    u16 manaCost;
    u16 targetFlags;
    u16 mineralCost;
    u16 gasCost;
    u32 flagValue;
};
