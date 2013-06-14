#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Try_Townpoint : public Opcode
  {
  public:
    // Ctor
    Try_Townpoint() : Opcode(Enum::TRY_TOWNPOINT) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
