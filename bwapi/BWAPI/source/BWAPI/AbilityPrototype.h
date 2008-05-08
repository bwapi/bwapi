#pragma  once

#include <string>
#include "../Types.h"

namespace BWAPI
{
  /**  
   * Represents abilitiy type (action) that can broodwar unit make. 
   * This class is (as it is in BWAPI package) our internal structure, 
   * so although it correspons to bw logic, it has nothing to do with bw's structure.
   */
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
      const std::string&  getName() const;
      u16                 getManaCost() const;
      u16                 getTargetFlags() const;
      s32                 getAbilityFlag() const;
    private :
      /** Name of the action */
      std::string name;
      /** Mana cost of the abiilty */
      u16 manaCost;
      /** 
       * Definition of possible target of the ability. 
       * @see TargetType
       */
      u16 targetFlags;
      /** Mineral cost of the ability (Lurker morph, Guardian morph etc) */
      u16 mineralCost;
      /** Gas cost of the ability (Lurker morph, Guardian morph etc) */
      u16 gasCost;
      /** 
       * Value unique to every Ability, is used in Unit to define accesible abilities.
       * This values is always equal to 2^n, as it is binary flag.
       */
      u32 flagValue;
  };
};
