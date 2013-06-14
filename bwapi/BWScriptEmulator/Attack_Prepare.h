#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Attack_Prepare : public Opcode
  {
  public:
    // Ctor
    Attack_Prepare(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
