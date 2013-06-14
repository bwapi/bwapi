#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Scout_With : public Opcode
  {
  public:
    // Ctor
    Scout_With() : Opcode(Enum::SCOUT_WITH) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
