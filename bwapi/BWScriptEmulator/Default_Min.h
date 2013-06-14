#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Default_Min : public Opcode
  {
  public:
    // Ctor
    Default_Min() : Opcode(Enum::DEFAULT_MIN) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
