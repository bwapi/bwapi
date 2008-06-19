#pragma once

#include "UpgradeID.h"

namespace BW
{
  /** Mapping of bw's upgrade representation. */
  class UpgradeType
  {
    public :
      UpgradeType();
      UpgradeType(const UpgradeID::Enum& id);
      bool operator == (const UpgradeID::Enum& id) const;
      UpgradeID::Enum getID() const;
      const char* getName() const;
      bool isValid() const;
      u8 upgradesMax() const;
    private :
      UpgradeID::Enum id;
  };
}