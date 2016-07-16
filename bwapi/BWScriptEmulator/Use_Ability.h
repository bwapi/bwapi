#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Use_Ability : public Opcode
  {
  public:
    // Ctor
    Use_Ability(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
