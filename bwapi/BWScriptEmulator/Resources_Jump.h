#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Resources_Jump : public Opcode
  {
  public:
    // Ctor
    Resources_Jump(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
