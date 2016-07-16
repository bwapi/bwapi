#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Wait_Upgrades : public Opcode
  {
  public:
    // Ctor
    Wait_Upgrades() : Opcode(Enum::WAIT_UPGRADES) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
