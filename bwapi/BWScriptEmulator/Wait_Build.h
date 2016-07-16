#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Wait_Build : public Opcode
  {
  public:
    // Ctor
    Wait_Build(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
