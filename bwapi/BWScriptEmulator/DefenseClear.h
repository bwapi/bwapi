#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class DefenseClear : public Opcode
  {
  public:
    // Ctor
    DefenseClear(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
