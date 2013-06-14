#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Prep_Down : public Opcode
  {
  public:
    // Ctor
    Prep_Down() : Opcode(Enum::PREP_DOWN) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
