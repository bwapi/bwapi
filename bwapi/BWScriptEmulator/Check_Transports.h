#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Check_Transports : public Opcode
  {
  public:
    // Ctor
    Check_Transports(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
