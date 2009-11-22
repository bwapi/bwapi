#pragma once

#include "TechID.h"
#include "UnitType.h"

namespace BW
{
  /**
   * Mapping of bw's tech representation. Note the difference between tech and upgrade, tech is only that kind
   * of stuff, that is connected with some special ability, so for example Stimpacks is tech, but Marine range
   * is upgrade.
   */
  class TechType
  {
    public :
      TechType(); /**< Default constructor techId will be BW::TechID::None. */
      TechType(const u8& id); /**< Conversion constructor */
      bool operator == (const u8& id);
      bool operator != (const u8& id);
      bool operator == (const TechType& type);
      const char* getName() const;
      u8 getID() const;
      /**
       * Some bw techs are not valid and they don't have even valid names, so any manipulation with it should
       * be avoided
       * @return @c true the tech is valid and can be accesed, @c false otherwise.
       */
      bool isValid() const;
      u16 getMineralPrice();
      u16 getGasPrice();
      u16 getEnergyPrice();
      /** Our internal way to determine what building should be used to invent certain tech. */
      UnitType whereToInvent();
    private :
      /**< Identificator of the tech. */
      u8 id;
  };
};
