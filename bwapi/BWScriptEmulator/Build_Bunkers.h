#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Build_Bunkers : public Opcode
  {
  public:
    // Ctor
    Build_Bunkers(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
