#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Region_Size : public Opcode
  {
  public:
    // Ctor
    Region_Size() : Opcode(Enum::REGION_SIZE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
