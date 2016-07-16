#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Attack_Add : public Opcode
  {
  public:
    // Ctor
    Attack_Add(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
