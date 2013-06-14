#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Switch_Rescue : public Opcode
  {
  public:
    // Ctor
    Switch_Rescue() : Opcode(Enum::SWITCH_RESCUE) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
