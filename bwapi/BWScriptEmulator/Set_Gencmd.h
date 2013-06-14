#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Set_Gencmd : public Opcode
  {
  public:
    // Ctor
    Set_Gencmd() : Opcode(Enum::SET_GENCMD) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
