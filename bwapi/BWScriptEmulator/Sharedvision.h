#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Sharedvision : public Opcode
  {
  public:
    // Ctor
    Sharedvision(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
