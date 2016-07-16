#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Default_Build : public Opcode
  {
  public:
    // Ctor
    Default_Build(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
