#pragma once

#include "TechID.h"
#include "UnitType.h"

namespace BW
{
  class TechType
  {
    public :
      TechType(); /**< Default constructor techId will be BW::TechID::None. */
      TechType(const BW::TechID::Enum& id); /**< Conversion constructor */
      bool operator == (const BW::TechID::Enum& id);
      bool operator != (const BW::TechID::Enum& id);
      bool operator == (const TechType& type);
      const char* getName() const;
      TechID::Enum getID() const;
      bool isValid() const;
      u16 getMineralPrice();
      u16 getGasPrice();
      u16 getEnergyPrice();
      UnitType whereToInvent();
    private :
      TechID::Enum id;
  };
}