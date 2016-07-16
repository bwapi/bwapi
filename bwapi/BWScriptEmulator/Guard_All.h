#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Guard_All : public Opcode
  {
  public:
    // Ctor
    Guard_All() : Opcode(Enum::GUARD_ALL) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
