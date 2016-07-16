#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Wait_Bunkers : public Opcode
  {
  public:
    // Ctor
    Wait_Bunkers(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
