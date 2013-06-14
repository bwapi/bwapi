#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Build : public Opcode
  {
  public:
    // Ctor
    Build() : Opcode(Enum::BUILD) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
