#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Create_Unit : public Opcode
  {
  public:
    // Ctor
    Create_Unit(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
