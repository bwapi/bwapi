#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Target_Expansion : public Opcode
  {
  public:
    // Ctor
    Target_Expansion() : Opcode(Enum::TARGET_EXPANSION) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
