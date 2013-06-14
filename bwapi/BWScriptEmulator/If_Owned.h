#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class If_Owned : public Opcode
  {
  public:
    // Ctor
    If_Owned(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
