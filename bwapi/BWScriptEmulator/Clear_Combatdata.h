#pragma once
#include "Opcode.h"

namespace AISCRIPT
{
  class Clear_Combatdata : public Opcode
  {
  public:
    // Ctor
    Clear_Combatdata() : Opcode(Enum::CLEAR_COMBATDATA) {};

    // Execute
    virtual bool execute(aithread &thread) const;
  };
}
