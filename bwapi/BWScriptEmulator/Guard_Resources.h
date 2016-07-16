#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Guard_Resources : public Opcode
  {
  public:
    // Ctor
    Guard_Resources() : Opcode(Enum::GUARD_RESOURCES) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
