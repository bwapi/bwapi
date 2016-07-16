#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Train : public Opcode
  {
  public:
    // Ctor
    Train(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
