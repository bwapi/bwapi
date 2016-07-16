#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class DefenseBuild : public Opcode
  {
  public:
    // Ctor
    DefenseBuild(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
