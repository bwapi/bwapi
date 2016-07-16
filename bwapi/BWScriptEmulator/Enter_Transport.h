#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Enter_Transport : public Opcode
  {
  public:
    // Ctor
    Enter_Transport(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
