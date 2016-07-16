#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Set_Attacks : public Opcode
  {
  public:
    // Ctor
    Set_Attacks() : Opcode(Enum::SET_ATTACKS) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
