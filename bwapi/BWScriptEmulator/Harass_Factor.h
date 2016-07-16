#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Harass_Factor : public Opcode
  {
  public:
    // Ctor
    Harass_Factor() : Opcode(Enum::HARASS_FACTOR) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
