#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Value_Area : public Opcode
  {
  public:
    // Ctor
    Value_Area() : Opcode(Enum::VALUE_AREA) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
