#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class DefenseUse : public Opcode
  {
  public:
    // Ctor
    DefenseUse(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
