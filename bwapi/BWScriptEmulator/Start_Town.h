#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Start_Town : public Opcode
  {
  public:
    // Ctor
    Start_Town(AISCRIPT::Enum::Enum n) : Opcode(n) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
