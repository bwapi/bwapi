#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Farms_Timing : public Opcode
  {
  public:
    // Ctor
    Farms_Timing(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
